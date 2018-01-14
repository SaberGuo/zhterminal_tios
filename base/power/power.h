/*
 * power.h
 *
 *  Created on: 2017Äê10ÔÂ25ÈÕ
 *      Author: saber
 */

#ifndef BASE_POWER_POWER_H_
#define BASE_POWER_POWER_H_

#include "Board.h"

#define RELAY_K_GSM 1
#define RELAY_K_CAMERA 2
#define RELAY_K_SENSOR 4
#define RELAY_K_IO 8

#define ENA_DC5V 	1
#define ENA_DC33V	2



extern void set_relay(uint8_t relay_no);
extern void reset_relay(uint8_t relay_no);
extern void power_enable(uint8_t power_no);
extern void power_disable(uint8_t power_no);

#endif /* BASE_POWER_POWER_H_ */
