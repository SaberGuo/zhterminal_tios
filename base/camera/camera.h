/*
 * camera.h
 *
 *  Created on: 2017Äê10ÔÂ25ÈÕ
 *      Author: saber
 */

#ifndef BASE_CAMERA_CAMERA_H_
#define BASE_CAMERA_CAMERA_H_

#include "Board.h"

extern void camera_init();

extern uint8_t camera_capture();

extern void camera_close();

extern char *get_img_name();

extern int32_t get_img_size();

#endif /* BASE_CAMERA_CAMERA_H_ */
