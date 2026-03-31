#include "sonar.h"
#include "main.h"
#include "tof.h"
#include "servo.h"

#include <stdio.h>

void Sonar(void) {
    ServoSetAngle(0);
    HAL_Delay(1000);

    // Sweep 0° → 180°
    for (uint8_t angle = 0; angle <= 180; angle++) {
        ServoSetAngle(angle);
        int distance = GetRange_mm();
        // printf("Angle: %u, Distance: %d\r\n", angle, distance);
        HAL_Delay(10);
    }

    ServoSetAngle(0);
    HAL_Delay(1000);
}