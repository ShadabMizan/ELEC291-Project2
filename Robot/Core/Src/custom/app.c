#include "app.h"
#include "automode.h"
#include "main.h"
#include "manualmode.h"
#include "motor.h"
#include "stm32l0xx_hal.h"
#include "stm32l0xx_hal_gpio.h"
#include "usart.h"
#include "vsensor.h"
#include "pwm.h"
#include "tof.h"
#include "ir.h"
#include "servo.h"
#include "sonar.h"

#include <stdio.h>

#define MANUAL_MODE     0
#define AUTO_MODE       1
#define SONAR_MODE      2

static uint8_t sonar_btn_flag = 0;

int _write(int file, char *ptr, int len) {
    HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, 100);
    return len;
}

void app(void) {
    InitVSensor();
    InitPWM();
    InitTOF();
    
    // IRTxInit();
    IRRxInit();

    ServoInit();
    ServoSetAngle(90);

    uint8_t cmd = 'S';
    uint8_t val = 0;

    HAL_GPIO_WritePin(STAT_LED_GPIO_Port, STAT_LED_Pin, GPIO_PIN_SET);
    printf("Starting...\r\n");

    uint8_t mode = MANUAL_MODE;
    uint8_t path = 0;
    uint8_t path_rst = 0;
    
    while (1) {
        if (sonar_btn_flag) {
            HAL_Delay(250);
            sonar_btn_flag = 0;
            if (mode != SONAR_MODE) {
                mode = SONAR_MODE;
            } else {
                mode = MANUAL_MODE;
            }
        }

        if (IRRxGet(&cmd, &val)) {
            printf("CMD: %c, VAL: %u\r\n", (char)cmd, val);
            if (cmd == 'M') {
                mode = MANUAL_MODE;
                ServoSetAngle(90);
                printf("Entering Manual Mode\r\n");
            } else if (cmd == 'A') {
                mode = AUTO_MODE;
                ServoSetAngle(90);
                printf("Entering Automatic Mode with Path %u\r\n", val);
                path = val;
                path_rst = 1;
            }
        }

        if (mode == MANUAL_MODE) {
            ManualMode(cmd, val);
        } else if (mode == AUTO_MODE) {
            AutoMode(path, path_rst);
            path_rst = 0;
        } else if (mode == SONAR_MODE) {
            Sonar();
        }

        HAL_Delay(1);
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    VSensorCplt();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == IR_TEST_Pin) {
        IRTxCallback();
    } else if (GPIO_Pin == SONAR_BTN_Pin) {
        sonar_btn_flag = 1;
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart2) {
        IRRxCallback();
    }
}