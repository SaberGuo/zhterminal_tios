/*
 * fatsd.c
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
#include <time.h>

#include <xdc/cfg/global.h>

#include <third_party/fatfs/ffcio.h>
#include <third_party/fatfs/ff.h>

#include <ti/drivers/GPIO.h>
#include <ti/drivers/SDSPI.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>

#include "fatsd.h"

#include "base/rt_clock/rt_clock.h"

/* Buffer size used for the file copy process */
#ifndef CPY_BUFF_SIZE
#define CPY_BUFF_SIZE       (2048*2)
#endif


/* File name prefix for this filesystem for use with TI C RTS */
char fatfsPrefix[] = "fat";

const char col_data_file[] = "fat:"STR(DRIVE_NUM)":cd";
const char config_file[] = "fat:"STR(DRIVE_NUM)":CONF.TXT";
const char image_list_file[] = "fat:"STR(DRIVE_NUM)":image_list.txt";

const char image_pattern[]="*.JPG";

unsigned char cpy_buff[CPY_BUFF_SIZE + 1];

SDSPI_Handle sdspiHandle = NULL;
SDSPI_Params sdspiParams;



const struct timespec ts =
{
    .tv_sec = 1469647026,
    .tv_nsec = 0
};

void enable_sd(){
    GPIO_write(MSP_EXP432P401R_GPIO_TFCARD_ENA, Board_GPIO_LOW);
}

void disable_sd(){
    //GPIO_write(MSP_EXP432P401R_GPIO_TFCARD_ENA, Board_GPIO_HIGH);
}

void init_spisd(){
    SDSPI_init();
    //clock_settime(CLOCK_REALTIME, &ts);
    /* add_device() should be called once and is used for all media types */
    add_device(fatfsPrefix, _MSA, ffcio_open, ffcio_close, ffcio_read,
        ffcio_write, ffcio_lseek, ffcio_unlink, ffcio_rename);
    SDSPI_Params_init(&sdspiParams);
    return;
}

uint8_t open_sd(){

    //Semaphore_pend(semaphore_fat, BIOS_WAIT_FOREVER);
    //enable_sd();
    sdspiHandle = SDSPI_open(Board_SDSPI0, DRIVE_NUM, &sdspiParams);
    if (sdspiHandle == NULL) {
        LOG_MSG("Error starting the SD card\n");
        return ZH_FAIL;
    }
    else {
        LOG_MSG("Drive %u is mounted\n", DRIVE_NUM);
    }
    return ZH_OK;
}

void test_sd(){
    FILE *src;
    char srcline[128];
    src = fopen(config_file,"r");
    while(src&&!feof(src)){
        fgets(srcline, 128, src);
        LOG_MSG("%s\n", srcline);
    }
    fclose(src);
}

uint8_t close_sd(){
    if(sdspiHandle){
        SDSPI_close(sdspiHandle);
        //sdspiHandle = NULL;
        //Semaphore_post(semaphore_fat);
        return ZH_OK;
    }
    return ZH_FAIL;
}
uint8_t fn[20];

time_t fileinfo_to_seconds(FILINFO* fno){
    memcpy(fn, fno->fname, sizeof(fno->fname));
    char* p = strchr(fn, '.');
    *p = '\0';
    time_t tt = atoi(fn)*100;
    return tt;
}

DIR tdp;
FILINFO fno;
char *find_file_first(const char *pattern, DWORD *fsize, uint32_t *psec){
    FRESULT res;
    res = f_findfirst(&tdp, &fno, "", pattern);
    if(FR_OK == res){
        *psec = fileinfo_to_seconds(&fno);
        *fsize = fno.fsize;
        return fno.fname;
    }
    return NULL;
}


char *find_file_next(DWORD *fsize, uint32_t *psec){
    if(FR_OK == f_findnext(&tdp, &fno)){
        *fsize = fno.fsize;
        *psec = fileinfo_to_seconds(&fno);
        return fno.fname;
    }
    return NULL;
}
/*
 *  ======== fatfs_getFatTime ========
 */
int32_t fatfs_getFatTime(void)
{
    time_t seconds;
    uint32_t fatTime;
    struct tm *pTime;

    /*
     *  TI time() returns seconds elapsed since 1900, while other tools
     *  return seconds from 1970.  However, both TI and GNU localtime()
     *  sets tm tm_year to number of years since 1900.
     */
    seconds = get_seconds();

    pTime = localtime(&seconds);

    /*
     *  localtime() sets pTime->tm_year to number of years
     *  since 1900, so subtract 80 from tm_year to get FAT time
     *  offset from 1980.
     */
    fatTime = ((uint32_t)(pTime->tm_year - 80) << 25) |
        ((uint32_t)(pTime->tm_mon) << 21) |
        ((uint32_t)(pTime->tm_mday) << 16) |
        ((uint32_t)(pTime->tm_hour) << 11) |
        ((uint32_t)(pTime->tm_min) << 5) |
        ((uint32_t)(pTime->tm_sec) >> 1);

    return ((int32_t)fatTime);
}
