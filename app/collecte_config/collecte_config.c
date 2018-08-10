/*
 * collecte_config.c
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
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/drivers/Timer.h>
#include "base/conf_parser/conf_parser.h"
#include "base/protocol/protocol.h"
#include "base/rt_clock/rt_clock.h"
#include "base/fatsd/fatsd.h"
#include "base/power/power.h"
#include "base/gsm/gsm.h"

#include "collecte_config.h"

#include "app/collecte_data/collecte_data.h"
#include "app/collecte_image/collecte_image.h"
#include "app/upload_data/upload_data.h"


_time_task_s collecte_config_task_info={.task_minutes_num = 1, .task_hours_num = 1, .p_task_hours={0}, .p_task_minutes={0}};
char col_config_task_stack[TASKSTACKSIZE];
char config_content[MAX_BUFFER];
Task_Struct col_config_task_struct;
Semaphore_Struct config_task_semStruct;

uint32_t col_config_count = 0;

#define COL_CONFIG_TIMEOUT 60 //1 min

void colconfig_timerCallback(Timer_Handle myHandle){
    if(col_config_count++>COL_CONFIG_TIMEOUT){
        gsm_close();
        /*power off*/
        reset_relay(RELAY_K_GSM);
        power_disable(ENA_DC5V);
        power_disable(ENA_DC33V);
        gsm_unlock();
        SysCtl_rebootDevice();
    }
}
Timer_Handle start_col_config_timer(){
    Timer_Handle th;
    Timer_Params params;
    params.period = 1000000;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = colconfig_timerCallback;
    th = Timer_open(Board_TIMER1, &params);
    Timer_start(th);
    return th;
}

void stop_col_config_timer(Timer_Handle th){
    Timer_stop(th);
    Timer_close(th);
}

void config_all_tasks(){
    uint8_t i= 0;
    //parse_config(config_file);
    for(;i<3;++i){
        if(parse_config(config_file) == ZH_OK){
            break;
        }
        //reset sd card
        disable_sd();
        Task_sleep(200);
        enable_sd();
    }
    config_collecte_data_task(get_data_capture_invl());
    config_upload_data_task(get_data_upload_invl());
    config_collecte_image_task(get_img_capture_invl());
}


uint8_t collecte_config_ex(){

    if(gsm_pb_done(GSM_RETRY_NUM) == ZH_FAIL){
        return ZH_FAIL;
    }
    Task_sleep(HUGE_TIME*2);
    if(gsm_is_reg(GSM_RETRY_NUM) == ZH_FAIL){
        return ZH_FAIL;
    }
    if(gsm_connect(GSM_RETRY_NUM) == ZH_FAIL){
        return ZH_FAIL;
    }
    LOG_MSG("gsm connect done\n");
    collecte_config(config_content);

    //LOG_MSG("config is:---\n%s--\n",config_content);
    /*power off*/
    if(gsm_disconnect(GSM_RETRY_NUM) == ZH_FAIL){
        LOG_MSG("error for gsm disconnect\n");
        return ZH_FAIL;
    }
    LOG_MSG("gsm disconnect done\n");
    //gsm_unlock();
    Task_sleep(LITTLE_TIME);
    /*write into sd*/
    enable_sd();
    if(open_sd() == ZH_FAIL&& strlen(config_content)<= 0){
        LOG_MSG("open sd fail or config receive fail!\n");
        close_sd();
        disable_sd();
        return ZH_FAIL;
    }
    else{
        FILE * cd_file;
        cd_file = fopen(config_file,"w+");
        if(cd_file){
            fwrite(config_content,1,strlen(config_content),cd_file);
            fflush(cd_file);
            fclose(cd_file);
            /*config tasks*/
            config_all_tasks();
        }
        else{
            LOG_MSG("write config file fail!");
            close_sd();
            disable_sd();
            return ZH_FAIL;
        }
    }
    close_sd();
    disable_sd();
    return ZH_OK;
}
void collecte_config_task(UArg arg){

    Timer_Handle col_config_th;
    Semaphore_Handle semph_handler = (Semaphore_Handle)arg;
    while(1){
        Semaphore_pend(semph_handler, BIOS_WAIT_FOREVER);
        Semaphore_pend(semaphore_task, BIOS_WAIT_FOREVER);
        /*clear buffer*/
        memset(config_content, MAX_BUFFER, 0);
        /*power on*/

        col_config_th = start_col_config_timer();
        gsm_open();
        //gsm_lock();
        if(collecte_config_ex() == ZH_FAIL){
            LOG_MSG("update config fail!\n");
        }



        gsm_close();
        stop_col_config_timer(col_config_th);
        Semaphore_post(semaphore_task);
    }
}

void init_collecte_config_task(){

    Task_Params taskParams;
    collecte_config_task_info.task_notify_semaphore_handler = semaphore_col_conf_t;

    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &col_config_task_stack;
    taskParams.arg0 = (UArg)collecte_config_task_info.task_notify_semaphore_handler;
    Task_construct(&col_config_task_struct, (Task_FuncPtr)collecte_config_task, &taskParams, NULL);

}

