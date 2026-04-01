#include "sonar.h"
#include "main.h"
#include "tof.h"
#include "servo.h"

#include <stdio.h>

void Sonar(void) {
    ServoSetAngle(0);
    HAL_Delay(1000);

    uint32_t t0 = HAL_GetTick();
    printf("SONAR BEGIN\r\n");

    // Sweep 0° → 180°
    for (uint8_t angle = 0; angle <= 180; angle++) {
        ServoSetAngle(angle);
        int distance = GetRange_mm();
        uint32_t t_ms = HAL_GetTick() - t0;

        printf("SONAR:%lu,%d,%u\r\n", t_ms, distance, angle);
        HAL_Delay(10);
    }

    printf("SONAR END\r\n");

    ServoSetAngle(0);
    HAL_Delay(1000);
}
