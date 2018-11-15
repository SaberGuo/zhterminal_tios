/*
 * diams.h
 *
 *  Created on: 2018Äê9ÔÂ19ÈÕ
 *      Author: saber
 */

#ifndef BASE_SENSORS_DIAMS_DIAMS_H_
#define BASE_SENSORS_DIAMS_DIAMS_H_

#include "Board.h"


extern uint8_t diams_open(uint8_t num);
extern uint8_t diams_close(uint8_t num);
extern uint8_t diams_process(uint8_t num);
extern float diams_get_data(uint8_t num, uint8_t key_num);

#endif /* BASE_SENSORS_DIAMS_DIAMS_H_ */
