

#include "button.h"

#define BUTTON_COUNT		4				// 버튼 갯수
#define DEBOUNCE_TIME 	200			// 디바운싱 시간 (ms)


/* =====================================================
   버튼 제어 구조체 배열
   - 각 버튼마다 GPIO 포트/핀, 눌림 판정 레벨(onState)을 저장
   ===================================================== */
static BUTTON_CONTROL button[BUTTON_COUNT] =
{
    {GPIOC, GPIO_PIN_9, GPIO_PIN_RESET}, // 버튼 0
    {GPIOB, GPIO_PIN_8, GPIO_PIN_RESET}, // 버튼 1
    {GPIOB, GPIO_PIN_9, GPIO_PIN_RESET}, // 버튼 2
    {GPIOA, GPIO_PIN_5, GPIO_PIN_RESET}, // 버튼 3
};


/* =====================================================
   버튼별 디바운싱 시간(prevTime) 초기화
   - 시스템 시작 시 1회 호출
   ===================================================== */
void ButtonInit(void)
{
    // 각 버튼의 마지막 유효 입력 시간을 0으로 초기화
    for (uint8_t i = 0; i < BUTTON_COUNT; i++)
    {
        button[i].prevTime = 0;
    }
}


/**
 * @brief  버튼이 눌렸는지 확인하는 함수 (디바운싱 포함)
 * @param  num : 확인할 버튼 인덱스(0~BUTTON_COUNT-1)
 * @return true  : 버튼이 눌림으로 판정됨(유효 입력)
 *         false : 눌리지 않음 또는 디바운싱으로 무시됨
 */
bool buttonGetPressed(uint8_t num)
{
    bool ret = false; // 반환값(기본은 눌리지 않음)

    // (권장) 인덱스 보호: num 범위를 벗어나면 false 반환
    // if (num >= BUTTON_COUNT) return false;

    // 1) 현재 입력이 "눌린 상태(onState)"인지 확인
    if (HAL_GPIO_ReadPin(button[num].port, button[num].number) == button[num].onState)
    {
        uint32_t currTime = HAL_GetTick(); // 현재 시간(ms)

        // 2) 마지막 유효 입력 시점(prevTime) 이후 DEBOUNCE_TIME(ms) 이상 지났을 때만 인정
        if (currTime - button[num].prevTime > DEBOUNCE_TIME)
        {
            // 3) 다시 한번 눌린 상태인지 재확인
            //    (채터링/노이즈로 순간적으로 눌린 것처럼 보이는 경우 방지)
            if (HAL_GPIO_ReadPin(button[num].port, button[num].number) == button[num].onState)
            {
                ret = true;                       // 유효 입력으로 판정
                button[num].prevTime = currTime;  // 마지막 인식 시간 갱신
            }
        }
    }

    return ret; // 버튼 눌림 여부 반환
}








