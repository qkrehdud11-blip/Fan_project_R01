/*
 * button.h
 *
 *  Created on: Jan 27, 2026
 *      Author: parkdoyoung
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_






#include "stm32f4xx_hal.h"
#include <stdbool.h>

/* =====================================================
   버튼 제어 구조체
   - 각 버튼마다 GPIO 정보와 디바운싱 정보를 저장
   ===================================================== */
typedef struct
{
    GPIO_TypeDef  *port;      // 버튼이 연결된 GPIO 포트 (예: GPIOA, GPIOB)
    uint16_t       number;    // 버튼 핀 번호 (예: GPIO_PIN_5)
    GPIO_PinState  onState;   // 눌렸을 때의 GPIO 상태 (RESET 또는 SET)
    uint32_t       prevTime;  // 마지막으로 유효 입력이 인식된 시간(ms)
} BUTTON_CONTROL;


/* =====================================================
   함수 원형 선언
   ===================================================== */

// 버튼 초기화 함수
// - 각 버튼의 prevTime(디바운싱 기준 시간) 초기화
// - 시스템 시작 시 1회 호출
void ButtonInit(void);


// 버튼 눌림 여부 확인 함수 (디바운싱 포함)
// num : 버튼 인덱스
// 반환값
//   true  → 유효한 눌림 입력 발생
//   false → 눌리지 않음 또는 디바운싱으로 무시됨
bool buttonGetPressed(uint8_t num);






#endif /* INC_BUTTON_H_ */
