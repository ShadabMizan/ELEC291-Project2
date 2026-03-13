#include "pwm.h"
#include "main.h"
#include "stm32l0xx_hal_tim.h"
#include "tim.h"
#include <stdio.h>

void InitPWM(void) {
    for (int i = 0; i < 4; i++) {
        SetDuty(0, i);
    }

    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
}

void SetDuty(float duty, uint32_t channel) {
    if (duty > 1.0f) duty = 1.0f;
    if (duty < 0.0f) duty = 0.0f;

    uint32_t compare = (uint32_t)(duty * TIM2->ARR);

    switch (channel) {
        case 0: TIM2->CCR1 = compare; break;
        case 1: TIM2->CCR2 = compare; break;
        case 2: TIM2->CCR3 = compare; break;
        case 3: TIM2->CCR4 = compare; break;
        default:
            printf("Invalid PWM Channel: %lu\r\n", channel);
            break;
    }
}