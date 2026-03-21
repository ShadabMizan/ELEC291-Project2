#include "motor.h"
#include "main.h"
#include "pwm.h"

#define LEFT_MOTOR_FWD_CH       0
#define LEFT_MOTOR_REV_CH       1
#define RIGHT_MOTOR_FWD_CH      2
#define RIGHT_MOTOR_REV_CH      3

#define SPIN180_DURATION_MS     2000

void GoRight(float speed) {
    float duty = 1.0f - speed;
    SetDuty(duty, LEFT_MOTOR_FWD_CH);
    SetDuty(1, LEFT_MOTOR_REV_CH);

    SetDuty(1, RIGHT_MOTOR_FWD_CH);
    SetDuty(duty, RIGHT_MOTOR_REV_CH);
}

void GoForward(float speed) {
    float duty = 1.0f - speed;
    SetDuty(duty, LEFT_MOTOR_FWD_CH);
    SetDuty(1, LEFT_MOTOR_REV_CH);

    SetDuty(duty, RIGHT_MOTOR_FWD_CH);
    SetDuty(1, RIGHT_MOTOR_REV_CH);
}

void GoLeft(float speed) {
    float duty = 1.0f - speed;
    SetDuty(1, LEFT_MOTOR_FWD_CH);
    SetDuty(duty, LEFT_MOTOR_REV_CH);

    SetDuty(duty, RIGHT_MOTOR_FWD_CH);
    SetDuty(1, RIGHT_MOTOR_REV_CH);
}

void GoBackward(float speed) {
    float duty = 1.0f - speed;
    SetDuty(1, LEFT_MOTOR_FWD_CH);
    SetDuty(duty, LEFT_MOTOR_REV_CH);

    SetDuty(1, RIGHT_MOTOR_FWD_CH);
    SetDuty(duty, RIGHT_MOTOR_REV_CH);
}

void StopMotors(void) {
    SetDuty(1, LEFT_MOTOR_FWD_CH);
    SetDuty(1, LEFT_MOTOR_REV_CH);

    SetDuty(1, RIGHT_MOTOR_FWD_CH);
    SetDuty(1, RIGHT_MOTOR_REV_CH);
}

void Spin180(void) {
    GoLeft(1);
    HAL_Delay(SPIN180_DURATION_MS);
    StopMotors();
}