/*
 * ap.h
 *
 *  Created on: Jan 29, 2026
 *      Author: parkdoyoung
 */

#ifndef INC_APP_H_
#define INC_APP_H_


#include "stm32f4xx_hal.h"








void App_Init(void);
void App_Task(void);
void App_UartCommand(uint8_t cmd);





#endif /* INC_APP_H_ */
