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
/** ============================================================================
 *  @file       MSP_EXP432P401R.h
 *
 *  @brief      MSP_EXP432P401R Board Specific APIs
 *
 *  The MSP_EXP432P401R header file should be included in an application as
 *  follows:
 *  @code
 *  #include <MSP_EXP432P401R.h>
 *  @endcode
 *
 *  ============================================================================
 */
#ifndef __MSP_EXP432P401R_H
#define __MSP_EXP432P401R_H

#ifdef __cplusplus
extern "C" {
#endif

/* LEDs on MSP_EXP432P401R are active high. */
#define MSP_EXP432P401R_GPIO_LED_OFF (0)
#define MSP_EXP432P401R_GPIO_LED_ON  (1)

#define MSP_EXP432P401R_GPIO_LOW (0)
#define MSP_EXP432P401R_GPIO_HIGH  (1)

#define TASKSTACKSIZE 2048
#define TASKSTACKSIZEBIG 2048
#define EVERY_TICK 0xff
#define SENSOR_BUFFER_SIZE 512
#define MAX_SENSOR_DATA 3
#define MAX_UPLOAD_SIZE 3
#define MAX_BUFFER 1024
#define MAX_DM_BUFFER 256
#define MAX_BUFFER_P 768
#define SENSOR_BUFFER 256

#define MAX_KEYS 8
#define MAX_KEY_LEN 20

#define ZH_OK 0
#define ZH_FAIL    1

#define LITTLE_TIME 200
#define HUGE_TIME 2000

/*resource part limits*/
#define MAX_UART_NUM 2
#define MAX_AD_NUM 8
/*!
 *  @def    MSP_EXP432P401R_ADCName
 *  @brief  Enum of ADC channels on the MSP_EXP432P401R dev board
 */
typedef enum MSP_EXP432P401R_ADCName {
    MSP_EXP432P401R_ADC0 = 0,
    MSP_EXP432P401R_ADC1,
    MSP_EXP432P401R_ADC2,
    MSP_EXP432P401R_ADC3,
    MSP_EXP432P401R_ADC4,
    MSP_EXP432P401R_ADC5,
    MSP_EXP432P401R_ADC6,
    MSP_EXP432P401R_ADC7,


    MSP_EXP432P401R_ADCCOUNT
} MSP_EXP432P401R_ADCName;

/*!
 *  @def    MSP_EXP432P401R_ADCBufName
 *  @brief  Enum of ADC hardware peripherals on the MSP_EXP432P401R dev board
 */
typedef enum MSP_EXP432P401R_ADCBufName {
    MSP_EXP432P401R_ADCBUF0 = 0,

    MSP_EXP432P401R_ADCBUFCOUNT
} MSP_EXP432P401R_ADCBufName;

/*!
 *  @def    MSP_EXP432P401R_ADCBuf0ChannelName
 *  @brief  Enum of ADCBuf channels on the MSP_EXP432P401R dev board
 */
typedef enum MSP_EXP432P401R_ADCBuf0ChannelName {
    MSP_EXP432P401R_ADCBUF0CHANNEL0 = 0,
    MSP_EXP432P401R_ADCBUF0CHANNEL1,

    MSP_EXP432P401R_ADCBUF0CHANNELCOUNT
} MSP_EXP432P401R_ADCBuf0ChannelName;

/*!
 *  @def    MSP_EXP432P401R_CaptureName
 *  @brief  Enum of Capture timer names on the MSP_EXP432P401R dev board
 */
typedef enum MSP_EXP432P401R_CaptureName {
    MSP_EXP432P401R_CAPTURE_TA1 = 0,
    MSP_EXP432P401R_CAPTURE_TA2,
    MSP_EXP432P401R_CAPTURE_TA3,

    MSP_EXP432P401R_CAPTURECOUNT
} MSP_EXP432P401R_CaptureName;

/*!
 *  @def    MSP_EXP432P401R_GPIOName
 *  @brief  Enum of GPIO names on the MSP_EXP432P401R dev board
 */
typedef enum MSP_EXP432P401R_GPIOName {
    /*MSP_EXP432P401R_GPIO_GSM_5V_STATUS = 0,
    MSP_EXP432P401R_GPIO_GSM_33V_STATUS,
    MSP_EXP432P401R_GPIO_CAMERA_33V_STATUS,
    MSP_EXP432P401R_GPIO_CAMERA_12V_STATUS,
    MSP_EXP432P401R_GPIO_SENSOR_12V_STATUS,
    MSP_EXP432P401R_GPIO_IO_5V_STATUS,
    MSP_EXP432P401R_GPIO_IO_33V_STATUS,*/

    MSP_EXP432P401R_GPIO_DI,
    /*
     * MSP_EXP432P401R_GPIO_LED_GREEN & MSP_EXP432P401R_GPIO_LED_BLUE are used for
     * PWM examples.  Uncomment the following lines if you would like to control
     * the LEDs with the GPIO driver.
     */
    //MSP_EXP432P401R_GPIO_LED_GREEN,
    //MSP_EXP432P401R_GPIO_LED_BLUE,

    /*
     * MSP_EXP432P401R_SPI_CS1 is used to control chip select pin for slave1
     * MSP_EXP432P401R_SPI_CS2 is used to control chip select pin for slave2
     */
    MSP_EXP432P401R_GPIO_GSM_RELAY_SET,
    MSP_EXP432P401R_GPIO_GSM_RELAY_RESET,
    MSP_EXP432P401R_GPIO_CAMERA_RELAY_SET,
    MSP_EXP432P401R_GPIO_CAMERA_RELAY_RESET,
    MSP_EXP432P401R_GPIO_SENSOR_RELAY_SET,
    MSP_EXP432P401R_GPIO_SENSOR_RELAY_RESET,
    MSP_EXP432P401R_GPIO_IO_RELAY_SET,
    MSP_EXP432P401R_GPIO_IO_RELAY_RESET,
    MSP_EXP432P401R_GPIO_485_1_RE,
    MSP_EXP432P401R_GPIO_485_0_RE,
    MSP_EXP432P401R_GPIO_DI_POWER,
    MSP_EXP432P401R_GPIO_5V_ENA,
    MSP_EXP432P401R_GPIO_33V_ENA,
    MSP_EXP432P401R_GPIO_TFCARD_ENA,
    MSP_EXP432P401R_GPIO_SPI_CAMERA_CS,
    MSP_EXP432P401R_GPIO_POWER_KEY,
    MSP_EXP432P401R_GPIO_DTR,
    MSP_EXP432P401R_GPIO_W5500_RESET,

    MSP_EXP432P401R_GPIO_AD0,
    MSP_EXP432P401R_GPIO_AD1,
    MSP_EXP432P401R_GPIO_AD2,
    MSP_EXP432P401R_GPIO_AD3,
    MSP_EXP432P401R_GPIO_AD4,
    MSP_EXP432P401R_GPIO_AD5,
    MSP_EXP432P401R_GPIO_AD6,

    MSP_EXP432P401R_GPIO_AD7,

    MSP_EXP432P401R_GPIO_CTRL1,
    MSP_EXP432P401R_GPIO_CTRL2,

    /*MSP_EXP432P401R_GPIO_P1_4,
    MSP_EXP432P401R_GPIO_P1_5,
    MSP_EXP432P401R_GPIO_P1_6,
    MSP_EXP432P401R_GPIO_P1_7,
    MSP_EXP432P401R_GPIO_P3_1,*/
    /*MSP_EXP432P401R_GPIO_P3_2,
    MSP_EXP432P401R_GPIO_P3_3,*/

    //MSP_EXP432P401R_GPIO_485_RO_1,
    //MSP_EXP432P401R_GPIO_485_DI_1,

    //MSP_EXP432P401R_GPIO_485_RO_0,
    //MSP_EXP432P401R_GPIO_485_DI_0,

    MSP_EXP432P401R_GPIO_LED,

    /*MSP_EXP432P401R_GPIO_10_0,
    MSP_EXP432P401R_GPIO_10_1,
    MSP_EXP432P401R_GPIO_10_3,
    MSP_EXP432P401R_GPIO_10_5,
    MSP_EXP432P401R_GPIO_2_0,
    MSP_EXP432P401R_GPIO_2_1,
    MSP_EXP432P401R_GPIO_2_4,
    MSP_EXP432P401R_GPIO_2_5,
    MSP_EXP432P401R_GPIO_2_6,
    MSP_EXP432P401R_GPIO_7_0,
    MSP_EXP432P401R_GPIO_7_1,
    MSP_EXP432P401R_GPIO_7_2,
    MSP_EXP432P401R_GPIO_7_3,
    MSP_EXP432P401R_GPIO_7_4,
    MSP_EXP432P401R_GPIO_7_5,
    MSP_EXP432P401R_GPIO_7_6,
    MSP_EXP432P401R_GPIO_7_7,
    MSP_EXP432P401R_GPIO_8_2,
    MSP_EXP432P401R_GPIO_8_3,
    MSP_EXP432P401R_GPIO_8_4,
    MSP_EXP432P401R_GPIO_8_5,
    MSP_EXP432P401R_GPIO_8_6,
    MSP_EXP432P401R_GPIO_8_7,
    MSP_EXP432P401R_GPIO_9_3,*/


    MSP_EXP432P401R_GPIOCOUNT
} MSP_EXP432P401R_GPIOName;

/*!
 *  @def    MSP_EXP432P401R_I2CName
 *  @brief  Enum of I2C names on the MSP_EXP432P401R dev board
 */
typedef enum MSP_EXP432P401R_I2CName {
    MSP_EXP432P401R_I2CB0 = 0,
    MSP_EXP432P401R_I2CB1,

    MSP_EXP432P401R_I2CCOUNT
} MSP_EXP432P401R_I2CName;

/*!
 *  @def    MSP_EXP432P401R_I2CSlaveName
 *  @brief  Enum of I2CSlave names on the MSP_EXP432P401R dev board
 */
typedef enum MSP_EXP432P401R_I2CSlaveName {
    MSP_EXP432P401R_I2CSLAVEB0 = 0,

    MSP_EXP432P401R_I2CSLAVECOUNT
} MSP_EXP432P401R_I2CSlaveName;

/*!
 *  @def    MSP_EXP432P401R_NVSName
 *  @brief  Enum of NVS names on the MSP_EXP432P401R dev board
 */
typedef enum MSP_EXP432P401R_NVSName {
    MSP_EXP432P401R_NVSMSP4320 = 0,

    MSP_EXP432P401R_NVSCOUNT
} MSP_EXP432P401R_NVSName;


/*!
 *  @def    MSP_EXP432P401R_PWMName
 *  @brief  Enum of PWM names on the MSP_EXP432P401R dev board
 */
typedef enum MSP_EXP432P401R_PWMName {
    MSP_EXP432P401R_PWM_TA1_1 = 0,
    MSP_EXP432P401R_PWM_TA1_2,

    MSP_EXP432P401R_PWMCOUNT
} MSP_EXP432P401R_PWMName;

/*!
 *  @def    MSP_EXP432P401R_SDSPIName
 *  @brief  Enum of SDSPI names on the MSP_EXP432P401R dev board
 */
typedef enum MSP_EXP432P401R_SDSPIName {
    MSP_EXP432P401R_SDSPIB0 = 0,

    MSP_EXP432P401R_SDSPICOUNT
} EMSP_EXP432P401R_SDSPIName;

/*!
 *  @def    MSP_EXP432P401R_SPIName
 *  @brief  Enum of SPI names on the MSP_EXP432P401R dev board
 */
typedef enum MSP_EXP432P401R_SPIName {
    MSP_EXP432P401R_SPI_CAMREA = 0,

    MSP_EXP432P401R_SPICOUNT
} MSP_EXP432P401R_SPIName;

/*!
 *  @def    MSP_EXP432P401R_TimerName
 *  @brief  Enum of Timer names on the MSP_EXP432P401R dev board
 */
typedef enum MSP_EXP432P401R_TimerName {
    MSP_EXP432P401R_TIMER_T32_0 = 0,
    MSP_EXP432P401R_TIMER_T32_1,
    MSP_EXP432P401R_TIMER_TA_1,
    MSP_EXP432P401R_TIMER_TA_2,
    MSP_EXP432P401R_TIMER_TA_3,

    MSP_EXP432P401R_TIMERCOUNT
} MSP_EXP432P401R_TimerName;

/*!
 *  @def    MSP_EXP432P401R_UARTName
 *  @brief  Enum of UART names on the MSP_EXP432P401R dev board
 */
typedef enum MSP_EXP432P401R_UARTName {
    MSP_EXP432P401R_UART_485_0 = 0,
    MSP_EXP432P401R_UART_485_1,
    MSP_EXP432P401R_UART_GSM,

    MSP_EXP432P401R_UARTCOUNT
} MSP_EXP432P401R_UARTName;

/*!
 *  @def    MSP_EXP432P401R_WatchdogName
 *  @brief  Enum of Watchdog names on the MSP_EXP432P401R dev board
 */
typedef enum MSP_EXP432P401R_WatchdogName {
    MSP_EXP432P401R_WATCHDOG = 0,

    MSP_EXP432P401R_WATCHDOGCOUNT
} MSP_EXP432P401R_WatchdogName;

/*!
 *  @brief  Initialize the general board specific settings
 *
 *  This function initializes the general board specific settings.
 */
extern void MSP_EXP432P401R_initGeneral(void);

#ifdef __cplusplus
}
#endif

#endif /* __MSP_EXP432P401R_H */
