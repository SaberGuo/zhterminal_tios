/*
 * protocol.h
 *
 *  Created on: 2017Äê10ÔÂ24ÈÕ
 *      Author: saber
 */

#ifndef BASE_PROTOCOL_PROTOCOL_H_
#define BASE_PROTOCOL_PROTOCOL_H_

#include "Board.h"
#include <third_party/fatfs/integer.h>
/*about upload data*/
extern uint8_t upload_data_start();
extern uint8_t upload_data_end();
extern uint8_t upload_data_str(char *data_str);
extern uint8_t upload_data(char * key, float value);
/*about update time*/
extern uint8_t update_time();
/*about collecte config*/
extern uint8_t collecte_config(char * config_content);
extern void construct_protocol_data(char *data_str, char *key, float value);
/*about upload image*/
void upload_image(char *key,uint8_t channel, char *file_name, DWORD file_size, uint32_t ts);

#endif /* BASE_PROTOCOL_PROTOCOL_H_ */
