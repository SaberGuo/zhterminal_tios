/*
 * nhzd10.h
 *
 *  Created on: 2017Äê11ÔÂ17ÈÕ
 *      Author: saber
 */

#ifndef BASE_SENSORS_NHZD10_NHZD10_H_
#define BASE_SENSORS_NHZD10_NHZD10_H_

#include "Board.h"

extern void nhzd10_init(void);

extern float get_nhzd10_value(void);

extern char *get_nhzd10_key(void);

#endif /* BASE_SENSORS_NHZD10_NHZD10_H_ */
