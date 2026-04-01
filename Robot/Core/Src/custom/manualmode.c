#include "manualmode.h"
#include "main.h"
#include "motor.h"
#include "ir.h"

#define STOP_CMD            'S'
#define LEFT_CMD            'L'
#define RIGHT_CMD           'R'
#define FORWARD_CMD         'F'
#define BACKWARD_CMD        'B'

#define TURN180_CMD         'T'

#define TOPRIGHT_CMD        'W'
#define TOPLEFT_CMD         'X'
#define BOTTOMLEFT_CMD      'Y'
#define BOTTOMRIGHT_CMD     'Z'

void ManualMode(uint8_t cmd, uint8_t val) {
    switch(cmd) {
        case STOP_CMD:   // 'S'
            StopMotors();
            break;
        case LEFT_CMD:   // 'L'
            GoLeft(val/100.0f);
            break;
        case RIGHT_CMD:   // 'R'
            GoRight(val/100.0f);
            break;
        case FORWARD_CMD:   // 'F'
            GoForward(val/100.0f);
            break;
        case BACKWARD_CMD:   // 'B'
            GoBackward(val/100.0f);
            break;
        case TURN180_CMD:
            GoRight(0.75);
            HAL_GPIO_TogglePin(STAT_LED_GPIO_Port, STAT_LED_Pin);
            HAL_Delay(1500);
            HAL_GPIO_TogglePin(STAT_LED_GPIO_Port, STAT_LED_Pin);
            StopMotors();
            break;
        case TOPRIGHT_CMD:
            GoTRDiagonal(val/100.0f);
            break;
        case TOPLEFT_CMD:
            GoTLDiagonal(val/100.0f);
            break;
        case BOTTOMLEFT_CMD:
            GoBLDiagonal(val/100.0f);
            break;
        case BOTTOMRIGHT_CMD:
            GoBRDiagonal(val/100.0f);
            break;
        default:
            StopMotors();
            break;
    }
}