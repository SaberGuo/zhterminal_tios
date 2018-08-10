/*
 * conf_parser.c
 *
 *  Created on: 2017Äê10ÔÂ23ÈÕ
 *      Author: saber
 */

#include <base/camera/camera.h>
#include "conf_parser.h"
#include "base/ini/ini.h"
#include "base/cJSON/cJSON.h"
#include "base/sensors/sensors.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>
#include <limits.h>
#include <ctype.h>

/* XDC module Headers */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Memory.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/heaps/HeapMem.h>
#include <ti/sysbios/heaps/HeapBuf.h>


//#define HEAPMEMSIZE 512

/* used to align heapmem/heapbuf buffers */
/*typedef union {
    double d;
    long l;
    void *p;
    char c;
    short s;
} AlignData;

AlignData heapMemBuffer[HEAPMEMSIZE / sizeof(AlignData)];

HeapMem_Struct heapMemStruct;

HeapMem_Handle configHeap;*/



s_config g_config ={.device_id = 30,
    .device_config_id = 100,
    .data_items = NULL,
    .image_items = NULL,
    .data_capture_invl = NULL,
    .img_capture_invl = NULL,
    .data_upload_invl = NULL
    };

void init_config_heap(){
    /*HeapMem_Params heapMemParams;
    HeapMem_Params_init(&heapMemParams);
    heapMemParams.size = HEAPMEMSIZE;
    heapMemParams.minBlockAlign = 1;
    heapMemParams.buf = heapMemBuffer;
    HeapMem_construct(&heapMemStruct, &heapMemParams);
    configHeap = HeapMem_handle(&heapMemStruct);*/

}

size_t total_size = 0;

Ptr malloc_config(size_t size){
    //IHeap_Handle heap = HeapMem_Handle_upCast(configHeap);
    //total_size+=size;
    //return Memory_alloc(heap, size, 0, NULL);
    return malloc(size);

}

void free_config(Ptr block, size_t size){
    //IHeap_Handle heap = HeapMem_Handle_upCast(configHeap);
    //Memory_free(heap, block, size);
    free(block);
}
void free_data_items(_data_item * pdi){
    if(pdi->next_data_item!=NULL){
        free_data_items(pdi->next_data_item);
    }
    free_config(pdi->port,strlen(pdi->port)+1);
    /*int j = 0;
    for(;j<MAX_KEYS;j++){
        if(pdi->keys[j]!=NULL){
            free_config(pdi->keys[j],strlen(pdi->keys[j])+1);
        }
    }
    free_config(pdi->keys,sizeof(pdi->keys));*/
    free_config(pdi, sizeof(_data_item));
}
uint8_t parser_config_image(cJSON* pJson){
    cJSON * pdata_items = cJSON_GetObjectItem(pJson, "image");
    uint8_t item_size = cJSON_GetArraySize(pdata_items);
    if(g_config.image_items!=NULL){
        free_data_items(g_config.image_items);
    }
    if(item_size<=0){
        return ZH_OK;
    }
    g_config.image_items = (_data_item*)malloc_config(sizeof(_data_item));
    _data_item * pdi = g_config.image_items;

    cJSON * tmp_pcj;
    uint8_t i = 0, k = 0;
    /*clear the keys*/
    /*for(i = 0;i<MAX_KEYS;++i){
        pdi->keys[i] = 0;
    }*/
    do{
        tmp_pcj = cJSON_GetArrayItem(pdata_items, i);
        cJSON* cp = cJSON_GetObjectItem(tmp_pcj, "port");
        if(cp == NULL){
            LOG_MSG(cJSON_GetErrorPtr());
            return ZH_FAIL;
        }
        pdi->port = (char*)malloc_config(strlen(cp->valuestring)+1);
        memset(pdi->port, 0, strlen(cp->valuestring)+1);
        memcpy(pdi->port, cp->valuestring, strlen(cp->valuestring));
        //cJSON_Delete(cp);
        // port num
        cJSON* cn = cJSON_GetObjectItem(tmp_pcj, "port_num");
        if(cp == NULL){
            LOG_MSG(cJSON_GetErrorPtr());
            return ZH_FAIL;
        }
        pdi->port_num = cn->valueint;
        //cJSON_Delete(cn);
        // sensor type
        cJSON* cst = cJSON_GetObjectItem(tmp_pcj, "sensor_type");
        if(cst == NULL){
            LOG_MSG(cJSON_GetErrorPtr());
            return ZH_FAIL;
        }
        for(k = 0;k<SENSOR_TYPE_COUNT;++k){
            if(NULL!=strstr(cst->valuestring, camera_type_strs[k])){
                pdi->sensor_type = k;
                break;
            }
        }

        //cJSON_Delete(cst);
        //keys
        cJSON* cks = cJSON_GetObjectItem(tmp_pcj, "keys");
        if(cks == NULL){
             LOG_MSG(cJSON_GetErrorPtr());
             return ZH_FAIL;
        }
        int j = 0;
        for(j = 0; j<MAX_KEYS; ++j){
            memset(pdi->keys[j],0,MAX_KEY_LEN);
        }
        for(j = 0;j<cJSON_GetArraySize(cks);++j){
             cJSON* ck = cJSON_GetArrayItem(cks, j);
             if(ck == NULL){
                 LOG_MSG(cJSON_GetErrorPtr());
                 return ZH_FAIL;
             }
             if(atoi(ck->string)>=MAX_KEYS){
                 LOG_MSG("keys num %d is larger",atoi(ck->string));
             }
             else{
                 //pdi->keys[atoi(ck->string)] = (char*)malloc_config(strlen(ck->valuestring)+1);
                 //memset(pdi->keys[atoi(ck->string)],0,MAX_KEY_LEN);
                 memcpy(pdi->keys[atoi(ck->string)], ck->valuestring, strlen(ck->valuestring));
             }
             //cJSON_Delete(ck);
         }
         //cJSON_Delete(cks);
         if(++i<item_size){
             pdi->next_data_item = (_data_item*)malloc_config(sizeof(_data_item));
             pdi = pdi->next_data_item;
         }
         else{
             pdi->next_data_item = NULL;
         }
    }while(i<item_size);
    return ZH_OK;
}
uint8_t parser_config_data(cJSON* pJson){
    cJSON * pdata_items = cJSON_GetObjectItem(pJson, "data");
    uint8_t item_size = cJSON_GetArraySize(pdata_items);
    if(g_config.data_items != NULL){
        free_data_items(g_config.data_items);
    }
    if(item_size<=0){
        return ZH_OK;
    }
    g_config.data_items = (_data_item*)malloc_config(sizeof(_data_item));
    _data_item * pdi = g_config.data_items;
    uint8_t i = 0,j = 0, k = 0;
    cJSON * tmp_pcj;
    /*clear the keys*/
    /*for(i = 0;i<MAX_KEYS;++i){
        pdi->keys[i] = 0;
    }*/
    do{
        tmp_pcj = cJSON_GetArrayItem(pdata_items, i);
        // port parse
        cJSON* cp = cJSON_GetObjectItem(tmp_pcj, "port");
        if(cp == NULL){
            LOG_MSG(cJSON_GetErrorPtr());
            return ZH_FAIL;
        }
        pdi->port = (char*)malloc_config(strlen(cp->valuestring)+1);
        memset(pdi->port,0,strlen(cp->valuestring)+1);
        memcpy(pdi->port, cp->valuestring, strlen(cp->valuestring));
        //cJSON_Delete(cp);
        // port num
        cJSON* cn = cJSON_GetObjectItem(tmp_pcj, "port_num");
        if(cn == NULL){
            LOG_MSG(cJSON_GetErrorPtr());
            return ZH_FAIL;
        }
        pdi->port_num = cn->valueint;
        //cJSON_Delete(cn);
        // sensor type
        cJSON* cst = cJSON_GetObjectItem(tmp_pcj, "sensor_type");
        if(cst == NULL){
            LOG_MSG(cJSON_GetErrorPtr());
            return ZH_FAIL;
        }
        for(k = 0;k<SENSOR_TYPE_COUNT;++k){
            if(NULL!=strstr(cst->valuestring, sensor_type_strs[k])){
                pdi->sensor_type = k;
                break;
            }
        }

        //cJSON_Delete(cst);

        // keys
        cJSON* cks = cJSON_GetObjectItem(tmp_pcj, "keys");
        if(cks == NULL){
            LOG_MSG(cJSON_GetErrorPtr());
            return ZH_FAIL;
        }
        for(j = 0; j<MAX_KEYS; ++j){
            memset(pdi->keys[j],0,MAX_KEY_LEN);
        }
        for(j = 0;j<cJSON_GetArraySize(cks);++j){
            cJSON* ck = cJSON_GetArrayItem(cks, j);
            if(ck == NULL){
                LOG_MSG(cJSON_GetErrorPtr());
                return ZH_FAIL;
            }
            if(atoi(ck->string)>=MAX_KEYS){
                LOG_MSG("keys num %d is larger",atoi(ck->string));
            }
            else{
                //pdi->keys[atoi(ck->string)] = (char*)malloc_config(strlen(ck->valuestring)+1);
                //memset(pdi->keys[atoi(ck->string)],0,MAX_KEY_LEN);
                memcpy(pdi->keys[atoi(ck->string)], ck->valuestring, strlen(ck->valuestring));
            }
            //cJSON_Delete(ck);
        }
        //cJSON_Delete(cks);
        if(++i<item_size){
            pdi->next_data_item = (_data_item*)malloc_config(sizeof(_data_item));
            pdi = pdi->next_data_item;
        }
        else{
            pdi->next_data_item = NULL;
        }

    }while(i<item_size);
    return ZH_OK;


}

uint8_t parser_config_control(cJSON* pJson){
    cJSON * pctl_items = cJSON_GetObjectItem(pJson, "control");
    if(pctl_items == NULL){
        LOG_MSG(cJSON_GetErrorPtr());
        return ZH_FAIL;
    }

    cJSON * pici = cJSON_GetObjectItem(pctl_items, "img_capture_invl");
    if(pici == NULL){
        LOG_MSG(cJSON_GetErrorPtr());
        return ZH_FAIL;
    }
    if(g_config.img_capture_invl!= NULL){
        free_config(g_config.img_capture_invl,strlen(g_config.img_capture_invl)+1);
    }
    g_config.img_capture_invl = (char*)malloc_config(1+strlen(pici->valuestring));
    memset(g_config.img_capture_invl,0, 1+strlen(pici->valuestring));
    memcpy(g_config.img_capture_invl,
           pici->valuestring,
           strlen(pici->valuestring));
    //cJSON_Delete(pici);

    cJSON * pdui = cJSON_GetObjectItem(pctl_items, "data_upload_invl");
    if(pdui == NULL){
        LOG_MSG(cJSON_GetErrorPtr());
        return ZH_FAIL;
    }
    if(g_config.data_upload_invl!=NULL){
        free_config(g_config.data_upload_invl,strlen(g_config.data_upload_invl)+1);
    }
    g_config.data_upload_invl = (char*)malloc_config(1+strlen(pdui->valuestring));
    memset(g_config.data_upload_invl,0, 1+strlen(pdui->valuestring));
    memcpy(g_config.data_upload_invl,
           pdui->valuestring,
           strlen(pdui->valuestring));
    //cJSON_Delete(pdui);

    cJSON * pdci = cJSON_GetObjectItem(pctl_items, "data_capture_invl");

    if(g_config.data_capture_invl!=NULL){
        free_config(g_config.data_capture_invl,strlen(g_config.data_capture_invl)+1);
    }
    g_config.data_capture_invl = (char*)malloc_config(1+strlen(pdci->valuestring));
    memset(g_config.data_capture_invl,0, 1+strlen(pdci->valuestring));
    memcpy(g_config.data_capture_invl,
           pdci->valuestring,
           strlen(pdci->valuestring));
    //cJSON_Delete(pdci);


    //cJSON_Delete(pctl_items);
    return ZH_OK;
}

/*json part*/
uint8_t parser_config(char * pMsg){
    if(NULL == pMsg){
        return ZH_FAIL;
    }
    cJSON* pJson = cJSON_Parse(pMsg);
    if(NULL == pJson){
        return ZH_FAIL;
    }
    cJSON * pdev_id = cJSON_GetObjectItem(pJson, "device_id");
    g_config.device_id = pdev_id->valueint;

    cJSON * pconf_id = cJSON_GetObjectItem(pJson, "device_config_id");
    g_config.device_config_id = pconf_id->valueint;


    if(parser_config_data(pJson) == ZH_FAIL){
        return ZH_FAIL;
    }

    if(parser_config_image(pJson) == ZH_FAIL){
            return ZH_FAIL;
        }

    if(parser_config_control(pJson) == ZH_FAIL){
        return ZH_FAIL;
    }
    cJSON_Delete(pJson);
    return ZH_OK;
}


uint8_t parse_config(const char* filename){

    FILE* f = fopen(filename, "r");
    uint32_t len;
    uint8_t res;
    char* msg;
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);
    msg = (char*)malloc(len+1);
    fread(msg, 1, len, f);
    res = parser_config(msg);
    free(msg);
    fclose(f);
    return res;

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
    LOG_MSG(invl);
    uint8_t i_min = 0, i_hour = 0, stamp = 0;
    token_min = strtok(invl," ");
    if(token_min == NULL){
        return ZH_FAIL;
    }

    token_hour = strtok(NULL," ");
    if(token_hour == NULL){
        return ZH_FAIL;
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


    return ZH_OK;

}
