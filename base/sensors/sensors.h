/*
 * sensors.h
 *
 *  Created on: 2017��10��23��
 *      Author: saber
 */

#ifndef BASE_SENSORS_SENSORS_H_
#define BASE_SENSORS_SENSORS_H_

#include "Board.h"


typedef struct sensor_config{
    uint8_t (*sensor_open)(uint8_t);/*port_num start from 0*/
    float (*sensor_get_data)(uint8_t, uint8_t);/*port_num, key_num, start from 0*/
    uint8_t (*sensor_process)(uint8_t);
    uint8_t (*sensor_close)(uint8_t);
}_sensor_config;

typedef enum sensor_Type{
    SENSOR_DAVIS_RAIN = 0,//��������rain
    SENSOR_MEC10,//������ʪ��,�¶�,ʪ��,�η�,serial
    SENSOR_NH122,//������ʪ��,�¶�,ʪ��,����ѹ,serial
    SENSOR_NHFS45BU,//����,vol
    SENSOR_NHFX46AU,//����,vol
    SENSOR_NHZD10,//����,vol
    SENSOR_TH10S,//������ʪ��,serial
    SENSOR_MS10VT,//�����¶�,vol
    SENSOR_MS10VH,//����ʪ��,vol
    SENSOR_WXT520,//������ʪ�ȣ����꣬���ٷ���,serial
    SENSOR_CSF11,
    SENSOR_ZWINSOFT,//��������������, CO, O3, NO2,SO2, PM2.5,PM10
    SENSOR_DIAMS,
    SENSOR_ZXWS6P, //������ȫҪ��, ����(m/s),����(��),�¶�(��),ʪ��(%),����ѹ��(hpa),��������(mm),Сʱ����(mm),24 Сʱ����(mm),���������ۼ�ֵ(mm) ,̫����ǿ�� (w/m2)
    SENSOR_WBSENSOR,//�������ӣ�ˮ�ʴ�����, �ܽ���DO���Ƕȡ�pH���絼��
    SENSOR_ADC345,//dfsdfsd
    SENSOR_TYPE_COUNT
}sensor_Type_Enum;

extern const char sensor_type_strs[SENSOR_TYPE_COUNT][20];

extern char sensor_data_buffer[MAX_SENSOR_DATA][SENSOR_BUFFER_SIZE];
extern void sensors_init(void);
extern void sensors_open(void);
extern void sensors_process(void);
extern char *sensors_construct();
extern void sensors_close();
extern void reset_uart_ports();
extern void read_nvs_sensors();
extern char *sensor_get_data_str();




#endif /* BASE_SENSORS_SENSORS_H_ */
