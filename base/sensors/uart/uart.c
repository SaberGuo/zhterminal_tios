/*
 * uart.c
 *
 *  Created on: 2018Äê1ÔÂ16ÈÕ
 *      Author: saber
 */
#include "uart.h"
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include "../sensors_comm.h"

#define MAX_UART_LEN 80

UART_Handle uart_handler[MAX_UART_NUM]={NULL, NULL};
uint8_t uart_nums[MAX_UART_NUM]={Board_UART_485_0,Board_UART_485_1};
uint8_t uart_flags[MAX_UART_NUM]={Board_GPIO_485_0_RE, Board_GPIO_485_1_RE};
uint8_t uart_buffer[MAX_UART_NUM][MAX_UART_LEN];

uint8_t serial_port_open(uint8_t num,UART_Params* param){
    if(uart_handler[num] == NULL){
        uart_handler[num] = UART_open(uart_nums[num], param);
    }
    if (uart_handler[num] == NULL) {
        LOG_MSG("th10s uart open fail!\n");
        return ZH_FAIL;
    }
    return ZH_OK;
}


uint8_t set_serial_flag(uint8_t num, uint8_t flag){
    if(num>=MAX_UART_NUM){
        return ZH_FAIL;
    }
    if(flag == SERIAL_READ){
        GPIO_write(uart_flags[num],Board_GPIO_LOW);
    }
    if(flag == SERIAL_WRITE){
        GPIO_write(uart_flags[num],Board_GPIO_HIGH);
    }
    return ZH_OK;
}
uint8_t get_modbus_nocrc_datas(uint8_t num,
                      char* command, uint8_t command_size,
                      uint8_t read_size,
                      uint16_t* data_buffer ,uint8_t data_size){
    uint8_t i = 0;
    int32_t write_num = 0;
    if(num>=MAX_UART_NUM){
        LOG_MSG("get_modbus_datas:num is larger %d",num);
        return ZH_FAIL;
    }
    memset(uart_buffer[num], 0, MAX_UART_LEN);
    if(set_serial_flag(num, SERIAL_WRITE) == ZH_FAIL){
        LOG_MSG("uart flag set error for num is larger %d",num);
        return ZH_FAIL;
    }
    write_num = UART_write(uart_handler[num],command, command_size);
    if(write_num != command_size){
        LOG_MSG("uart write error!");
        return ZH_FAIL;
    }
    delay(2000);
    if(set_serial_flag(num, SERIAL_READ) == ZH_FAIL){
        LOG_MSG("uart flag set error for num is larger %d",num);
        return ZH_FAIL;
    }
    delay(2000);
    uint8_t count = 0;
    for(;count<20;count++){
        UART_read(uart_handler[num],uart_buffer[num],1);

        if(uart_buffer[num][0] == command[0]){
            break;
        }
    }
    if(count == 20){
        LOG_MSG("received error!");
        return ZH_FAIL;
    }
    UART_read(uart_handler[num],&(uart_buffer[num][1]),read_size-1);
    /*if(crc_16(uart_buffer[num],read_size) == 1){
        LOG_MSG("crc error!");
        return ZH_FAIL;
    }*/
    for(i = 0;i<data_size;i++){
        data_buffer[i] = (((uint16_t)uart_buffer[num][3+i*2])<<8|uart_buffer[num][4+i*2]);
    }
    return ZH_OK;
}
uint8_t send_modbus_command(uint8_t num, char* command, uint8_t command_size){
    int32_t write_num = 0;
    if(num>=MAX_UART_NUM){
        LOG_MSG("get_modbus_datas:num is larger %d",num);
        return ZH_FAIL;
    }
    if(set_serial_flag(num, SERIAL_WRITE) == ZH_FAIL){
        LOG_MSG("uart flag set error for num is larger %d",num);
        return ZH_FAIL;
    }
    write_num = UART_write(uart_handler[num],command, command_size);
    if(write_num != command_size){
        LOG_MSG("uart write error!");
        return ZH_FAIL;
    }
    delay(10000);
    return ZH_OK;
}

uint8_t get_modbus_datas(uint8_t num,
                      char* command, uint8_t command_size,
                      uint8_t read_size,
                      uint16_t* data_buffer ,uint8_t data_size){
    uint8_t i = 0;
    int32_t write_num = 0;
    if(num>=MAX_UART_NUM){
        LOG_MSG("get_modbus_datas:num is larger %d",num);
        return ZH_FAIL;
    }
    memset(uart_buffer[num], 0, MAX_UART_LEN);
    if(set_serial_flag(num, SERIAL_WRITE) == ZH_FAIL){
        LOG_MSG("uart flag set error for num is larger %d",num);
        return ZH_FAIL;
    }
    write_num = UART_write(uart_handler[num],command, command_size);
    if(write_num != command_size){
        LOG_MSG("uart write error!");
        return ZH_FAIL;
    }
    delay(3000);
    if(set_serial_flag(num, SERIAL_READ) == ZH_FAIL){
        LOG_MSG("uart flag set error for num is larger %d",num);
        return ZH_FAIL;
    }
    delay(3000);
    uint8_t count = 0;
    for(;count<20;count++){
        UART_read(uart_handler[num],uart_buffer[num],1);

        if(uart_buffer[num][0] == command[0]){
            break;
        }
    }
    if(count == 20){
        LOG_MSG("received error!");
        return ZH_FAIL;
    }
    UART_read(uart_handler[num],&(uart_buffer[num][1]),read_size-1);
    if(crc_16(uart_buffer[num],read_size) == 1){
        LOG_MSG("crc error!");
        return ZH_FAIL;
    }
    for(i = 0;i<data_size;i++){
        data_buffer[i] = (((uint16_t)uart_buffer[num][3+i*2])<<8|uart_buffer[num][4+i*2]);
    }
    return ZH_OK;
}

uint8_t serial_port_close(uint8_t num){
    if(num>=MAX_UART_NUM){
        LOG_MSG("serial_port_close:num is larger %d",num);
        return ZH_FAIL;
    }
    if(uart_handler[num]!=NULL){
        UART_close(uart_handler[num]);
    }
    uart_handler[num] = NULL;
    return ZH_OK;
}
