#include "app.h"
#include "main.h"
#include "usart.h"
#include "vsensor.h"
#include "pwm.h"

#include <stdio.h>

int _write(int file, char *ptr, int len) {
    HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, HAL_MAX_DELAY);
    return len;
}

void app(void) {
    InitVSensor();
    InitPWM();

    SetDuty(0.5, 0);
    SetDuty(0.8, 1);
    SetDuty(0.1, 2);
    SetDuty(1, 3);

    while (1) {
        RunVSensor();
        printf("CH0: %.2f\nCH1: %.2f\r\n", GetVolts(0), GetVolts(1));

        HAL_Delay(500);
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    VSensorCplt();
}