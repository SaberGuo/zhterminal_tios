/*
 * wbsensor.h
 *
 *  Created on: 2018Äê11ÔÂ15ÈÕ
 *      Author: saber
 */

#ifndef BASE_SENSORS_WBSENSOR_WBSENSOR_H_
#define BASE_SENSORS_WBSENSOR_WBSENSOR_H_

#include "Board.h"



extern uint8_t wbsensor_open(uint8_t num);
extern uint8_t wbsensor_close(uint8_t num);
extern uint8_t wbsensor_process(uint8_t num);
extern float wbsensor_get_data(uint8_t num, uint8_t key_num);




#endif /* BASE_SENSORS_WBSENSOR_WBSENSOR_H_ */
