/*
 * davis_rain.h
 *
 *  Created on: 2017Äê10ÔÂ24ÈÕ
 *      Author: saber
 */

#ifndef BASE_SENSORS_DAVIS_RAIN_DAVIS_RAIN_H_
#define BASE_SENSORS_DAVIS_RAIN_DAVIS_RAIN_H_

#include "Board.h"

extern void davis_rain_init(void);

extern float get_davis_rain(void);

extern char *get_davis_rain_key(void);
#endif /* BASE_SENSORS_DAVIS_RAIN_DAVIS_RAIN_H_ */
