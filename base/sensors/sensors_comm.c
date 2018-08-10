/*
 * sensors_comm.c
 *
 *  Created on: 2017Äê12ÔÂ12ÈÕ
 *      Author: saber
 */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "sensors_comm.h"

uint8_t crc_16(const uint8_t* Data , uint32_t length){
    uint16_t Reg_CRC=0xffff;
    uint8_t Temp_reg=0x00;
    uint16_t CalculatedCRC,rec_crc;
    rec_crc = Data[length-2];
    rec_crc <<= 8 ;
    rec_crc += Data[length-1];
    uint8_t i,j;
    for(i=0;i<length-2;i++){
        Reg_CRC ^= *Data++;
        for(j=0;j<8;j++){
            if(Reg_CRC&0x0001)
                Reg_CRC=Reg_CRC>>1^0xA001;
            else
                Reg_CRC>>=1;
        }
    }
    Temp_reg=Reg_CRC>>8;
    CalculatedCRC = Reg_CRC<<8|Temp_reg;

    if(CalculatedCRC == rec_crc)
        return ZH_OK;
    else
        return ZH_FAIL;
}
