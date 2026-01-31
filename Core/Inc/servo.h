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


// 서보 각도 정의
#define SERVO_LEFT			40
#define SERVO_CENTER		140
#define SERVO_RIGHT			240




// 서보 회전 상태
typedef enum
{
	SERVO_STOP,
	SERVO_ROTATE
}SERVO_STATE;


// 서보 초기화
void Servo_Init();


// 버튼_4를 위한 회전
void Servo_Swap();


// 버튼_1을 위한 강제 종료
void Servo_Stop();


// main 함수 호출용
void Servo_Run();



bool Servo_IsRunning(void);



#endif /* INC_SERVO_H_ */
