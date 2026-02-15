/*
 * servo.h
 *
 *  Created on: Jan 27, 2026
 *      Author: parkdoyoung
 */

#ifndef INC_SERVO_H_
#define INC_SERVO_H_



#include "stm32f4xx_hal.h"
#include "stdbool.h"

/* =====================================================
   서보 PWM 비교값 정의 (각 위치에 해당하는 CCR 값)
   ※ TIM3->CCR1 에 들어갈 값
   ===================================================== */

// 왼쪽 끝 위치
#define SERVO_LEFT			40

// 중앙 위치 (초기 기본 위치)
#define SERVO_CENTER		140

// 오른쪽 끝 위치
#define SERVO_RIGHT			240




/* =====================================================
   서보 상태 정의
   ===================================================== */
typedef enum
{
	SERVO_STOP,		// 정지 상태
	SERVO_ROTATE	// 좌우 반복 회전 상태
}SERVO_STATE;


/* =====================================================
   함수 원형 선언
   ===================================================== */

// 서보 초기 설정
// - 상태 초기화
// - 시작 위치 설정
// - PWM CCR 값 세팅
void Servo_Init();


// 서보 동작 토글 함수
// - STOP ↔ ROTATE 전환
// - 버튼 제어용
void Servo_Swap();


// 서보 강제 정지
// - 어떤 상태든 STOP으로 전환
void Servo_Stop();


// 메인 루프에서 계속 호출해야 하는 실행 함수
// - 20ms 주기로 위치 갱신
// - 방향 전환 처리
void Servo_Run();


// 현재 서보가 회전 중인지 확인
// true  : 동작 중
// false : 정지 상태
bool Servo_IsRunning(void);



#endif /* INC_SERVO_H_ */
