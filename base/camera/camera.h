/*
 * camera.h
 *
 *  Created on: 2017Äê10ÔÂ25ÈÕ
 *      Author: saber
 */

#ifndef BASE_CAMERA_CAMERA_H_
#define BASE_CAMERA_CAMERA_H_

#include "Board.h"

#define CAMERA_MAX_NUM 2

#define CAMERA_BUFFER_NUM 4

typedef struct image_info{
    char filename[40];
    uint32_t size;
    uint32_t seconds;
}_image_info;


typedef struct camera_config{
    uint8_t (*camera_open)(uint8_t);/*port_num start from 0*/
    uint8_t (*camera_process)(uint8_t, char*, char *, uint32_t*);/*file_name*/
    uint8_t (*camera_close)(uint8_t);
}_camera_config;

typedef enum camera_Type{
    CAMERA_HISI = 0,//hisilicon
    CAMERA_TYPE_COUNT
}camera_Type_Enum;

extern const char camera_type_strs[CAMERA_TYPE_COUNT][20];

extern uint8_t camera_process();
extern uint8_t camera_open();
extern uint8_t camera_close();
extern uint8_t get_camera_num();

extern char* get_camera_file_name(uint8_t num);
extern uint32_t get_camera_file_size(uint8_t num);
extern char* get_camera_key(uint8_t num);


#endif /* BASE_CAMERA_CAMERA_H_ */
