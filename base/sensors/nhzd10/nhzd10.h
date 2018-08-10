/*
 * nhzd10.h
 *
 *  Created on: 2017Äê11ÔÂ17ÈÕ
 *      Author: saber
 */

#ifndef BASE_SENSORS_NHZD10_NHZD10_H_
#define BASE_SENSORS_NHZD10_NHZD10_H_

#include "Board.h"

extern uint8_t nhzd10_open(uint8_t num);
extern uint8_t nhzd10_close(uint8_t num);
extern uint8_t nhzd10_process(uint8_t num);
extern float nhzd10_get_data(uint8_t num, uint8_t key_num);


#endif /* BASE_SENSORS_NHZD10_NHZD10_H_ */
