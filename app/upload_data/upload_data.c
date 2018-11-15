/*
 * uplpad_data.c
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
#include "upload_data.h"
#include "base/protocol/protocol.h"
#include "base/conf_parser/conf_parser.h"
#include "base/rt_clock/rt_clock.h"
#include "base/power/power.h"
#include "base/gsm/gsm.h"
#include "base/sensors/sensors.h"


_time_task_s upload_data_task_info;
char upload_data_task_stack[TASKSTACKSIZE];
Task_Struct upload_data_task_struct;
Semaphore_Struct semStruct_upload_data;

extern UART_Handle gsm_uart;

uint32_t upload_data_count = 0;

#define UPLOAD_DATA_TIMEOUT 60 //6 min


void upload_data_process(char * str);

void uploaddata_timerCallback(Timer_Handle myHandle){
    if(upload_data_count++>UPLOAD_DATA_TIMEOUT){
        gsm_close();
        /*power off*/
        reset_relay(RELAY_K_GSM);
        power_disable(ENA_DC5V);
        power_disable(ENA_DC33V);
        gsm_unlock();
        SysCtl_rebootDevice();
    }
}
Timer_Handle start_upload_data_timer(){
    Timer_Handle th;
    Timer_Params params;
    params.period = 1000000;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = uploaddata_timerCallback;
    th = Timer_open(Board_TIMER1, &params);
    Timer_start(th);
    upload_data_count =0;
    return th;
}

void stop_upload_data_timer(Timer_Handle th){
    Timer_stop(th);
    Timer_close(th);
}



void upload_data_task(UArg arg){

    //char *pbuf = NULL;
    //uint8_t upload_count = 0;
    Timer_Handle upload_data_th;
    Semaphore_Handle semph_handler = (Semaphore_Handle)arg;
    while(1){
        Semaphore_pend(semph_handler, BIOS_WAIT_FOREVER);
        /*power on*/

        upload_data_th = start_upload_data_timer();
        gsm_open();

        /*if(upload_data_ex() == ZH_FAIL){
            LOG_MSG("upload data fail!\n");
        }*/
        //gsm_lock();
        /*gsm_open();
        if(gsm_pb_done(GSM_RETRY_NUM) == ZH_FAIL){
            goto UP_DATA_END;
        }
        Task_sleep(HUGE_TIME);
        if(gsm_is_reg(GSM_RETRY_NUM) == ZH_FAIL){
            goto UP_DATA_END;
        }
        if(gsm_connect(GSM_RETRY_NUM) == ZH_FAIL){
            goto UP_DATA_END;
        }
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
            goto UP_DATA_END;
        }
        LOG_MSG("good for gsm disconnect\n");
        */
//UP_DATA_END:
        gsm_close();
        stop_upload_data_timer(upload_data_th);

        //gsm_unlock();
        Task_sleep(LITTLE_TIME);
        //Semaphore_post(semaphore_task);
    }
}

void init_upload_data_task(){
    upload_data_task_info.task_notify_semaphore_handler = semaphore_up_data_t;
    Task_Params taskParams;
    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &upload_data_task_stack;
    taskParams.priority = 1;
    taskParams.arg0 = (UArg)upload_data_task_info.task_notify_semaphore_handler;
    Task_construct(&upload_data_task_struct, (Task_FuncPtr)upload_data_task, &taskParams, NULL);

}

void upload_data_process(char * str){
	/*upload command*/
    upload_data_str(str);
}


void config_upload_data_task(char* invl_param){
	if(parse_crontab(invl_param,
                  upload_data_task_info.p_task_minutes,
                  &upload_data_task_info.task_minutes_num,
                  upload_data_task_info.p_task_hours,
                  &upload_data_task_info.task_hours_num))
    {
        LOG_MSG("parse fail!\n");
        return;
    }
}
