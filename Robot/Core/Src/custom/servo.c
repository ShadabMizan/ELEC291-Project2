#include "servo.h"

// Starts the PWM signal on the specified timer and channel
void SERVO_Init(TIM_HandleTypeDef *htim, uint32_t channel) {
    HAL_TIM_PWM_Start(htim, channel);
}

// Sets the servo to a specific angle (0 to 180)
void SERVO_SetAngle(TIM_HandleTypeDef *htim, uint32_t channel, uint8_t angle) {
    if (angle > 180) {
        angle = 180; // Clamp the angle to a maximum of 180 degrees
    }
    
    // Linearly map the angle (0-180) to the corresponding pulse width (500-2500us)
    uint32_t pulse_length = SERVO_MIN_PULSE + ((angle * (SERVO_MAX_PULSE - SERVO_MIN_PULSE)) / 180);
    
    // Set the Timer Compare Register (CCR) to update the PWM duty cycle
    __HAL_TIM_SET_COMPARE(htim, channel, pulse_length);
}

// Dedicated function to snap the servo exactly to 180 degrees
void SERVO_Turn180(TIM_HandleTypeDef *htim, uint32_t channel) {
    SERVO_SetAngle(htim, channel, 180);
}