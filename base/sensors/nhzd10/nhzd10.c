/*
 * nhzd10.c
 *
 *  Created on: 2017Äê11ÔÂ17ÈÕ
 *      Author: saber
 */

#include "nhzd10.h"
#define NHZD10_SAMPLE_COUNT 5
#define NHZD10_MAX_VOL 2000.0
#define NHZD10_MIN_VALUE 0.0
#define NHZD10_MAX_VALUE 200000.0
uint16_t nhzd10_adcValue;
uint32_t nhzd10_adcValueMicroVolt;
float nhzd10_value;
ADC_Handle   nhzd10_adc;
ADC_Params   nhzd10_params;

char nhzd10_key[] = "solar";

void nhzd10_init(void){
    ADC_Params_init(&nhzd10_params);
    nhzd10_adc = ADC_open(Board_ADC7, &nhzd10_params);

}

extern float get_nhzd10_value(void){
    uint8_t i = 0;
    int_fast16_t res;
    nhzd10_value = 0;

    for (; i < NHZD10_SAMPLE_COUNT; i++) {
        res = ADC_convert(nhzd10_adc, &nhzd10_adcValue);

        if (res == ADC_STATUS_SUCCESS) {

            nhzd10_adcValueMicroVolt = ADC_convertRawToMicroVolts(nhzd10_adc, nhzd10_adcValue);
            LOG_MSG("ADC channel 1 convert result (%d): %d uV\n", i, nhzd10_adcValueMicroVolt);
            nhzd10_value+= nhzd10_adcValueMicroVolt;
        }
        else {

            LOG_MSG("ADC channel 1 convert failed (%d)\n", i);
        }
    }
    ADC_close(nhzd10_adc);
    nhzd10_value = nhzd10_value/NHZD10_SAMPLE_COUNT;
    nhzd10_value = nhzd10_value*2.0/NHZD10_MAX_VOL*(NHZD10_MAX_VALUE-NHZD10_MIN_VALUE);
    return nhzd10_value;

}

extern char *get_nhzd10_key(void){
    return nhzd10_key;
}
