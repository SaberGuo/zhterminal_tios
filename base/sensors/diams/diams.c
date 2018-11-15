/*
 * diams.c
 *
 *  Created on: 2018Äê9ÔÂ19ÈÕ
 *      Author: saber
 */
#include "diams.h"
#include "../adc/adc.h"

#define DIAMS_REF_VOLT 2.0

ADC_Params*   diams_params;

uint8_t diams_open(uint8_t num){
    return adc_open(num, diams_params);
}
uint8_t diams_close(uint8_t num){
    adc_close(num);
    return ZH_OK;
}
uint8_t diams_process(uint8_t num){
    /*nothing*/
    return ZH_OK;
}
float diams_get_data(uint8_t num, uint8_t key_num){
    return adc_get_value(num);
}
