/*
 * nhfs45bu.c
 *
 *  Created on: 2017Äê11ÔÂ18ÈÕ
 *      Author: saber
 */
#include "nhfs45bu.h"
#include "../adc/adc.h"
#define NHFS45BU_MIN_VALUE 0.0
#define NHFS45BU_MAX_VALUE 60.0
#define NHFS45BU_REF_VOLT 5.0

ADC_Params*   nhfs45bu_params;

uint8_t nhfs45bu_open(uint8_t num){
    return adc_open(num, nhfs45bu_params);

}

uint8_t nhfs45bu_close(uint8_t num){
    adc_close(num);
    return ZH_OK;
}

uint8_t nhfs45bu_process(uint8_t num){
    /*nothing*/
    return ZH_OK;
}

float nhfs45bu_get_data(uint8_t num, uint8_t key_num){
    //float tmp = adc_get_value(num)/NHFS45BU_REF_VOLT*(NHFS45BU_MAX_VALUE-NHFS45BU_MIN_VALUE)+NHFS45BU_MIN_VALUE;
    adc_get_value(num);
    adcValues[num] = adcValues[num]/NHFS45BU_REF_VOLT*(NHFS45BU_MAX_VALUE-NHFS45BU_MIN_VALUE)+NHFS45BU_MIN_VALUE;
    return adcValues[num];
}


