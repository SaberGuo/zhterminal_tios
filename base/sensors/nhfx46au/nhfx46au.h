/*
 * nhfx46au.h
 *
 *  Created on: 2017Äê11ÔÂ18ÈÕ
 *      Author: saber
 */

#ifndef BASE_SENSORS_NHFX46AU_NHFX46AU_H_
#define BASE_SENSORS_NHFX46AU_NHFX46AU_H_
#include "Board.h"

extern uint8_t nhfx46au_open(uint8_t num);
extern uint8_t nhfx46au_close(uint8_t num);
extern uint8_t nhfx46au_process(uint8_t num);
extern float nhfx46au_get_data(uint8_t num, uint8_t key_num);


#endif /* BASE_SENSORS_NHFX46AU_NHFX46AU_H_ */
