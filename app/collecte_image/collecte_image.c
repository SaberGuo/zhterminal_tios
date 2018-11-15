/*
 * collecte_image.c
 *
 *  Created on: 2017Äê10ÔÂ23ÈÕ
 *      Author: saber
 */

#include <base/camera/camera.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <file.h>
#include <xdc/cfg/global.h>
#include <third_party/fatfs/integer.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/drivers/Timer.h>

#include "base/protocol/protocol.h"
#include "base/rt_clock/rt_clock.h"
#include "base/power/power.h"
#include "base/fatsd/fatsd.h"
#include "base/gsm/gsm.h"
#include "base/conf_parser/conf_parser.h"
#include "collecte_image.h"

_time_task_s collecte_image_task_info;
char collecte_image_task_stack[TASKSTACKSIZEBIG];
Task_Struct collecte_image_task_struct;
Semaphore_Struct collecte_image_semStruct;

//Watchdog_Handle watchdogHandle_col_img;

char *get_image_file_info(DWORD* fsize, uint32_t* ts, bool is_first){
    char *pf_name;
    if(is_first){
        pf_name = find_file_first(image_pattern, fsize, ts);
    }
    else{
        pf_name = find_file_next(fsize, ts);
    }

    return pf_name;
    
}

uint32_t col_image_count = 0;

#define COL_IMAGE_TIMEOUT 1200 //6 min

void colimage_timerCallback(Timer_Handle myHandle){
    if(col_image_count++>COL_IMAGE_TIMEOUT){
        camera_close();
        //close_sd();
        disable_sd();
        reset_relay(RELAY_K_CAMERA);
        SysCtl_rebootDevice();
    }
}

Timer_Handle start_col_image_timer(){
    Timer_Handle th;
    Timer_Params params;
    params.period = 1000000;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = colimage_timerCallback;
    th = Timer_open(Board_TIMER1, &params);
    Timer_start(th);
    col_image_count = 0;
    return th;
}

void stop_col_image_timer(Timer_Handle th){
    Timer_stop(th);
    Timer_close(th);
}

uint8_t upload_image_ex(){
    uint8_t count;
    //for(tt = 0;tt<get_camera_num();++tt){
    for(count = 0;count<1;++count){
        /*_image_info *piif = get_image_info(count);
        if(piif == NULL){
            continue;
        }

        if(piif->size<=0){
            continue;
        }

        gsm_open();

        if(gsm_pb_done(GSM_RETRY_NUM) == ZH_FAIL){

            return ZH_FAIL;
        }
        Task_sleep(HUGE_TIME*2);
        if(gsm_is_reg(GSM_RETRY_NUM) == ZH_FAIL){
            return ZH_FAIL;
        }

        if(gsm_to_data_mode(GSM_RETRY_NUM) == ZH_FAIL){

            return ZH_FAIL;
        }
        if(gsm_connect_data_mode(GSM_RETRY_NUM) == ZH_FAIL){
            return ZH_FAIL;
        }

        upload_image(get_camera_key(count),count, piif->filename, piif->size, piif->seconds);
        tmp = count;
        ret_up = ret&(1<<tmp);
        if(ret_up == 0){

            //upload_image(get_camera_key(count), get_camera_file_name(count), get_camera_file_size(count), get_seconds()+count);

        }
        if(gsm_disconnect(GSM_RETRY_NUM) == ZH_FAIL){
             LOG_MSG("error for gsm disconnect\n");

        }
        LOG_MSG("good for gsm disconnect\n");
        gsm_close();*/
        Task_sleep(HUGE_TIME);
    }
}
uint8_t collecte_image_ex(){

    uint8_t count = 0;

    power_enable(ENA_DC33V|ENA_DC5V);
    delay(LITTLE_TIME);
    set_relay(RELAY_K_CAMERA);

    camera_open();
//#ifndef ENABLE_DEBUG
    enable_sd();
    if(open_sd() == ZH_FAIL){
        LOG_MSG("open sd fail!\n");
        camera_close();
        reset_relay(RELAY_K_CAMERA);
        return ZH_FAIL;
     }
//#endif
    /*if(camera_process() == ZH_FAIL){
        camera_close();
        reset_relay(RELAY_K_CAMERA);
        return ZH_FAIL;
    }*/
    camera_process();
    camera_close();
    reset_relay(RELAY_K_CAMERA);

    /*if(upload_image_ex() == ZH_FAIL){
        return ZH_FAIL;
    }*/

    //for(tt = 0;tt<get_camera_num();++tt){
     for(count = 0;count<get_camera_num();++count){
         _image_info *piif = get_image_info(count);
         if(piif == NULL){
             continue;
         }

         if(piif->size<=0){
             continue;
         }

         gsm_open();

         if(gsm_pb_done(GSM_RETRY_NUM) == ZH_FAIL){
             gsm_close();
             close_sd();
             disable_sd();
             return ZH_FAIL;
         }
        Task_sleep(HUGE_TIME*3);
         if(gsm_is_reg(GSM_RETRY_NUM) == ZH_FAIL){
             gsm_close();
             close_sd();
             disable_sd();
             return ZH_FAIL;
         }

         if(gsm_to_data_mode(GSM_RETRY_NUM) == ZH_FAIL){
             gsm_close();
             close_sd();
             disable_sd();
             return ZH_FAIL;
         }
         if(gsm_connect_data_mode(1) == ZH_FAIL){
             gsm_close();
             close_sd();
             disable_sd();
             return ZH_FAIL;
         }

         upload_image(get_camera_key(count),count, piif->filename, piif->size, piif->seconds);

         if(gsm_disconnect(GSM_RETRY_NUM) == ZH_FAIL){
              LOG_MSG("error for gsm disconnect\n");

         }
         LOG_MSG("good for gsm disconnect\n");
         gsm_close();
         Task_sleep(HUGE_TIME);
     }
    gsm_close();
    close_sd();
    disable_sd();
    return ZH_OK;
}

void collecte_image_task(UArg arg){

    Semaphore_Handle semph_handler = (Semaphore_Handle)arg;
    Timer_Handle col_image_th;
    while(1){
        Semaphore_pend(semph_handler, BIOS_WAIT_FOREVER);
        Semaphore_pend(semaphore_task, BIOS_WAIT_FOREVER);
        col_image_th = start_col_image_timer();
        /*get the time and gps*/
        //gsm_open();
        //update_time_ex();
        //gsm_close();

        /*camera power on*/
        if(collecte_image_ex() == ZH_FAIL){
            LOG_MSG("collecte image ex fail!\n");
        }

        stop_col_image_timer(col_image_th);

        Task_sleep(LITTLE_TIME);
        Semaphore_post(semaphore_task);
    }
}

void init_collecte_image_task(){
    Task_Params taskParams;
    collecte_image_task_info.task_notify_semaphore_handler = semaphore_col_img_t;
    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZEBIG;
    taskParams.stack = &collecte_image_task_stack;
    taskParams.arg0 = (UArg)collecte_image_task_info.task_notify_semaphore_handler;
    Task_construct(&collecte_image_task_struct, (Task_FuncPtr)collecte_image_task, &taskParams, NULL);

}


void config_collecte_image_task(char* invl_param){
	if(parse_crontab(invl_param,
                  collecte_image_task_info.p_task_minutes,
                  &collecte_image_task_info.task_minutes_num,
                  collecte_image_task_info.p_task_hours,
                  &collecte_image_task_info.task_hours_num))
    {
        LOG_MSG("parse fail!\n");
        return;
    }
}
