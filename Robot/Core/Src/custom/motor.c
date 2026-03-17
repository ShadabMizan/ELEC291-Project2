#include "motor.h"
#include "main.h"
#include "pwm.h"

#define LEFT_MOTOR_FWD_CH       0
#define LEFT_MOTOR_REV_CH       1
#define RIGHT_MOTOR_FWD_CH      2
#define RIGHT_MOTOR_REV_CH      3

void GoRight(float speed) {
    SetDuty(speed, LEFT_MOTOR_FWD_CH);
    SetDuty(0, LEFT_MOTOR_REV_CH);

    SetDuty(0, RIGHT_MOTOR_FWD_CH);
    SetDuty(speed, RIGHT_MOTOR_REV_CH);
}

void GoForward(float speed) {
    SetDuty(speed, LEFT_MOTOR_FWD_CH);
    SetDuty(0, LEFT_MOTOR_REV_CH);

    SetDuty(speed, RIGHT_MOTOR_FWD_CH);
    SetDuty(0, LEFT_MOTOR_REV_CH);
}

void GoLeft(float speed) {
    SetDuty(0, LEFT_MOTOR_FWD_CH);
    SetDuty(speed, LEFT_MOTOR_REV_CH);

    SetDuty(speed, RIGHT_MOTOR_FWD_CH);
    SetDuty(0, LEFT_MOTOR_REV_CH);
}

void GoBackward(float speed) {
    SetDuty(0, LEFT_MOTOR_FWD_CH);
    SetDuty(speed, LEFT_MOTOR_REV_CH);

    SetDuty(0, RIGHT_MOTOR_FWD_CH);
    SetDuty(speed, LEFT_MOTOR_REV_CH);
}

void Stop(void) {
    SetDuty(0, LEFT_MOTOR_FWD_CH);
    SetDuty(0, LEFT_MOTOR_REV_CH);

    SetDuty(0, RIGHT_MOTOR_FWD_CH);
    SetDuty(0, LEFT_MOTOR_REV_CH);
}