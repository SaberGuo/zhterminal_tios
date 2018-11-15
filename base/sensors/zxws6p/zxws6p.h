/*
 * zxws6p.h
 *
 *  Created on: 2018Äê10ÔÂ12ÈÕ
 *      Author: saber
 */

#ifndef BASE_SENSORS_ZXWS6P_ZXWS6P_H_
#define BASE_SENSORS_ZXWS6P_ZXWS6P_H_

#include "Board.h"



extern uint8_t zxws6p_open(uint8_t num);
extern uint8_t zxws6p_close(uint8_t num);
extern uint8_t zxws6p_process(uint8_t num);
extern float zxws6p_get_data(uint8_t num, uint8_t key_num);



#endif /* BASE_SENSORS_ZXWS6P_ZXWS6P_H_ */
