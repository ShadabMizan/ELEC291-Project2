#include "app.h"
#include "main.h"
#include "usart.h"
#include "vsensor.h"

#include <stdio.h>

int _write(int file, char *ptr, int len) {
    HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, HAL_MAX_DELAY);
    return len;
}

void app(void) {
    InitVSensor();

    while (1) {
        RunVSensor();
        printf("CH0: %.2f\nCH1: %.2f\r\n", GetVolts(0), GetVolts(1));

        HAL_Delay(500);
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    VSensorCplt();
}