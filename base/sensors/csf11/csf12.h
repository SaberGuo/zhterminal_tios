/*
 * csf11.h
 *
 *  Created on: 2018Äê3ÔÂ30ÈÕ
 *      Author: saber
 */

#ifndef BASE_SENSORS_CSF11L_CSF11_H_
#define BASE_SENSORS_CSF11L_CSF11_H_

#include "Board.h"

extern uint8_t csf12_open(uint8_t num);
extern uint8_t csf12_close(uint8_t num);
extern uint8_t csf12_process(uint8_t num);
extern float csf12_get_data(uint8_t num, uint8_t key_num);



#endif /* BASE_SENSORS_CSF11L_CSF11_H_ */
