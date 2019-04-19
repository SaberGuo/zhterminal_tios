/*
 * csf12.c
 *
 *  Created on: 2018Äê3ÔÂ30ÈÕ
 *      Author: saber
 */
#include <base/sensors/csf11/csf12.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include "../uart/uart.h"



#define csf12_READ_SIZE 9
#define MAX_SENSORS_SINGLE_PORT 5
#define MAX_PORT_NUM 2
const float csf12_offsets[]={0.0,30.0};
const char csf12_modbus_command[MAX_SENSORS_SINGLE_PORT][8]={{0x01, 0x03, 0x00, 0x00, 0x00, 0x02, 0xc4, 0x0b},
                                                            {0x02, 0x03, 0x00, 0x00, 0x00, 0x02, 0xc4, 0x38},
                                                            {0x03, 0x03, 0x00, 0x00, 0x00, 0x02, 0xc5, 0xe9},
                                                            {0x04, 0x03, 0x00, 0x00, 0x00, 0x02, 0xc4, 0x5e},
                                                            {0x05, 0x03, 0x00, 0x00, 0x00, 0x02, 0xc5, 0x8f},};

float csf12_units[] = {10.0,10.0};
uint16_t csf12_res_buffer[MAX_SENSORS_SINGLE_PORT][2];
UART_Params* csf12_uartParams = NULL;

uint8_t csf12_open(uint8_t num){
    if(csf12_uartParams == NULL){
        csf12_uartParams = malloc(sizeof(UART_Params));
        UART_Params_init(csf12_uartParams);
        csf12_uartParams->writeDataMode = UART_DATA_BINARY;
        csf12_uartParams->readDataMode = UART_DATA_BINARY;
        csf12_uartParams->readReturnMode = UART_RETURN_FULL;
        csf12_uartParams->readTimeout = 1000;
        csf12_uartParams->readEcho = UART_ECHO_OFF;
        csf12_uartParams->baudRate = 9600;
    }
    return serial_port_open(num%MAX_PORT_NUM, csf12_uartParams);
}
uint8_t csf12_close(uint8_t num){
    serial_port_close(num%MAX_PORT_NUM);
    return ZH_OK;
}
uint8_t csf12_process(uint8_t num){
    int index1 = num%MAX_PORT_NUM;// num for port
    int index2 = num/MAX_PORT_NUM;// num for sensor
    get_modbus_datas(index1,csf12_modbus_command[index2], sizeof(csf12_modbus_command[index2]),
                                csf12_READ_SIZE,
                                csf12_res_buffer[index2],sizeof(csf12_res_buffer[index2])/sizeof(uint16_t));
    delay(1000);
    return get_modbus_datas(index1,csf12_modbus_command[index2], sizeof(csf12_modbus_command[index2]),
                            csf12_READ_SIZE,
                            csf12_res_buffer[index2],sizeof(csf12_res_buffer[index2])/sizeof(uint16_t));
}
float csf12_get_data(uint8_t num, uint8_t key_num){
    if(key_num>=2){
        return ZH_OK;
    }
    float temp = (float)(((int16_t)csf12_res_buffer[num/MAX_PORT_NUM][key_num])/csf12_units[key_num]);
    return temp-csf12_offsets[key_num];
}
