/*
 * th10s.h
 *
 *  Created on: 2017Äê10ÔÂ24ÈÕ
 *      Author: saber
 */

#ifndef BASE_SENSORS_TH10S_TH10S_H_
#define BASE_SENSORS_TH10S_TH10S_H_

#include "Board.h"

extern uint8_t th10s_init();
extern void th10s_close();

extern void th10s_process();
extern float get_temperature();

extern float get_humility();

#endif /* BASE_SENSORS_TH10S_TH10S_H_ */
