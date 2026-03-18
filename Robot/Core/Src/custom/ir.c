#include "ir.h"
#include "main.h"
#include "motor.h"
#include "pwm.h"
#include "stm32l0xx_hal_tim.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include <string.h>
#include <stdio.h>

#define IR_RX_BUF_SIZE 24

static uint8_t rxbytes[2];

static volatile uint8_t rxflag = 0;

// FOR TESTING
void IRTxInit(void) {
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
    HAL_UART_Receive_IT(&huart2, rxbytes, 2);
}

void IRUpdateCMD(void) {
    if (rxflag) {
        uint8_t cmd = rxbytes[0];
        uint8_t val = rxbytes[1];
        rxflag = 0;

        switch (cmd) {
            case 'L':   
                printf("Going Left\r\n");
                GoLeft(val/100.0f); 
                break;
            case 'R':   
                printf("Going Right\r\n");
                GoRight(val/100.0f);
                break;
            case 'F':   
                printf("Going Forward\r\n"); 
                GoForward(val/100.0f);
                break;
            case 'B':   
                printf("Going Backward\r\n"); 
                GoBackward(val/100.0f);
                break;
            case 'S':   
                printf("Stopping\r\n"); 
                Stop();
                break;
            default:    
                printf("Not a CMD: %u\r\n", cmd); 
                break;
        }

        printf("Value: %u\r\n", rxbytes[1]);
    }
}

void IRRxCallback(void) {
    // printf("%c\r\n", (char)rxbyte);
    rxflag = 1;
    HAL_UART_Receive_IT(&huart2, rxbytes, 2);
}