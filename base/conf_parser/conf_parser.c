/*
 * conf_parser.c
 *
 *  Created on: 2017Äê10ÔÂ23ÈÕ
 *      Author: saber
 */

#include "conf_parser.h"
#include "base/ini/ini.h"
#include <string.h>
#include <stdlib.h>

s_config g_config ={.device_id = 30,
	.device_config_id = 100,
	.data_capture_invl = "10,20,40,50 *",
	.img_capture_invl = "15 9,10,14,16",
	.data_upload_invl = "0,30 *"
	};

static int handler(void* conf, const char* section, const char* name,
                   const char* value){
	s_config* pconfig = (s_config*)conf;
	#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
	if (MATCH("common", "device_id")) {
        pconfig->device_id = atoi(value);
    } else if (MATCH("common", "device_config_id")) {
        pconfig->device_config_id = atoi(value);
    } else if (MATCH("control", "img_capture_invl")) {
        pconfig->img_capture_invl = strdup(value);
    }else if (MATCH("control", "data_capture_invl")) {
        pconfig->data_capture_invl = strdup(value);
    }else if (MATCH("control", "data_upload_invl")) {
        pconfig->data_upload_invl = strdup(value);
    } else {
        return 0;  /* unknown section/name, error */
    }
    return 1;
}


uint8_t parse_config(const char* filename){
	if (ini_parse(filename, handler, &g_config) < 0) {
	    LOG_MSG("Can't load '%s'\n", filename);
        return 1;
    }
    return 0;
}

uint32_t get_device_id(){
	return g_config.device_id;
}

char *get_data_capture_invl(){
    return g_config.data_capture_invl;
}

char *get_img_capture_invl(){
    return g_config.img_capture_invl;
}

char *get_data_upload_invl(){
    return g_config.data_upload_invl;
}

uint32_t get_device_config_id(){
	return g_config.device_config_id;
}

uint8_t parse_crontab(char* invl,
                      uint8_t* minutes, uint8_t* minutes_num,
                      uint8_t* hours, uint8_t* hours_num ){
    char *token_min, *token_hour, *strstrn;
    uint8_t i_min = 0, i_hour = 0, stamp = 0;
    token_min = strtok(invl," ");
    if(token_min == NULL){
        return 1;
    }

    token_hour = strtok(NULL," ");
    if(token_hour == NULL){
        return 1;
    }

    *minutes_num = 1;
    *hours_num = 1;

    /*minutes parser*/
    if(strstrn = strchr(token_min, '/')){
        stamp = atoi(strstrn+1);
        *minutes_num = (uint8_t)(60/stamp);
        for(i_min = 0;i_min<*minutes_num;++i_min){
            minutes[i_min] =stamp*(i_min+1)%60;
        }
    }
    else{
        strstrn = strchr(token_min,',');
        while(strstrn!=NULL){
            strstrn = strchr(strstrn+1,',');
            (*minutes_num)++;
        }
        token_min=strtok(token_min, ",");
        while(token_min !=NULL){
            if(strchr(token_min,'*')){
                minutes[i_min++] = EVERY_TICK;
            }else{
                minutes[i_min++] = atoi(token_min);
            }
            token_min=strtok(NULL, ",");
        }
    }

    /*hour parser*/
    if(strstrn = strchr(token_hour, '/')){
        stamp = atoi(strstrn+1);
        *hours_num = (uint8_t)(24/stamp);
        for(i_hour = 0;i_hour<*hours_num;++i_hour){
            hours[i_hour] =stamp*(i_hour+1)%24;
        }
    }
    else{
        strstrn = strchr(token_hour,',');
        while(strstrn!=NULL){
            strstrn = strchr(strstrn+1,',');
            (*hours_num)++;
        }

        token_hour=strtok(token_hour, ",");
        while(token_hour !=NULL){
            if(strchr(token_hour,'*')){
                hours[i_hour++] = EVERY_TICK;
            }else{
                hours[i_hour++] = atoi(token_hour);
            }
            token_hour=strtok(NULL, ",");
        }
    }


    return 0;

}
