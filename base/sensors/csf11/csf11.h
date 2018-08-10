/*
 * csf11.h
 *
 *  Created on: 2018��3��30��
 *      Author: saber
 */

#ifndef BASE_SENSORS_CSF11_CSF11_H_
#define BASE_SENSORS_CSF11_CSF11_H_

#include "Board.h"

extern uint8_t csf11_open(uint8_t num);
extern uint8_t csf11_close(uint8_t num);
extern uint8_t csf11_process(uint8_t num);
extern float csf11_get_data(uint8_t num, uint8_t key_num);



#endif /* BASE_SENSORS_CSF11_CSF11_H_ */
