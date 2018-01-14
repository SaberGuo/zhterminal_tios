/*
 * nh122.c
 *
 *  Created on: 2017Äê11ÔÂ17ÈÕ
 *      Author: saber
 */
#include "nh122.h"
#include <string.h>
#include "base/rt_clock/rt_clock.h"
#include "../sensors_comm.h"
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>


const char nh122_modbus_command[]={0x15,0x04,0x00,0x00,0x00,0x03,0xB3,0x1F};
uint8_t nh122_res_buffer[SENSOR_BUFFER];
uint8_t nh122_pbuffer=0;
float nh122_temp_value;
float nh122_hum_value;
float nh122_bar_value;
char nh122_temp_key[] ="Ta";
char nh122_hum_key[] = "Ua";
char nh122_bar_key[] = "Pa";


UART_Handle nh122_uart;
UART_Params nh122_uartParams;

void nh122_init(){
    UART_Params_init(&nh122_uartParams);
    nh122_uartParams.writeDataMode = UART_DATA_BINARY;
    nh122_uartParams.readDataMode = UART_DATA_BINARY;
    nh122_uartParams.readReturnMode = UART_RETURN_FULL;
    nh122_uartParams.readTimeout = 1000;
    nh122_uartParams.readEcho = UART_ECHO_OFF;
    nh122_uartParams.baudRate = 9600;
}

uint8_t nh122_open(){
    nh122_uart = UART_open(Board_UART_485_0, &nh122_uartParams);
    if (nh122_uart == NULL) {
        LOG_MSG("th10s uart open fail!\n");
        return 1;
    }
    return 0;
}
void set_nh122_serial_flag(uint8_t flag){
    if(flag == SERIAL_READ){
        GPIO_write(Board_GPIO_485_0_RE,Board_GPIO_LOW);
    }

    if(flag == SERIAL_WRITE){
        GPIO_write(Board_GPIO_485_0_RE,Board_GPIO_HIGH);
    }
}

void nh122_close(){
    UART_close(nh122_uart);
    nh122_uart = NULL;

}


void nh122_process(){
    int16_t res = 0;
    /*wait for sensor capture*/
    Task_sleep(10000);
    memset(nh122_res_buffer, 0, SENSOR_BUFFER);
    set_nh122_serial_flag(SERIAL_WRITE);
    UART_write(nh122_uart,nh122_modbus_command, sizeof(nh122_modbus_command));
    delay(3000);
    set_nh122_serial_flag(SERIAL_READ);
    delay(3000);

    UART_read(nh122_uart,nh122_res_buffer,11);
    if(crc_16(nh122_res_buffer,11) == 1){
        LOG_MSG("crc16 error!\n");
    }else{
    //{
        res = (int16_t)(((uint16_t)nh122_res_buffer[3])<<8|nh122_res_buffer[4]);
        nh122_temp_value = res/10.0;
        res = (int16_t)(((uint16_t)nh122_res_buffer[5])<<8|nh122_res_buffer[6]);
        nh122_hum_value = res/10.0;
        res = (int16_t)(((uint16_t)nh122_res_buffer[7])<<8|nh122_res_buffer[8]);
        nh122_bar_value = res/10.0;
    }


}

float get_nh122_temperature(){
    return nh122_temp_value;
}
char* get_nh122_temp_key(){
    return nh122_temp_key;
}
float get_nh122_humidity(){
    return nh122_hum_value;
}
char* get_nh122_hum_key(){
    return nh122_hum_key;
}
float get_nh122_bar(){
    return nh122_bar_value;
}
char* get_nh122_bar_key(){
    return nh122_bar_key;
}
