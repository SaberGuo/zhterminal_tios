/*
 * di.c
 *
 *  Created on: 2018Äê1ÔÂ16ÈÕ
 *      Author: saber
 */
#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include "di.h"

uint8_t di_power_ports[1]={Board_GPIO_DI_POWER};
uint8_t di_ports[1] = {Board_GPIO_DI};
uint32_t di_value[2];
void di_fxn(uint_least8_t index){
    di_value[index]++;
}

void di_init(uint8_t num){
    GPIO_write(di_power_ports[num], Board_GPIO_LOW);
    GPIO_setCallback(di_ports[num], di_fxn);
    GPIO_enableInt(di_ports[num]);
}

uint32_t get_di_value(uint8_t num){
    return di_value[num];
}

void clear_di_value(uint8_t num){
    di_value[num] = 0;
}

