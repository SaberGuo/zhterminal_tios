/*
 * gsm.h
 *
 *  Created on: 2017Äê10ÔÂ24ÈÕ
 *      Author: saber
 */

#ifndef BASE_GSM_GSM_H_
#define BASE_GSM_GSM_H_

#include "Board.h"


#define GSM_RETRY_NUM 10


extern void init_gsm();
extern char* msg_recieve();
extern uint8_t gsm_ipconfig(uint8_t count);
extern uint8_t gsm_connect(uint8_t count);
extern uint8_t gsm_disconnect(uint8_t count);
extern uint8_t gsm_is_reg(uint8_t count);
extern uint8_t gsm_pb_done(uint8_t count);
extern uint8_t gsm_open();
extern char* msg_dm_recieve(uint16_t max_buf);
extern char* msg_at_recieve(int count);
extern void gsm_close();
extern uint8_t gsm_to_data_mode(uint8_t count);
extern uint8_t gsm_connect_data_mode(uint8_t count);
extern uint8_t msg_send(char *message);
extern uint8_t msg_send_onsize(char *message, uint32_t size);
extern uint8_t gsm_gps_info(uint8_t count);
extern uint8_t gsm_open_gps(uint8_t count);
extern void gsm_lock();
extern void gsm_unlock();
#endif /* BASE_GSM_GSM_H_ */
