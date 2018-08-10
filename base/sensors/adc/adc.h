/*
 * adc.h
 *
 *  Created on: 2018Äê1ÔÂ16ÈÕ
 *      Author: saber
 */

#ifndef BASE_SENSORS_ADC_ADC_H_
#define BASE_SENSORS_ADC_ADC_H_
#include "Board.h"

extern float adcValues[MAX_AD_NUM];
extern uint8_t adc_open(uint8_t num, ADC_Params* param);
extern void adc_close(uint8_t num);

extern float adc_get_data(uint8_t num);

#endif /* BASE_SENSORS_ADC_ADC_H_ */
