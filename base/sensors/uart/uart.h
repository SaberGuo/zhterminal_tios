/*
 * uart.h
 *
 *  Created on: 2018Äê1ÔÂ16ÈÕ
 *      Author: saber
 */

#ifndef BASE_SENSORS_UART_UART_H_
#define BASE_SENSORS_UART_UART_H_
#include "Board.h"


extern uint8_t serial_port_open(uint8_t num,UART_Params* param);

extern uint8_t serial_port_close(uint8_t num);

extern uint8_t get_modbus_nocrc_datas(uint8_t num,
                                char* command, uint8_t command_size,
                                uint8_t read_size,
                                uint16_t* data_buffer ,uint8_t data_size);
extern uint8_t get_modbus_datas(uint8_t num,
                                char* command, uint8_t command_size,
                                uint8_t read_size,
                                uint16_t* data_buffer ,uint8_t data_size);
#endif /* BASE_SENSORS_UART_UART_H_ */
