/*
 * collecte_data.c
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
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerMSP432.h>
#include <ti/drivers/Timer.h>
#include "collecte_data.h"
#include "base/conf_parser/conf_parser.h"
#include "base/rt_clock/rt_clock.h"
#include "base/fatsd/fatsd.h"
#include "base/sensors/sensors.h"
#include "base/gsm/gsm.h"
#include "app/update_time/update_time.h"

_time_task_s collecte_data_task_info;
char col_data_task_stack[TASKSTACKSIZE];
Task_Struct col_data_task_struct;

uint32_t col_data_count = 0;

#define COL_DATA_TIMEOUT 180 //80s


void coldata_timerCallback(Timer_Handle myHandle){
    if(col_data_count++>COL_DATA_TIMEOUT){
        sensors_close();
        SysCtl_rebootDevice();
    }
}
Timer_Handle start_col_data_timer(){
    Timer_Handle th;
    Timer_Params params;
    params.period = 1000000;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = coldata_timerCallback;
    th = Timer_open(Board_TIMER1, &params);
    Timer_start(th);
    col_data_count =0;
    return th;
}

void stop_col_data_timer(Timer_Handle th){
    Timer_stop(th);
    Timer_close(th);
}

uint8_t col_data_filename[20]={};

uint8_t collecte_data_ex(){
    sensors_open();
    Task_sleep(HUGE_TIME*3);
    /*collect data*/
    sensors_process();
    char *pdata_str = sensors_construct();
    sensors_close();

    /*construct data string*/
    LOG_MSG("data str:%s", pdata_str);

    record_data(pdata_str);

    return ZH_OK;
}

void record_data(char *pdata_str){
    /*write to file*/
    enable_sd();
    if(open_sd() == ZH_FAIL){
        LOG_MSG("open sd fail!\n");
        return ZH_FAIL;
     }
    FILE * cd_file;
    sprintf(col_data_filename, "%s%d%d%s",col_data_file, get_year(),get_month(), ".txt");
    cd_file = fopen(col_data_filename,"a");
    if(cd_file){
        fwrite(pdata_str,1,strlen(pdata_str),cd_file);
        fwrite("\r\n",1,strlen("\r\n"),cd_file);
        fflush(cd_file);
        fclose(cd_file);
    }
    close_sd();
    disable_sd();
}
uint8_t upload_data_ex(){
    char *pbuf = NULL;
    uint8_t upload_count = 0;
    if(gsm_pb_done(GSM_RETRY_NUM) == ZH_FAIL){
        return ZH_FAIL;
    }

    Task_sleep(HUGE_TIME*3);
    if(gsm_is_reg(GSM_RETRY_NUM) == ZH_FAIL){
        return ZH_FAIL;
    }
    if(gsm_connect(GSM_RETRY_NUM) == ZH_FAIL){
        return ZH_FAIL;
    }
    /*every time updae the time*/
    /*if(update_time() == ZH_FAIL)
    {
        LOG_MSG("update time fail!\n");
        return ZH_FAIL;
    }*/

    for(upload_count = 0;upload_count<MAX_UPLOAD_SIZE;++upload_count){
        pbuf = sensor_get_data_str();
        if(!pbuf){
            break;
        }
        LOG_MSG("pbuf:%s\n",pbuf);
        upload_data_process(pbuf);
    }
    if(gsm_disconnect(GSM_RETRY_NUM) == ZH_FAIL){
        LOG_MSG("error for gsm disconnect\n");
        return ZH_FAIL;
    }
    LOG_MSG("good for gsm disconnect\n");
    return ZH_OK;
}

void collecte_data_task(UArg arg){
    Semaphore_Handle semph_handler = (Semaphore_Handle)arg;
    Timer_Handle col_data_th;
    while(1){
        Semaphore_pend(semph_handler, BIOS_WAIT_FOREVER);
        Semaphore_pend(semaphore_task, BIOS_WAIT_FOREVER);
        /*init sensors ports*/

        col_data_th = start_col_data_timer();
        collecte_data_ex();
        stop_col_data_timer(col_data_th);
        col_data_th = start_col_data_timer();
        gsm_open();

        if(upload_data_ex() == ZH_FAIL){
            LOG_MSG("upload data fail!\n");
        }
        gsm_close();
        stop_col_data_timer(col_data_th);

        gsm_open();
        update_time_ex();
        gsm_close();
        Semaphore_post(semaphore_task);
    }

}

Semaphore_Struct semStruct_col_data;


void init_collecte_data_task(){

    Task_Params taskParams;
    collecte_data_task_info.task_notify_semaphore_handler = semaphore_col_data_t;

    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &col_data_task_stack;
    taskParams.priority = 1;
    taskParams.arg0 = (UArg)collecte_data_task_info.task_notify_semaphore_handler;
    Task_construct(&col_data_task_struct, (Task_FuncPtr)collecte_data_task, &taskParams, NULL);

}

void config_collecte_data_task(char* invl_param){
    if(parse_crontab(invl_param,
                  collecte_data_task_info.p_task_minutes,
                  &collecte_data_task_info.task_minutes_num,
                  collecte_data_task_info.p_task_hours,
                  &collecte_data_task_info.task_hours_num))
    {
        LOG_MSG("parse fail!\n");
        return;
    }

}
