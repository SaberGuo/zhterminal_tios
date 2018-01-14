/*
 * fatsd.h
 *
 *  Created on: 2017Äê10ÔÂ23ÈÕ
 *      Author: saber
 */

#ifndef BASE_FATSD_FATSD_H_
#define BASE_FATSD_FATSD_H_
#include "Board.h"
#include <third_party/fatfs/integer.h>

/* Drive number used for FatFs */
#define DRIVE_NUM           0

#define image_file_pre "fat:"STR(DRIVE_NUM)":"
#define image_file_bre ".JPG"

extern const char col_data_file[];
extern const char config_file[];
extern const char image_list_file[];
extern const char image_pattern[];

extern void enable_sd();
extern void disable_sd();

extern void init_spisd();

extern uint8_t open_sd();
extern uint8_t close_sd();

extern void test_sd();

extern char *find_file_first(const char *pattern, DWORD *fsize, uint32_t *psec);
extern char *find_file_next(DWORD *fsize, uint32_t *psec);
#endif /* BASE_FATSD_FATSD_H_ */
