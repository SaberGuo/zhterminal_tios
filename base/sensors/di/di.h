/*
 * di.h
 *
 *  Created on: 2018Äê1ÔÂ16ÈÕ
 *      Author: saber
 */

#ifndef BASE_SENSORS_DI_DI_H_
#define BASE_SENSORS_DI_DI_H_
#include "Board.h"

extern void di_init(uint8_t num);

extern uint32_t get_di_value(uint8_t num);

extern void clear_di_value(uint8_t num);
#endif /* BASE_SENSORS_DI_DI_H_ */
