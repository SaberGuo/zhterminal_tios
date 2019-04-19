/*
 * zwinsoft.c
 *
 *  Created on: 2018Äê7ÔÂ18ÈÕ
 *      Author: saber
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include "../uart/uart.h"
#include "zwinsoft.h"
#define ZWINSOFT_MAX_NUM 6

UART_Params* zwinsoft_uartParams = NULL;
const uint8_t zwinsoft_read_size = 17;
const char zwinsoft_modbus_command[]={0x40,0x03,0x0,0x0,0x0,0x06,0xCA,0xD9};

float zwinsoft_units[] = {100.0,1.0,1.0,1.0,10.0,10.0};
uint16_t zwinsoft_res_buffer[ZWINSOFT_MAX_NUM];


uint8_t zwinsoft_open(uint8_t num){
    if(zwinsoft_uartParams == NULL){
        zwinsoft_uartParams = (UART_Params*)malloc(sizeof(UART_Params));
            UART_Params_init(zwinsoft_uartParams);
            zwinsoft_uartParams->writeDataMode = UART_DATA_BINARY;
            zwinsoft_uartParams->readDataMode = UART_DATA_BINARY;
            zwinsoft_uartParams->readReturnMode = UART_RETURN_FULL;
            zwinsoft_uartParams->readEcho = UART_ECHO_OFF;
            zwinsoft_uartParams->readTimeout = 1000;
            zwinsoft_uartParams->baudRate = 9600;
        }
        return serial_port_open(num, zwinsoft_uartParams);
}
uint8_t zwinsoft_close(uint8_t num){
    serial_port_close(num);
    return ZH_OK;
}
uint8_t zwinsoft_process(uint8_t num){
    Task_sleep(100000);
    return get_modbus_nocrc_datas(num,zwinsoft_modbus_command, sizeof(zwinsoft_modbus_command),
                            zwinsoft_read_size,
                               zwinsoft_res_buffer,sizeof(zwinsoft_res_buffer)/sizeof(uint16_t));
}
float zwinsoft_get_data(uint8_t num, uint8_t key_num){
    if(key_num>=ZWINSOFT_MAX_NUM){
        return ZH_OK;
    }
    return (float)(((int16_t)zwinsoft_res_buffer[key_num])/zwinsoft_units[key_num]);
}
