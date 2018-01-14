/*
 * mec10.c
 *
 *  Created on: 2017Äê11ÔÂ17ÈÕ
 *      Author: saber
 */
#include "mec10.h"
#include <string.h>
#include "base/rt_clock/rt_clock.h"

const char mec10_modbus_command[]={0x01,0x04,0x00,0x00,0x00,0x03,0xB0,0x0B};
uint8_t mec10_res_buffer[SENSOR_BUFFER];
uint8_t mec10_pbuffer=0;
float mec10_temp_value;
float mec10_hum_value;
float mec10_ec_value;
char mec10_temp_key[] ="temp_soil";
char mec10_hum_key[] = "humidity_soil";
char mec10_ec_key[] = "ec_soil";


UART_Handle mec10_uart;
UART_Params mec10_uartParams;

uint8_t mec10_init(){
    UART_Params_init(&mec10_uartParams);
    mec10_uartParams.writeDataMode = UART_DATA_BINARY;
    mec10_uartParams.readDataMode = UART_DATA_BINARY;
    mec10_uartParams.readReturnMode = UART_RETURN_FULL;
    mec10_uartParams.readEcho = UART_ECHO_OFF;
    mec10_uartParams.baudRate = 9600;

    mec10_uart = UART_open(Board_UART_485_1, &mec10_uartParams);
    if (mec10_uart == NULL) {
        LOG_MSG("mec10 uart open fail!\n");
        return 1;
    }
    return 0;
}
void set_mec10_serial_flag(uint8_t flag){
    if(flag == SERIAL_READ){
        GPIO_write(Board_GPIO_485_1_RE,Board_GPIO_LOW);
    }

    if(flag == SERIAL_WRITE){
        GPIO_write(Board_GPIO_485_1_RE,Board_GPIO_HIGH);
    }
}

void mec10_close(){
    UART_close(mec10_uart);
    mec10_uart = NULL;
}


void mec10_process(){
    int16_t res = 0;

    memset(mec10_res_buffer, 0, SENSOR_BUFFER);
    set_mec10_serial_flag(SERIAL_WRITE);
    UART_write(mec10_uart,mec10_modbus_command, sizeof(mec10_modbus_command));
    delay(3000);
    set_mec10_serial_flag(SERIAL_READ);
    delay(3000);
    UART_read(mec10_uart,mec10_res_buffer,11);
    res = (int16_t)(((uint16_t)mec10_res_buffer[3])<<8|mec10_res_buffer[4]);
    mec10_temp_value = res/100.0;
    res = (int16_t)(((uint16_t)mec10_res_buffer[5])<<8|mec10_res_buffer[6]);
    mec10_hum_value = res/100.0;
    res = (int16_t)(((uint16_t)mec10_res_buffer[7])<<8|mec10_res_buffer[8]);
    mec10_ec_value = res;

}

float get_mec10_temperature(){
    return mec10_temp_value;
}
char* get_mec10_temp_key(){
    return mec10_temp_key;
}
float get_mec10_humidity(){
    return mec10_hum_value;
}
char* get_mec10_hum_key(){
    return mec10_hum_key;
}
float get_mec10_ec(){
    return mec10_ec_value;
}
char* get_mec10_ec_key(){
    return mec10_ec_key;
}
