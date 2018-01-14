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

#define RETRY 2

char *PUSH_DATA_HEAD="device_id:%d,device_config_id:%d,method:push_data,count:%d;";
char *COL_CONFIG_HEAD="device_id:%d,device_config_id:%d,method:pull_param;";
char *PUSH_IMAGE_HEAD="device_id:%d,device_config_id:%d,method:push_image,key:img_env,size:%d,ts:%d;";
char *UPDATE_TIME_HEAD="device_id:%d,device_config_id:%d,method:update_time;";


char *PACKAGE_FORMAT="%s:%.2f,ts:%d;";
char *PUSH_DATA_FINISH="method:push_data_finish;";
char *PUSH_DATA_READY="push_data_ready";
char *PUSH_IMAGE_READY="push_image_ready";
char *DATA_UPLOADED="data_uploaded";
char *IMAGE_UPLOADED="image_uploaded";
char *PARAM_UPDATED ="method:param_updated";
char *FAIL="ERROR";
char *TIME_SECOND="ts:";

char protocol_buffer[MAX_BUFFER_P];


void clear_buffer(){
    memset(protocol_buffer, 0, MAX_BUFFER_P);
}
uint8_t upload_data_start(){
	char* p = NULL;
    char* pres = NULL;
    uint8_t i = 0;
    char send_buf[50];
    for(;i<RETRY;++i){
        clear_buffer();
        
        sprintf(send_buf, PUSH_DATA_HEAD, get_device_id(), get_device_config_id(), get_sensor_num());
        if(msg_send(send_buf) == 1){
            continue;
        }
        //delay_ms(1000);
        p = msg_recieve();
        LOG_MSG("rec msg is:%s\n", p);
        pres = strstr(p, PUSH_DATA_READY);
        if(pres){
            return 0;
        }
    }
    return 1;
}

uint8_t upload_data_end(){
	char* p = NULL;
    char* pres = NULL;
    uint8_t i = 0;
    for(;i<RETRY;++i){
        clear_buffer();
        if(msg_send(PUSH_DATA_FINISH) == 1){
            continue;
        }
        p = msg_recieve();
        LOG_MSG(p);
        pres = strstr(p, DATA_UPLOADED);
        if(pres){
           LOG_MSG("data uploaded ok\n");
           return 0;
        }
    }
    return 1;
}

uint8_t upload_data_str(char *data_str){
	return msg_send(data_str);
}

void construct_protocol_data(char *data_str, char *key, float value){

    sprintf((char *)data_str, PACKAGE_FORMAT, key, value, get_seconds());
}

uint8_t upload_data(char *key, float value){

    memset(protocol_buffer,0,MAX_BUFFER_P);
    construct_protocol_data(protocol_buffer, key, value);

    return msg_send(protocol_buffer);

}

uint8_t collecte_config(char * config_content){

    char* p = NULL;
    uint8_t i = 0;
    char send_buf[50];
    
    for(;i<RETRY;++i){
        sprintf(send_buf, COL_CONFIG_HEAD, get_device_id(), get_device_config_id());
        if(msg_send(send_buf) ==1){
            continue;
        }
        p = msg_recieve();
        if(p){
            memcpy(config_content,p, strlen(p+1));
            msg_send(PARAM_UPDATED);
            return 0;
        }

    }
    return 1;
}

uint8_t update_time(){
    char* p = NULL;
    char* pres = NULL;
    uint8_t i = 0;
    char send_buf[50]={0};
    for(;i<RETRY;++i){
        sprintf(send_buf, UPDATE_TIME_HEAD, get_device_id(), get_device_config_id());
        if(msg_send(send_buf)==1){
            continue;
        }
        p = msg_recieve();
        pres = strstr(p, TIME_SECOND);
        if(*pres){
            set_seconds(atoi(pres+strlen(TIME_SECOND)));
            return 0;
        }
    }
    return 1;
}

void upload_image(char *file_name, DWORD file_size, uint32_t ts){
    char* p = NULL;
    char* pres = NULL;
    char send_buf[100];
    char fn[50];
    FILE *pf;
    uint16_t r_size;
    //sprintf(fn,"%s%s",image_file_pre,file_name);
    sprintf(fn,"%s", file_name);
    sprintf(send_buf, PUSH_IMAGE_HEAD, get_device_id(), get_device_config_id(),file_size,ts);
    msg_send_data_mode(send_buf);
    p = msg_dm_recieve();
    //pres = strstr(p, PUSH_IMAGE_READY);
    //if(*pres){
    //delay(4000);
    if(1){
        pf = fopen(fn, "r");
        if(pf){
            while(!feof(pf)){
                r_size = fread(protocol_buffer,1,MAX_BUFFER_P,pf);
                msg_send_onsize_data_mode(protocol_buffer, r_size);
            }
            fclose(pf);
            //delay(4000);
            p = msg_dm_recieve();
            //pres = strstr(p, IMAGE_UPLOADED);
            //if(*pres){
            if(1){
                remove(fn);
            }
        }
    }
    /*msg_send(send_buf);
    p = msg_recieve();
    pres = strstr(p, PUSH_IMAGE_READY);
    if(*pres){
        pf = fopen(fn, "r");
        if(pf){
            while(!feof(pf)){
                r_size = fread(protocol_buffer,1,1024,pf);
                msg_send_onsize(protocol_buffer, r_size);
            }
            fclose(pf);
            p = msg_recieve();
            pres = strstr(p, IMAGE_UPLOADED);
            if(*pres){
                remove(file_name);
            }
            
        }
    }
    */
}

