/*
 * led.h
 *
 *  Created on: Jan 27, 2026
 *      Author: parkdoyoung
 */

#ifndef INC_LED_H_
#define INC_LED_H_



#include "stm32f4xx_hal.h"


// LED 제어를 위한 구조체
typedef struct
{

	GPIO_TypeDef		*port;
	uint16_t				number;
	GPIO_PinState		onState;
	GPIO_PinState		offState;

}LED_CONTROL;





void ledOn(uint8_t num);
void ledOff(uint8_t num);
void ledToggle(uint8_t num);

void ledLeftShift(uint8_t num);


void ledLeftShiftFan_Start(void);
void ledLeftShiftFan_Stop(void);
void ledLeftShiftFan_Task(uint8_t num);







#endif /* INC_LED_H_ */
