/*
 * csf11.c
 *
 *  Created on: 2018Äê3ÔÂ30ÈÕ
 *      Author: saber
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include "../uart/uart.h"

#include "csf11.h"


#define CSF11_READ_SIZE 9

const char csf11_modbus_command[]={0x02,0x03,0x00,0x00,0x00,0x02,0xc4,0x38};

float csf11_units[] = {10.0,10.0};
uint16_t csf11_res_buffer[2][2];
UART_Params* csf11_uartParams = NULL;

uint8_t csf11_open(uint8_t num){
    if(csf11_uartParams == NULL){
        csf11_uartParams = malloc(sizeof(UART_Params));
        UART_Params_init(csf11_uartParams);
        csf11_uartParams->writeDataMode = UART_DATA_BINARY;
        csf11_uartParams->readDataMode = UART_DATA_BINARY;
        csf11_uartParams->readReturnMode = UART_RETURN_FULL;
        csf11_uartParams->readTimeout = 1000;
        csf11_uartParams->readEcho = UART_ECHO_OFF;
        csf11_uartParams->baudRate = 9600;
    }
    return serial_port_open(num, csf11_uartParams);
}
uint8_t csf11_close(uint8_t num){
    serial_port_close(num);
    return ZH_OK;
}
uint8_t csf11_process(uint8_t num){
    get_modbus_datas(num,csf11_modbus_command, sizeof(csf11_modbus_command),
                                CSF11_READ_SIZE,
                                csf11_res_buffer[num],sizeof(csf11_res_buffer[num])/sizeof(uint16_t));
    delay(1000);
    return get_modbus_datas(num,csf11_modbus_command, sizeof(csf11_modbus_command),
                            CSF11_READ_SIZE,
                            csf11_res_buffer[num],sizeof(csf11_res_buffer[num])/sizeof(uint16_t));
}
float csf11_get_data(uint8_t num, uint8_t key_num){
    if(key_num>=2){
        return ZH_OK;
    }
    return (float)(((int16_t)csf11_res_buffer[num][key_num])/csf11_units[key_num]);
}
