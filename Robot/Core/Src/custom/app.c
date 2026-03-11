#include "app.h"
#include "main.h"
#include "usart.h"

#include <stdio.h>

int _write(int file, char *ptr, int len) {
    HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, HAL_MAX_DELAY);
    return len;
}

void app(void) {

    while (1) {
        printf("HELLO!!!\r\n");
        HAL_Delay(1000);
    }
}