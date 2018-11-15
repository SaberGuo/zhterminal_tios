/*
 * zxws6p.c
 *
 *  Created on: 2018Äê10ÔÂ12ÈÕ
 *      Author: saber
 */

#include "zxws6p.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include "../uart/uart.h"

#define ZXWS6P_READ_SIZE 45

const char zxws6p_modbus_command[]={0x01,0x03,0x00,0x00,0x00,0x14,0x45,0xc5};

float zxws6p_units[] = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
uint16_t zxws6p_res_buffer[2][20];

UART_Params* zxws6p_uartParams = NULL;


uint8_t zxws6p_open(uint8_t num){
    if(zxws6p_uartParams == NULL){
        zxws6p_uartParams = malloc(sizeof(UART_Params));
        UART_Params_init(zxws6p_uartParams);
        zxws6p_uartParams->writeDataMode = UART_DATA_BINARY;
        zxws6p_uartParams->readDataMode = UART_DATA_BINARY;
        zxws6p_uartParams->readReturnMode = UART_RETURN_FULL;
        zxws6p_uartParams->readTimeout = 1000;
        zxws6p_uartParams->readEcho = UART_ECHO_OFF;
        zxws6p_uartParams->baudRate = 9600;
    }
    return serial_port_open(num, zxws6p_uartParams);
}

uint8_t zxws6p_close(uint8_t num){
    serial_port_close(num);
    return ZH_OK;
}

uint8_t zxws6p_process(uint8_t num){
    get_modbus_datas(num,zxws6p_modbus_command, sizeof(zxws6p_modbus_command),
                     ZXWS6P_READ_SIZE,
                     zxws6p_res_buffer[num],sizeof(zxws6p_res_buffer[num])/sizeof(uint16_t));
}



float zxws6p_get_data(uint8_t num, uint8_t key_num){
    if(key_num>=10){
        return ZH_OK;
    }
        //float tt = 192.96875;
    uint8_t buf[4]={0};
        //memcpy(buf, (uint8_t *)(&tt),4);
    buf[3] = (uint8_t)(zxws6p_res_buffer[num][key_num*2+1]>>8);
    buf[2] = (uint8_t)(zxws6p_res_buffer[num][key_num*2+1]&0x00ff);
    buf[1] = (uint8_t)(zxws6p_res_buffer[num][key_num*2]>>8);
    buf[0] = (uint8_t)(zxws6p_res_buffer[num][key_num*2]&0x00ff);
        //memcpy(buf, (char *)(zxws6p_res_buffer[num][key_num*2+1]), 2);
        //memcpy(buf+2, (char *)(zxws6p_res_buffer[num][key_num*2]), 2);
    float* temp = (float*)buf;
    return (*temp);


}
