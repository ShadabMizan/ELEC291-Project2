#ifndef SERVO_H
#define SERVO_H

#include "main.h" // Automatically pulls in the correct STM32 HAL for your board

// Standard SG90 / Hobby Servo Pulse Widths in microseconds
// Note: Some servos use 1000 to 2000. Adjust these if your servo under/over rotates.
#define SERVO_MIN_PULSE 500  // Pulse width for 0 degrees
#define SERVO_MAX_PULSE 2500 // Pulse width for 180 degrees

void SERVO_Init(TIM_HandleTypeDef *htim, uint32_t channel);
void SERVO_SetAngle(TIM_HandleTypeDef *htim, uint32_t channel, uint8_t angle);
void SERVO_Turn180(TIM_HandleTypeDef *htim, uint32_t channel);

#endif /* SERVO_H */