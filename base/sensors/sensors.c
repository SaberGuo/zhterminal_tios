/*
 * sensors.c
 *
 *  Created on: 2017Äê10ÔÂ23ÈÕ
 *      Author: saber
 */
#include <base/sensors/wxt520/wxt520.h>
#include "sensors.h"
#include <string.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/drivers/NVS.h>
#include <stdlib.h>

#include "./davis_rain/davis_rain.h"
#include "./mec10/mec10.h"
#include "./nhzd10/nhzd10.h"
#include "./nhgh09bu/nhgh09bu.h"
#include "./nhfx46au/nhfx46au.h"
#include "./nhfs45bu/nhfs45bu.h"
#include "./nh122/nh122.h"
#include "./wxt520/wxt520.h"
#include "./th10s/th10s.h"
#include "./ms10_volt/ms10_volt.h"
#include "./csf11/csf11.h"
#include "./csf11/csf12.h"
#include "./zwinsoft/zwinsoft.h"
#include "./diams/diams.h"
#include "./zxws6p/zxws6p.h"
#include "./nhfs15bu/nhfs15bu.h"
#include "./wbsensor/wbsensor.h"

#include "base/conf_parser/conf_parser.h"
#include "base/rt_clock/rt_clock.h"
#include "base/protocol/protocol.h"
#include "base/power/power.h"
#include "base/cJSON/cJSON.h"

#define NVS_SENSORS_OFFSET 0x1000
#define NVS_SENSORS_PWBUF_OFFSET (NVS_SENSORS_OFFSET+1)
#define NVS_SENSORS_PRBUF_OFFSET (NVS_SENSORS_OFFSET+2)
#define NVS_SENSORS_BUF_OFFSET (NVS_SENSORS_OFFSET+3)

extern NVS_Handle nvsHandle;

char sensor_buffer[SENSOR_BUFFER_SIZE];
char sensor_data_buffer[MAX_SENSOR_DATA][SENSOR_BUFFER_SIZE];
uint8_t sensor_sign = 0;
uint8_t p_w_buffer = 0;
uint8_t p_r_buffer = 0;

const char sensor_type_strs[SENSOR_TYPE_COUNT][20]={
                                   "davis_rain",
                                   "mec10",
                                   "nh122",
                                   "nhfs45bu",
                                   "nhfx46au",
                                   "nhzd10",
                                   "nhgh09bu",
                                   "th10s",
                                   "ms10vt",
                                   "ms10vh",
                                   "wxt520",
                                   "csf11",
                                   "csf12",
                                   "zwinsoft",
                                   "diams",
                                   "zxws6p",
								   "nhfs15bu",
                                   "wbsensor"
};

_sensor_config sensor_configs[SENSOR_TYPE_COUNT]={
                                                  /*davis rain*/
                                                  {
                                                   .sensor_open = davis_open,
                                                   .sensor_close = davis_close,
                                                   .sensor_process = davis_process,
                                                   .sensor_get_data = davis_get_data,
                                                  },
                                                  /*mec10*/
                                                  {
                                                   .sensor_open = mec10_open,
                                                   .sensor_close = mec10_close,
                                                   .sensor_process = mec10_process,
                                                   .sensor_get_data = mec10_get_data,
                                                  },
                                                  /*nh122*/
                                                  {
                                                   .sensor_open = nh122_open,
                                                   .sensor_close = nh122_close,
                                                   .sensor_process = nh122_process,
                                                   .sensor_get_data = nh122_get_data
                                                  },
                                                  /*nhfs45bu*/
                                                  {
                                                   .sensor_open = nhfs45bu_open,
                                                   .sensor_close = nhfs45bu_close,
                                                   .sensor_process = nhfs45bu_process,
                                                   .sensor_get_data = nhfs45bu_get_data,
                                                  },
                                                  /*nhfx46au*/
                                                  {
                                                   .sensor_open = nhfx46au_open,
                                                   .sensor_close = nhfx46au_close,
                                                   .sensor_process = nhfx46au_process,
                                                   .sensor_get_data = nhfx46au_get_data
                                                  },
                                                  /*nhzd10*/
                                                  {
                                                   .sensor_open = nhzd10_open,
                                                   .sensor_close = nhzd10_close,
                                                   .sensor_process = nhzd10_process,
                                                   .sensor_get_data = nhzd10_get_data
                                                  },
                                                  /*nhgh09bu*/
                                                  {
                                                   .sensor_open = nhgh09bu_open,
                                                   .sensor_close = nhgh09bu_close,
                                                   .sensor_process = nhgh09bu_process,
                                                   .sensor_get_data = nhgh09bu_get_data
                                                  },
                                                  /*th10s*/
                                                  {
                                                   .sensor_open =th10s_open,
                                                   .sensor_close = th10s_close,
                                                   .sensor_process = th10s_process,
                                                   .sensor_get_data = th10s_get_data,
                                                  },
                                                  /*ms10vt*/
                                                  {
                                                   .sensor_open =ms10_volt_temp_open,
                                                   .sensor_close = ms10_volt_temp_close,
                                                   .sensor_process = ms10_volt_temp_process,
                                                   .sensor_get_data = ms10_volt_temp_get_data,
                                                  },
                                                  /*ms10vh*/
                                                  {
                                                   .sensor_open =ms10_volt_hum_open,
                                                   .sensor_close = ms10_volt_hum_close,
                                                   .sensor_process = ms10_volt_hum_process,
                                                   .sensor_get_data = ms10_volt_hum_get_data,
                                                  },
                                                  /*wxt520*/
                                                  {
                                                    .sensor_open = wxt520_open,
                                                    .sensor_close = wxt520_close,
                                                    .sensor_process = wxt520_process,
                                                    .sensor_get_data = wxt520_get_data
                                                  },
                                                  /*csf11*/
                                                  {
                                                    .sensor_open = csf11_open,
                                                    .sensor_close = csf11_close,
                                                    .sensor_process = csf11_process,
                                                    .sensor_get_data = csf11_get_data
                                                  },
                                                  /*csf12*/
                                                  {
                                                    .sensor_open = csf12_open,
                                                    .sensor_close = csf12_close,
                                                    .sensor_process = csf12_process,
                                                    .sensor_get_data = csf12_get_data
                                                  },
                                                  /*zwinsoft*/
                                                  {
                                                   .sensor_open = zwinsoft_open,
                                                   .sensor_close = zwinsoft_close,
                                                   .sensor_process = zwinsoft_process,
                                                   .sensor_get_data = zwinsoft_get_data
                                                  },
                                                  /*diams*/
                                                  {
                                                   .sensor_open = diams_open,
                                                   .sensor_close = diams_close,
                                                   .sensor_process = diams_process,
                                                   .sensor_get_data = diams_get_data
                                                  },
                                                  /*zxws6p*/
                                                  {
                                                   .sensor_open = zxws6p_open,
                                                   .sensor_close = zxws6p_close,
                                                   .sensor_process = zxws6p_process,
                                                   .sensor_get_data = zxws6p_get_data
                                                  },
                                                  /*nhfs15bu*/
                                                  {
                                                   .sensor_open = nhfs15bu_open,
                                                   .sensor_close = nhfs15bu_close,
                                                   .sensor_process = nhfs15bu_process,
                                                   .sensor_get_data = nhfs15bu_get_data
                                                  },
                                                  /*wbsensor*/
                                                  {
                                                   .sensor_open = wbsensor_open,
                                                   .sensor_close = wbsensor_close,
                                                   .sensor_process = wbsensor_process,
                                                   .sensor_get_data = wbsensor_get_data
                                                  },
};

void read_nvs_sensors(void){
    NVS_read(nvsHandle, NVS_SENSORS_OFFSET, (void *) sensor_sign, sizeof(sensor_sign));
    if(sensor_sign ==0){
        return;
    }

    NVS_read(nvsHandle, NVS_SENSORS_PWBUF_OFFSET, (void *) p_w_buffer, sizeof(p_w_buffer));
    NVS_read(nvsHandle, NVS_SENSORS_PRBUF_OFFSET, (void *) p_r_buffer, sizeof(p_r_buffer));
    NVS_read(nvsHandle, NVS_SENSORS_BUF_OFFSET, (void *) sensor_data_buffer, MAX_SENSOR_DATA*SENSOR_BUFFER_SIZE);
}
void write_nvs_sensors(void){
    NVS_write(nvsHandle, NVS_SENSORS_OFFSET, (void *) sensor_sign, sizeof(sensor_sign),
                        NVS_WRITE_POST_VERIFY);
    NVS_write(nvsHandle, NVS_SENSORS_PWBUF_OFFSET, (void *) p_w_buffer, sizeof(p_w_buffer),
                            NVS_WRITE_POST_VERIFY);
    NVS_write(nvsHandle, NVS_SENSORS_PRBUF_OFFSET, (void *) p_r_buffer, sizeof(p_r_buffer),
                                NVS_WRITE_POST_VERIFY);
    NVS_write(nvsHandle, NVS_SENSORS_BUF_OFFSET, (void *) sensor_data_buffer, MAX_SENSOR_DATA*SENSOR_BUFFER_SIZE,
                                NVS_WRITE_POST_VERIFY);

}


void sensors_init(void){
    read_nvs_sensors();
    di_init(0);
    close_input();
    close_output();
}

void sensors_open(void){
    ADC_init();
    /*port init*/
    _data_item* pdi = g_config.data_items;
    while(pdi!=NULL){
        sensor_configs[pdi->sensor_type].sensor_open(pdi->port_num);
        pdi = pdi->next_data_item;
    }
    /*power ctrl*/
    power_enable(ENA_DC5V);
    power_enable(ENA_DC33V);
    set_relay(RELAY_K_IO);
    Task_sleep(200);
    set_relay(RELAY_K_SENSOR);
}

void sensors_process(void){
    _data_item* pdi = g_config.data_items;
    while(pdi!=NULL){
       sensor_configs[pdi->sensor_type].sensor_process(pdi->port_num);
       pdi = pdi->next_data_item;
    }

}
char *sensors_construct(void){
    cJSON * root = cJSON_CreateObject();
    cJSON * packages = cJSON_CreateObject();
    cJSON * package = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "device_id", cJSON_CreateNumber(g_config.device_id));
    cJSON_AddItemToObject(root, "device_config_id", cJSON_CreateNumber(g_config.device_config_id));
    cJSON_AddItemToObject(root, "method", cJSON_CreateString("push_data"));
    cJSON_AddItemToObject(root, "package", packages);
    char time_str[20];
    sprintf(time_str,"%d",get_seconds());
    cJSON_AddItemToObject(packages, time_str,package);
    _data_item* pdi = g_config.data_items;
    int i = 0;
    while(pdi!=NULL){
        for(i = 0;i<MAX_KEYS;++i){
            if(strlen(pdi->keys[i])>0){
                cJSON * item = cJSON_CreateObject();
                cJSON_AddItemToObject(item,
                                      "value",
                                      cJSON_CreateNumber(sensor_configs[pdi->sensor_type].sensor_get_data(pdi->port_num, i)));
                cJSON_AddItemToObject(package,pdi->keys[i],item);
            }
        }
        pdi = pdi->next_data_item;
    }
    char *pbuf = sensor_data_buffer[p_w_buffer];
    memset(pbuf,0,SENSOR_BUFFER_SIZE);
    sprintf(pbuf,"%s",cJSON_PrintUnformatted(root));
    cJSON_Delete(root);
    p_w_buffer = (++p_w_buffer)%MAX_SENSOR_DATA;
    write_nvs_sensors();
    return pbuf;
}

char *sensor_get_data_str(){
    char *pbuf = NULL;
    if(p_r_buffer != p_w_buffer){
        pbuf = sensor_data_buffer[p_r_buffer];
        p_r_buffer = (++p_r_buffer)%MAX_SENSOR_DATA;
    }
    return pbuf;
}

void reset_ad_ports(){
    /*reset AD ports*/
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN5);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN4);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN3);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN2);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN1);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN0);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN7);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN6);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN5);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN4);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN5);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN4);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN3);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN2);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN1);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN0);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN7);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN4);
}

void reset_uart_ports(){
    /*485_1*/
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN1);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN2);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN3);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN1);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN2);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN3);
    /*485_0*/
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P9, GPIO_PIN5);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P9, GPIO_PIN6);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P9, GPIO_PIN7);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P9, GPIO_PIN5);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P9, GPIO_PIN6);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P9, GPIO_PIN7);
}
void sensors_close(){

    reset_relay(RELAY_K_SENSOR);
    Task_sleep(LITTLE_TIME);
    reset_relay(RELAY_K_IO);
    Task_sleep(LITTLE_TIME);
    power_disable(ENA_DC5V);
    Task_sleep(LITTLE_TIME);
    power_disable(ENA_DC33V);


    _data_item* pdi = g_config.data_items;
    while(pdi!=NULL){
        sensor_configs[pdi->sensor_type].sensor_close(pdi->port_num);
        pdi = pdi->next_data_item;
    }

    reset_ad_ports();
    reset_uart_ports();
}
