/*
 * nhfs15bu.h
 *
 *  Created on: 2019Äê3ÔÂ4ÈÕ
 *      Author: Administrator
 */

#ifndef BASE_SENSORS_NHFS15BU_NHFS15BU_H_
#define BASE_SENSORS_NHFS15BU_NHFS15BU_H_


#include "Board.h"

extern uint8_t nhfs15bu_open(uint8_t num);
extern uint8_t nhfs15bu_close(uint8_t num);
extern uint8_t nhfs15bu_process(uint8_t num);
extern float nhfs15bu_get_data(uint8_t num, uint8_t key_num);





#endif /* BASE_SENSORS_NHFS15BU_NHFS15BU_H_ */
