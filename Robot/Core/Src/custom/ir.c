#include "ir.h"
#include "main.h"
#include "motor.h"
#include "pwm.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "manualmode.h"

#include <string.h>
#include <stdio.h>

static uint8_t rxbytes[2];

static volatile uint8_t rxflag = 0;

static uint8_t start_flag = 0;

// FOR TESTING
void IRTxInit(void) {
    htim22.Init.Period = 842 - 1;
    HAL_TIM_Base_Init(&htim22);
    
    HAL_TIM_PWM_Start(&htim22, TIM_CHANNEL_1);
}

void IRTx(uint8_t msg) {
    uint8_t byte = msg;
    HAL_UART_Transmit(&huart2, &byte, 1, 100);
}

// Call this in the GPIO ISR. UART_TX is wired directly into the GPIO Interrupt Pin
void IRTxCallback(void) {
    GPIO_PinState state = HAL_GPIO_ReadPin(IR_TEST_GPIO_Port, IR_TEST_Pin);
    if (state == GPIO_PIN_RESET) {
        // Enable 38kHz PWM
        TIM22->CCR1 = 421;
    } else if (state == GPIO_PIN_SET) {
        // Disable
        TIM22->CCR1 = 0;
    }
}

// IR Receiver
void IRRxInit(void) {
    rxflag = 0;
    HAL_UART_Receive_IT(&huart2, rxbytes, 1);
}

// Returns 1 for new data, 0 otherwise.
uint8_t IRRxGet(uint8_t *cmd, uint8_t *val) {
    if (rxflag) {
        *cmd = rxbytes[0];
        *val = rxbytes[1];
        rxflag = 0;

        return 1;
    }
    
    return 0;
}

void IRRxCallback(void) {
    if (rxbytes[0] == 'N') {
        printf("Started IR!\r\n");
        HAL_UART_Receive_IT(&huart2, rxbytes, 2);
    } else {
        rxflag = 1;
        HAL_UART_Receive_IT(&huart2, rxbytes, 2);
    }
}