/*
 * ad345.c
 *
 *  Created on: 2019Äê1ÔÂ3ÈÕ
 *      Author: saber
 */

#include <base/sensors/adc345/adc345.h>
#include "../adc/adc.h"
#define ADC345_REF_VOLT 5.0
#define ADC345_MIN_VALUE 0.0
#define ADC345_MAX_VALUE 360.0

ADC_Params*   adc345_params;

uint8_t adc345_open(uint8_t num){
    return adc_open(num, adc345_params);
}
uint8_t adc345_close(uint8_t num){
    adc_close(num);
    return ZH_OK;
}
uint8_t adc345_process(uint8_t num){
    /*nothing*/
    return ZH_OK;
}
float adc345_get_data(uint8_t num, uint8_t key_num){
    adc_get_value(num);
    return adcValues[num]/ADC345_REF_VOLT*(ADC345_MAX_VALUE-ADC345_MIN_VALUE)+ADC345_MIN_VALUE;
}
