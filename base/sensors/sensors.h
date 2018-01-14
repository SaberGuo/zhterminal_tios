/*
 * sensors.h
 *
 *  Created on: 2017Äê10ÔÂ23ÈÕ
 *      Author: saber
 */

#ifndef BASE_SENSORS_SENSORS_H_
#define BASE_SENSORS_SENSORS_H_

#include "Board.h"

extern char sensor_data_buffer[MAX_SENSOR_DATA][SENSOR_BUFFER_SIZE];
extern void init_sensors(void);
extern void open_sensors(void);
extern void col_sensors(void);
extern char *construct_data_str();
extern char *get_data_str();
extern void close_sensors();
extern void reset_uart_ports();
extern void read_nvs_sensors();

extern uint8_t get_sensor_num();



#endif /* BASE_SENSORS_SENSORS_H_ */
