/*
 * sensors.c
 *
 *  Created on: 2017Äê10ÔÂ23ÈÕ
 *      Author: saber
 */
#include "sensors.h"
#include <string.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/drivers/NVS.h>

#include "./vaisal/vaisal.h"
#include "./davis_rain/davis_rain.h"
#include "./mec10/mec10.h"
#include "./nhzd10/nhzd10.h"
#include "./nhfx46au/nhfx46au.h"
#include "./nhfs45bu/nhfs45bu.h"
#include "./nh122/nh122.h"

#include "base/rt_clock/rt_clock.h"
#include "base/protocol/protocol.h"
#include "base/power/power.h"

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

void init_sensors(void){
    read_nvs_sensors();
    nh122_init();
	davis_rain_init();
}

void open_sensors(void){
    //vaisal_open();
    //mec10_init();
    //UART_init();
    ADC_init();
    nh122_open();
    nhzd10_init();
    nhfx46au_init();
    nhfs45bu_init();

    power_enable(ENA_DC5V);
    power_enable(ENA_DC33V);
    set_relay(RELAY_K_IO);
    Task_sleep(200);
    set_relay(RELAY_K_SENSOR);
}

void col_sensors(void){

	//vaisal_process();
    //mec10_process();
    nh122_process();

}

char tbuf[100];
float res;

char *construct_data_str(){
	char *pbuf = sensor_data_buffer[p_w_buffer];
	char *pkey;
	memset(pbuf,0,MAX_BUFFER);
	/*vaisal part*/
	/*
	 uint8_t i = 0;
	 for(i = 0;i<TH_DATAS_NUM;++i){
	    memset(tbuf,0,100);
	    construct_protocol_data(tbuf, vaisal_datas[i].sign, vaisal_datas[i].avg_value);
		pbuf = strcat(pbuf, tbuf);
		pbuf = strcat(pbuf," ");
	}
	memset(tbuf,0,100);*/
	/*rainfall part*/
	/*construct_protocol_data(tbuf, get_davis_rain_key(), get_davis_rain());
	pbuf = strcat(pbuf, tbuf);

    */
	/*mec10 soil poart*/
	/*construct_protocol_data(tbuf,get_mec10_temp_key(), get_mec10_temperature());
	pbuf = strcat(pbuf, tbuf);
	pbuf = strcat(pbuf," ");
	construct_protocol_data(tbuf,get_mec10_hum_key(), get_mec10_humidity());
	pbuf = strcat(pbuf, tbuf);
	pbuf = strcat(pbuf," ");
	construct_protocol_data(tbuf,get_mec10_ec_key(), get_mec10_ec());
	pbuf = strcat(pbuf, tbuf);
	pbuf = strcat(pbuf," ");
	memset(tbuf,0,100);*/
	res = get_nh122_temperature();
	pkey = get_nh122_temp_key();
	construct_protocol_data(tbuf, pkey, res);
	pbuf = strcat(pbuf, tbuf);
	pbuf = strcat(pbuf," ");
	memset(tbuf,0,100);
	res = get_nh122_humidity();
	pkey = get_nh122_hum_key();
	construct_protocol_data(tbuf, pkey, res);
	pbuf = strcat(pbuf, tbuf);
	pbuf = strcat(pbuf," ");
	memset(tbuf,0,100);
	res = get_nh122_bar();
	pkey = get_nh122_bar_key();
	construct_protocol_data(tbuf,pkey, res);
	pbuf = strcat(pbuf, tbuf);
	pbuf = strcat(pbuf," ");
	memset(tbuf,0,100);
	res = get_nhzd10_value();
	pkey = get_nhzd10_key();
	construct_protocol_data(tbuf,pkey, res);
	pbuf = strcat(pbuf, tbuf);
	pbuf = strcat(pbuf," ");
	memset(tbuf,0,100);
	pkey = get_nhfx46au_key();
	res = get_nhfx46au_value();
	construct_protocol_data(tbuf,pkey, res);
	pbuf = strcat(pbuf, tbuf);
	pbuf = strcat(pbuf," ");
	memset(tbuf,0,100);
	pkey =get_nhfs45bu_key();
	res = get_nhfs45bu_value();
	construct_protocol_data(tbuf,pkey, res);
	pbuf = strcat(pbuf, tbuf);
	memset(tbuf,0,100);



	p_w_buffer = (++p_w_buffer)%MAX_SENSOR_DATA;
	write_nvs_sensors();
	return pbuf;
}

char *get_data_str(){
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
void close_sensors(){
	//vaisal_close();
    //mec10_close();


    reset_relay(RELAY_K_SENSOR);
    Task_sleep(200);
    reset_relay(RELAY_K_IO);
    Task_sleep(200);
    power_disable(ENA_DC5V);
    Task_sleep(200);
    power_disable(ENA_DC33V);


    nh122_close();

    reset_ad_ports();
    reset_uart_ports();

}

uint8_t get_sensor_num(){
    //return TH_DATAS_NUM+1;
    return 6;
}
