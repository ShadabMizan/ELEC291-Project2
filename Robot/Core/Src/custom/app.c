#include "app.h"
#include "main.h"
#include "motor.h"
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
    
    IRTxInit();
    IRRxInit();

    uint32_t delay_ms = 2000;

    while (1) {
        // RunVSensor();
        printf("Test...\r\n");
        
        // printf("CH0: %.2f\nCH1: %.2f\r\n", GetVolts(0), GetVolts(1));
        // printf("Range: %dmm\r\n", GetRange_mm());
        
        IRTx('L');
        IRTx(100);
        HAL_Delay(1);
        IRUpdateCMD();
        HAL_Delay(delay_ms);

        IRTx('R');
        IRTx(90);
        HAL_Delay(1);
        IRUpdateCMD();
        HAL_Delay(delay_ms);

        IRTx('F');
        IRTx(50);
        HAL_Delay(1);
        IRUpdateCMD();
        HAL_Delay(delay_ms);

        IRTx('B');
        IRTx(100);
        HAL_Delay(1);
        IRUpdateCMD();
        HAL_Delay(delay_ms);

        IRTx('S');
        IRTx(0);
        HAL_Delay(1);
        IRUpdateCMD();
        HAL_Delay(delay_ms);
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    VSensorCplt();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == IR_TEST_Pin) {
        IRTxCallback();
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart2) {
        IRRxCallback();
    }
}