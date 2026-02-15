/*
 * fnd.c
 *
 *  Created on: Jan 28, 2026
 *      Author: parkdoyoung
 */


#include "fnd.h"


/* =====================================================
   0~9 숫자에 대한 7-Segment 데이터 테이블
   각 비트는 a~g 세그먼트에 대응
   (Common Anode 기준으로 작성)
   ===================================================== */
static const uint8_t fndData[10] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x27, // 7
    0x7F, // 8
    0x67  // 9
};


/* =====================================================
   세그먼트(a~g) 출력 함수
   data의 각 비트를 읽어 해당 세그먼트 ON/OFF
   ===================================================== */
static void fndWriteSegment(uint8_t data)
{

    // Common Anode 방식이므로
    // 1이면 LOW 출력(켜짐), 0이면 HIGH 출력(꺼짐)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3,  (data & (1<<0)) ? GPIO_PIN_RESET : GPIO_PIN_SET); // a
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6,  (data & (1<<1)) ? GPIO_PIN_RESET : GPIO_PIN_SET); // b
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7,  (data & (1<<2)) ? GPIO_PIN_RESET : GPIO_PIN_SET); // c
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,  (data & (1<<3)) ? GPIO_PIN_RESET : GPIO_PIN_SET); // d
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, (data & (1<<4)) ? GPIO_PIN_RESET : GPIO_PIN_SET); // e
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4,  (data & (1<<5)) ? GPIO_PIN_RESET : GPIO_PIN_SET); // f
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,  (data & (1<<6)) ? GPIO_PIN_RESET : GPIO_PIN_SET); // g
}


/* =====================================================
   모든 자리 OFF
   (멀티플렉싱 시 Ghost 방지용)
   ===================================================== */
void fndAllDigitOff(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4,  GPIO_PIN_RESET);
}


/* =====================================================
   소수점 OFF
   ===================================================== */
void fndDotOff(void)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET); // anode여서 ON
}


/* =====================================================
   소수점 ON
   ===================================================== */
void fndDotOn(void)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);   // anode여서 OFF
}



/* =====================================================
   4자리 FND 표시 함수 (멀티플렉싱 방식)
   data 예: 1234 → 12초 34(10ms 단위)
   ===================================================== */
void fndDisplay(uint16_t data)
{
    static uint8_t position = 0;		// 현재 표시 중인 자리
    uint8_t value;

    uint8_t sec = data / 100;   // 앞 2자리 (sec 단위)
    uint8_t ms  = data % 100;   // 뒤 2자리 (10ms 단위)

    /* 모든 자리 OFF */
    fndAllDigitOff();

    /* 세그먼트 OFF (Anode) */
    fndWriteSegment(0x00);
    fndDotOff();

    /* 3. 현재 position에 맞는 숫자 선택 */
    switch (position)
    {
        case 0: value = sec / 10; break;   // 초 tens
        case 1: value = sec % 10; break;   // 초 ones
        case 2: value = ms  / 10; break;   // ms tens
        case 3: value = ms  % 10; break;   // ms ones
    }

    /* 4. 세그먼트 데이터 출력 */
    fndWriteSegment(fndData[value]);

    // 두 번째 자리 뒤에 소수점 표시
    if (position == 1)
        {
            fndDotOn();
        }

    /* 5. 현재 자리 ON */
    switch (position)
    {
        case 0: HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET); break;
        case 1: HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET); break;
        case 2: HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET); break;
        case 3: HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4,  GPIO_PIN_SET); break;
    }

    // 다음 자리로 이동 (0 → 1 → 2 → 3 → 0 반복)
    position = (position + 1) % 4;
}
