/*
 * nh122.h
 *
 *  Created on: 2017Äê11ÔÂ17ÈÕ
 *      Author: saber
 */

#ifndef BASE_SENSORS_NH122_NH122_H_
#define BASE_SENSORS_NH122_NH122_H_
#include "Board.h"

extern void nh122_init();
extern uint8_t nh122_open();
extern void nh122_close();

extern void nh122_process();
extern float get_nh122_temperature();
extern char* get_nh122_temp_key();
extern float get_nh122_humidity();
extern char* get_nh122_hum_key();
extern float get_nh122_bar();
extern char* get_nh122_bar_key();

#endif /* BASE_SENSORS_NH122_NH122_H_ */
