#include "servo.h"
#include "main.h"
#include "stm32l0xx_hal_tim.h"
#include "tim.h"
#include <stdio.h>

#define SERVO_MIN_PW_US     500   // 0.5ms = 0°
#define SERVO_MAX_PW_US     2500  // 2.5ms = 180°

#define SERVO_MAX_ANGLE     180

void ServoInit(void) {
    htim22.Init.Prescaler = 32 - 1;
    htim22.Init.Period = 65535;
    HAL_TIM_Base_Init(&htim22);
    HAL_TIM_PWM_Start(&htim22, TIM_CHANNEL_2);
}

void ServoSetAngle(uint8_t angle) {
    if (angle > SERVO_MAX_ANGLE) {
        angle = SERVO_MAX_ANGLE;
    }

    uint16_t pulse_us = SERVO_MIN_PW_US + ((uint32_t)angle * (SERVO_MAX_PW_US - SERVO_MIN_PW_US))/SERVO_MAX_ANGLE;
    TIM22->CCR2 = pulse_us - 1;
}