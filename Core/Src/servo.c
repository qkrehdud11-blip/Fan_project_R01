


#include "servo.h"
#include "tim.h"



// 구조체 변수
static SERVO_STATE servoState;


// 현재 서보모터 위치
static uint16_t Servo_Position;


// 방향 설정
static uint8_t direction;


// 동작 시간
static uint32_t prevTime;



// 서보 초기화
void Servo_Init(void)
{
  servoState = SERVO_STOP;
  Servo_Position   = SERVO_CENTER;
  direction        = 1;
  prevTime   = 0;

  TIM3->CCR1 = Servo_Position;
}




// 서보 회전
void Servo_Swap()
{
  if (servoState == SERVO_STOP)
  {
      servoState = SERVO_ROTATE;
  }
  else
  {
      servoState = SERVO_STOP;
  }
}




// 버튼_1을 위한 강제 종료
void Servo_Stop()
{
  servoState = SERVO_STOP;
}


// main 함수 호출용
void Servo_Run(void)
{
  if (servoState == SERVO_STOP)				return;


  /* 20ms마다 한 칸 이동 */
  if (HAL_GetTick() - prevTime < 20) 	return;

  prevTime = HAL_GetTick();

  if (direction == 1)
  {
      Servo_Position = Servo_Position + 1;
  }
  else
  {
      Servo_Position = Servo_Position - 1;
  }

  if (Servo_Position >= SERVO_RIGHT)
  {
      Servo_Position = SERVO_RIGHT;
      direction = 0;
  }

  if (Servo_Position <= SERVO_LEFT)
  {
      Servo_Position = SERVO_LEFT;
      direction = 1;
  }

  TIM3->CCR1 = Servo_Position;
}




bool Servo_IsRunning(void)
{
    return (servoState == SERVO_ROTATE);
}


