/*
 * vaisal.h
 *
 *  Created on: 2017Äê10ÔÂ24ÈÕ
 *      Author: saber
 */

#ifndef BASE_SENSORS_VAISAL_VAISAL_H_
#define BASE_SENSORS_VAISAL_VAISAL_H_

#include "Board.h"

#define MAX_REV_NUM 4

#define TH_DATAS_NUM 5

typedef struct vaisal_data_t{
    float value;
    uint8_t pv;
    char sign[5];
    float avg_value;
    uint8_t completed;
}vaisal_data_struct;


extern vaisal_data_struct vaisal_datas[TH_DATAS_NUM];
extern uint8_t vaisal_open(void);
extern uint8_t vaisal_process();
extern void vaisal_close();
extern void reset_vaisal_datas();

#endif /* BASE_SENSORS_VAISAL_VAISAL_H_ */
