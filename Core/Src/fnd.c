/*
 * fnd.c
 *
 *  Created on: Jan 28, 2026
 *      Author: parkdoyoung
 */


#include "fnd.h"



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

static void fndWriteSegment(uint8_t data)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3,  (data & (1<<0)) ? GPIO_PIN_RESET : GPIO_PIN_SET); // a
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6,  (data & (1<<1)) ? GPIO_PIN_RESET : GPIO_PIN_SET); // b
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7,  (data & (1<<2)) ? GPIO_PIN_RESET : GPIO_PIN_SET); // c
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,  (data & (1<<3)) ? GPIO_PIN_RESET : GPIO_PIN_SET); // d
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, (data & (1<<4)) ? GPIO_PIN_RESET : GPIO_PIN_SET); // e
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4,  (data & (1<<5)) ? GPIO_PIN_RESET : GPIO_PIN_SET); // f
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,  (data & (1<<6)) ? GPIO_PIN_RESET : GPIO_PIN_SET); // g
}

void fndAllDigitOff(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4,  GPIO_PIN_RESET);
}

void fndDotOff(void)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET); // anode여서 ON
}

void fndDotOn(void)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);   // anode여서 OFF
}

void fndDisplay(uint16_t data)
{
    static uint8_t position = 0;
    uint8_t value;

    uint8_t sec = data / 100;   // 앞 2자리
    uint8_t ms  = data % 100;   // 뒤 2자리 (10ms 단위)

    /* digit OFF */
    fndAllDigitOff();

    /* 그먼트 OFF (Anode) */
    fndWriteSegment(0x00);
    fndDotOff();

    /* 자리별 값 선택 */
    switch (position)
    {
        case 0: value = sec / 10; break;   // 초 tens
        case 1: value = sec % 10; break;   // 초 ones
        case 2: value = ms  / 10; break;   // ms tens
        case 3: value = ms  % 10; break;   // ms ones
    }

    /* 세그먼트 출력 */
    fndWriteSegment(fndData[value]);

    if (position == 1)
        {
            fndDotOn();
        }

    /* 자리 선택 */
    switch (position)
    {
        case 0: HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET); break;
        case 1: HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET); break;
        case 2: HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET); break;
        case 3: HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4,  GPIO_PIN_SET); break;
    }

    position = (position + 1) % 4;
}
