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


_time_task_s update_time_task_info = {.task_minutes_num = 1, .task_hours_num = 12, .p_task_hours={0,2,4,6,8,10,12,14,16,18,20,22}, .p_task_minutes={1}};
char update_time_task_stack[TASKSTACKSIZE];
Task_Struct update_time_task_struct;
Semaphore_Struct semStruct_update_time;

extern UART_Handle gsm_uart;

void update_time_task(UArg arg){
    //Semaphore_Handle semph_handler = (Semaphore_Handle)arg;
    while(1){
        Semaphore_pend(update_time_task_info.task_notify_semaphore_handler, BIOS_WAIT_FOREVER);
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
        //Power_setPerformanceLevel(3);
        gsm_open();


        if(gsm_pb_done(10) == 1){
            goto UP_TIME_END;
        }
        Task_sleep(10000);
        if(gsm_is_reg(20) == 1){
            goto UP_TIME_END;
        }
        if(gsm_connect(10) == 1){
            goto UP_TIME_END;
        }
        if(update_time() == 1)
        {
            LOG_MSG("update time fail!\n");
            goto UP_TIME_END;
        }
        if(gsm_disconnect(10) == 1){
            LOG_MSG("error for gsm disconnect\n");
            goto UP_TIME_END;
        }
        LOG_MSG("good for gsm disconnect\n");
UP_TIME_END:
        gsm_close();

        /*power off*/
        reset_relay(RELAY_K_GSM);
        power_disable(ENA_DC5V|ENA_DC33V);
        gsm_unlock();
        Task_sleep(200);
        Semaphore_post(semaphore_task);
        //Power_setPerformanceLevel(0);
    }
}

void init_update_time_task(){
    /*Semaphore_Params semParams;
    Semaphore_Params_init(&semParams);
    semParams.instance->name = "uptime_sem";
    Semaphore_construct(&semStruct_update_time, 0, &semParams);*/
    update_time_task_info.task_notify_semaphore_handler = semaphore_up_time_t;

    Task_Params taskParams;
    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &update_time_task_stack;
    taskParams.priority = 1;
    taskParams.arg0 = (UArg)update_time_task_info.task_notify_semaphore_handler;
    Task_construct(&update_time_task_struct, (Task_FuncPtr)update_time_task, &taskParams, NULL);

}
