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
#include "../di/di.h"

#include "davis_rain.h"
#define RAIN_UINT 0.2


uint8_t davis_open(uint8_t num){
    /*nothing*/
    return ZH_OK;
}
uint8_t davis_close(uint8_t num){
    clear_di_value(num);
    return ZH_OK;
}
uint8_t davis_process(uint8_t num){
    /*nothing*/
    return ZH_OK;
}
float davis_get_data(uint8_t num, uint8_t key_num){
    return get_di_value(num)*RAIN_UINT;
}

