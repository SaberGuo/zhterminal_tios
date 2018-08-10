/*
 * nhfs45bu.h
 *
 *  Created on: 2017Äê11ÔÂ18ÈÕ
 *      Author: saber
 */

#ifndef BASE_SENSORS_NHFS45BU_NHFS45BU_H_
#define BASE_SENSORS_NHFS45BU_NHFS45BU_H_
#include "Board.h"

extern uint8_t nhfs45bu_open(uint8_t num);
extern uint8_t nhfs45bu_close(uint8_t num);
extern uint8_t nhfs45bu_process(uint8_t num);
extern float nhfs45bu_get_data(uint8_t num, uint8_t key_num);



#endif /* BASE_SENSORS_NHFS45BU_NHFS45BU_H_ */
