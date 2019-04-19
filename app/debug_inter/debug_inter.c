/*
 * debug_inter.c
 *
 *  Created on: 2019Äê4ÔÂ12ÈÕ
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
#include "debug_inter.h"


typedef enum {
    HEADER = 0,
    RECIEVE_DATA,
    RECIEVE_END,
}debug_status;


char debug_inter_task_stack[TASKSTACKSIZE];
Task_Struct debug_inter_task_struct;
UART_Handle debug_uart_handler;
UART_Params* debug_uartParams = NULL;
char data_buffer[MAX_DM_BUFFER];


void init_debug_uart(){
    if(debug_uartParams == NULL){
        debug_uartParams = (UART_Params*)malloc(sizeof(UART_Params));
        UART_Params_init(debug_uartParams);
        debug_uartParams->writeDataMode = UART_DATA_BINARY;
        debug_uartParams->readDataMode = UART_DATA_BINARY;
        debug_uartParams->readReturnMode = UART_RETURN_FULL;
        debug_uartParams->readEcho = UART_ECHO_OFF;
        debug_uartParams->readTimeout = 1000;
        debug_uartParams->baudRate = 19200;
    }
    debug_uart_handler = UART_open(Board_UART_DEBUG, debug_uartParams);
}

void command_fcn(){
    char *token,*command,*params,*strstrn;
    uint32_t i = 0;
    token=strtok(data_buffer,",");
    command=strtok(NULL,",");
    params=strtok(NULL,",");

}

void debug_inter_task(UArg arg){
    uint8_t index = 0;
    debug_status ds = HEADER;
    char res;
    while(1){
        UART_read(debug_uart_handler,&res,1);
        switch(ds){
        case HEADER:
            if(res == '$'){
                ds = RECIEVE_DATA;
                data_buffer[index++] = res;
            }
            break;
        case RECIEVE_DATA:
            if(res == 0x0d){
                ds = RECIEVE_END;
            }
            data_buffer[index++] = res;
            break;
        case RECIEVE_END:
            if(res == 0x0a){

                data_buffer[index++] = res;
                data_buffer[index++] = '\0';
                command_fcn();

            }
            memset(data_buffer,'\0',sizeof(data_buffer));
            index=0;
            ds = HEADER;

            break;
        }

    }
}


void init_debug_inter_task(){

    Task_Params taskParams;
    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &debug_inter_task_stack;
    taskParams.priority = 1;

    Task_construct(&debug_inter_task_struct, (Task_FuncPtr)debug_inter_task, &taskParams, NULL);
}

