/*
 * led.h
 *
 *  Created on: Jan 27, 2026
 *      Author: parkdoyoung
 */

#ifndef INC_LED_H_
#define INC_LED_H_



#include "stm32f4xx_hal.h"


/* =====================================================
   LED 제어용 구조체
   - 각 LED가 연결된 GPIO 포트/핀 정보와
     ON/OFF일 때의 출력 레벨을 저장
   ===================================================== */
typedef struct
{

	GPIO_TypeDef		*port;		// LED가 연결된 GPIO 포트
	uint16_t			number;		// LED가 연결된 핀 번호
	GPIO_PinState		onState;	// LED를 켤 때 출력할 상태
	GPIO_PinState		offState;	// LED를 끌 때 출력할 상태

}LED_CONTROL;




/* =====================================================
   LED 제어 함수 원형
   ===================================================== */

void ledOn(uint8_t num);
void ledOff(uint8_t num);
void ledToggle(uint8_t num);

void ledLeftShift(uint8_t num);

/* =====================================================
   팬 모드(논블로킹) : main loop에서 Task 호출 방식
   ===================================================== */

// 팬 모드 시작(초기화 + 동작 플래그 ON)
void ledLeftShiftFan_Start(void);

// 팬 모드 정지(동작 플래그 OFF + LED OFF)
void ledLeftShiftFan_Stop(void);

// 팬 모드 동작 함수 (주기적으로 호출해야 함)
void ledLeftShiftFan_Task(uint8_t num);







#endif /* INC_LED_H_ */
