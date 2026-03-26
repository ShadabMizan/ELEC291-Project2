#include "app.h"
#include "automode.h"
#include "main.h"
#include "manualmode.h"
#include "motor.h"
#include "stm32l0xx_hal_gpio.h"
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
    // InitTOF();   // Will cause +9% FLASH usage
    
    IRTxInit();
    IRRxInit();

    // char steps[] = {'L', 'R', 'B', 'F', 'S'};
    // int i = 0;

    HAL_GPIO_TogglePin(STAT_LED_GPIO_Port, STAT_LED_Pin);
    
    while (1) {
        // printf("Test...\r\n");

        // IRTx(steps[i++]);
        // if (i > sizeof(steps) - 1) {
        //     i = 0;
        // }

        // IRTx(80);
        // HAL_Delay(1);

        // ManualMode();
        AutoMode();
        HAL_Delay(1);

        
        // printf("CH0: %.2f\nCH1: %.2f\r\n", GetVolts(0), GetVolts(1));
        // printf("Range: %dmm\r\n", GetRange_mm());
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