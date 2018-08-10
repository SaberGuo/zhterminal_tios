/*
 * vaisal.h
 *
 *  Created on: 2017��10��24��
 *      Author: saber
 */

#ifndef BASE_SENSORS_WXT520_WXT520_H_
#define BASE_SENSORS_WXT520_WXT520_H_

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

extern uint8_t wxt520_open(uint8_t num);
extern uint8_t wxt520_close(uint8_t num);
extern uint8_t wxt520_process(uint8_t num);
extern float wxt520_get_data(uint8_t num, uint8_t key_num);


extern vaisal_data_struct vaisal_datas[TH_DATAS_NUM];
extern uint8_t vaisal_open(void);
extern uint8_t vaisal_process();
extern void vaisal_close();
extern void reset_vaisal_datas();

#endif /* BASE_SENSORS_WXT520_WXT520_H_ */
