/*
 * rt_clock.c
 *
 *  Created on: 2017Äê10ÔÂ22ÈÕ
 *      Author: saber
 */
#include <time.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/Watchdog.h>
#include <ti/drivers/power/PowerMSP432.h>
#include <ti/drivers/NVS.h>

/* XDC module Headers */
#include <xdc/std.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/System.h>

/* BIOS module Headers */
#include <ti/sysbios/BIOS.h>


#include "rt_clock.h"
#define NVS_RTCLOCK_OFFSET 0
#define NVS_RTCLOCK_SEC_OFFSET (NVS_RTCLOCK_OFFSET+1)
#define NVS_RTCLOCK_MIN_OFFSET (NVS_RTCLOCK_OFFSET+5)
#define NVS_RTCLOCK_HOUR_OFFSET (NVS_RTCLOCK_OFFSET+9)
#define NVS_RTCLOCK_DAY_OFFSET (NVS_RTCLOCK_OFFSET+13)
#define NVS_RTCLOCK_MON_OFFSET (NVS_RTCLOCK_OFFSET+17)
#define NVS_RTCLOCK_YEAR_OFFSET (NVS_RTCLOCK_OFFSET+21)

Watchdog_Handle watchdogHandle;
extern NVS_Handle nvsHandle;

/*real time calculate*/
uint8_t rt_clock_sign = 0;

double ticks;
int microsecond;
int millisecond;
int second;
int minute;
int hour;
int day;
int month;
int year;
int century;
int millenium;

Clock_Handle clkHandle;
Clock_Params clkParams;
Clock_Struct clkStruct;

_time_task_s* p_time_tasks_info = NULL;

void delay(int num)
{
    int i=num;
    int j=num;
    for(;i>0;i--)
        for(;j>0;j--);
}

void add_time_task(_time_task_s* p_task_info){
    _time_task_s** ptt = &p_time_tasks_info;
    while(*ptt!=NULL){
        ptt = &((*ptt)->p_tts_next);
    }
    (*ptt) = p_task_info;
}

Void clockPrd(UArg arg){
    Watchdog_clear(watchdogHandle);
    _time_task_s* p_tasks_info = (_time_task_s*)arg;
    uint8_t i_hour = 0,i_minute = 0;
    while(p_tasks_info!=NULL){
        /*judge task is executable*/
        for(i_hour=0;i_hour<p_tasks_info->task_hours_num;++i_hour){
            if(p_tasks_info->p_task_hours[i_hour] == get_hour()||p_tasks_info->p_task_hours[i_hour] == EVERY_TICK){
                for(i_minute=0;i_minute<p_tasks_info->task_minutes_num;++i_minute){
                    if(p_tasks_info->p_task_minutes[i_minute] == get_minute()||p_tasks_info->p_task_minutes[i_minute] == EVERY_TICK){
                        if(get_second() == 0){
                            Semaphore_post(p_tasks_info->task_notify_semaphore_handler);
                        }
                    }
                }
            }
        }
        p_tasks_info = p_tasks_info->p_tts_next;
    }
    tick();
    return;
}

Clock_Handle delay_clkHandle;
Clock_Params delay_clkParams;
Clock_Struct delay_clkStruct;
uint32_t delay_ticks;

void delay_ms(uint32_t delay_ms){
    uint32_t start = delay_ticks;
    while (delay_ticks - start < delay_ms) ; // nop
}

void delay_clockPrd(UArg arg){
    delay_ticks++;
}

void init_delay_clock(){
    Clock_Params_init(&delay_clkParams);
    clkParams.period = 1;
    clkParams.startFlag = TRUE;
    Clock_construct(&delay_clkStruct, (Clock_FuncPtr)delay_clockPrd,
                        1, &delay_clkParams);
    delay_clkHandle = Clock_handle(&delay_clkStruct);
    Clock_start(delay_clkHandle);
}


void init_rt_clock(){
    NVS_read(nvsHandle, NVS_RTCLOCK_OFFSET, (void *) rt_clock_sign, sizeof(rt_clock_sign));
    if(rt_clock_sign==0){
        ticks = 0;
        microsecond = 0;
        millisecond = 0;
        second = 0;
        minute = 0;
        hour = 0;
        day = 19;
        month = 8;
        year = 2017;
        century = 20;
        millenium = 0;
    }
    else{
        NVS_read(nvsHandle, NVS_RTCLOCK_SEC_OFFSET, (void *) second, sizeof(second));
        NVS_read(nvsHandle, NVS_RTCLOCK_MIN_OFFSET, (void *) minute, sizeof(minute));
        NVS_read(nvsHandle, NVS_RTCLOCK_HOUR_OFFSET, (void *) hour, sizeof(hour));
        NVS_read(nvsHandle, NVS_RTCLOCK_DAY_OFFSET, (void *) day, sizeof(day));
        NVS_read(nvsHandle, NVS_RTCLOCK_MON_OFFSET, (void *) month, sizeof(month));
        NVS_read(nvsHandle, NVS_RTCLOCK_YEAR_OFFSET, (void *) year, sizeof(year));
    }


    Clock_Params_init(&clkParams);
    clkParams.period = 1000;
    clkParams.startFlag = TRUE;

    clkParams.arg = (UArg)p_time_tasks_info;
    Clock_construct(&clkStruct, (Clock_FuncPtr)clockPrd,
                    1000, &clkParams);
    clkHandle = Clock_handle(&clkStruct);
    Clock_start(clkHandle);
}

/*common part*/
void tick(){
    ticks++;
    set_second();
    rt_clock_sign = 1;
    NVS_write(nvsHandle, NVS_RTCLOCK_OFFSET, (void *) rt_clock_sign, sizeof(rt_clock_sign),
                    NVS_WRITE_POST_VERIFY);
    NVS_write(nvsHandle, NVS_RTCLOCK_SEC_OFFSET, (void *) second, sizeof(second),
                NVS_WRITE_POST_VERIFY);
    NVS_write(nvsHandle, NVS_RTCLOCK_MIN_OFFSET, (void *) minute, sizeof(minute),
                    NVS_WRITE_POST_VERIFY);
    NVS_write(nvsHandle, NVS_RTCLOCK_HOUR_OFFSET, (void *) hour, sizeof(hour),
                        NVS_WRITE_POST_VERIFY);
    NVS_write(nvsHandle, NVS_RTCLOCK_DAY_OFFSET, (void *) day, sizeof(day),
                        NVS_WRITE_POST_VERIFY);
    NVS_write(nvsHandle, NVS_RTCLOCK_MON_OFFSET, (void *) month, sizeof(month),
                        NVS_WRITE_POST_VERIFY);
    NVS_write(nvsHandle, NVS_RTCLOCK_YEAR_OFFSET, (void *) year, sizeof(year),
                        NVS_WRITE_POST_VERIFY);

    LOG_MSG("second:%d,min:%d\n",second,minute);
    //LOG_MSG("MCLK = %d\n", CS_getMCLK());

}

time_t get_seconds(){
    struct tm* pTime;
    time_t seconds;
    pTime = localtime(NULL);
    pTime->tm_year = get_year()-1970;
    pTime->tm_mon = get_month()-1;
    pTime->tm_mday = get_day();
    pTime->tm_hour = get_hour();
    pTime->tm_min = get_minute();
    pTime->tm_sec = get_second()*10;

    seconds = mktime(pTime);
    //LOG_MSG("seconds is %d\n", seconds);
    return seconds;
}

void set_seconds(uint32_t secs){
    struct tm *pTime;
    LOG_MSG("set sec is %u", secs);
    time_t seconds = (time_t)secs;
    pTime = localtime(&seconds);
    year = pTime->tm_year+1970;
    month = pTime->tm_mon+1;
    day = pTime->tm_mday;
    hour = pTime->tm_hour;
    minute = pTime->tm_min;
    second = pTime->tm_sec/10;
}

/*get part*/
int get_microsecond(){
    return microsecond;
}
int get_millisecond(){
    return millisecond;
}
int get_second(){
    return second;
}
int get_minute(){
    return minute;
}
int get_hour(){
    return hour;
}
int get_day(){
    return day;
}
int get_month(){
    return month;
}
int get_year(){
    return year;
}
int get_century(){
    return century;
}
int get_millenium(){
    return millenium;
}

/*set part*/

void set_microsecond(){
    if (microsecond >= 999) {
        set_millisecond();
        microsecond = 0;
    }
    else {
        microsecond++;
    }

    return;
}
void set_millisecond(){
    if (millisecond >= 999) {
        set_second();
        millisecond = 0;
    }
    else {
        millisecond++;
    }

    return;
}
void set_millisecond_multi(int nMilliseconds){
    int secs;

    millisecond += nMilliseconds;
    secs = millisecond / 1000;
    millisecond %= 1000;

    while (secs--) {
        set_second();
    }

    return;
}

void set_second(){
    if (second == 59) {
        set_minute();
        second = 0;
    }
    else {
        second++;
    }

    return;
}
void set_minute(){
    if (minute == 59) {
        set_hour();
        minute = 0;
    }
    else {
        minute++;
    }
    return;
}

void set_hour(){
    if (hour == 23) {
        set_day();
        hour = 0;
    }
    else {
        hour++;
    }

    return;
}
void set_day(){
    bool thirtydays = false;
    bool feb = false;
    bool leap = false;

    if (month == 4 || month == 6 || month == 9 || month == 11) {
        // April, June, September, November.
        thirtydays = true;
    }

    if (month == 2) {  // Test for February
        feb = true;
    }
    /*
    * A year is a leap year if it is divisible by 4, but not by 100.
    *
    * If a year is divisible by 4 and by 100, it is a leap year only
    * if it is also divisible by 400.
    */
    if ((year%4 == 0 && year%100 != 0) ||
            (year%4 == 0 && year%100 == 0 && year%400 == 0)) {
        leap = true;
    }

    if ((day == 28) && (feb) && (!leap)) {
        set_month();
        day = 1;
    }
    else if ((day == 29) && (feb) && (leap)) {
        set_month();
        day = 1;
    }
    else if ((day == 30) && (thirtydays == true)) {
        set_month();
        day = 1;
    }
    else if ((day == 31) && (thirtydays == false)) {
        set_month();
        day = 1;
    }
    else {
        day++;
    }

    return;
}
void set_month(){
    if (month >= 12) {
        set_year();
        month = 1;
    }
    else {
        month++;
    }

    return;
}
void set_year(){
    year++;
    if ((year%100) == 0) {
        set_century();
    }

    return;
}
void set_century(){
    century++;
    if ((century%10) == 0) {
        set_millenium();
    }
    return;
}
void set_millenium(){
    millenium++;

    return;
}
