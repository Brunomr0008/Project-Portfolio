/*
 * button.h
 *
 *  Created on: Dec 6, 2024
 *      Author: bruno
 */

#ifndef INC_DATA_VARIABLES_H_
#define INC_DATA_VARIABLES_H_

#include <stdint.h>

extern int8_t button;
extern float ldr_value;

extern char command_send[16];
extern uint8_t flag_bloqueado;

extern uint8_t flag_callback_timer6;
extern uint8_t flag_callback_timer7;

extern uint32_t time_yellow_5Hz;

int compare_buf(char *msg, char *comparar);


#endif /* INC_DATA_VARIABLES_H_ */
