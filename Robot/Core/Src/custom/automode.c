#include "automode.h"
#include "motor.h"
#include "vsensor.h"
#include "main.h"
#include "tof.h"

#include <math.h>
#include <stdio.h>

#define VCOIL_DIFF_THRESHOLD    0.01f

#define TEST_SPEED              0.2f

void AutoMode(void) {
    RunVSensor();

    float left_coilV = GetVolts(LEFT_COIL);
    float right_coilV = GetVolts(RIGHT_COIL);

    printf("%.4f, %.4f\r\n", left_coilV, right_coilV);

    float coil_diff = left_coilV - right_coilV;

    if (fabsf(coil_diff) < VCOIL_DIFF_THRESHOLD) {
        GoForward(TEST_SPEED);
        // printf("Decision: Forward\r\n");
    } else if (coil_diff > 0.0f) {
        GoRight(TEST_SPEED);
        // printf("Decision: Right\r\n");
    } else {
        GoLeft(TEST_SPEED);
        // printf("Decision: Left\r\n");
    }
}
