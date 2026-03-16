#include "app.h"
#include "main.h"
#include "stm32l0xx_hal.h"
#include "usart.h"
#include "vsensor.h"
#include "pwm.h"
#include "tof.h"
#include "ir.h"

#include <stdio.h>

int _write(int file, char *ptr, int len) {
    HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, HAL_MAX_DELAY);
    return len;
}

void app(void) {
    InitVSensor();
    InitPWM();
    // InitTOF();
    
    IRRxInit();

    SetDuty(0.5, 0);
    SetDuty(0.8, 1);
    SetDuty(0.1, 2);
    SetDuty(1, 3);

    // char msg[32];

    while (1) {
        // RunVSensor();
        printf("Test...\r\n");
        // printf("CH0: %.2f\nCH1: %.2f\r\n", GetVolts(0), GetVolts(1));
        // printf("Range: %dmm\r\n", GetRange_mm());
        HAL_Delay(250);
        IRTx("L");
        HAL_Delay(250);
        IRTx("R");
        HAL_Delay(250);
        IRTx("F");
        HAL_Delay(250);
        IRTx("S");
        HAL_Delay(250);
        // if (IR_MessageAvailable()) {
        //     IR_GetMessage(msg);

        //     printf("Received: %s\r\n", msg);
        // }
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    VSensorCplt();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == IR_TEST_Pin) {
        IRTx_Callback();
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart2) {
        IRRx_Callback();
    }
}