#include "coindetect.h"
#include "main.h"
#include <stdio.h>

static volatile uint8_t coin_detected_flag = 0;

void RunCoinDetector(void) {
    if (coin_detected_flag == 1) {
        printf("COIN!!!\r\n");
        coin_detected_flag = 0;
    }
}

void CoinDetected(void) {
    coin_detected_flag = 1;
}