/*
 * nhfs15bu.c
 *
 *  Created on: 2019Äê3ÔÂ4ÈÕ
 *      Author: Administrator
 */

#include "nhfs15bu.h"
#include "../adc/adc.h"
#define NHFS15BU_REF_VOLT 2.0
#define NHFS15BU_MIN_VALUE 0.0
#define NHFS15BU_MAX_VALUE 2000.0

ADC_Params*   nhfs15bu_params;
ADC_Handle nhfs15bu_handle;

uint8_t nhfs15bu_open(uint8_t num){

    return adc_open(num, nhfs15bu_params);

}
uint8_t nhfs15bu_close(uint8_t num){
    adc_close(num);
    return ZH_OK;
}
uint8_t nhfs15bu_process(uint8_t num){
    /*nothing*/
    return ZH_OK;
}
float nhfs15bu_get_data(uint8_t num, uint8_t key_num){
    adc_get_value(num);
    return adcValues[num]/NHFS15BU_REF_VOLT*(NHFS15BU_MAX_VALUE-NHFS15BU_MIN_VALUE)+NHFS15BU_MIN_VALUE;
}





