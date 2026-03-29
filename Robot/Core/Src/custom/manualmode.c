#include "manualmode.h"
#include "main.h"
#include "motor.h"
#include "ir.h"

#include <stdio.h>

void ManualMode(void) {
    uint8_t cmd = 'S';
    uint8_t val = 0;

    if (IRRxGet(&cmd, &val)) {
        // Update
        printf("Recieved: %c (%u), %u\r\n", (char)cmd, cmd, val);
        switch(cmd) {
            case 'S':   // 'S'
                printf("Stopped\r\n");
                StopMotors();
                break;
            case 'L':   // 'L'
                printf("Left\r\n");
                GoLeft(val/100.0f);
                break;
            case 'R':   // 'R'
                printf("Right\r\n");
                GoRight(val/100.0f);
                break;
            case 'F':   // 'F'
                printf("Forward\r\n");
                GoForward(val/100.0f);
                break;
            case 'B':   // 'B'
                printf("Backward\r\n");
                GoBackward(val/100.0f);
                break;
            default:
                break;
        }
    }
}