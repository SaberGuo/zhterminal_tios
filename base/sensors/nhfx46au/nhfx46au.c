/*
 * nhfx46au.c
 *
 *  Created on: 2017Äê11ÔÂ18ÈÕ
 *      Author: saber
 */
#include "nhfx46au.h"

#define NHFX46AU_SAMPLE_COUNT 5
#define NHFX46AU_MAX_VOL 5000000.0
#define NHFX46AU_MIN_VALUE 0.0
#define NHFX46AU_MAX_VALUE 360.0
uint16_t nhfx46au_adcValue;
uint32_t nhfx46au_adcValueMicroVolt;
float nhfx46au_value;
ADC_Handle   nhfx46au_adc;
ADC_Params   nhfx46au_params;

char nhfx46au_key[] = "Dm";

void nhfx46au_init(void){
    ADC_Params_init(&nhfx46au_params);
    nhfx46au_adc = ADC_open(Board_ADC6, &nhfx46au_params);
}

float get_nhfx46au_value(void){
    uint8_t i = 0;
    int_fast16_t res;
    nhfx46au_value = 0;

    for (; i < NHFX46AU_SAMPLE_COUNT; i++) {
        res = ADC_convert(nhfx46au_adc, &nhfx46au_adcValue);

        if (res == ADC_STATUS_SUCCESS) {

            nhfx46au_adcValueMicroVolt = ADC_convertRawToMicroVolts(nhfx46au_adc, nhfx46au_adcValue);
            LOG_MSG("ADC channel 1 convert result (%d): %d uV\n", i, nhfx46au_adcValueMicroVolt);
            nhfx46au_value+= nhfx46au_adcValueMicroVolt;
        }
        else {

            LOG_MSG("ADC channel 1 convert failed (%d)\n", i);
        }
    }
    ADC_close(nhfx46au_adc);
    nhfx46au_value = nhfx46au_value/NHFX46AU_SAMPLE_COUNT;
    nhfx46au_value = nhfx46au_value*2.0/NHFX46AU_MAX_VOL*(NHFX46AU_MAX_VALUE-NHFX46AU_MIN_VALUE);
    return nhfx46au_value;

}

char *get_nhfx46au_key(void){
    return nhfx46au_key;
}


