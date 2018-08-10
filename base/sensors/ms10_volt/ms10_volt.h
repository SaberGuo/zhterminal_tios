/*
 * ms10_volt.h
 *
 *  Created on: 2018Äê3ÔÂ15ÈÕ
 *      Author: saber
 */

#ifndef BASE_SENSORS_MS10_VOLT_MS10_VOLT_H_
#define BASE_SENSORS_MS10_VOLT_MS10_VOLT_H_

#include "Board.h"


extern uint8_t ms10_volt_temp_open(uint8_t num);
extern uint8_t ms10_volt_temp_close(uint8_t num);
extern uint8_t ms10_volt_temp_process(uint8_t num);
extern float ms10_volt_temp_get_data(uint8_t num, uint8_t key_num);


extern uint8_t ms10_volt_hum_open(uint8_t num);
extern uint8_t ms10_volt_hum_close(uint8_t num);
extern uint8_t ms10_volt_hum_process(uint8_t num);
extern float ms10_volt_hum_get_data(uint8_t num, uint8_t key_num);



#endif /* BASE_SENSORS_MS10_VOLT_MS10_VOLT_H_ */
