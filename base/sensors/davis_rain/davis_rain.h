/*
 * davis_rain.h
 *
 *  Created on: 2017Äê10ÔÂ24ÈÕ
 *      Author: saber
 */

#ifndef BASE_SENSORS_DAVIS_RAIN_DAVIS_RAIN_H_
#define BASE_SENSORS_DAVIS_RAIN_DAVIS_RAIN_H_

#include "Board.h"

extern uint8_t davis_open(uint8_t num);
extern uint8_t davis_close(uint8_t num);
extern uint8_t davis_process(uint8_t num);
extern float davis_get_data(uint8_t num, uint8_t key_num);

#endif /* BASE_SENSORS_DAVIS_RAIN_DAVIS_RAIN_H_ */
