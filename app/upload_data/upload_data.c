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
#include "upload_data.h"
#include "base/protocol/protocol.h"
#include "base/conf_parser/conf_parser.h"
#include "base/rt_clock/rt_clock.h"
#include "base/power/power.h"
#include "base/gsm/gsm.h"
#include "base/sensors/sensors.h"
#include <ti/drivers/Timer.h>


_time_task_s upload_data_task_info;
char upload_data_task_stack[TASKSTACKSIZE];
Task_Struct upload_data_task_struct;
Semaphore_Struct semStruct_upload_data;

extern UART_Handle gsm_uart;

void upload_data_process(char * str);

void uploaddata_timerCallback(Timer_Handle myHandle){
    gsm_close();
    //Power_setPerformanceLevel(0);
    /*power off*/
    reset_relay(RELAY_K_GSM);
    power_disable(ENA_DC5V);
    power_disable(ENA_DC33V);
    gsm_unlock();
    SysCtl_rebootDevice();
}


void upload_data_task(UArg arg){
    //Semaphore_Handle semph_handler = (Semaphore_Handle)arg;
    char *pbuf = NULL;
    uint8_t upload_count = 0;
    Timer_Params params;
    Timer_Handle timer1;
    Timer_Params_init(&params);
    params.period = 1000000000;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_ONESHOT_CALLBACK;
    params.timerCallback = uploaddata_timerCallback;
    timer1 = Timer_open(Board_TIMER1, &params);
    while(1){
        Semaphore_pend(upload_data_task_info.task_notify_semaphore_handler, BIOS_WAIT_FOREVER);
        Semaphore_pend(semaphore_task, BIOS_WAIT_FOREVER);
        /*power on*/
        Timer_start(timer1);
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
            goto UP_DATA_END;
        }
        Task_sleep(10000);
        if(gsm_is_reg(20) == 1){
            goto UP_DATA_END;
        }
        if(gsm_connect(10) == 1){
            goto UP_DATA_END;
        }
        for(upload_count = 0;upload_count<MAX_UPLOAD_SIZE;++upload_count){
            pbuf = get_data_str();

            if(!pbuf){
                break;
            }
            LOG_MSG("pbuf:%s\n",pbuf);
            upload_data_process(pbuf);
        }
        if(gsm_disconnect(10) == 1){
            LOG_MSG("error for gsm disconnect\n");
            goto UP_DATA_END;
        }
        LOG_MSG("good for gsm disconnect\n");
UP_DATA_END:
        Timer_stop(timer1);
        gsm_close();
        //Power_setPerformanceLevel(0);
        /*power off*/
        reset_relay(RELAY_K_GSM);
        power_disable(ENA_DC5V);
        power_disable(ENA_DC33V);
        gsm_unlock();
        Task_sleep(200);
        Semaphore_post(semaphore_task);
    }
}

void init_upload_data_task(){
    /*Semaphore_Params semParams;
    Semaphore_Params_init(&semParams);
    semParams.instance->name = "updata_sem";
    Semaphore_construct(&semStruct_upload_data, 0, &semParams);
    */
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
	upload_data_start();

	/*upload data*/
	char *token;
	token = strtok(str," ");
	while(token !=NULL){
	    upload_data_str(token);
		token = strtok(NULL," ");
	}
	upload_data_end();
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
