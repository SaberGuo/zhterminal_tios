/*
 * gsm.c
 *
 *  Created on: 2017Äê10ÔÂ24ÈÕ
 *      Author: saber
 */
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include <ti/sysbios/BIOS.h>
#include <xdc/cfg/global.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "base/power/power.h"
#include "base/rt_clock/rt_clock.h"
#include "gsm.h"

#define AT_CIPSEND "AT+CIPSEND=0,\r\n"
#define AT_CIPSEND_SIZE "AT+CIPSEND=0,%d\r\n"
char *AT_IPR="AT+IPR=921600\r\n";
char *AT_CGSOCKCONT_IP_COM="AT+CGSOCKCONT=1,\"IP\",\"cmnet\"\r\n";
char *AT_CSOCKSETPN="AT+CSOCKSETPN=1\r\n";
char *AT_NETOPEN="AT+NETOPEN\r\n";
char *AT_NETCLOSE="AT+NETCLOSE\r\n";
char *AT_CIPCLOSE="AT+CIPCLOSE=0\r\n";

char *AT_CNMI="AT+CNMI=2,2\r\n";
char *AT_CREG="AT+CREG?\r\n";
char *AT_GPSHOT="AT+CGPS=1,2\r\n";
char *AT_GPSINFO="AT+CGPSINFO\r\n";
char *AT_CIPHEAD="AT+CIPHEAD=1\r\n";
char *AT_CIPSRIP="AT+CIPSRIP=0\r\n";
char *AT_CSCLK="AT+CSCLK=1\r\n";
char *AT_CGSOCKCONT="AT+CGSOCKCONT=1,\"IP\",\"cmnet\"\r\n";
char *AT_CIPOPEN_TCP="AT+CIPOPEN=0,\"TCP\",\"%s\",%s\r\n";
char *AT_CIPCCFG="AT+CIPCCFG=3,500,0,1,1,0,500\r\n";
char *AT_DATA_MODE="AT+CIPMODE=1\r\n";

char *IP="120.77.34.29";
char *COM="8800";

char AT_END=0x1A;

UART_Handle gsm_uart;
UART_Params gsm_uartParams;
Semaphore_Struct gsm_semStruct;
Semaphore_Handle gsm_semHandle;
Semaphore_Params gsm_semParams;

char gsm_rec_buffer[MAX_BUFFER];

uint16_t gsm_rec_index = 0;

char* msg_recieve_size(uint32_t size);

void _msg_send(char *message)
{
    UART_write(gsm_uart, message, strlen(message));
}

void _msg_send_onsize(char *message, uint32_t size){
    UART_write(gsm_uart, message, size);
}
uint8_t _start_msg_send_atsize(uint32_t size){
    char *pres;
    char buf[30];
    sprintf(buf,AT_CIPSEND_SIZE,size);
    _msg_send(buf);
    pres = msg_at_recieve(0);//recieve send msg
    pres = msg_at_recieve(0);//recieve >
    if(!strstr(pres,">")){
        LOG_MSG("error for start msg send,and recieved msg:%s",pres);
        return 1;
    }
    return 0;
}
uint8_t _start_msg_send(){
    char *pres;
    _msg_send(AT_CIPSEND);
    pres = msg_at_recieve(0);//recieve send msg
    pres = msg_at_recieve(0);//recieve >
    if(!strstr(pres,">")){
        LOG_MSG("error for start msg send,and recieved msg:%s",pres);
        return 1;
    }
    return 0;
}

void _stop_msg_send(){
    UART_write(gsm_uart,&AT_END,1);
}


uint8_t _open_net(){
    char *pres;
	_msg_send(AT_NETOPEN);
	pres = msg_at_recieve(0);//recieve send msg
	pres = msg_at_recieve(0);//recieve OK
	if(strstr(pres,"OK")){
	    pres = msg_at_recieve(0);// recieve 0d0a
	    pres = msg_at_recieve(0);//recieve +netopen:0
	    if(!(pres = strstr(pres,"+NETOPEN:"))){
	        return 0;
	    }
	    return atoi(pres+strlen("+NETOPEN:"));
	}
	if(strstr(pres,"opened")){
	    pres = msg_at_recieve(0);//recieve ERROR
	    return 0;
	}
	return 1;


}
uint8_t _close_net(){
	_msg_send(AT_NETCLOSE);
	return ZH_OK;

}

uint8_t _set_gsm_net(char *ip,char *com, uint8_t is_datamode)
{
    char *pres;
    char ip_buffer[50];
    sprintf((char*)ip_buffer,AT_CIPOPEN_TCP,ip,com);
    _msg_send(ip_buffer);
    if(is_datamode == 0){
        pres = msg_at_recieve(0);//recieve send msg
        pres = msg_at_recieve(0);//recieve OK
        if(!strstr(pres,"OK")){
            LOG_MSG("error for close net,and recieved msg:%s",pres);
            return ZH_FAIL;
        }
        pres = msg_at_recieve(0);//recieve 0d0a
        pres = msg_at_recieve(0);//recieve +cipopen:0,0
        if(!strstr(pres,"+CIPOPEN")){
            LOG_MSG("error for CIPOPEN:%s",pres);
            return ZH_FAIL;
        }
        if(!(pres = strstr(pres,","))){
            LOG_MSG("error for ,:%s",pres);
            return ZH_FAIL;
        }
        return atoi(pres+strlen(","));
    }
    if(is_datamode == 1){
        pres = msg_at_recieve(100);//recieve send msg
        pres = msg_at_recieve(30);//recieve CONNECT 115200
        if(pres && strstr(pres,"CONNECT")){
            return ZH_OK;
        }
        else{
            LOG_MSG("error for connect data mode,and recieved msg:%s",pres);
            return ZH_FAIL;
        }
        return ZH_OK;
    }
}
void msg_send_data_mode(char *message){
    _msg_send(message);
}

uint8_t msg_send(char *message){
    if(_start_msg_send() == ZH_FAIL){
        LOG_MSG("start msg send error\n");
        return ZH_FAIL;
    }
    _msg_send(message);

    _stop_msg_send();
    msg_at_recieve(0);
    while(!strstr(msg_at_recieve(0),"+CIPSEND"));

    return ZH_OK;
}
void msg_send_onsize_data_mode(char *message, uint32_t size){
    _msg_send_onsize(message, size);
}

uint8_t msg_send_onsize(char *message, uint32_t size){
    if(_start_msg_send_atsize(size) == ZH_FAIL){
        LOG_MSG("start msg send error\n");
        return ZH_FAIL;
    }
    _msg_send_onsize(message, size);

    while(!strstr(msg_at_recieve(0),"+CIPSEND"));

    return ZH_OK;
}

char* msg_recieve(){
    //char * at_msg = msg_at_recieve(0);//recieve msg about: recv from:[IP]:[PORT]
    uint32_t size;
    char* at_msg = msg_at_recieve(0);
    if(strstr(at_msg, "+IPD")){
        LOG_MSG("pre size is %s\n",at_msg+strlen("+IPD"));
        size = atoi(at_msg+strlen("+IPD"));
        LOG_MSG("size is %d\n",size);
        return msg_recieve_size(size);
    }
    else{
        return NULL;
    }
}

char* msg_recieve_size(uint32_t size){
    UART_read(gsm_uart, gsm_rec_buffer, size);
    *(gsm_rec_buffer+size) = '\0';
    LOG_MSG(gsm_rec_buffer);
    return gsm_rec_buffer;
}
char* msg_dm_recieve(uint16_t max_buf){
    for(gsm_rec_index=0;gsm_rec_index<max_buf-1;gsm_rec_index++){
         UART_read(gsm_uart, &gsm_rec_buffer[gsm_rec_index], 1);
         if(gsm_rec_buffer[gsm_rec_index] == 0x03){
             gsm_rec_buffer[gsm_rec_index] = '\0';
             LOG_MSG(gsm_rec_buffer);
             LOG_MSG("\n");
             return gsm_rec_buffer;
         }
    }
    return NULL;
}
uint8_t msg_at_res = 0;

char* msg_at_recieve(int count){
    if(count == 0){
        count = MAX_BUFFER-1;
    }

    for(gsm_rec_index=0;gsm_rec_index<count;gsm_rec_index++){
        msg_at_res = UART_read(gsm_uart, &gsm_rec_buffer[gsm_rec_index], 1);
         if(msg_at_res==1 && (gsm_rec_buffer[gsm_rec_index] == 0x0a ||gsm_rec_buffer[gsm_rec_index] == 0x03|| gsm_rec_buffer[gsm_rec_index] == '>')){
             gsm_rec_buffer[gsm_rec_index+1] = '\0';
             LOG_MSG("rec at:");
             LOG_MSG(gsm_rec_buffer);
             return gsm_rec_buffer;
         }
    }
    return NULL;
}
uint8_t gsm_connect_data_mode(uint8_t count){
    uint8_t res = 0,i = 0;
    for(i = 0;i<count;++i){
        if((res = _open_net()) !=0){
            LOG_MSG("error for _open_net,code:%d\n",res);
            continue;
        }
        LOG_MSG("open net done\n");
        if(0!=(res = _set_gsm_net((char*)IP,(char*)COM, 1))){
            LOG_MSG("error for _set_gsm_net data mode,code:%d\n",res);
            continue;
        }
        return ZH_OK;
    }
    return ZH_FAIL;
}

uint8_t gsm_ipconfig(uint8_t count){
    char * pres;
    uint8_t res = 0,i = 0;
    for(i = 0;i<count;++i){
        _msg_send(AT_CIPCCFG);
        //UART_read(gsm_uart, &res, 1);
        pres = msg_at_recieve(0);//rec send msg
        pres = msg_at_recieve(0);//rec OK
        if(!strstr(pres,"OK")){
            LOG_MSG("error for at ciphead,and recieved msg:%s",pres);
            continue;
        }
        LOG_MSG("AT_CIPCCFG done\n");
        return ZH_OK;
    }
    return ZH_FAIL;
}
uint8_t gsm_connect(uint8_t count){
    char * pres;
    uint8_t res = 0,i = 0;
    for(i = 0;i<count;++i){
        if((res = _open_net()) !=0){
            LOG_MSG("error for _open_net,code:%d\n",res);
            continue;
        }
        LOG_MSG("open net done\n");
        _msg_send(AT_CIPHEAD);
        pres = msg_at_recieve(0);//rec send msg
        pres = msg_at_recieve(0);//rec ok
        if(!strstr(pres,"OK")){
            LOG_MSG("error for at ciphead,and recieved msg:%s",pres);

            continue;
        }
        LOG_MSG("AT_CIPHEAD done\n");
        _msg_send(AT_CIPSRIP);
        pres = msg_at_recieve(0);//rec send msg
        pres = msg_at_recieve(0);//rec ok
        if(!strstr(pres,"OK")){
            LOG_MSG("error for cipsrip,and recieved msg:%s",pres);
            continue;
        }
        LOG_MSG("AT_CIPSRIP done\n");
        if(0!=(res = _set_gsm_net((char*)IP,(char*)COM, 0))){
            LOG_MSG("error for set_gsm_net,code:%d\n",res);
            continue;
        }

        LOG_MSG("_set_gsm_net done\n");
        return ZH_OK;
    }
    return ZH_FAIL;
	
}
uint8_t gsm_to_data_mode(uint8_t count){
    uint8_t i = 0;
    char * pres;
    for(;i<count;++i){
        _msg_send(AT_DATA_MODE);
        pres = msg_at_recieve(0);//rec send msg
        pres = msg_at_recieve(0);//rec OK
        if(!strstr(pres, "OK")){
            continue;
        }
        return ZH_OK;
    }
    return ZH_FAIL;
}


uint8_t gsm_disconnect(uint8_t count){
    uint8_t i = 0;
    for(;i<count;++i){
        if(0 == _close_net()){
            return ZH_OK;
        }
    }
	return ZH_FAIL;
}

uint8_t gsm_is_reg(uint8_t count){
    uint8_t res,i = 0;
    char * pres;
    for(;i<count;++i){

        _msg_send(AT_CREG);
        //UART_read(gsm_uart, &res, 1);
        pres = msg_at_recieve(0);//rec send msg
        pres = msg_at_recieve(0);//rec +CREG:1,1
        if(!strstr(pres,"CREG")){
            continue;
        }
        pres = strstr(pres,",");
        if(!pres){
            continue;
        }
        res = atoi(pres+strlen(","));
        pres = msg_at_recieve(0);//rec 0d 0a
        pres = msg_at_recieve(0);//rec OK
        if(res == 1){
            return ZH_OK;
        }
        Task_sleep(5000);

    }
    return ZH_FAIL;
}

uint8_t gsm_pb_done(uint8_t count){
    char *pres = NULL;
    uint8_t i = 0;
    for(;i<count;i++){
        pres = msg_at_recieve(0);//rec commond
        if(strstr(pres,"PB DONE")){
            return ZH_OK;
        }
    }
    return ZH_FAIL;
}



void gsm_lock(){
    //Semaphore_pend(semaphore_gsm, BIOS_WAIT_FOREVER);
}
void gsm_unlock(){
    //Semaphore_post(semaphore_gsm);
}

uint8_t gsm_open_gps(uint8_t count){
    uint8_t res,i = 0;
    char * pres;
    for(;i<count;++i){

        _msg_send(AT_GPSHOT);
        //UART_read(gsm_uart, &res, 1);
        pres = msg_at_recieve(0);//rec send msg: AT+CGPSHOT
        pres = msg_at_recieve(0);//rec OK
        if(strstr(pres,"OK")){
            return ZH_OK;
        }

        Task_sleep(5000);

    }
    return ZH_FAIL;
}
#define GPS_DATAS_NUM 6
float gsm_gps_lat, gsm_gps_lon;
int gsm_year, gsm_month,gsm_day,gsm_hour,gsm_minute, gsm_second;

uint8_t gsm_gps_info(uint8_t count){
    uint8_t res,i = 0;
    char *pres;
    char *token;
    char *pinfo;
    char pres_info[128];
    char datetime_str[3];
    for(;i<count;++i){

        _msg_send(AT_GPSINFO);

        pres = msg_at_recieve(0);//rec send msg: AT+CGPSINFO
        pres = msg_at_recieve(0);//rec CGPSINFO:
        memcpy(pres_info, pres, strlen(pres));
        LOG_MSG(pres_info);
        pres = msg_at_recieve(0);//rec \r\n
        pres = msg_at_recieve(0);//rec OK
        pinfo = strstr(pres_info,"+CGPSINFO:")+1;
        LOG_MSG("pinfo:");
        LOG_MSG(pinfo);
        LOG_MSG("\r\n");
        token=strtok(pinfo,",");
        if(token !=NULL){
            //lat
            token=token+10;
            LOG_MSG("token:");
            LOG_MSG(token);
            LOG_MSG("\r\n");
            //token=strtok(NULL,",");

            if(strlen(token)<3){
                res = ZH_FAIL;
                Task_sleep(20000);
                continue;
            }
            if(token){
                memcpy(datetime_str, token,2);
                gsm_gps_lat = atof(token+2)/60.0+atoi(datetime_str);
                memset(datetime_str,0, sizeof(datetime_str));
            }
            //lat
            token=strtok(NULL,",");
            if(strlen(token)==0){
                res = ZH_FAIL;
                Task_sleep(20000);
                continue;
            }
            if(token){
                if(strstr(token, "S")){
                    gsm_gps_lat = -gsm_gps_lat;
                }
            }
            //lon
            token=strtok(NULL,",");
            if(strlen(token)==0){
                res = ZH_FAIL;
                Task_sleep(20000);
                continue;
            }
            if(token){
                memcpy(datetime_str, token,3);
                gsm_gps_lon = atof(token+3)/60.0+atoi(datetime_str);
                memset(datetime_str,0, sizeof(datetime_str));
            }
            //lon
            token=strtok(NULL,",");
            if(token){
                if(strstr(token, "W")){
                    gsm_gps_lon = -gsm_gps_lon;
                }
            }
            //date
            token=strtok(NULL,",");
            if(strlen(token)==0){
                res = ZH_FAIL;
                Task_sleep(20000);
                continue;
            }
            if(token){
                memcpy(datetime_str, token,2);
                gsm_day = atoi(datetime_str);
                memcpy(datetime_str, token+2,2);
                gsm_month = atoi(datetime_str);
                memcpy(datetime_str, token+4,2);
                gsm_year = atoi(datetime_str)+2000;
            }
            //time
            token=strtok(NULL,",");
            if(strlen(token)==0){
                res = ZH_FAIL;
                Task_sleep(20000);
                continue;

            }
            if(token){
                memcpy(datetime_str, token,2);
                gsm_hour = atoi(datetime_str);
                memcpy(datetime_str, token+2,2);
                gsm_minute = atoi(datetime_str);
                memcpy(datetime_str, token+4,2);
                gsm_second = atoi(datetime_str);
                res = ZH_OK;
            }


        }
        if(strstr(pres, "OK") && res == ZH_OK){
            set_sencods_gps(gsm_year, gsm_month, gsm_day, gsm_hour, gsm_minute, gsm_second);
            return ZH_OK;
        }

        Task_sleep(20000);
    }
    return ZH_FAIL;
}

void init_gsm(){
    UART_Params_init(&gsm_uartParams);
    gsm_uartParams.writeDataMode = UART_DATA_BINARY;
    gsm_uartParams.readDataMode = UART_DATA_BINARY;
    gsm_uartParams.readTimeout = 2000;
    gsm_uartParams.readReturnMode = UART_RETURN_FULL;
    gsm_uartParams.readEcho = UART_ECHO_OFF;
    gsm_uartParams.baudRate = 230400;
}
uint8_t gsm_open(){
    Task_sleep(200);
    GPIO_write(Board_GPIO_POWER_KEY, Board_GPIO_LOW);
    GPIO_write(Board_GPIO_DTR, Board_GPIO_LOW);
    power_enable(ENA_DC5V);
    power_enable(ENA_DC33V);
    set_relay(RELAY_K_GSM);

    GPIO_write(Board_GPIO_POWER_KEY, Board_GPIO_HIGH);
    delay(100000);
    GPIO_write(Board_GPIO_POWER_KEY, Board_GPIO_LOW);
    if(gsm_uart == NULL){
        gsm_uart = UART_open(Board_UART_GSM, &gsm_uartParams);
    }
    if (gsm_uart == NULL) {
        LOG_MSG("gsm uart open fail!\n");
    	return ZH_FAIL;
    }
    return ZH_OK;
}
void gsm_port_clear(){
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN3);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN3);
}
void gsm_close(){
    if(gsm_uart !=NULL){
        UART_close(gsm_uart);
        gsm_uart = NULL;
    }
    gsm_port_clear();
    reset_relay(RELAY_K_GSM);
    //power_disable(ENA_DC5V);
    //power_disable(ENA_DC33V);

}
