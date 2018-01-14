/*
 * camera.c
 *
 *  Created on: 2017Äê10ÔÂ25ÈÕ
 *      Author: saber
 */
#include <stdio.h>
#include "camera.h"

#include "base/rt_clock/rt_clock.h"
#include "base/fatsd/fatsd.h"

#include "./self_cam/self_cam.h"


char camera_buffer[MAX_BUFFER];
char img_file_name[100];

void camera_init(){
    time_t sec = get_seconds();
    sprintf(img_file_name, "%s%u%s",image_file_pre,sec/100,image_file_bre);
	self_cam_init(img_file_name);
}

uint8_t camera_capture(){
    memset(camera_buffer, 0, MAX_BUFFER);
	return self_cam_capture(camera_buffer);
}

void camera_close(){
    self_cam_close();
}

char *get_img_name(){
    return img_file_name;
}

int32_t get_img_size(){
    return get_self_image_size();
}
