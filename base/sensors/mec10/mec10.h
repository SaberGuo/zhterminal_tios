/*
 * mec10.h
 *
 *  Created on: 2017Äê11ÔÂ17ÈÕ
 *      Author: saber
 */

#ifndef BASE_SENSORS_MEC10_MEC10_H_
#define BASE_SENSORS_MEC10_MEC10_H_

#include "Board.h"

extern uint8_t mec10_init();
extern void mec10_close();

extern void mec10_process();
extern float get_mec10_temperature();
extern char* get_mec10_temp_key();
extern float get_mec10_humidity();
extern char* get_mec10_hum_key();
extern float get_mec10_ec();/*electrical conductivity*/
extern char* get_mec10_ec_key();

#endif /* BASE_SENSORS_MEC10_MEC10_H_ */
