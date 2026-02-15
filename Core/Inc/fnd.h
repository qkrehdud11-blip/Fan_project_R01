/*
 * fnd.h
 *
 *  Created on: Jan 28, 2026
 *      Author: parkdoyoung
 */

#ifndef INC_FND_H_
#define INC_FND_H_






#include "stm32f4xx_hal.h"



/* =====================================================
   FND(4자리 7-Segment) 제어 함수 원형
   ===================================================== */

// 4자리 멀티플렉싱 표시 함수
// - data를 "앞2자리(sec) + 뒤2자리(10ms 단위)" 형태로 표시
// - main loop 또는 타이머 주기에서 계속 호출해야 깜빡임 없이 표시됨
void fndDisplay(uint16_t data);


// 모든 자리(digit) 선택 핀 OFF
// - 자리 전환 시 ghost 현상 방지용
void fndAllDigitOff(void);


// 소수점(dot) OFF
void fndDotOff(void);


// 소수점(dot) ON
void fndDotOn(void);








#endif /* INC_FND_H_ */
