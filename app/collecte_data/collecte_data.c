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

_time_task_s collecte_data_task_info;
char col_data_task_stack[TASKSTACKSIZE];
Task_Struct col_data_task_struct;

void coldata_timerCallback(Timer_Handle myHandle){
    close_sensors();
    SysCtl_rebootDevice();
}


void collecte_data_task(UArg arg){
    FILE * cd_file;
    uint8_t count = 0;
    Timer_Params params;
    Timer_Handle timer2;
    Timer_Params_init(&params);
    params.period = 60000000;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_ONESHOT_CALLBACK;
    params.timerCallback = coldata_timerCallback;
    timer2 = Timer_open(Board_TIMER2, &params);
    //init_collecte_data_task();
    while(1){
        Semaphore_pend(collecte_data_task_info.task_notify_semaphore_handler, BIOS_WAIT_FOREVER);
        Semaphore_pend(semaphore_task, BIOS_WAIT_FOREVER);
        /*init sensors ports*/
        //Power_setPerformanceLevel(3);
        Timer_start(timer2);
        open_sensors();
        /*collect data*/
        col_sensors();
        char *pdata_str = construct_data_str();
        close_sensors();

        /*construct data string*/
        LOG_MSG("data str:%s", pdata_str);

        /*write into sd*/
        /*do{
            if(open_sd()){
                LOG_MSG("open sd fail!\n");
            }
            else{

                cd_file = fopen(col_data_file,"a+");
                if(cd_file){
                    fwrite(pdata_str,1,strlen(pdata_str),cd_file);
                    fwrite("\r\n",1,strlen("\r\n"),cd_file);
                    fflush(cd_file);
                    fclose(cd_file);
                }
            }
            close_sd();
            count++;
        }while(cd_file == NULL && count< 3);
        */
        Timer_stop(timer2);
        Semaphore_post(semaphore_task);
    }

}

Semaphore_Struct semStruct_col_data;


void init_collecte_data_task(){

    Task_Params taskParams;

    /*Semaphore_Params semParams;
    Semaphore_Params_init(&semParams);
    semParams.instance->name = "coldata_sem";
    Semaphore_construct(&semStruct_col_data, 0, &semParams);
    */
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
