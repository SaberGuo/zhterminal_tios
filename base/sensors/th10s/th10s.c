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

#include "th10s.h"

uint8_t th_res_buffer[SENSOR_BUFFER];
uint8_t th_pbuffer=0;

UART_Handle th10s_uart;
UART_Params th10s_uartParams;

char tp_modbus_command[]={0x02,0x03,0x00,0x2A,0x00,0x01,0xA5,0xF1};
char hm_modbus_command[]={0x02,0x03,0x00,0x2B,0x00,0x01,0xF4,0x31};

float temperature_value = 0;
float humility_value =0;

uint8_t th10s_init(){
	UART_Params_init(&th10s_uartParams);
    th10s_uartParams.writeDataMode = UART_DATA_BINARY;
    th10s_uartParams.readDataMode = UART_DATA_BINARY;
    th10s_uartParams.readReturnMode = UART_RETURN_FULL;
    th10s_uartParams.readEcho = UART_ECHO_OFF;
    th10s_uartParams.baudRate = 9600;

    th10s_uart = UART_open(Board_UART_485_1, &th10s_uartParams);
    if (th10s_uart == NULL) {
        LOG_MSG("th10s uart open fail!\n");
    	return 1;
    }
    return 0;
}

void set_th10s_serial_flag(uint8_t flag){
    if(flag == SERIAL_READ){
        GPIO_write(Board_GPIO_485_1_RE,Board_GPIO_LOW);
    }

    if(flag == SERIAL_WRITE){
        GPIO_write(Board_GPIO_485_1_RE,Board_GPIO_HIGH);
    }
}

uint16_t get_modbus_value(char * command, uint8_t size){
    uint16_t res = 0;
    memset(th_res_buffer, 0, SENSOR_BUFFER);
    set_th10s_serial_flag(SERIAL_WRITE);
    UART_write(th10s_uart,command, size);
   
    //delay_ms(3000);
    set_th10s_serial_flag(SERIAL_READ);
    //delay_ms(100000);
    UART_read(th10s_uart,th_res_buffer,8);
    res = ((uint16_t)th_res_buffer[3])<<8|th_res_buffer[4];
    return res;
}

void th10s_close(){
	UART_close(th10s_uart);
	th10s_uart = NULL;
}

void th10s_process(){
	uint16_t res = 0;

    res = get_modbus_value(tp_modbus_command, sizeof(tp_modbus_command));
    temperature_value = res/10.0;

    res = get_modbus_value(hm_modbus_command, sizeof(hm_modbus_command));
    humility_value = res/10.0;
}

float get_temperature(){
    return temperature_value;
}

float get_humility(){
    return humility_value;
}
