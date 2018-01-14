/*
 * rt_clock.h
 *
 *  Created on: 2017Äê10ÔÂ22ÈÕ
 *      Author: saber
 */

#ifndef APP_RT_CLOCK_RT_CLOCK_H_
#define APP_RT_CLOCK_RT_CLOCK_H_

#include "Board.h"
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Clock.h>
#include <time.h>


typedef struct time_task_s{
    uint8_t p_task_minutes[20];
    uint8_t  task_minutes_num;
    uint8_t p_task_hours[20];
    uint8_t  task_hours_num;
    Semaphore_Handle task_notify_semaphore_handler;
    struct time_task_s* p_tts_next;
}_time_task_s;

extern void add_time_task(_time_task_s* p_task_info);
extern void delay_ms();
extern void init_delay_clock();
extern void init_rt_clock();
extern void tick();
extern int get_microsecond();
extern int get_millisecond();
extern int get_second();
extern int get_minute();
extern int get_hour();
extern int get_day();
extern int get_month();
extern int get_year();
extern int get_century();
extern int get_millenium();
extern void set_microsecond();
extern void set_millisecond();
extern void set_millisecond_multi(int nMilliseconds);
extern void set_second();
extern void set_minute();
extern void set_hour();
extern void set_day();
extern void set_month();
extern void set_year();
extern void set_century();
extern void set_millenium();

extern time_t get_seconds();
extern void set_seconds(uint32_t secs);
extern void delay(int num);

#endif /* APP_RT_CLOCK_RT_CLOCK_H_ */
