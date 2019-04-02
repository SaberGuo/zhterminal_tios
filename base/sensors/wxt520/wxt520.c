/*
 * vaisal.c
 *
 *  Created on: 2017Äê10ÔÂ24ÈÕ
 *      Author: saber
 */
#include <base/sensors/wxt520/wxt520.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include "../uart/uart.h"


typedef enum {
    HEADER_1 = 0,
    HEADER_2,
    RECIEVE_DATA,
    RECIEVE_END,
}vaisal_status;

UART_Handle vaisal_uart;
UART_Params* wxt520_uartParams = NULL;

vaisal_data_struct vaisal_datas[TH_DATAS_NUM] = {{0,0,"Dm",0,0},{0,0,"Sm",0,0},{0,0,"Ta",0,0},{0,0,"Ua",0,0},{0,0,"Pa",0,0}};



void reset_vaisal_datas(){
    uint32_t i = 0;
    for(i=0;i<TH_DATAS_NUM;++i){
        vaisal_datas[i].value = 0;
        vaisal_datas[i].pv = 0;
        vaisal_datas[i].completed = 0;
    }
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
                vaisal_datas[i].value=atof(token+3);
                //vaisal_datas[i].value+=atof(token+3);
                //if(++vaisal_datas[i].pv == MAX_REV_NUM){
                //    vaisal_datas[i].avg_value =vaisal_datas[i].value/vaisal_datas[i].pv;
                //    vaisal_datas[i].value = 0;
                //    vaisal_datas[i].pv = 0;
                //    vaisal_datas[i].completed = 1;
                //}
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
	va_pbuffer = 0;
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

	                vaisal_fcn();
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
	return ZH_FAIL;
}
uint8_t wxt520_open(uint8_t num){
    if(wxt520_uartParams == NULL){
        wxt520_uartParams = (UART_Params*)malloc(sizeof(UART_Params));
        UART_Params_init(wxt520_uartParams);
        wxt520_uartParams->writeDataMode = UART_DATA_BINARY;
        wxt520_uartParams->readDataMode = UART_DATA_BINARY;
        wxt520_uartParams->readReturnMode = UART_RETURN_FULL;
        wxt520_uartParams->readEcho = UART_ECHO_OFF;
        wxt520_uartParams->readTimeout = 1000;
        wxt520_uartParams->baudRate = 9600;
    }
    return serial_port_open(num, wxt520_uartParams);
}
uint8_t wxt520_close(uint8_t num){
    serial_port_close(num);
    reset_vaisal_datas();
    return ZH_OK;
}
uint8_t wxt520_process(uint8_t num){
    return vaisal_process();
}
float wxt520_get_data(uint8_t num, uint8_t key_num){
    return vaisal_datas[key_num].value;
}
