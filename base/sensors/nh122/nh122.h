/*
 * nh122.h
 *
 *  Created on: 2017Äê11ÔÂ17ÈÕ
 *      Author: saber
 */

#ifndef BASE_SENSORS_NH122_NH122_H_
#define BASE_SENSORS_NH122_NH122_H_
#include "Board.h"

extern uint8_t nh122_open(uint8_t num);
extern uint8_t nh122_close(uint8_t num);
extern uint8_t nh122_process(uint8_t num);
extern float nh122_get_data(uint8_t num, uint8_t key_num);



#endif /* BASE_SENSORS_NH122_NH122_H_ */
