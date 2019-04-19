/*
 * nhgh09bu.h
 *
 *  Created on: 2019Äê3ÔÂ4ÈÕ
 *      Author: Administrator
 */

#ifndef BASE_SENSORS_NHGH09BU_NHGH09BU_H_
#define BASE_SENSORS_NHGH09BU_NHGH09BU_H_

#include "Board.h"

extern uint8_t nhgh09bu_open(uint8_t num);
extern uint8_t nhgh09bu_close(uint8_t num);
extern uint8_t nhgh09bu_process(uint8_t num);
extern float nhgh09bu_get_data(uint8_t num, uint8_t key_num);




#endif /* BASE_SENSORS_NHGH09BU_NHGH09BU_H_ */
