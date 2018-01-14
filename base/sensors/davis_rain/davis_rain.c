/*
 * davis_rain.c
 *
 *  Created on: 2017Äê10ÔÂ24ÈÕ
 *      Author: saber
 */

#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>

#include "davis_rain.h"
#define RAIN_UINT 0.2
static uint32_t rainfall_value = 0;
char davis_rain_key[] = "rainfall";

void di_fxn(uint_least8_t index){
	rainfall_value++;
}

void davis_rain_init(){
	GPIO_write(Board_GPIO_DI_POWER, Board_GPIO_HIGH);
	GPIO_setCallback(Board_GPIO_DI, di_fxn);
}

void clear_davis_rain(void){
    rainfall_value = 0;
}

float get_davis_rain(void){
    return rainfall_value*RAIN_UINT;
}

char *get_davis_rain_key(void){
    return davis_rain_key;
}
