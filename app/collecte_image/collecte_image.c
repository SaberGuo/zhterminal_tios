/*
 * collecte_image.c
 *
 *  Created on: 2017Äê10ÔÂ23ÈÕ
 *      Author: saber
 */

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
#include "base/camera/camera.h"
#include "base/fatsd/fatsd.h"
#include "base/gsm/gsm.h"
#include "base/conf_parser/conf_parser.h"
#include "collecte_image.h"

_time_task_s collecte_image_task_info;
char collecte_image_task_stack[TASKSTACKSIZE];
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
void colimage_timerCallback(Timer_Handle myHandle){
    camera_close();
    close_sd();
    disable_sd();
    reset_relay(RELAY_K_CAMERA);
    SysCtl_rebootDevice();
}
void collecte_image_task(UArg arg){
    //Semaphore_Handle semph_handler = (Semaphore_Handle)arg;
    DWORD fsize;
    uint32_t ts;
    uint8_t ret;

    bool is_first = true;
    char *pf_name =NULL;
    uint8_t count = 0;
    Timer_Params params;
    Timer_Handle timer0;
    Timer_Params_init(&params);
    params.period = 4000000000;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_ONESHOT_CALLBACK;
    params.timerCallback = colimage_timerCallback;
    timer0 = Timer_open(Board_TIMER0, &params);
    while(1){
        Semaphore_pend(collecte_image_task_info.task_notify_semaphore_handler, BIOS_WAIT_FOREVER);
        Semaphore_pend(semaphore_task, BIOS_WAIT_FOREVER);
        /*camera power on*/
        //Power_setPerformanceLevel(3);
        Timer_start(timer0);
        //watchdogHandle_col_img = Watchdog_open(Board_WATCHDOG1, &params);
        power_enable(ENA_DC33V|ENA_DC5V);
        delay(1000);
        set_relay(RELAY_K_CAMERA);

        camera_init();
        enable_sd();
        if(open_sd()){
            LOG_MSG("open sd fail!\n");
         }
        else{

            ret = camera_capture();
            if(ret == 1){
                camera_close();
                reset_relay(RELAY_K_CAMERA);
                continue;
            }
        }
        camera_close();
        reset_relay(RELAY_K_CAMERA);

        //Timer_stop(timer0);
        gsm_lock();
        Task_sleep(200);
        GPIO_write(Board_GPIO_POWER_KEY, Board_GPIO_LOW);
        GPIO_write(Board_GPIO_DTR, Board_GPIO_LOW);
        power_enable(ENA_DC5V);
        power_enable(ENA_DC33V);
        set_relay(RELAY_K_GSM);

        GPIO_write(Board_GPIO_POWER_KEY, Board_GPIO_HIGH);
        delay(100000);
        GPIO_write(Board_GPIO_POWER_KEY, Board_GPIO_LOW);
        //Timer_start(timer0);
        gsm_open();
        if(gsm_pb_done(10) == 1){
            goto UP_IMG_END;
        }
        Task_sleep(10000);
        if(gsm_is_reg(20) == 1){
            goto UP_IMG_END;
        }
        if(gsm_to_data_mode(10) == 1){
            goto UP_IMG_END;
        }
        if(gsm_connect_data_mode(10) == 1){
            goto UP_IMG_END;
        }

        /*upload image*/
        /*wait connect ok*/
        //open_sd();

        count = 0;
        /*is_first = true;
        do{
            pf_name = get_image_file_info(&fsize, &ts, is_first);
            is_first =false;
            if(pf_name == NULL||strstr(pf_name,"JPG")==NULL || fsize == 0){
                count++;
            }
            else{
                upload_image(pf_name, fsize, ts);
                count++;
            }
            
        }while(pf_name!=NULL && count<2);
        */
        upload_image(get_img_name(), get_img_size(), get_seconds());

        if(gsm_disconnect(10) == 1){
          LOG_MSG("error for gsm disconnect\n");
          goto UP_IMG_END;
        }
        LOG_MSG("good for gsm disconnect\n");
UP_IMG_END:
        Timer_stop(timer0);
        gsm_close();
        close_sd();
        //Watchdog_close(watchdogHandle_col_img);

        /*power off*/
        reset_relay(RELAY_K_GSM);
        power_disable(ENA_DC5V);
        power_disable(ENA_DC33V);
        disable_sd();
        gsm_unlock();
        Task_sleep(200);
        Semaphore_post(semaphore_task);
        //Power_setPerformanceLevel(0);
    }
}

void init_collecte_image_task(){
    Task_Params taskParams;
    /*Semaphore_Params semParams;
    Semaphore_Params_init(&semParams);
    semParams.instance->name = "colimg_sem";
    Semaphore_construct(&collecte_image_semStruct, 0, &semParams);
    */
    collecte_image_task_info.task_notify_semaphore_handler = semaphore_col_img_t;


    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
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
