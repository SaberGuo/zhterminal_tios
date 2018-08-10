/*
 * zwinsoft.h
 *
 *  Created on: 2018Äê7ÔÂ18ÈÕ
 *      Author: saber
 */

#ifndef BASE_SENSORS_ZWINSOFT_ZWINSOFT_H_
#define BASE_SENSORS_ZWINSOFT_ZWINSOFT_H_

#include "Board.h"

extern uint8_t zwinsoft_open(uint8_t num);
extern uint8_t zwinsoft_close(uint8_t num);
extern uint8_t zwinsoft_process(uint8_t num);
extern float zwinsoft_get_data(uint8_t num, uint8_t key_num);

#endif /* BASE_SENSORS_ZWINSOFT_ZWINSOFT_H_ */
