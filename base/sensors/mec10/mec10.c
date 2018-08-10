/*
 * mec10.c
 *
 *  Created on: 2017Äê11ÔÂ17ÈÕ
 *      Author: saber
 */
#include "mec10.h"
#include <string.h>
#include "base/rt_clock/rt_clock.h"
#include "../uart/uart.h"
#define MEC10_READ_SIZE 11
const char mec10_modbus_command[]={0x01,0x04,0x00,0x00,0x00,0x03,0xB0,0x0B};
uint16_t mec10_res_buffer[3];
float mec10_units[] = {100.0,100.0,1};
UART_Params* mec10_uartParams = NULL;



uint8_t mec10_open(uint8_t num){
    if(mec10_uartParams == NULL){
        mec10_uartParams = malloc(sizeof(UART_Params));
        UART_Params_init(mec10_uartParams);
        mec10_uartParams->writeDataMode = UART_DATA_BINARY;
        mec10_uartParams->readDataMode = UART_DATA_BINARY;
        mec10_uartParams->readReturnMode = UART_RETURN_FULL;
        mec10_uartParams->readTimeout = 1000;
        mec10_uartParams->readEcho = UART_ECHO_OFF;
        mec10_uartParams->baudRate = 9600;
    }
    return serial_port_open(num, mec10_uartParams);
}
uint8_t mec10_close(uint8_t num){
    serial_port_close(num);
    return ZH_OK;
}
uint8_t mec10_process(uint8_t num){
    return get_modbus_datas(num,mec10_modbus_command, sizeof(mec10_modbus_command),
                     MEC10_READ_SIZE,
                     mec10_res_buffer,sizeof(mec10_res_buffer)/sizeof(uint16_t));
}
float mec10_get_data(uint8_t num, uint8_t key_num){
    if(key_num>=3){
        return 0;
    }
    return (float)(((int16_t)mec10_res_buffer[key_num])/mec10_units[key_num]);
}

