#include "manualmode.h"
#include "main.h"
#include "motor.h"
#include "ir.h"

void ManualMode(uint8_t cmd, uint8_t val) {
    switch(cmd) {
        case 'S':   // 'S'
            StopMotors();
            break;
        case 'L':   // 'L'
            GoLeft(val/100.0f);
            break;
        case 'R':   // 'R'
            GoRight(val/100.0f);
            break;
        case 'F':   // 'F'
            GoForward(val/100.0f);
            break;
        case 'B':   // 'B'
            GoBackward(val/100.0f);
            break;
        case 'T':
            GoRight(0.75);
            HAL_GPIO_TogglePin(STAT_LED_GPIO_Port, STAT_LED_Pin);
            HAL_Delay(1500);
            HAL_GPIO_TogglePin(STAT_LED_GPIO_Port, STAT_LED_Pin);
            StopMotors();
            break;
        case '1':
            GoTRDiagonal(val/100.0f);
            break;
        case '2':
            GoTLDiagonal(val/100.0f);
            break;
        case '3':
            GoBLDiagonal(val/100.0f);
            break;
        case '4':
            GoBRDiagonal(val/100.0f);
            break;
        default:
            break;
    }
}