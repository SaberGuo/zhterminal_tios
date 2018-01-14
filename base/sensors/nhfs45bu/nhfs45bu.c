/*
 * nhfs45bu.c
 *
 *  Created on: 2017Äê11ÔÂ18ÈÕ
 *      Author: saber
 */
#include "nhfs45bu.h"

#define NHFS45BU_SAMPLE_COUNT 5
#define NHFS45BU_MAX_VOL 5000000.0
#define NHFS45BU_MIN_VALUE 0.0
#define NHFS45BU_MAX_VALUE 60.0
uint16_t nhfs45bu_adcValue;
uint32_t nhfs45bu_adcValueMicroVolt;
float nhfs45bu_value;
ADC_Handle   nhfs45bu_adc;
ADC_Params   nhfs45bu_params;

char nhfs45bu_key[] = "Sm";

void nhfs45bu_init(void){
    ADC_Params_init(&nhfs45bu_params);
    nhfs45bu_adc = ADC_open(Board_ADC5, &nhfs45bu_params);
}

float get_nhfs45bu_value(void){
    uint8_t i = 0;
    int_fast16_t res;
    nhfs45bu_value = 0;

    for (; i < NHFS45BU_SAMPLE_COUNT; i++) {
        res = ADC_convert(nhfs45bu_adc, &nhfs45bu_adcValue);

        if (res == ADC_STATUS_SUCCESS) {

            nhfs45bu_adcValueMicroVolt = ADC_convertRawToMicroVolts(nhfs45bu_adc, nhfs45bu_adcValue);
            LOG_MSG("ADC channel 1 convert result (%d): %d uV\n", i, nhfs45bu_adcValueMicroVolt);
            nhfs45bu_value+= nhfs45bu_adcValueMicroVolt;
        }
        else {

            LOG_MSG("ADC channel 1 convert failed (%d)\n", i);
        }
    }
    ADC_close(nhfs45bu_adc);

    nhfs45bu_value = nhfs45bu_value/NHFS45BU_SAMPLE_COUNT;
    nhfs45bu_value = nhfs45bu_value*2.0/NHFS45BU_MAX_VOL*(NHFS45BU_MAX_VALUE-NHFS45BU_MIN_VALUE);
    return nhfs45bu_value;

}

char *get_nhfs45bu_key(void){
    return nhfs45bu_key;
}

