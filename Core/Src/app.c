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





static uint32_t startTick = 0;
static uint8_t runFlag = 0;
static uint8_t ledCount = 0;

static uint8_t rotateFlag = 0; // 모터 회전 변수 추가



void App_Init(void)
{
    ledOff(8);
}

void App_HandleStop(void)
{
    Servo_Stop();
    TIM2->CCR1 = 0;
    runFlag = 0;
    ledCount = 0;
    ledOff(8);
    ledLeftShiftFan_Stop();
    rotateFlag = 0;
}

void App_Handle50(void)
{
    TIM2->CCR1 = 1000;
    startTick = HAL_GetTick();
    runFlag = 1;
    ledCount = 4;
    ledOff(8);
    ledOn(4);
    rotateFlag = 1;
}

void App_Handle100(void)
{
    TIM2->CCR1 = 2000;
    startTick = HAL_GetTick();
    runFlag = 1;
    ledCount = 8;
    ledOff(8);
    ledOn(8);
    rotateFlag = 1;
}

void App_HandleServo(void)
{
  Servo_Swap();

  if (Servo_IsRunning())
  {
      ledLeftShiftFan_Start();
  }
  else
  {
      ledLeftShiftFan_Stop();
      ledOff(8);

      if (runFlag)
      {
      	ledOn(ledCount);
      }

  }
}

void App_Task(void)
{
    uint16_t display = 0;

    if (buttonGetPressed(0))
    {
    	App_HandleStop();
    }
    else if (buttonGetPressed(1)){
    	App_Handle50();
    }
    else if (buttonGetPressed(2))
    {
    	App_Handle100();
    }


    if (rotateFlag)
    {
    	if (buttonGetPressed(3))
    	{
    		App_HandleServo();
    	}
    }

    Servo_Run();

    if (Servo_IsRunning())
        ledLeftShiftFan_Task(ledCount);

    if (runFlag)
    {
        uint32_t now = HAL_GetTick() - startTick;
        if (now >= 9000)
        {
        	App_HandleStop();
        }
        else
            display = ((now / 1000) % 100) * 100 + (now % 1000) / 10;
    }

    fndDisplay(display);
}

void App_UartCommand(uint8_t cmd)
{
    if (cmd == '0') App_HandleStop();
    else if (cmd == '1') App_Handle50();
    else if (cmd == '2') App_Handle100();

    if (runFlag == 1)
    {
    	if (cmd == '3') App_HandleServo();
    }
}
