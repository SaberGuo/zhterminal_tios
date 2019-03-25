/*
 * sensors.h
 *
 *  Created on: 2017年10月23日
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
    SENSOR_DAVIS_RAIN = 0,//降雨量，rain
    SENSOR_MEC10,//土壤温湿度,温度,湿度,盐分,serial
    SENSOR_NH122,//空气温湿度,温度,湿度,大气压,serial
    SENSOR_NHFS45BU,//风速,vol
    SENSOR_NHFX46AU,//风向,vol
    SENSOR_NHZD10,//光照,vol
    SENSOR_TH10S,//土壤温湿度,serial
    SENSOR_MS10VT,//土壤温度,vol
    SENSOR_MS10VH,//土壤湿度,vol
    SENSOR_WXT520,//空气温湿度，降雨，风速风向,serial
    SENSOR_CSF11,
    SENSOR_ZWINSOFT,//空气质量传感器, CO, O3, NO2,SO2, PM2.5,PM10
    SENSOR_DIAMS,
    SENSOR_ZXWS6P, //智翔宇全要素, 风速(m/s),风向(°),温度(℃),湿度(%),大气压力(hpa),分钟雨量(mm),小时雨量(mm),24 小时雨量(mm),测量雨量累加值(mm) ,太阳光强度 (w/m2)
    SENSOR_WBSENSOR,//苏州蛙视，水质传感器, 溶解氧DO、浊度、pH，电导率
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
