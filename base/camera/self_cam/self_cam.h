/*
 * self_cam.h
 *
 *  Created on: 2017Äê10ÔÂ24ÈÕ
 *      Author: saber
 */

#ifndef BASE_CAMERA_SELF_CAM_H_
#define BASE_CAMERA_SELF_CAM_H_

#include "Board.h"

extern void self_cam_init(char *image_file_name);
extern void self_cam_close();
extern uint8_t self_cam_capture(char *buffer);
extern int32_t get_self_image_size();
#endif /* BASE_CAMERA_SELF_CAM_H_ */
