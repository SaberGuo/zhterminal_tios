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




_time_task_s collecte_config_task_info={.task_minutes_num = 1, .task_hours_num = 1, .p_task_hours={0}, .p_task_minutes={1}};
char col_config_task_stack[TASKSTACKSIZE];
char config_content[MAX_BUFFER];
Task_Struct col_config_task_struct;
Semaphore_Struct config_task_semStruct;

void config_all_tasks(){
    uint8_t i= 0;
    for(;i<3;++i){
        if(parse_config(config_file) == 0){
            disable_sd();
            Task_sleep(200);
            enable_sd();
            break;
        }
    }

    config_collecte_data_task(get_data_capture_invl());
    config_upload_data_task(get_data_upload_invl());
    config_collecte_image_task(get_img_capture_invl());

}

void collecte_config_task(UArg arg){
    FILE * cd_file;
    while(1){
        Semaphore_pend(collecte_config_task_info.task_notify_semaphore_handler, BIOS_WAIT_FOREVER);
        Semaphore_pend(semaphore_task, BIOS_WAIT_FOREVER);
        /*power on*/
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

        gsm_open();
        if(gsm_pb_done(10) == 1){
            goto COL_CONFIG_END;
        }
        if(gsm_is_reg(20) == 1){
            goto COL_CONFIG_END;
        }
        if(gsm_connect(10) == 1){
            goto COL_CONFIG_END;
        }
        LOG_MSG("gsm connect done\n");
        collecte_config(config_content);

        LOG_MSG("config is:---\n%s--\n",config_content);
        /*power off*/
        if(gsm_disconnect(10) == 1){
            LOG_MSG("error for gsm disconnect\n");
            goto COL_CONFIG_END;
        }
        LOG_MSG("gsm disconnect done\n");
COL_CONFIG_END:
        gsm_close();
        reset_relay(RELAY_K_GSM);
        power_disable(ENA_DC5V);
        power_disable(ENA_DC33V);
        gsm_unlock();
        Task_sleep(200);
        /*write into sd*/
        if(open_sd() == 1&& strlen(config_content)<= 0){
            LOG_MSG("open sd fail or config receive fail!\n");
        }
        else{
            cd_file = fopen(config_file,"w+");
            if(cd_file){
                fwrite(config_content,1,strlen(config_content),cd_file);
                fflush(cd_file);
                fclose(cd_file);
                /*config tasks*/
                config_all_tasks();
            }
        }
        close_sd();
        Semaphore_post(semaphore_task);
    }
}

void init_collecte_config_task(){

    Task_Params taskParams;
    /*Semaphore_Params semParams;
    Semaphore_Params_init(&semParams);
    semParams.instance->name = "colconfig_sem";
    Semaphore_construct(&config_task_semStruct, 0, &semParams);
    */
    collecte_config_task_info.task_notify_semaphore_handler = semaphore_col_conf_t;

    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &col_config_task_stack;
    taskParams.arg0 = (UArg)collecte_config_task_info.task_notify_semaphore_handler;
    Task_construct(&col_config_task_struct, (Task_FuncPtr)collecte_config_task, &taskParams, NULL);

}

