/*
 * wiznet.c
 *
 *  Created on: 2017年10月24日
 *      Author: saber
 */
#include <stdint.h>
#include <stddef.h>

#include <ti/drivers/SPI.h>
/* Driverlib Header files */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "./Ethernet/wizchip_conf.h"
#include "base/rt_clock/rt_clock.h"
#include "wiznet.h"

#define CHIPSELECTENABLE  (0)
#define CHIPSELECTDISABLE (1)

unsigned char masterRxBuffer[MAX_BUFFER];
unsigned char masterTxBuffer[MAX_BUFFER];

SPI_Handle wiznet_spi;
SPI_Params wiznet_spiParams;
SPI_Transaction masterTransaction;


const eUSCI_SPI_MasterConfig spiMasterhighConfig =
{
        EUSCI_B_SPI_CLOCKSOURCE_SMCLK,             // SMCLK Clock Source
        24000000,                                   // SMCLK = DCO = 24MHZ
        12000000,                                    // SPICLK = 12MHZ
        EUSCI_B_SPI_MSB_FIRST,                     // MSB First
        EUSCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT,    // 时锟斤拷锟斤拷位0
        EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW,
        EUSCI_SPI_3PIN
};

void wizchip_cs_sel(){
    //wiznet_enable();
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN4);
}

void wizchip_cs_desel(){
    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN4);
    //wiznet_disable();
}
int wizchip_spi_read_write(uint8_t data)
{

    while (!(SPI_getInterruptStatus(EUSCI_B2_BASE,EUSCI_B_SPI_TRANSMIT_INTERRUPT)));

    SPI_transmitData(EUSCI_B2_BASE,data);

    while (!(SPI_getInterruptStatus(EUSCI_B2_BASE,EUSCI_B_SPI_RECEIVE_INTERRUPT)));

    return SPI_receiveData(EUSCI_B2_BASE);
}
uint8_t wizchip_spi_readbyte(void){
    return wizchip_spi_read_write(0);
}

void wizchip_spi_writebyte(uint8_t wb){
    wizchip_spi_read_write(wb);
}
void wizchip_spi_readburst(uint8_t* pBuf, uint16_t len){
    register uint16_t retry;
    for(retry=0; retry<len; retry++)
    {
      *(pBuf+retry) = wizchip_spi_read_write(0);
    }
}
void wizchip_spi_writeburst(uint8_t* pBuf, uint16_t len){
    register uint16_t retry;
    for(retry=0; retry<len; retry++)
    {
        wizchip_spi_read_write(*pBuf++);
    }
}

void wiznet_init(void){

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,
                    GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
    SPI_initMaster(EUSCI_B2_BASE, &spiMasterhighConfig);

    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN4);
    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN4);
    reg_wizchip_cs_cbfunc(wizchip_cs_sel, wizchip_cs_desel);
    reg_wizchip_spi_cbfunc(wizchip_spi_readbyte, wizchip_spi_writebyte);
    reg_wizchip_spiburst_cbfunc(wizchip_spi_readburst, wizchip_spi_writeburst);
}

void wiznet_reset(void){
    GPIO_write(Board_GPIO_W5500_RESET, Board_GPIO_LOW);
    delay(10000);
    GPIO_write(Board_GPIO_W5500_RESET, Board_GPIO_HIGH);
    delay(10000);
}

SPI_Params wizParams;
void wiznet_open(void){
    SPI_enableModule(EUSCI_B2_BASE);
}

void wiznet_config(void){
    ctlwizchip(CW_RESET_WIZCHIP,NULL);
    wiz_NetTimeout mWIZNETTIMEOUT = {.retry_cnt = 5, .time_100us = 1000};
    wizchip_settimeout(&mWIZNETTIMEOUT);
    wizchip_gettimeout(&mWIZNETTIMEOUT);

    LOG_MSG("retry_cnt = %d, time 100us = %d\n", mWIZNETTIMEOUT.retry_cnt, mWIZNETTIMEOUT.time_100us);
    wiz_NetInfo mWIZNETINFO= {.mac = {0x00, 0x08, 0xDC, 0x44, 0xef, 0x62},
            .ip = {192, 168, 1, 105},
            .sn = {255, 255, 255, 0},
            .gw = {192, 168, 1, 1},
            .dns = {192,168,1,1},
            .dhcp = NETINFO_STATIC };
    ctlnetwork(CN_SET_NETINFO, (void *)&mWIZNETINFO);
    ctlnetwork(CN_GET_NETINFO, (void *)&mWIZNETINFO);
    LOG_MSG("mac: 0x%02X-0x%02X-0x%02X-0x%02X-0x%02X-0x%02X\n",
               mWIZNETINFO.mac[0], mWIZNETINFO.mac[1], mWIZNETINFO.mac[2], mWIZNETINFO.mac[3], mWIZNETINFO.mac[4], mWIZNETINFO.mac[5]);
    LOG_MSG("ip: %d.%d.%d.%d\n", mWIZNETINFO.ip[0],mWIZNETINFO.ip[1],mWIZNETINFO.ip[2],mWIZNETINFO.ip[3]);
    LOG_MSG("sn: %d.%d.%d.%d\n", mWIZNETINFO.sn[0],mWIZNETINFO.sn[1],mWIZNETINFO.sn[2],mWIZNETINFO.sn[3]);
    LOG_MSG("gw: %d.%d.%d.%d\n", mWIZNETINFO.gw[0],mWIZNETINFO.gw[1],mWIZNETINFO.gw[2],mWIZNETINFO.gw[3]);
}

void wiznet_close(void){
    SPI_disableModule(EUSCI_B2_BASE);
}


