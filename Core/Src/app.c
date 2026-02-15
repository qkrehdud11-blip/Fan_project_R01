/*
 * app.c
 *
 *  Created on: Jan 29, 2026
 *      Author: parkdoyoung
 */






#include "app.h"
#include "led.h"
#include "servo.h"
#include "fnd.h"
#include "button.h"
#include "tim.h"

/* =====================================================
   내부 제어 변수
   ===================================================== */

// 동작 시작 시간 저장
static uint32_t startTick = 0;

// 팬 동작 여부 (1: 동작 중)
static uint8_t runFlag = 0;

// 현재 켜질 LED 개수 (속도 단계에 따라 4개 또는 8개)
static uint8_t ledCount = 0;

// 서보 동작 허용 플래그 (속도 선택 후에만 사용 가능)
static uint8_t rotateFlag = 0;


/* =====================================================
   초기화
   ===================================================== */
void App_Init(void)
{
    ledOff(8);  // 시작 시 모든 LED OFF
}


/* =====================================================
   정지 처리
   ===================================================== */
void App_HandleStop(void)
{
    Servo_Stop();         // 서보 정지
    TIM2->CCR1 = 0;       // 모터 PWM OFF
    runFlag = 0;          // 동작 플래그 OFF
    ledCount = 0;
    ledOff(8);            // LED 전체 OFF
    ledLeftShiftFan_Stop();
    rotateFlag = 0;       // 서보 사용 불가
}


/* =====================================================
   50% 속도 처리
   ===================================================== */
void App_Handle50(void)
{
    TIM2->CCR1 = 1000;          // PWM 듀티 설정
    startTick = HAL_GetTick();  // 시작 시간 기록
    runFlag = 1;
    ledCount = 4;               // LED 4개 점등
    ledOff(8);
    ledOn(4);
    rotateFlag = 1;             // 서보 사용 가능
}


/* =====================================================
   100% 속도 처리
   ===================================================== */
void App_Handle100(void)
{
    TIM2->CCR1 = 2000;          // PWM 듀티 설정
    startTick = HAL_GetTick();
    runFlag = 1;
    ledCount = 8;               // LED 8개 점등
    ledOff(8);
    ledOn(8);
    rotateFlag = 1;
}


/* =====================================================
   서보 동작 처리
   ===================================================== */
void App_HandleServo(void)
{
    Servo_Swap();  // 서보 상태 토글

    if (Servo_IsRunning())
    {
        // 서보가 동작 중이면 LED 팬 모드 시작
        ledLeftShiftFan_Start();
    }
    else
    {
        // 서보가 멈추면 팬 모드 중지
        ledLeftShiftFan_Stop();
        ledOff(8);

        // 팬이 동작 중이면 원래 LED 상태 복구
        if (runFlag)
        {
            ledOn(ledCount);
        }
    }
}


/* =====================================================
   메인 태스크 (main loop에서 반복 호출)
   ===================================================== */
void App_Task(void)
{
    uint16_t display = 0;

    /* ---------- 버튼 입력 처리 ---------- */
    if (buttonGetPressed(0))
    {
        App_HandleStop();
    }
    else if (buttonGetPressed(1))
    {
        App_Handle50();
    }
    else if (buttonGetPressed(2))
    {
        App_Handle100();
    }

    // 속도 설정 후에만 서보 버튼 허용
    if (rotateFlag)
    {
        if (buttonGetPressed(3))
        {
            App_HandleServo();
        }
    }

    /* ---------- 서보 실행 ---------- */
    Servo_Run();

    // 서보가 회전 중이면 LED 팬 애니메이션 실행
    if (Servo_IsRunning())
        ledLeftShiftFan_Task(ledCount);

    /* ---------- 시간 표시 ---------- */
    if (runFlag)
    {
        uint32_t now = HAL_GetTick() - startTick;

        // 9초 경과 시 자동 정지
        if (now >= 9000)
        {
            App_HandleStop();
        }
        else
        {
            // FND 표시 형식:
            // 앞 2자리 = 초
            // 뒤 2자리 = 10ms 단위
            display = ((now / 1000) % 100) * 100
                    + (now % 1000) / 10;
        }
    }

    // 4자리 FND 멀티플렉싱 출력
    fndDisplay(display);
}


/* =====================================================
   UART 명령 처리
   '0' : 정지
   '1' : 50%
   '2' : 100%
   '3' : 서보 토글
   ===================================================== */
void App_UartCommand(uint8_t cmd)
{
    if (cmd == '0')      App_HandleStop();
    else if (cmd == '1') App_Handle50();
    else if (cmd == '2') App_Handle100();

    // 동작 중일 때만 서보 제어 허용
    if (runFlag == 1)
    {
        if (cmd == '3')
            App_HandleServo();
    }
}
