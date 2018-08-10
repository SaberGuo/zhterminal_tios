/*
 * ms10_volt.c
 *
 *  Created on: 2018Äê3ÔÂ15ÈÕ
 *      Author: saber
 */
#include "ms10_volt.h"
#include "../adc/adc.h"

#define MS10_VOLT_TEMP_MIN_VALUE (-40.0)
#define MS10_VOLT_TEMP_MAX_VALUE 80.0

#define MS10_VOLT_HUM_MIN_VALUE 0.0
#define MS10_VOLT_HUM_MAX_VALUE 100.0

#define MS10_VOLT_REF_VOLT 2.0

ADC_Params*   ms10_volt_temp_params;
ADC_Params*   ms10_volt_hum_params;

uint8_t ms10_volt_temp_open(uint8_t num){
    return adc_open(num, ms10_volt_temp_params);
}
uint8_t ms10_volt_temp_close(uint8_t num){
    adc_close(num);
    return ZH_OK;
}
uint8_t ms10_volt_temp_process(uint8_t num){
    /*nothing*/
    return ZH_OK;
}
float ms10_volt_temp_get_data(uint8_t num, uint8_t key_num){
    adc_get_value(num);
    return adcValues[num]/MS10_VOLT_REF_VOLT*(MS10_VOLT_TEMP_MAX_VALUE-MS10_VOLT_TEMP_MIN_VALUE)+MS10_VOLT_TEMP_MIN_VALUE;
}


uint8_t ms10_volt_hum_open(uint8_t num){
    return adc_open(num, ms10_volt_hum_params);
}
uint8_t ms10_volt_hum_close(uint8_t num){
    adc_close(num);
    return ZH_OK;
}
uint8_t ms10_volt_hum_process(uint8_t num){
    /*nothing*/
    return ZH_OK;
}
float ms10_volt_hum_get_data(uint8_t num, uint8_t key_num){
    adc_get_value(num);
    return adcValues[num]/MS10_VOLT_REF_VOLT*(MS10_VOLT_HUM_MAX_VALUE-MS10_VOLT_HUM_MIN_VALUE)+MS10_VOLT_HUM_MIN_VALUE;

}
