/*
 * th10s.c
 *
 *  Created on: 2017Äê10ÔÂ24ÈÕ
 *      Author: saber
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include "../uart/uart.h"
#include "th10s.h"

#define TH10S_READ_SIZE 9

const char th10s_modbus_command[]={0x02,0x03,0x00,0x2A,0x00,0x02,0xA5,0xF1};

float th10s_units[] = {10.0,10.0};
uint16_t th10s_res_buffer[2];
UART_Params* th10s_uartParams = NULL;

uint8_t th10s_open(uint8_t num){
    if(th10s_uartParams == NULL){
        th10s_uartParams = malloc(sizeof(UART_Params));
        UART_Params_init(th10s_uartParams);
        th10s_uartParams->writeDataMode = UART_DATA_BINARY;
        th10s_uartParams->readDataMode = UART_DATA_BINARY;
        th10s_uartParams->readReturnMode = UART_RETURN_FULL;
        th10s_uartParams->readTimeout = 1000;
        th10s_uartParams->readEcho = UART_ECHO_OFF;
        th10s_uartParams->baudRate = 9600;
    }
    return serial_port_open(num, th10s_uartParams);
}

uint8_t th10s_close(uint8_t num){
    serial_port_close(num);
    return ZH_OK;
}

uint8_t th10s_process(uint8_t num){
    return get_modbus_datas(num,th10s_modbus_command, sizeof(th10s_modbus_command),
                            TH10S_READ_SIZE,
                            th10s_res_buffer,sizeof(th10s_res_buffer)/sizeof(uint16_t));
}

float th10s_get_data(uint8_t num, uint8_t key_num){
    if(key_num>=2){
        return ZH_OK;
    }
    return (float)(((int16_t)th10s_res_buffer[key_num])/th10s_units[key_num]);
}

