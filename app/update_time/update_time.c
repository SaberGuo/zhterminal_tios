/*
 * update_time.c
 *
 *  Created on: 2017Äê11ÔÂ16ÈÕ
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

#include "update_time.h"
#include "base/protocol/protocol.h"
#include "base/rt_clock/rt_clock.h"
#include "base/power/power.h"
#include "base/gsm/gsm.h"



_time_task_s update_time_task_info = {.task_minutes_num = 1, .task_hours_num = 11, .p_task_hours={0,2,4,6,8,10,12,14,16,18,20}, .p_task_minutes={1}};
char update_time_task_stack[TASKSTACKSIZE];
Task_Struct update_time_task_struct;
Semaphore_Struct semStruct_update_time;

extern UART_Handle gsm_uart;

uint8_t update_time_ex(){
    //gsm_open();

    if(gsm_pb_done(GSM_RETRY_NUM) == ZH_FAIL){
        return ZH_FAIL;
    }
    Task_sleep(HUGE_TIME*3);
    /*if(gsm_is_reg(GSM_RETRY_NUM) == ZH_FAIL){
        return ZH_FAIL;
    }
    if(gsm_connect(GSM_RETRY_NUM) == ZH_FAIL){
        return ZH_FAIL;
    }*/
    if(gsm_open_gps(GSM_RETRY_NUM) == ZH_FAIL){
        return ZH_FAIL;
    }
    if(gsm_gps_info(3) == ZH_FAIL){
        if(gsm_is_reg(GSM_RETRY_NUM) == ZH_FAIL){
            return ZH_FAIL;
        }
        if(gsm_connect(GSM_RETRY_NUM) == ZH_FAIL){
            return ZH_FAIL;
        }
        if(update_time() == ZH_FAIL)
        {
            LOG_MSG("update time fail!\n");
            return ZH_FAIL;
        }
    }

    if(gsm_disconnect(GSM_RETRY_NUM) == ZH_FAIL){
        LOG_MSG("error for gsm disconnect\n");
        return ZH_FAIL;
    }
    LOG_MSG("good for gsm disconnect\n");
    return ZH_OK;
}

void update_time_task(UArg arg){
    Semaphore_Handle semph_handler = (Semaphore_Handle)arg;
    while(1){
        Semaphore_pend(semph_handler, BIOS_WAIT_FOREVER);
        //Semaphore_pend(semaphore_task, BIOS_WAIT_FOREVER);
        /*power on*/
        //gsm_lock();

        gsm_open();

        if(update_time_ex() == ZH_FAIL){
            LOG_MSG("update time ex fail!\n");
        }
//UP_TIME_END:
        gsm_close();

        if(upload_image_ex(0) == ZH_FAIL){
            LOG_MSG("upload images on update time task!\n");
        }
        gsm_close();

        //gsm_unlock();
        Task_sleep(LITTLE_TIME);
        //Semaphore_post(semaphore_task);

    }
}

void init_update_time_task(){

    update_time_task_info.task_notify_semaphore_handler = semaphore_up_time_t;

    Task_Params taskParams;
    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &update_time_task_stack;
    taskParams.priority = 1;
    taskParams.arg0 = (UArg)update_time_task_info.task_notify_semaphore_handler;
    Task_construct(&update_time_task_struct, (Task_FuncPtr)update_time_task, &taskParams, NULL);

}
