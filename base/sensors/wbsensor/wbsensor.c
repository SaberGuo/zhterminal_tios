/*
 * wbsensor.c
 *
 *  Created on: 2018年11月15日
 *      Author: saber
 */
#include "wbsensor.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include <ti/sysbios/knl/Task.h>
#include "../uart/uart.h"

#define WBS_RETRY_NUM 5

#define WBSENSOR_READ_SIZE 61
#define MAX_CTRL_PORT 2
#define CTRL_INPUT_PORT 0// 控制抽水泵 AD8
#define CTRL_OUTPUT_PORT 1//控制排水阀 AD9
uint8_t wdsensor_power_ports[MAX_CTRL_PORT]={Board_GPIO_CTRL1,Board_GPIO_CTRL2};
const char wbsensor_modbus_command[]={0x01,0x03,0x00,0x30,0x00,0x1C,0x44,0x0C};
const char wbsensor_modbus_ctrl[]={0x01,0x10,0x00,0x19,0x00,0x01,0x02,0x00,0x00,0xA4,0x59};
float wbsensor_units[] = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
uint16_t wbsensor_res_buffer[2][30];

UART_Params* wbsensor_uartParams = NULL;

void open_input(){

    GPIO_write(wdsensor_power_ports[CTRL_INPUT_PORT], Board_GPIO_HIGH);
}
void close_input(){
    GPIO_write(wdsensor_power_ports[CTRL_INPUT_PORT], Board_GPIO_LOW);
}
void open_output(){
    GPIO_write(wdsensor_power_ports[CTRL_OUTPUT_PORT], Board_GPIO_HIGH);
}
void close_output(){
    GPIO_write(wdsensor_power_ports[CTRL_OUTPUT_PORT], Board_GPIO_LOW);
}

uint8_t wbsensor_open(uint8_t num){
    close_input();
    close_output();

    if(wbsensor_uartParams == NULL){
        wbsensor_uartParams = malloc(sizeof(UART_Params));
        UART_Params_init(wbsensor_uartParams);
        wbsensor_uartParams->writeDataMode = UART_DATA_BINARY;
        wbsensor_uartParams->readDataMode = UART_DATA_BINARY;
        wbsensor_uartParams->readReturnMode = UART_RETURN_FULL;
        wbsensor_uartParams->readTimeout = 1000;
        wbsensor_uartParams->readEcho = UART_ECHO_OFF;
        wbsensor_uartParams->baudRate = 9600;
    }
    return serial_port_open(num, wbsensor_uartParams);
}
uint8_t wbsensor_close(uint8_t num){
    serial_port_close(num);
    return ZH_OK;
}
uint8_t wbsensor_process(uint8_t num){
    int i = 0;
    //open the input and output
    open_input();
    delay(300);
    open_output();
    //delay for a time
    Task_sleep(1*10*1); //4min
    //close the input wait for the tank empty
    close_input();
    Task_sleep(1*10*1);
    //close the output and open the input
    close_output();
    delay(300);
    open_input();
    //wait for the tank full
    Task_sleep(1*10*1);
    close_input();
    //collect the data
    Task_sleep(1*10*1);//delay for 2min
    for(i = 0; i<WBS_RETRY_NUM;i++){
        if(ZH_FAIL != get_modbus_datas(num, wbsensor_modbus_command, sizeof(wbsensor_modbus_command),
                         WBSENSOR_READ_SIZE,
                         wbsensor_res_buffer[num],sizeof(wbsensor_res_buffer[num])/sizeof(uint16_t))){
            break;
        }
        Task_sleep(100);
    }

    //return default status

}
float wbsensor_get_data(uint8_t num, uint8_t key_num){
    if(key_num>=20){
        return ZH_OK;
    }
        //float tt = 192.96875;
    uint8_t buf[4]={0};
        //memcpy(buf, (uint8_t *)(&tt),4);
    buf[2] = (uint8_t)(wbsensor_res_buffer[num][key_num*2+1]>>8);
    buf[3] = (uint8_t)(wbsensor_res_buffer[num][key_num*2+1]&0x00ff);
    buf[0] = (uint8_t)(wbsensor_res_buffer[num][key_num*2]>>8);
    buf[1] = (uint8_t)(wbsensor_res_buffer[num][key_num*2]&0x00ff);
        //memcpy(buf, (char *)(zxws6p_res_buffer[num][key_num*2+1]), 2);
        //memcpy(buf+2, (char *)(zxws6p_res_buffer[num][key_num*2]), 2);
    float* temp = (float*)buf;
    return (*temp);
}
