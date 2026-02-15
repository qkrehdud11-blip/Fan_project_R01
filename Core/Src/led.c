/*
 * led.c
 *
 *  Created on: Jan 27, 2026
 *      Author: parkdoyoung
 */


#include "led.h"

/* =====================================================
   내부(static) 제어 변수
   ===================================================== */


// 팬 모드 동작 여부 (1: 동작, 0: 정지)
static uint8_t  fanRun = 0;

// 팬 모드 타이밍 제어용 Tick 저장
static uint32_t fanTick = 0;

// 현재 켜질 LED 인덱스
static uint8_t  fanIndex = 0;


/* =====================================================
   LED 제어 구조체 배열
   (총 8개 LED)
   ===================================================== */
LED_CONTROL led[8]=
		{

				{GPIOC, GPIO_PIN_8, GPIO_PIN_SET, GPIO_PIN_RESET},
				{GPIOC, GPIO_PIN_6, GPIO_PIN_SET, GPIO_PIN_RESET},
				{GPIOC, GPIO_PIN_5, GPIO_PIN_SET, GPIO_PIN_RESET},
				{GPIOA, GPIO_PIN_12, GPIO_PIN_SET, GPIO_PIN_RESET},
				{GPIOA, GPIO_PIN_11, GPIO_PIN_SET, GPIO_PIN_RESET},
				{GPIOB, GPIO_PIN_12, GPIO_PIN_SET, GPIO_PIN_RESET},
				{GPIOB, GPIO_PIN_2, GPIO_PIN_SET, GPIO_PIN_RESET},
				{GPIOB, GPIO_PIN_1, GPIO_PIN_SET, GPIO_PIN_RESET},

		};



/* =====================================================
   지정 개수만큼 LED ON
   ===================================================== */

void ledOn(uint8_t num)
{
	for(uint8_t i = 0; i < num; i++)
	{
		HAL_GPIO_WritePin(led[i].port, led[i].number, led[i].onState);
	}
}


/* =====================================================
   지정 개수만큼 LED OFF
   ===================================================== */
void ledOff(uint8_t num)
{
	for(uint8_t i = 0; i < num; i++)
	{
		HAL_GPIO_WritePin(led[i].port, led[i].number, led[i].offState);
	}
}


/* =====================================================
   특정 LED 1개 토글
   ===================================================== */
void ledToggle(uint8_t num)
{
	HAL_GPIO_TogglePin(led[num].port, led[num].number);
};


/* =====================================================
   왼쪽으로 순차 점등 (블로킹 방식)
   ===================================================== */
void ledLeftShift(uint8_t num){
	for(uint8_t i = 0; i < num; i++){
		ledOn(i);
		HAL_Delay(100);
	}
	HAL_Delay(500);
};


/* =====================================================
   팬 모드 시작 (논블로킹)
   ===================================================== */
void ledLeftShiftFan_Start(void)
{
    fanRun = 1;
    fanIndex = 0;
    fanTick = HAL_GetTick();
}


/* =====================================================
   팬 모드 정지
   ===================================================== */
void ledLeftShiftFan_Stop(void)
{
    fanRun = 0;
    fanIndex = 0;
    ledOff(8);
}





/* =====================================================
   팬 모드 동작 함수 (논블로킹)
   main loop에서 계속 호출해야 함
   ===================================================== */

//static int8_t fanDir   = 1;   // +1: 증가(→), -1: 감소(←)

void ledLeftShiftFan_Task(uint8_t num)
{
    static int8_t direction = 1; // 1이면 증가, -1이면 감소

    if (!fanRun) return;

    if (HAL_GetTick() - fanTick >= 200)
    {
        fanTick = HAL_GetTick();

        // 1. 모든 LED를 먼저 off
        ledOff(8);

        // 2. 현재 위치의 LED만on
        ledOn(fanIndex);

        // 3. 방향에 따라 인덱스 변경
        fanIndex += direction;

        // 4. 끝에 도달하면 방향 반전
        // num이 8이라면: 8에 도달하는 순간 다음은 7이 되도록 설정
        if (fanIndex >= num)
        {
            fanIndex = num;
            direction = -1;
        }
        else if (fanIndex <= 0)
        {
            fanIndex = 0;
            direction = 1;
        }
    }
}



























