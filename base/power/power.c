/*
 * power.c
 *
 *  Created on: 2017Äê10ÔÂ25ÈÕ
 *      Author: saber
 */
#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/sysbios/knl/Task.h>
#include "base/rt_clock/rt_clock.h"

#include "power.h"




/*open the relay*/
void set_relay(uint8_t relay_no)
{
    volatile uint32_t i;
    if(relay_no&RELAY_K_GSM)
    {
        GPIO_write(Board_GPIO_GSM_RELAY_SET, Board_GPIO_HIGH);
        Task_sleep(300);
        GPIO_write(Board_GPIO_GSM_RELAY_SET, Board_GPIO_LOW);
    }
    if(relay_no&RELAY_K_CAMERA)
    {
        GPIO_write(Board_GPIO_CAMERA_RELAY_SET, Board_GPIO_HIGH);
        Task_sleep(300);
            
        GPIO_write(Board_GPIO_CAMERA_RELAY_SET, Board_GPIO_LOW);
    }
    if(relay_no&RELAY_K_SENSOR)
    {
        GPIO_write(Board_GPIO_SENSOR_RELAY_SET, Board_GPIO_HIGH);
        Task_sleep(300);
        GPIO_write(Board_GPIO_SENSOR_RELAY_SET, Board_GPIO_LOW);
    }
    if(relay_no&RELAY_K_IO)
    {
        GPIO_write(Board_GPIO_IO_RELAY_SET, Board_GPIO_HIGH);
        Task_sleep(300);
        GPIO_write(Board_GPIO_IO_RELAY_SET, Board_GPIO_LOW);
    }
}

/*close the relay*/
void reset_relay(uint8_t relay_no)
{
    volatile uint32_t i;
    if(relay_no&RELAY_K_GSM)
    {
        GPIO_write(Board_GPIO_GSM_RELAY_RESET, Board_GPIO_HIGH);
        Task_sleep(400);
        GPIO_write(Board_GPIO_GSM_RELAY_RESET, Board_GPIO_LOW);
    }
    if(relay_no&RELAY_K_CAMERA)
    {
        GPIO_write(Board_GPIO_CAMERA_RELAY_RESET, Board_GPIO_HIGH);
        Task_sleep(300);
        GPIO_write(Board_GPIO_CAMERA_RELAY_RESET, Board_GPIO_LOW);
    }
    if(relay_no&RELAY_K_SENSOR)
    {
        GPIO_write(Board_GPIO_SENSOR_RELAY_RESET, Board_GPIO_HIGH);
        Task_sleep(300);
        GPIO_write(Board_GPIO_SENSOR_RELAY_RESET, Board_GPIO_LOW);
    }
    if(relay_no&RELAY_K_IO)
    {
        GPIO_write(Board_GPIO_IO_RELAY_RESET, Board_GPIO_HIGH);
        Task_sleep(300);
        GPIO_write(Board_GPIO_IO_RELAY_RESET, Board_GPIO_LOW);
    }
}

void power_enable(uint8_t power_no){
    if(power_no&ENA_DC5V)
    {
        GPIO_write(Board_GPIO_5V_ENA, Board_GPIO_LOW);
    }
    if(power_no&ENA_DC33V)
    {
        GPIO_write(Board_GPIO_33V_ENA, Board_GPIO_LOW);
    }
   
}

void power_disable(uint8_t power_no){
    if(power_no&ENA_DC5V)
    {
        GPIO_write(Board_GPIO_5V_ENA, Board_GPIO_HIGH);
    }
    if(power_no&ENA_DC33V)
    {
        GPIO_write(Board_GPIO_33V_ENA, Board_GPIO_HIGH);
    }
  
}
