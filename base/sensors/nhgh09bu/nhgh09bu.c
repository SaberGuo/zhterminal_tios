/*
 * nhgh09bu.c
 *
 *  Created on: 2019Äê3ÔÂ4ÈÕ
 *      Author: Administrator
 */

#include "nhgh09bu.h"
#include "../adc/adc.h"
#define NHGH09BU_REF_VOLT 2.0
#define NHGH09BU_MIN_VALUE 0.0
#define NHGH09BU_MAX_VALUE 2000.0

ADC_Params*   nhgh09bu_params;
ADC_Handle nhgh09bu_handle;

uint8_t nhgh09bu_open(uint8_t num){

    return adc_open(num, nhgh09bu_params);

}
uint8_t nhgh09bu_close(uint8_t num){
    adc_close(num);
    return ZH_OK;
}
uint8_t nhgh09bu_process(uint8_t num){
    /*nothing*/
    return ZH_OK;
}
float nhgh09bu_get_data(uint8_t num, uint8_t key_num){
    adc_get_value(num);
    return adcValues[num]/NHGH09BU_REF_VOLT*(NHGH09BU_MAX_VALUE-NHGH09BU_MIN_VALUE)+NHGH09BU_MIN_VALUE;
}



