/*
 * adc345.h
 *
 *  Created on: 2019��1��3��
 *      Author: saber
 */

#ifndef BASE_SENSORS_ADC345_ADC345_H_
#define BASE_SENSORS_ADC345_ADC345_H_

#include "Board.h"

extern uint8_t adc345_open(uint8_t num);
extern uint8_t adc345_close(uint8_t num);
extern uint8_t adc345_process(uint8_t num);
extern float adc345_get_data(uint8_t num, uint8_t key_num);



#endif /* BASE_SENSORS_ADC345_ADC345_H_ */
