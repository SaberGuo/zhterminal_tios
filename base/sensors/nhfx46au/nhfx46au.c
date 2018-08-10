/*
 * nhfx46au.c
 *
 *  Created on: 2017Äê11ÔÂ18ÈÕ
 *      Author: saber
 */
#include "nhfx46au.h"
#include "../adc/adc.h"
#define NHFX46AU_REF_VOLT 5.0
#define NHFX46AU_MIN_VALUE 0.0
#define NHFX46AU_MAX_VALUE 360.0

ADC_Params*   nhfx46au_params;

uint8_t nhfx46au_open(uint8_t num){
    return adc_open(num, nhfx46au_params);
}
uint8_t nhfx46au_close(uint8_t num){
    adc_close(num);
    return ZH_OK;
}
uint8_t nhfx46au_process(uint8_t num){
    /*nothing*/
    return ZH_OK;
}
float nhfx46au_get_data(uint8_t num, uint8_t key_num){
    adc_get_value(num);
    return adcValues[num]/NHFX46AU_REF_VOLT*(NHFX46AU_MAX_VALUE-NHFX46AU_MIN_VALUE)+NHFX46AU_MIN_VALUE;
}



