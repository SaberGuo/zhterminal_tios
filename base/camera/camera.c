/*
 * camera.c
 *
 *  Created on: 2017Äê10ÔÂ25ÈÕ
 *      Author: saber
 */
#include <base/camera/camera.h>
#include <base/camera/hisi/hisi.h>
#include <stdio.h>
#include "base/rt_clock/rt_clock.h"
#include "base/fatsd/fatsd.h"
#include "base/conf_parser/conf_parser.h"

const char camera_type_strs[CAMERA_TYPE_COUNT][20]={
                                   "hisi",
};

_camera_config camera_configs[CAMERA_TYPE_COUNT]={
                                                  /*hisilicon*/
                                                  {.camera_open = hisi_open,
                                                  .camera_close = hisi_close,
                                                  .camera_process = hisi_process,
                                                  }

};

//char camera_file_names[CAMERA_MAX_NUM][40];
//uint32_t camera_file_sizes[CAMERA_MAX_NUM];
_image_info image_infos[CAMERA_MAX_NUM][CAMERA_BUFFER_NUM];
uint8_t p_cw_buffer[CAMERA_MAX_NUM] = {0,0,0,0};
uint8_t p_cr_buffer[CAMERA_MAX_NUM] = {0,0,0,0};

char camera_buffer[MAX_BUFFER];

_image_info *get_image_info(uint8_t channel){

    _image_info *pbuf = NULL;
    if(channel<CAMERA_MAX_NUM && p_cr_buffer[channel] != p_cw_buffer[channel]){
        pbuf = &image_infos[channel][p_cr_buffer[channel]];
        p_cr_buffer[channel] = (++p_cr_buffer[channel])%CAMERA_BUFFER_NUM;
    }
    return pbuf;
}

void save_image_info(uint8_t channel, _image_info info){
    if(channel<CAMERA_MAX_NUM ){
        _image_info *pbuf = &image_infos[channel][p_cw_buffer[channel]];
        memcpy(pbuf->filename, info.filename, strlen(info.filename));
        pbuf->seconds = info.seconds;
        pbuf->size = info.size;
        p_cw_buffer[channel] = (++p_cw_buffer[channel])%CAMERA_BUFFER_NUM;
    }
}

uint8_t camera_open(){
    _data_item* pdi = g_config.image_items;
    uint8_t i = 0;
    while(pdi!=NULL){
        for(;i<MAX_KEYS;i++){
            if(strlen(pdi->keys[i])>0){
                camera_configs[pdi->sensor_type].camera_open(i);
            }
        }
        pdi = pdi->next_data_item;
    }
    return ZH_OK;
}
_image_info tiif;

uint8_t camera_process(){
    _data_item* pdi = g_config.image_items;
    uint8_t i = 0,count_c = 0;
    uint8_t ret = 0,res = 0;


    while(pdi!=NULL){
        for(;i<MAX_KEYS;i++){
            if(strlen(pdi->keys[i])>0){
                //camera_file_sizes[count_c] = 0;
                //memset(camera_file_names[count_c],0,40);
                //ret|=camera_configs[pdi->sensor_type].camera_process(count_c,camera_file_names[count_c],camera_buffer, &camera_file_sizes[count_c]);
                res = camera_configs[pdi->sensor_type].camera_process(count_c,tiif.filename,camera_buffer, &tiif.size);
                tiif.seconds = get_seconds()+count_c;
                if(res == ZH_OK){
                    save_image_info(count_c, tiif);
                }
                ret|=res;
                ret = ret<<1;
                count_c++;
            }
        }
        pdi = pdi->next_data_item;
    }
    return ret;
}



uint8_t camera_close(){
    _data_item* pdi = g_config.image_items;
    uint8_t count = 0;
    while(pdi!=NULL){
        camera_configs[pdi->sensor_type].camera_close(count++);
        pdi = pdi->next_data_item;
    }
    return ZH_OK;
}

uint8_t get_camera_num(){
    _data_item* pdi = g_config.image_items;
    uint8_t i = 0, count =0;
    while(pdi!=NULL){
        for(;i<MAX_KEYS;i++){
            if(strlen(pdi->keys[i])>0){
                count++;
            }
        }
        pdi = pdi->next_data_item;
    }
    return count;
}
char* get_camera_key(uint8_t num){
    _data_item* pdi = g_config.image_items;
    uint8_t i = 0, count =0;
    while(pdi!=NULL){
        for(;i<MAX_KEYS;i++){
            if(strlen(pdi->keys[i])>0){
                if(count == num){
                    return &(pdi->keys[i]);
                }
                count++;
            }
        }
        pdi = pdi->next_data_item;
    }
    return NULL;
}

/*char* get_camera_file_name(uint8_t num){
    if(num<CAMERA_MAX_NUM){
        return camera_file_names[num];
    }
    else{
        return NULL;
    }
}
uint32_t get_camera_file_size(uint8_t num){
    if(num<CAMERA_MAX_NUM){
        return camera_file_sizes[num];
    }
    else{
        return 0;
    }
}*/
