#include "sonar.h"
#include "main.h"
#include "tof.h"
#include "servo.h"

#include <stdio.h>

void Sonar(void) {
    ServoSetAngle(0);
    HAL_Delay(500);

    printf("SONAR BEGIN\r\n");

    // Sweep 0° → 180°
    for (uint8_t angle = 0; angle <= 180; angle++) {
        ServoSetAngle(angle);
        int distance = GetRange_mm();
        // max distance at 1000 if there is -1 (read error) or -2 (out of range error)
        if (distance < 0) distance = 1000;      

        printf("SONAR:%d,%u\r\n", distance, angle);
        HAL_Delay(10);
    }

    printf("SONAR END\r\n");

    ServoSetAngle(0);
    HAL_Delay(500);
}
