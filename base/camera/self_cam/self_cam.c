/*
 * self_cam.c
 *
 *  Created on: 2017Äê10ÔÂ24ÈÕ
 *      Author: saber
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <file.h>

#include "base/rt_clock/rt_clock.h"
#include "self_cam.h"
#include "base/network/wiznet.h"
#include "base/network/Ethernet/socket.h"
#include "base/fatsd/fatsd.h"

#define CAMERA_SOCK 0
#define ANY_PORT 50000
#define DATA_BUF_SIZE 1024

#define SELF_CAM_PROCESS 0
#define SELF_CAM_FINISH 1
#define SELF_CAM_FAIL 2

#define SELF_CAM_TIMEOUT (3000)

uint8_t c_des_ip[4] = {192,168,1,10};
uint16_t c_des_port = 4444;



typedef enum{
    START = 0,
    IMAGE_CAPUTED,
    IMAGE_SIZE_REVING,
    IMAGE_RECIEVING,
    IMAGE_RECIEVED
}camera_process_type;

camera_process_type camera_stat = START;

char pimgfile[100];
FILE *fimg;
int32_t image_size = 0;

void self_cam_init(char *image_file_name){
    memcpy(pimgfile,image_file_name,strlen(image_file_name));
    wiznet_init();
    wiznet_reset();
    wiznet_open();

    wiznet_config();
}

void self_cam_close(){
    if(fimg!=NULL){
        fclose(fimg);
    }
    wiznet_close();
}

int32_t get_self_image_size(){
    return image_size;
}
int32_t self_cam_tcpc_run(char *buffer, uint8_t *sign, time_t start_sec){
    int32_t ret; // return value for SOCK_ERRORs
    uint16_t size = 0;
    uint32_t write_size = 0;
    image_size = 0;
    int32_t rev_image_size = 0;
    uint8_t count = 0;
    time_t end_sec;

    switch(getSn_SR(CAMERA_SOCK)){
        case SOCK_ESTABLISHED:
            if(getSn_IR(CAMERA_SOCK) & Sn_IR_CON)   // Socket n interrupt register mask; TCP CON interrupt = connection with peer is successful
            {
                setSn_IR(CAMERA_SOCK, Sn_IR_CON);  // this interrupt should be write the bit cleared to '1'
            }
            ret = send(CAMERA_SOCK, "image_capture", strlen("image_capture")+1);
            if( ret <0){
                sclose(CAMERA_SOCK); // socket close
                return ret;
            }
            camera_stat = START;
            while(1){
                end_sec = get_seconds();
                if(end_sec - start_sec> SELF_CAM_TIMEOUT){
                    LOG_MSG("inner timeout!%d,%d\n",end_sec,start_sec);
                    return SELF_CAM_FAIL;
                }
                if((size = getSn_RX_RSR(CAMERA_SOCK)) > 0) // Sn_RX_RSR: Socket n Received Size Register, Receiving data length
                {
                    if(size > DATA_BUF_SIZE) size = DATA_BUF_SIZE; // DATA_BUF_SIZE means user defined buffer size (array)
                    ret = recv(CAMERA_SOCK, buffer, size); // Data Receive process (H/W Rx socket buffer -> User's buffer)
                    size = ret;
                    if(ret <= 0) return ret;

                    switch(camera_stat){
                        case START:
                            if(strcmp(buffer, "image_captured") == 0)
                            {
                                ret = send(CAMERA_SOCK, "image_size", strlen("image_size")+1);
                                if( ret <0){
                                    sclose(CAMERA_SOCK); // socket close
                                    return ret;
                                }
                                if(ret == strlen("image_size")+1){
                                    camera_stat = IMAGE_SIZE_REVING;
                                    rev_image_size = 0;
                                }
                            }
                            break;
                        case IMAGE_SIZE_REVING:
                            image_size = atoi(buffer);
                            ret = send(CAMERA_SOCK, "image_recieving", strlen("image_recieving")+1);
                            if( ret <0 || image_size <0){
                                sclose(CAMERA_SOCK); // socket close
                                return ret;
                            }
                            else{

                                camera_stat = IMAGE_RECIEVING;
                                do{
                                    fimg = fopen(pimgfile, "w");
                                    if(fimg == NULL){
                                        sclose(CAMERA_SOCK);
                                        LOG_MSG("open file failed!%d\n",ret);
                                        *sign = SELF_CAM_FAIL;
                                        return ret;
                                    }
                                    count++;
                                }while(fimg == NULL && count<3);
                            }
                            break;
                        case IMAGE_RECIEVING:
                            LOG_MSG("image file recieved size:%d\n", rev_image_size);
                            if(rev_image_size<image_size){
                                write_size = fwrite(buffer,1,size,fimg);
                                rev_image_size+=write_size;
                                if(rev_image_size == image_size){
                                    LOG_MSG("image received!\n");
                                    fclose(fimg);

                                    camera_stat = IMAGE_RECIEVED;
                                    ret = send(CAMERA_SOCK, "image_recieved", strlen("image_recieved")+1);
                                    sclose(CAMERA_SOCK);
                                    *sign = SELF_CAM_FINISH;
                                    return ret;
                                }
                                else{
                                    ret = send(CAMERA_SOCK, "image_recieving", strlen("image_recieving")+1);
                                    if(ret == strlen("image_recieving")+1 && image_size>0){
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
            if((ret=disconnect(CAMERA_SOCK)) != SOCK_OK) return ret;
            break;
        case SOCK_INIT:
            if( (ret = connect(CAMERA_SOCK, c_des_ip, c_des_port)) != SOCK_OK) return ret;  //  Try to TCP connect to the TCP server (destination)
            break;
        case SOCK_CLOSED:
            close(CAMERA_SOCK);
            if((ret=socket(CAMERA_SOCK, Sn_MR_TCP, ANY_PORT, 0x00)) != CAMERA_SOCK) return ret; // TCP socket open with 'any_port' port number
            break;
        default:
            break;

    }
    return ret;
}
uint8_t self_cam_capture(char *buffer){
    int8_t sign = SELF_CAM_PROCESS;
    time_t start_sec = get_seconds();
    time_t end_sec;
    while(sign == SELF_CAM_PROCESS){
        self_cam_tcpc_run(buffer,&sign, start_sec);
        end_sec = get_seconds();
        if(end_sec - start_sec> SELF_CAM_TIMEOUT){
            LOG_MSG("timeout!%d,%d\n",end_sec,start_sec);
            break;
        }
    }
    if(sign == SELF_CAM_FINISH){
        return 0;
    }
    return 1;
}
