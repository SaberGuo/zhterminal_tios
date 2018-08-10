/*
 * th10s.h
 *
 *  Created on: 2017Äê10ÔÂ24ÈÕ
 *      Author: saber
 */

#ifndef BASE_SENSORS_TH10S_TH10S_H_
#define BASE_SENSORS_TH10S_TH10S_H_

#include "Board.h"

extern uint8_t th10s_open(uint8_t num);
extern uint8_t th10s_close(uint8_t num);
extern uint8_t th10s_process(uint8_t num);
extern float th10s_get_data(uint8_t num, uint8_t key_num);


#endif /* BASE_SENSORS_TH10S_TH10S_H_ */
