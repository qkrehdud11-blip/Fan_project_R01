/*
 * ap.h
 *
 *  Created on: Jan 29, 2026
 *      Author: parkdoyoung
 */

#ifndef INC_APP_H_
#define INC_APP_H_


#include "stm32f4xx_hal.h"







/* =====================================================
   App 모듈 함수 원형
   ===================================================== */

// App 초기화
// - LED, 내부 상태 변수 등의 초기 상태 설정
// - main 시작 시 1회 호출
void App_Init(void);

// App 메인 태스크
// - 버튼 입력 처리
// - 서보/LED/FND 갱신
// - main while(1) 루프에서 계속 호출
void App_Task(void);

// UART 명령 처리
// cmd:
//  '0' : 정지
//  '1' : 50% 동작
//  '2' : 100% 동작
//  '3' : 서보 토글(동작 중일 때만)
void App_UartCommand(uint8_t cmd);



#endif /* INC_APP_H_ */
