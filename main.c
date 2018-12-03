/*
 * Copyright (c) 2015-2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== hello.c ========
 */

/* XDC Module Headers */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Module Headers */
#include <ti/sysbios/BIOS.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/Watchdog.h>
#include <ti/drivers/power/PowerMSP432.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/drivers/NVS.h>
/* POSIX Header files */
#include <pthread.h>

/* Example/Board Header files */
#include "Board.h"

/*self define*/
#include "base/rt_clock/rt_clock.h"
#include "base/sensors/sensors.h"
#include "base/fatsd/fatsd.h"
#include "base/power/power.h"
#include "base/gsm/gsm.h"
#include "base/conf_parser/conf_parser.h"
#include "base/debuger/debuger.h"

#include "app/collecte_config/collecte_config.h"
#include "app/collecte_data/collecte_data.h"
#include "app/collecte_image/collecte_image.h"
#include "app/update_time/update_time.h"
#include "app/upload_data/upload_data.h"

extern _time_task_s collecte_data_task_info;
extern _time_task_s collecte_config_task_info;
extern _time_task_s collecte_image_task_info;
extern _time_task_s upload_data_task_info;
extern _time_task_s update_time_task_info;
extern Watchdog_Handle watchdogHandle;


NVS_Handle nvsHandle;

void gpio_clear(){
    GPIO_write(MSP_EXP432P401R_GPIO_AD0,Board_GPIO_LOW);
    GPIO_write(MSP_EXP432P401R_GPIO_AD1,Board_GPIO_LOW);
    GPIO_write(MSP_EXP432P401R_GPIO_AD2,Board_GPIO_LOW);
    GPIO_write(MSP_EXP432P401R_GPIO_AD3,Board_GPIO_LOW);
    GPIO_write(MSP_EXP432P401R_GPIO_AD4,Board_GPIO_LOW);
    GPIO_write(MSP_EXP432P401R_GPIO_AD5,Board_GPIO_LOW);
    GPIO_write(MSP_EXP432P401R_GPIO_AD5,Board_GPIO_LOW);
    GPIO_write(MSP_EXP432P401R_GPIO_AD6,Board_GPIO_LOW);
    GPIO_write(MSP_EXP432P401R_GPIO_AD7,Board_GPIO_LOW);

    GPIO_write(MSP_EXP432P401R_GPIO_GSM_RELAY_SET,Board_GPIO_LOW);
    GPIO_write(MSP_EXP432P401R_GPIO_GSM_RELAY_RESET,Board_GPIO_LOW);
    GPIO_write(MSP_EXP432P401R_GPIO_CAMERA_RELAY_SET,Board_GPIO_LOW);
    GPIO_write(MSP_EXP432P401R_GPIO_CAMERA_RELAY_RESET,Board_GPIO_LOW);
    GPIO_write(MSP_EXP432P401R_GPIO_SENSOR_RELAY_SET,Board_GPIO_LOW);
    GPIO_write(MSP_EXP432P401R_GPIO_SENSOR_RELAY_RESET,Board_GPIO_LOW);
    GPIO_write(MSP_EXP432P401R_GPIO_IO_RELAY_SET,Board_GPIO_LOW);
    GPIO_write(MSP_EXP432P401R_GPIO_IO_RELAY_RESET,Board_GPIO_LOW);

    GPIO_write(MSP_EXP432P401R_GPIO_5V_ENA, Board_GPIO_HIGH);
    GPIO_write(MSP_EXP432P401R_GPIO_33V_ENA, Board_GPIO_HIGH);
    //GPIO_write(MSP_EXP432P401R_GPIO_TFCARD_ENA, Board_GPIO_HIGH);

    GPIO_write(MSP_EXP432P401R_GPIO_LED,Board_GPIO_HIGH);


}
void other_clear(){
    //MAP_CS_setReferenceOscillatorFrequency(CS_REFO_32KHZ);
    //MAP_CS_initClockSignal(CS_MCLK,CS_REFOCLK_SELECT,CS_CLOCK_DIVIDER_1);

    /* Turns off the Comparator module. */
    MAP_COMP_E_disableModule(COMP_E0_BASE);
    MAP_COMP_E_disableModule(COMP_E1_BASE);

    /* Disabling high side voltage monitor/supervisor */
    MAP_PSS_disableHighSide();

    /* Disables the internal temperature sensor to save power consumption */
    MAP_REF_A_disableTempSensor();
    /* Disables a set of banks in the SRAM */
    /*MAP_SysCtl_disableSRAMBank(SYSCTL_SRAM_BANK1);
    MAP_SysCtl_disableSRAMBank(SYSCTL_SRAM_BANK2);
    MAP_SysCtl_disableSRAMBank(SYSCTL_SRAM_BANK3);
    MAP_SysCtl_disableSRAMBank(SYSCTL_SRAM_BANK4);
    MAP_SysCtl_disableSRAMBank(SYSCTL_SRAM_BANK5);
    MAP_SysCtl_disableSRAMBank(SYSCTL_SRAM_BANK6);
    MAP_SysCtl_disableSRAMBank(SYSCTL_SRAM_BANK7);*/
    /* Disables the I2C block */
    MAP_I2C_disableModule(EUSCI_B0_BASE);
    MAP_I2C_disableModule(EUSCI_B1_BASE);
    MAP_I2C_disableModule(EUSCI_B2_BASE);
    MAP_I2C_disableModule(EUSCI_B3_BASE);
    /* Disables the DMA controller for use */
    //MAP_DMA_disableModule();

    /* Disables the floating-point unit */
    //MAP_FPU_disableModule();
    /* Disables the SPI block */
    /*MAP_SPI_disableModule(EUSCI_A0_BASE);
    MAP_SPI_disableModule(EUSCI_A1_BASE);
    MAP_SPI_disableModule(EUSCI_A2_BASE);
    MAP_SPI_disableModule(EUSCI_A3_BASE);
    MAP_SPI_disableModule(EUSCI_B0_BASE);
    MAP_SPI_disableModule(EUSCI_B3_BASE);*/
    /*

    MAP_SPI_disableModule(EUSCI_B1_BASE);
    MAP_SPI_disableModule(EUSCI_B2_BASE);
    */
    /* Disables the UART block */
    //MAP_UART_disableModule(EUSCI_A2_BASE);
    /*MAP_UART_disableModule(EUSCI_A0_BASE);
    MAP_UART_disableModule(EUSCI_A1_BASE);

    MAP_UART_disableModule(EUSCI_A3_BASE);*/
    /* Disable operation of the ADC block. */
    //MAP_ADC14_disableModule();
    /* Disables the MPU for use */
    //MAP_MPU_disableModule();
    /* Disables the processor interrupt */
    //MAP_Interrupt_disableMaster();

    /* Disables read buffering on accesses to a specified bank of flash memory */
    //MAP_FlashCtl_disableReadBuffering(FLASH_BANK0,FLASH_DATA_READ);
    //MAP_FlashCtl_disableReadBuffering(FLASH_BANK1,FLASH_DATA_READ);

}
void mainThread(UArg arg0){
    Watchdog_Params params;

    power_enable(ENA_DC5V);
    power_enable(ENA_DC33V);


    reset_relay(RELAY_K_GSM);

    Task_sleep(200);
    reset_relay(RELAY_K_CAMERA);

    Task_sleep(200);
    reset_relay(RELAY_K_SENSOR);

    Task_sleep(200);
    reset_relay(RELAY_K_IO);
    Task_sleep(200);


    power_enable(ENA_DC5V);
    power_enable(ENA_DC33V);


    /*parse config & setting task info*/
    enable_sd();
    if(open_sd() == 1){
       LOG_MSG("main open sd fail!\n");
    }
    Task_sleep(200);
    config_all_tasks();
    close_sd();
    disable_sd();
    Task_sleep(200);
    /*debug for once*/
    /*update time*/
    gsm_open();
    update_time_ex();
    gsm_close();
    /*config update*/
    //gsm_open();
    //collecte_config_ex();
    //gsm_close();
    /*collect data*/
    collecte_data_ex();
    /*update data*/
    gsm_open();
    upload_data_ex();
    gsm_close();
    /*collect image*/
    collecte_image_ex();
    /*init tasks*/
    init_collecte_data_task();
    //init_upload_data_task();
    //init_collecte_config_task();
    init_collecte_image_task();
    //init_update_time_task();

    /*add time task list*/
    add_time_task(&collecte_data_task_info);
    //add_time_task(&upload_data_task_info);
    //add_time_task(&collecte_config_task_info);
    add_time_task(&collecte_image_task_info);
    //add_time_task(&update_time_task_info);




    Watchdog_Params_init(&params);
    params.resetMode = Watchdog_RESET_ON;
    watchdogHandle = Watchdog_open(Board_WATCHDOG0, &params);
    /*
    *  normal BIOS programs, would call BIOS_start() to enable interrupts
    *  and start the scheduler and kick BIOS into gear.  But, this program
    *  is a simple sanity test and calls BIOS_exit() instead.
    */
    init_rt_clock();
    gsm_open();
    update_time_ex();
    gsm_close();
    other_clear();
}

/*
 *  ======== main ========
 */
int main(){
    //Task_Params taskParams;
    /* Call driver init functions */
    Board_initGeneral();

    NVS_Params nvsParams;


    SPI_init();
    UART_init();
    GPIO_init();
    Timer_init();

    NVS_init();

    NVS_Params_init(&nvsParams);
    nvsHandle = NVS_open(Board_NVS0, &nvsParams);


    Watchdog_init();


    init_config_heap();
    init_spisd();
    sensors_init();
    init_gsm();
    //gpio_clear();

    BIOS_start();
    return (0);
}

