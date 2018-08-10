/*
 * debuger.c
 *
 *  Created on: 2018Äê3ÔÂ23ÈÕ
 *      Author: saber
 */

#include "debuger.h"
#include "../fatsd/fatsd.h"

const char debuger_fn[]="fat:"STR(DRIVE_NUM)":log.txt";
FILE *fl;

void init_debuger(void){
    fl= fopen(debuger_fn, "a+");
}

