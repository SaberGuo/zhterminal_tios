/*
 * config.h
 *
 *  Created on: 2017Äê10ÔÂ23ÈÕ
 *      Author: saber
 */

#ifndef BASE_CONF_PARSER_CONF_PARSER_H_
#define BASE_CONF_PARSER_CONF_PARSER_H_

#include "Board.h"

typedef struct{
    uint32_t device_id;
    uint32_t device_config_id;
    char* img_capture_invl;
    char* data_capture_invl;
    char* data_upload_invl;
}s_config;

extern uint8_t parse_config(const char* filename);
extern uint8_t parse_crontab(char* invl,
                             uint8_t* minutes, uint8_t* minutes_num,
                             uint8_t* hours, uint8_t* hours_num );

extern char *get_data_upload_invl();
extern char *get_img_capture_invl();
extern char *get_data_capture_invl();
extern uint32_t get_device_id();
extern uint32_t get_device_config_id();
#endif /* BASE_CONF_PARSER_CONF_PARSER_H_ */
