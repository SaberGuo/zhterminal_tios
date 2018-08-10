/*
 * nh122.c
 *
 *  Created on: 2017Äê11ÔÂ17ÈÕ
 *      Author: saber
 */
#include "nh122.h"
#include <string.h>
#include "base/rt_clock/rt_clock.h"
#include "../uart/uart.h"
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#define NH122_READ_SIZE 11

const char nh122_modbus_command[]={0x15,0x04,0x00,0x00,0x00,0x03,0xB3,0x1F};
float nh122_units[] = {10.0,10.0,10.0};
uint16_t nh122_res_buffer[3];
UART_Params* nh122_uartParams = NULL;

uint8_t nh122_open(uint8_t num){
    if(nh122_uartParams == NULL){
        nh122_uartParams = malloc(sizeof(UART_Params));
        UART_Params_init(nh122_uartParams);
        nh122_uartParams->writeDataMode = UART_DATA_BINARY;
        nh122_uartParams->readDataMode = UART_DATA_BINARY;
        nh122_uartParams->readReturnMode = UART_RETURN_FULL;
        nh122_uartParams->readTimeout = 1000;
        nh122_uartParams->readEcho = UART_ECHO_OFF;
        nh122_uartParams->baudRate = 9600;
    }
    return serial_port_open(num, nh122_uartParams);
}
uint8_t nh122_close(uint8_t num){
    serial_port_close(num);
    return 0;
}
uint8_t nh122_process(uint8_t num){
    get_modbus_datas(num,nh122_modbus_command, sizeof(nh122_modbus_command),
                             NH122_READ_SIZE,
                             nh122_res_buffer,sizeof(nh122_res_buffer)/sizeof(uint16_t));
    delay(1000);
    return get_modbus_datas(num,nh122_modbus_command, sizeof(nh122_modbus_command),
                         NH122_READ_SIZE,
                         nh122_res_buffer,sizeof(nh122_res_buffer)/sizeof(uint16_t));
}
float nh122_get_data(uint8_t num, uint8_t key_num){
    if(key_num>=3){
        return 0;
    }
    return (float)(((int16_t)nh122_res_buffer[key_num])/nh122_units[key_num]);
}

