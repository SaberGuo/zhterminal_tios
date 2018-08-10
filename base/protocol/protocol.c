/*
 * protocol.c
 *
 *  Created on: 2017Äê10ÔÂ24ÈÕ
 *      Author: saber
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <file.h>

#include "protocol.h"
#include "base/gsm/gsm.h"
#include "base/rt_clock/rt_clock.h"
#include "base/conf_parser/conf_parser.h"
#include "base/sensors/sensors.h"
#include "base/fatsd/fatsd.h"
#include "base/camera/camera.h"

#define RETRY 1

char *COL_CONFIG_HEAD="{\"device_id\":%d,\"device_config_id\":%d,\"method\":\"pull_param\"}";
char *PUSH_IMAGE_HEAD="{\"device_id\":%d,\"device_config_id\":%d,\"method\":\"push_image\",\"key\":\"%s\",\"size\":%d,\"ts\":%d}";
char *UPDATE_TIME_HEAD="{\"method\":\"update_time\"}";
char *PARAM_UPDATED ="{\"device_id\":%d, \"method\":\"param_updated\"}";

char *PUSH_DATA_READY="push_data_ready";
char *PUSH_IMAGE_READY="push_image_ready";
char *DATA_UPLOADED="data_uploaded";
char *IMAGE_UPLOADED="image_uploaded";

char *FAIL="ERROR";
char *TIME_SECOND="\"ts\":";

char protocol_buffer[MAX_BUFFER_P];


void clear_buffer(){
    memset(protocol_buffer, 0, MAX_BUFFER_P);
}

uint8_t upload_data_str(char *data_str){
	msg_send(data_str);
	char* p = NULL;
	char* pres = NULL;
	p = msg_recieve();
	pres = strstr(p, "data_uploaded");
	if(*pres){
	    return ZH_OK;
	}
	return ZH_FAIL;

}


uint8_t collecte_config(char * config_content){

    char* p = NULL;
    uint8_t i = 0;
    uint32_t psize;
    char send_buf[50];
    for(;i<RETRY;++i){
        sprintf(send_buf, COL_CONFIG_HEAD, get_device_id(), get_device_config_id());
        if(msg_send(send_buf) ==1){
            continue;
        }
        p = msg_recieve();
        if(p){
            psize = strlen(p);
            memset(config_content,0,psize+1);
            memcpy(config_content,p, psize);
            sprintf(send_buf, PARAM_UPDATED, get_device_id());
            msg_send(send_buf);
            return ZH_OK;
        }

    }
    return ZH_FAIL;
}

uint8_t update_time(){
    char* p = NULL;
    char* pres = NULL;
    uint8_t i = 0;
    for(;i<RETRY;++i){
        if(msg_send(UPDATE_TIME_HEAD)==1){
            continue;
        }
        p = msg_recieve();
        pres = strstr(p, TIME_SECOND);

        if(*pres){
            set_seconds(atoi(pres+strlen(TIME_SECOND)));
            return ZH_OK;
        }
    }
    return ZH_FAIL;
}

void upload_image(char* key, uint8_t channel, char *file_name, DWORD file_size, uint32_t ts){
    char* p = NULL;
    char* pres = NULL;
    char send_buf[256];
    FILE *pf;
    uint16_t r_size;
    //file_size = 512;
    sprintf(send_buf, PUSH_IMAGE_HEAD, get_device_id(), get_device_config_id(),key, file_size,ts);
    LOG_MSG(send_buf);
    LOG_MSG("\n");
    //msg_send(send_buf);
    msg_send_data_mode(send_buf);
    p = msg_dm_recieve(256);
    pres = strstr(p, PUSH_IMAGE_READY);
    if(*pres){
        pf = fopen(file_name, "r");
        if(pf){
            while(!feof(pf)){
                r_size = fread(protocol_buffer,1,MAX_BUFFER_P,pf);
                //r_size = 512;
                msg_send_onsize_data_mode(protocol_buffer, r_size);
                //t_size+=r_size;
            }
            //delay(10000);
            p = msg_dm_recieve(60);
            LOG_MSG("upload image finished\n");
            fclose(pf);
            //
            pres = strstr(p, IMAGE_UPLOADED);
            //if(1){
            if(*pres){
                remove(file_name);
            }
            else{
                _image_info iif;
                memcpy(iif.filename,file_name, strlen(file_name));
                iif.seconds = ts;
                iif.size = file_size;
                save_image_info(channel, iif);
            }
        }
    }
}

