/*
 * self_cam.c
 *
 *  Created on: 2017Äê10ÔÂ24ÈÕ
 *      Author: saber
 */

#include <base/camera/hisi/hisi.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <file.h>
#include <ti/drivers/Timer.h>

#include "base/rt_clock/rt_clock.h"
#include "base/network/wiznet.h"
#include "base/network/Ethernet/socket.h"
#include "base/fatsd/fatsd.h"
#include "base/power/power.h"
#define CAMERA_SOCK 0
#define ANY_PORT 50000
#define DATA_BUF_SIZE 1024

#define SELF_CAM_PROCESS 0
#define SELF_CAM_FINISH 1
#define SELF_CAM_FAIL 2

#define SELF_CAM_TIMEOUT (100)

#define IP_MAX_NUM 4

#define HISI_TIMEOUT 120

uint8_t c_des_ip[IP_MAX_NUM][4] = {{192,168,1,10},{192,168,1,11},{192,168,1,12},{192,168,1,13}};
uint8_t c_socket_num[IP_MAX_NUM] = {0,0,0,0};
uint16_t c_des_port = 4444;



typedef enum{
    START = 0,
    IMAGE_CAPUTED,
    IMAGE_SIZE_REVING,
    IMAGE_RECIEVING,
    IMAGE_RECIEVED
}camera_process_type;

camera_process_type camera_stat = START;


FILE *fimg;
int32_t image_size = 0;
uint8_t flag_hisi = 0;

uint32_t hisi_count = 0;

void hisi_timerCallback(Timer_Handle myHandle){
    if(hisi_count++>HISI_TIMEOUT){
        flag_hisi = 1;
        /*if(fimg!=NULL){
            fclose(fimg);
            fimg = NULL;
        }
        camera_close();
        //close_sd();
        disable_sd();
        reset_relay(RELAY_K_CAMERA);
        SysCtl_rebootDevice();*/
    }
}

Timer_Handle start_hisi_timer(){
    Timer_Handle th;
    Timer_Params params;
    params.period = 1000000;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = hisi_timerCallback;
    th = Timer_open(Board_TIMER0, &params);
    Timer_start(th);
    hisi_count = 0;
    flag_hisi = 0;
    return th;
}

void stop_hisi_timer(Timer_Handle th){
    Timer_stop(th);
    Timer_close(th);
}

int32_t hisi_tcpc_run(uint8_t num, char *buffer,char *file_name, uint8_t *sign, time_t start_sec, uint32_t *psize){
    int32_t ret; // return value for SOCK_ERRORs
    uint16_t size = 0;
    uint32_t write_size = 0;
    image_size = 0;
    int32_t rev_image_size = 0;
    uint8_t count = 0;
    time_t end_sec;

    switch(getSn_SR(c_socket_num[num])){
        case SOCK_ESTABLISHED:
            if(getSn_IR(c_socket_num[num]) & Sn_IR_CON)   // Socket n interrupt register mask; TCP CON interrupt = connection with peer is successful
            {
                setSn_IR(c_socket_num[num], Sn_IR_CON);  // this interrupt should be write the bit cleared to '1'
            }
            ret = send(c_socket_num[num], "image_capture", strlen("image_capture"));
            if( ret <0){
                sclose(c_socket_num[num]); // socket close
                return ret;
            }
            camera_stat = START;
            while(1){
                //end_sec = get_seconds();
                //if(end_sec - start_sec> SELF_CAM_TIMEOUT){
                //    LOG_MSG("inner timeout!%d,%d\n",end_sec,start_sec);
                //    return SELF_CAM_FAIL;
                //}
                if(flag_hisi == 1){
                    LOG_MSG("inner timeout!\n");
                    if(fimg!=NULL){
                        fclose(fimg);
                        fimg = NULL;
                    }
                    sclose(c_socket_num[num]);
                    *sign = SELF_CAM_FAIL;
                    return ret;
                }
                if((size = getSn_RX_RSR(c_socket_num[num])) > 0) // Sn_RX_RSR: Socket n Received Size Register, Receiving data length
                {
                    if(size > DATA_BUF_SIZE) size = DATA_BUF_SIZE; // DATA_BUF_SIZE means user defined buffer size (array)
                    ret = recv(c_socket_num[num], buffer, size); // Data Receive process (H/W Rx socket buffer -> User's buffer)
                    size = ret;
                    if(ret <= 0) return ret;

                    switch(camera_stat){
                        case START:
                            if(strcmp(buffer, "image_captured") == 0)
                            {
                                ret = send(c_socket_num[num], "image_size", strlen("image_size")+1);
                                if( ret <0){
                                    sclose(c_socket_num[num]); // socket close
                                    //*sign = SELF_CAM_FAIL;
                                    return ret;
                                }
                                if(ret == strlen("image_size")+1){
                                    camera_stat = IMAGE_SIZE_REVING;
                                    rev_image_size = 0;
                                }
                            }
                            break;
                        case IMAGE_SIZE_REVING:
                            /*if(strcmp(buffer, "image_captured") == 0){
                                continue;
                            }*/
                            image_size = atoi(buffer);
                            LOG_MSG("image size is %d\n", image_size);
                            ret = send(c_socket_num[num], "image_recieving", strlen("image_recieving"));
                            if( ret <0 || image_size <=0){
                                sclose(c_socket_num[num]); // socket close
                                //*sign = SELF_CAM_FAIL;
                                return ret;
                            }
                            else{

                                camera_stat = IMAGE_RECIEVING;
                                do{
                                    fimg = fopen(file_name, "w");
                                    if(fimg == NULL){
                                        sclose(c_socket_num[num]);
                                        LOG_MSG("open file failed!%d\n",ret);
                                        *sign = SELF_CAM_FAIL;
                                        return ret;
                                    }
                                    count++;
                                }while(fimg == NULL && count<3);
                            }
                            break;
                        case IMAGE_RECIEVING:
                            //LOG_MSG("image file recieved size:%d\n", rev_image_size);
                            if(rev_image_size<image_size){
                                write_size = fwrite(buffer,1,size,fimg);
                                rev_image_size+=write_size;
                                if(rev_image_size == image_size){
                                    *psize = rev_image_size;
                                    LOG_MSG("image received!\n");
                                    fclose(fimg);

                                    camera_stat = IMAGE_RECIEVED;
                                    ret = send(c_socket_num[num], "image_recieved", strlen("image_recieved"));
                                    sclose(c_socket_num[num]);
                                    *sign = SELF_CAM_FINISH;
                                    return ret;
                                }
                                else{
                                    ret = send(c_socket_num[num], "image_recieving", strlen("image_recieving"));
                                    if(ret == strlen("image_recieving") && image_size>0){
                                        camera_stat = IMAGE_RECIEVING;
                                    }
                                }
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
        case SOCK_CLOSE_WAIT:
            if((ret=disconnect(c_socket_num[num])) != SOCK_OK) return ret;
            break;
        case SOCK_INIT:
            if( (ret = connect(c_socket_num[num], c_des_ip[num], c_des_port)) != SOCK_OK) return ret;  //  Try to TCP connect to the TCP server (destination)
            break;
        case SOCK_CLOSED:
            sclose(c_socket_num[num]);
            if((ret=socket(c_socket_num[num], Sn_MR_TCP, ANY_PORT, 0x00)) != c_socket_num[num]) return ret; // TCP socket open with 'any_port' port number
            break;
        default:
            break;

    }
    return ret;
}

uint8_t hisi_capture(uint8_t num, char *pbuffer, char *file_name, uint32_t *psize){
    int8_t sign = SELF_CAM_PROCESS;
    time_t start_sec = get_seconds();
    Timer_Handle hisi_th;
    hisi_th = start_hisi_timer();
    //time_t end_sec;
    while(sign == SELF_CAM_PROCESS){
        hisi_tcpc_run(num, pbuffer,file_name,&sign,start_sec, psize);
        //end_sec = get_seconds();
        //if(end_sec - start_sec> SELF_CAM_TIMEOUT){
        //    LOG_MSG("timeout!%d,%d\n",end_sec,start_sec);
        //    break;
        //}
    }
    stop_hisi_timer(hisi_th);
    if(sign == SELF_CAM_FINISH){
        return ZH_OK;
    }
    return ZH_FAIL;
}

/*port_num start from 0*/
uint8_t hisi_open(uint8_t num){
    wiznet_init();
    wiznet_reset();
    wiznet_open();
    wiznet_config();
    return ZH_OK;
}


uint8_t hisi_process(uint8_t num, char * file_name, char *pbuffer, uint32_t* psize){
    time_t sec = get_seconds();
    sprintf(file_name, "%s%u%d%s",image_file_pre, sec/1000,num, image_file_bre);
    return hisi_capture(num, pbuffer, file_name, psize);
}

uint8_t hisi_close(uint8_t num){
    wiznet_close();
    return ZH_OK;
}


