


#include "servo.h"
#include "tim.h"

/* ===============================
   내부(static) 전역 변수들
   =============================== */

// 현재 서보 상태 (정지 / 회전)
static SERVO_STATE servoState;


// 현재 서보 PWM 비교값 (각도에 해당)
static uint16_t Servo_Position;


// 회전 방향
// 1 : RIGHT 방향 증가
// 0 : LEFT 방향 감소
static uint8_t direction;


// 마지막으로 위치를 갱신한 시간 (ms)
static uint32_t prevTime;




/* ===============================
   서보 초기화 함수
   =============================== */
void Servo_Init(void)
{
  servoState = SERVO_STOP;				// 초기 상태는 정지
  Servo_Position   = SERVO_CENTER;		// 중앙 위치에서 시작
  direction        = 1;					// 초기 방향은 RIGHT
  prevTime   = 0;						// 시간 초기화

  // TIM3 CH1 PWM 비교값 설정 → 실제 서보 각도 반영
  TIM3->CCR1 = Servo_Position;
}




/* ===============================
   서보 동작 토글 함수
   (버튼 등으로 호출)
   =============================== */
void Servo_Swap()
{
	// 현재 정지 상태면 회전 시작
	if (servoState == SERVO_STOP)
	{
		servoState = SERVO_ROTATE;
	}

	// 이미 회전 중이면 정지
	else
	{
		servoState = SERVO_STOP;
	}
}




/* ===============================
   강제 정지 함수
   =============================== */
void Servo_Stop()
{
  servoState = SERVO_STOP;		// 상태를 정지로 변경
}




/* ===============================
   주기적으로 호출되는 실행 함수
   main loop에서 계속 호출해야 함
   =============================== */
void Servo_Run(void)
{

	// 정지 상태면 아무 동작도 하지 않음
	if (servoState == SERVO_STOP)				return;


	// 20ms 간격으로 한 스텝씩 이동
	// (너무 빠르게 움직이지 않도록 속도 제어)
	if (HAL_GetTick() - prevTime < 20) 	return;

	prevTime = HAL_GetTick();


	// 현재 방향에 따라 위치 증가/감소
	if (direction == 1)
	{
		Servo_Position = Servo_Position + 1;	// 오른쪽 방향
	}
	else
	{
		Servo_Position = Servo_Position - 1;	// 왼쪽 방향
	}


	// 오른쪽 끝에 도달하면 방향 전환
	if (Servo_Position >= SERVO_RIGHT)
	{
		Servo_Position = SERVO_RIGHT;
		direction = 0;							// 왼쪽으로 변경
	}


	// 왼쪽 끝에 도달하면 방향 전환
	if (Servo_Position <= SERVO_LEFT)
	{
		Servo_Position = SERVO_LEFT;
		direction = 1;							// 오른쪽으로 변경
	}


	// 변경된 위치를 PWM 레지스터에 반영
	TIM3->CCR1 = Servo_Position;
}



/* ===============================
   현재 서보가 동작 중인지 확인
   =============================== */
bool Servo_IsRunning(void)
{
    return (servoState == SERVO_ROTATE);
}


