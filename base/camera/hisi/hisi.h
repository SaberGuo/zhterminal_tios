/*
 * self_cam.h
 *
 *  Created on: 2017Äê10ÔÂ24ÈÕ
 *      Author: saber
 */

#ifndef BASE_CAMERA_SELF_CAM_H_
#define BASE_CAMERA_SELF_CAM_H_

#include "Board.h"

extern uint8_t hisi_open(uint8_t num);/*port_num start from 0*/
extern uint8_t hisi_process(uint8_t num, char * file_name, char *pbuffer, uint32_t* psize);
extern uint8_t hisi_close(uint8_t num);

#endif /* BASE_CAMERA_SELF_CAM_H_ */
