/*
 * vaisal.c
 *
 *  Created on: 2017Äê10ÔÂ24ÈÕ
 *      Author: saber
 */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>


#include "vaisal.h"

typedef enum {
    HEADER_1 = 0,
    HEADER_2,
    RECIEVE_DATA,
    RECIEVE_END,
}vaisal_status;

UART_Handle vaisal_uart;
UART_Params vaisal_uartParams;

vaisal_data_struct vaisal_datas[TH_DATAS_NUM] = {{0,0,"Dm",0,0},{0,0,"Sm",0,0},{0,0,"Ta",0,0},{0,0,"Ua",0,0},{0,0,"Pa",0,0}};


void reset_vaisal_datas(){
    uint32_t i = 0;
    for(i=0;i<TH_DATAS_NUM;++i){
        vaisal_datas[i].value = 0;
        vaisal_datas[i].pv = 0;
        vaisal_datas[i].completed = 0;
    }
}

uint8_t vaisal_open(void){
	UART_Params_init(&vaisal_uartParams);
    vaisal_uartParams.writeDataMode = UART_DATA_BINARY;
    vaisal_uartParams.readDataMode = UART_DATA_BINARY;
    vaisal_uartParams.readReturnMode = UART_RETURN_FULL;
    vaisal_uartParams.readEcho = UART_ECHO_OFF;
    vaisal_uartParams.baudRate = 9600;

    vaisal_uart = UART_open(Board_UART_485_0, &vaisal_uartParams);
    if (vaisal_uart == NULL) {
        LOG_MSG("vaisal uart open fail!\n");
    	return 1;
    }
    return 0;
}

void vaisal_close(){
	UART_close(vaisal_uart);
	vaisal_uart = NULL;
}

uint8_t va_res_buffer[SENSOR_BUFFER];
uint8_t va_pbuffer=0;

uint8_t vaisal_fcn(){
	char *token,*strstrn;
    uint32_t i = 0;
    token=strtok(va_res_buffer,",");
    while(token !=NULL){

        token=strtok(NULL,",");

        for(i = 0;i<TH_DATAS_NUM;++i)
        {
            strstrn=strstr(token,vaisal_datas[i].sign);
            if(*strstrn){
                vaisal_datas[i].value+=atof(token+3);
                if(++vaisal_datas[i].pv == MAX_REV_NUM){
                    vaisal_datas[i].avg_value =vaisal_datas[i].value/vaisal_datas[i].pv;
                    vaisal_datas[i].value = 0;
                    vaisal_datas[i].pv = 0;
                    vaisal_datas[i].completed = 1;
                }
            }
        }
    }
    uint8_t all_completed = 1;
    for(i = 0;i<TH_DATAS_NUM;++i){
        all_completed&=vaisal_datas[i].completed;
    }
    return all_completed;
}


uint8_t vaisal_process(){
	uint8_t res;
	vaisal_status vaisal_stat = HEADER_1;
	uint16_t count = 0;
	for(;count<300;count++){
		UART_read(vaisal_uart, &res, 1);
		switch(vaisal_stat){
			case HEADER_1:

	            if(res == '0'){
	                vaisal_stat = HEADER_2;
	                va_res_buffer[va_pbuffer++] = res;
	            }
	            break;
	        case HEADER_2:
	            if(res == 'R'){
	                vaisal_stat = RECIEVE_DATA;
	                va_res_buffer[va_pbuffer++] = res;
	            }
	            else{
	                vaisal_stat = HEADER_1;
	                va_pbuffer = 0;
	            }
	            break;
	        case RECIEVE_DATA:
	            if(res == 0x0d){
	                va_res_buffer[va_pbuffer++] = res;
	                vaisal_stat = RECIEVE_END;
	            }else{
	                va_res_buffer[va_pbuffer++] = res;
	            }
	            break;
	        case RECIEVE_END:
	            if(res == 0x0a){
	                va_res_buffer[va_pbuffer++] = res;
	                va_res_buffer[va_pbuffer++] = '\0';
	                //deal with the data format
	                vaisal_stat = HEADER_1;

	                if(vaisal_fcn()){
	                    //finished
	                    return 0;

	                }
	                memset(va_res_buffer,'\0',sizeof(va_res_buffer));
	                va_pbuffer=0;
	            }
	            else{
	                vaisal_stat = HEADER_1;
	                va_pbuffer = 0;
	            }
	            break;
	        default:
	            break;
	    }
	}
	return 1;
}
