//
// Created by lenovo on 2026/11/5.
//
#include "sg90.h"

#include "stm32f1xx_hal_tim.h"
#include "tim.h"

void SG90_Init(void)
{
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
    HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
}

void SG001_SetAngle(uint8_t angle)
{
    uint32_t pulse=500+(angle*2000/180);

    if (pulse < 500) {pulse=500;}
    if (pulse>2500) {pulse=2500;}

    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,pulse);
}

void SG002_SetAngle(uint8_t angle)
{
    uint32_t pulse=500+(angle*2000/180);

    if (pulse < 500) {pulse=500;}
    if (pulse>2500) {pulse=2500;}

    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,pulse);

}
void SG003_SetAngle(uint8_t angle)
{
    uint32_t pulse=500+(angle*2000/180);

    if (pulse < 500) {pulse=500;}
    if (pulse>2500) {pulse=2500;}

    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,pulse);

}

void SG004_SetAngle(uint8_t angle)
{
    uint32_t pulse=500+(angle*2000/180);

    if (pulse < 500) {pulse=500;}
    if (pulse>2500) {pulse=2500;}

    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,pulse);
}

void SG005_SetAngle(uint8_t angle)
{
    uint32_t pulse=500+(angle*2000/180);

    if (pulse < 500) {pulse=500;}
    if (pulse>2500) {pulse=2500;}

    __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,pulse);

}