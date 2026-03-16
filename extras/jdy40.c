#include "jdy40.h"
#include "main.h"
#include "usart.h"
#include "gpio.h"

#include <string.h>
#include <stdio.h>

#define DEVICE_ID   "1738"

static uint8_t rxbyte = 0;
static char rxbuf[20];
static uint8_t rxindex = 0;

static uint8_t rxflag = 0;

void setDeviceID(char *id);

void JDYInit(void) {
    setDeviceID(DEVICE_ID);

    HAL_UART_Receive_IT(&huart2, &rxbyte, 1);
}

void setDeviceID(char *id) {
    char buf[20] = {0};
    sprintf(buf, "AT+DVID%4s\r\n", id);

    HAL_GPIO_WritePin(JDY_AT_GPIO_Port, JDY_AT_Pin, GPIO_PIN_RESET);
    HAL_Delay(50);

    HAL_UART_Transmit(&huart2, (uint8_t *)buf, strlen(buf), HAL_MAX_DELAY);
    HAL_Delay(50);

    HAL_GPIO_WritePin(JDY_AT_GPIO_Port, JDY_AT_Pin, GPIO_PIN_SET);
}

void JDYTransmit(char *msg) {
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}

void JDYReceive(void) {
    char c = (char)rxbyte;
    if ((c == ' ') || (c == '\r') || (c == '\n')) {
        rxflag = 1;
    } else {
        rxbuf[rxindex] = c;
        rxindex++;
    }
    
    HAL_UART_Receive_IT(&huart2, &rxbyte, 1);
}

void JDYGetMsg(void) {
    char msg[20] = {0};
    if (rxflag) {
        // Received a message
        strcpy(msg, rxbuf);
        rxindex = 0;
        memset(rxbuf, 0, sizeof(rxbuf));
        rxflag = 0;
        
        // Temp
        printf("Message Received: %s\r\n", msg);
    }
}