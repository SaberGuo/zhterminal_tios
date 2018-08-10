/*
 * adc.c
 *
 *  Created on: 2018Äê1ÔÂ16ÈÕ
 *      Author: saber
 */

#include "adc.h"
#define SAMPLE_COUNT 5
#define REF_VOLT (2.5)

ADC_Handle   adchandles[MAX_AD_NUM];
uint16_t adcintValues[MAX_AD_NUM];
float adcValues[MAX_AD_NUM];
/*num start from 1*/

uint8_t adc_open(uint8_t num, ADC_Params* param){
    if(param == NULL){
        param = (ADC_Params*)malloc(sizeof(ADC_Params));
        ADC_Params_init(param);
    }
    if(num>=MAX_AD_NUM){
        free(param);
        return ZH_FAIL;
    }
    adchandles[num] = ADC_open(num, param);
    if(adchandles[num] == NULL){
        return ZH_FAIL;
    }
    return ZH_OK;
}

void adc_close(uint8_t num){
    ADC_close(adchandles[num]);
}
float adc_get_value(uint8_t num){
    float adc_mVolt, res;
    uint8_t i = 0;
    adcValues[num] = 0;
    for (; i < SAMPLE_COUNT; i++) {
        res = ADC_convert(adchandles[num], &adcintValues[num]);

        if (res == ADC_STATUS_SUCCESS) {
            //adc_mVolt = ADC_convertRawToMicroVolts(adchandles[num], adcValues[num]);
            adc_mVolt = adcintValues[num]/16383.0*REF_VOLT;
            //LOG_MSG("ADC channel convert result (%d): %f uV\n", i, adc_mVolt);
            adcValues[num]+= adc_mVolt;
        }
        else {
            LOG_MSG("ADC channel convert failed (%d)\n", i);
        }
    }
    adcValues[num]/=(SAMPLE_COUNT);
    adcValues[num]*=2.0;

    return adcValues[num];
}
