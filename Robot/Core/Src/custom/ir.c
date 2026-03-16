#include "ir.h"
#include "main.h"
#include "pwm.h"
#include "stm32l0xx_hal_gpio.h"
#include "stm32l0xx_hal_uart.h"
#include "usart.h"
#include "gpio.h"

#include <string.h>
#include <stdio.h>

#define IR_RX_BUF_SIZE 24

static uint8_t rxbyte;
static char ir_rxbuf[IR_RX_BUF_SIZE];

static volatile uint8_t rxindex = 0;
static volatile uint8_t rxflag = 0;

// FOR TESTING
void IRTx(char *msg) {
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 100);
}

void IRTx_Callback(void) {
    GPIO_PinState state = HAL_GPIO_ReadPin(IR_TEST_GPIO_Port, IR_TEST_Pin);
    if (state == GPIO_PIN_RESET) {
        // Enable 38kHz PWM
        SetDuty(0.5, 0);
    } else if (state == GPIO_PIN_SET) {
        // Disable
        SetDuty(0, 0);
    }
}

void IRRxInit(void) {
    rxindex = 0;
    rxflag = 0;
    HAL_UART_Receive_IT(&huart2, &rxbyte, 1);
}

uint8_t IR_MessageAvailable(void) {
    return rxflag;
}

void IR_GetMessage(char *dest) {
    if (rxflag) {
        strcpy(dest, ir_rxbuf);
        rxflag = 0;
    }
}

void IRRx_Callback(void) {
    printf("%c\r\n", (char)rxbyte);
    HAL_UART_Receive_IT(&huart2, &rxbyte, 1);
}