/*
 * nhzd10.c
 *
 *  Created on: 2017Äê11ÔÂ17ÈÕ
 *      Author: saber
 */

#include "nhzd10.h"
#include "../adc/adc.h"
#define NHZD10_REF_VOLT 2.0
#define NHZD10_MIN_VALUE 0.0
#define NHZD10_MAX_VALUE 200000.0

ADC_Params*   nhzd10_params;
ADC_Handle nhzd10_handle;

uint8_t nhzd10_open(uint8_t num){

    return adc_open(num, nhzd10_params);

}
uint8_t nhzd10_close(uint8_t num){
    adc_close(num);
    return ZH_OK;
}
uint8_t nhzd10_process(uint8_t num){
    /*nothing*/
    return ZH_OK;
}
float nhzd10_get_data(uint8_t num, uint8_t key_num){
    adc_get_value(num);
    return adcValues[num]/NHZD10_REF_VOLT*(NHZD10_MAX_VALUE-NHZD10_MIN_VALUE)+NHZD10_MIN_VALUE;
}


