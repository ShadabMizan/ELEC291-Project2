#include "automode.h"
#include "motor.h"
#include "vsensor.h"
#include "main.h"
#include "tof.h"

#include <math.h>
#include <stdio.h>
#include <sys/_intsup.h>

#define VCOIL_DIFF_THRESHOLD    0.1f

#define STOP_DISTANCE_MM        100.0   

#define AUTOMODE_SPEED          0.75f

#define CENTRECOIL_THRESH       0.4f
#define CROSSROAD_DELAY_MS      750
#define CROSSROAD_DETECTED_DELAY_MS     150

static const char path_table[3][8] = {
    {'F', 'L', 'L', 'F', 'R', 'L', 'R', 'S'},
    {'L', 'R', 'L', 'R', 'F', 'F', 'S', 'S'},
    {'R', 'F', 'R', 'L', 'R', 'L', 'F', 'S'}
};

static uint8_t intersection_cnt = 0;
static uint8_t at_intersection = 0;
static uint8_t done = 0;

void AutoMode(uint8_t path, uint8_t rst) {
    if (rst) {
        intersection_cnt = 0;
        done = 0;
    } else if (done) {
        StopMotors();
    }

    RunVSensor();

    float left_coilV = GetVolts(LEFT_COIL);
    float right_coilV = GetVolts(RIGHT_COIL);
    float centre_coilV = GetVolts(CENTRE_COIL);

    float collision_dist_mm = GetRange_mm();

    // printf("L: %.4fV, R: %.4fV, C: %.4fV\r\n", left_coilV, right_coilV, centre_coilV);
    // printf("D: %.4fmm\r\n", collision_dist_mm);

    float coil_diff = left_coilV - right_coilV;

    if (collision_dist_mm <= STOP_DISTANCE_MM && collision_dist_mm > 0) {
        StopMotors();
        return;
    }

    if (centre_coilV > CENTRECOIL_THRESH) {
        if (!at_intersection) {
            at_intersection = 1;
            char cmd = path_table[path][intersection_cnt];
            intersection_cnt++;
            printf("Path #%u, Intersection #%u: %c\r\n", path, intersection_cnt, cmd);

            HAL_Delay(CROSSROAD_DETECTED_DELAY_MS);

            switch(cmd) {
                case 'F':
                    GoForward(AUTOMODE_SPEED); 
                    HAL_GPIO_TogglePin(STAT_LED_GPIO_Port, STAT_LED_Pin);
                    HAL_Delay(CROSSROAD_DELAY_MS); 
                    HAL_GPIO_TogglePin(STAT_LED_GPIO_Port, STAT_LED_Pin);
                    break;
                case 'L': 
                    GoLeft(AUTOMODE_SPEED); 
                    HAL_GPIO_TogglePin(STAT_LED_GPIO_Port, STAT_LED_Pin);
                    HAL_Delay(CROSSROAD_DELAY_MS);
                    HAL_GPIO_TogglePin(STAT_LED_GPIO_Port, STAT_LED_Pin);
                    break;
                case 'R': 
                    GoRight(AUTOMODE_SPEED); 
                    HAL_GPIO_TogglePin(STAT_LED_GPIO_Port, STAT_LED_Pin);
                    HAL_Delay(CROSSROAD_DELAY_MS);
                    HAL_GPIO_TogglePin(STAT_LED_GPIO_Port, STAT_LED_Pin);
                    break;
                case 'S': 
                    StopMotors();
                    done = 1;
                    break;
            }
            return;
        }

        return;    
    } else {
        at_intersection = 0;
    }

    if (fabsf(coil_diff) < VCOIL_DIFF_THRESHOLD) {
        GoForward(AUTOMODE_SPEED);
    } else if (coil_diff > 0.0f) {
        GoRight(AUTOMODE_SPEED);
        // GoTRDiagonal(AUTOMODE_SPEED);
    } else {
        GoLeft(AUTOMODE_SPEED);
        // GoTLDiagonal(AUTOMODE_SPEED);
    }
}

