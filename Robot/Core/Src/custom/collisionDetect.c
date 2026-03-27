/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    collisionDetect.c
  * @brief   Distance-based vehicle stop algorithm using VL53L0X TOF sensor.
  *          Proportionally brakes the vehicle and stops it at STOP_DISTANCE_MM.
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "collisionDetect.h"
#include "tof.h"
#include "pwm.h"

/* USER CODE BEGIN 0 */

/* Left motor:  CH0 = forward, CH1 = reverse  (PA0, PA1)                     */
/* Right motor: CH2 = forward, CH3 = reverse  (PA2, PA3)                     */
#define L_FWD   0
#define L_BWD   1
#define R_FWD   2
#define R_BWD   3

#define POLL_MS  20

typedef enum {
    STATE_APPROACH,
    STATE_BRAKING,
    STATE_STOPPED,
    STATE_SENSOR_ERR
} VehicleState_t;

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Motor Helpers                                                               */
/*----------------------------------------------------------------------------*/

/* USER CODE BEGIN 1 */

static void DriveForward(float duty)
{
    SetDuty(duty,  L_FWD);
    SetDuty(0.0f,  L_BWD);
    SetDuty(duty,  R_FWD);
    SetDuty(0.0f,  R_BWD);
}

static void DriveStop(void)
{
    SetDuty(0.0f, L_FWD);
    SetDuty(0.0f, L_BWD);
    SetDuty(0.0f, R_FWD);
    SetDuty(0.0f, R_BWD);
}

/* USER CODE END 1 */

/*----------------------------------------------------------------------------*/
/* Proportional Braking                                                        */
/*----------------------------------------------------------------------------*/

/* USER CODE BEGIN 2 */

static float CalcBrakingDuty(int distance_mm)
{
    if (distance_mm <= STOP_DISTANCE_MM) return 0.0f;
    if (distance_mm >= SLOW_ZONE_MM)     return 1.0f;

    float range = (float)(SLOW_ZONE_MM - STOP_DISTANCE_MM);
    float delta = (float)(distance_mm  - STOP_DISTANCE_MM);

    return delta / range;
}

/* USER CODE END 2 */

/*----------------------------------------------------------------------------*/
/* Main Algorithm                                                              */
/*----------------------------------------------------------------------------*/

/* USER CODE BEGIN 3 */

void CollisionDetect(void)
{
    InitTOF();
    InitPWM();
    DriveForward(1.0f);

    VehicleState_t state = STATE_APPROACH;

    while (1)
    {
        int distance = GetRange_mm();

        if (distance == -1)
        {
            /* I2C / sensor fault -- stop and blink LED */
            state = STATE_SENSOR_ERR;
            DriveStop();
            HAL_GPIO_TogglePin(STAT_LED_GPIO_Port, STAT_LED_Pin);
            HAL_Delay(100);
            continue;
        }

        if (distance == -2)
        {
            /* Out of range -- no obstacle detected, full speed */
            state = STATE_APPROACH;
            DriveForward(1.0f);
            HAL_GPIO_WritePin(STAT_LED_GPIO_Port, STAT_LED_Pin, GPIO_PIN_RESET);
            HAL_Delay(POLL_MS);
            continue;
        }

        if (distance <= STOP_DISTANCE_MM)
        {
            state = STATE_STOPPED;
            DriveStop();
            HAL_GPIO_WritePin(STAT_LED_GPIO_Port, STAT_LED_Pin, GPIO_PIN_SET);
        }
        else if (distance <= SLOW_ZONE_MM)
        {
            state = STATE_BRAKING;
            DriveForward(CalcBrakingDuty(distance));
            HAL_GPIO_WritePin(STAT_LED_GPIO_Port, STAT_LED_Pin, GPIO_PIN_RESET);
        }
        else
        {
            state = STATE_APPROACH;
            DriveForward(1.0f);
            HAL_GPIO_WritePin(STAT_LED_GPIO_Port, STAT_LED_Pin, GPIO_PIN_RESET);
        }

        HAL_Delay(POLL_MS);
    }
}

/* USER CODE END 3 */
