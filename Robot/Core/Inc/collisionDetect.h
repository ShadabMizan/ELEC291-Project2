/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    collisionDetect.h
  * @brief   This file contains the common defines and prototypes for
  *          the collision detection and distance-based stop algorithm.
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COLLISION_DETECT_H
#define __COLLISION_DETECT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define STOP_DISTANCE_MM    200     /* Target stopping distance in mm        */
#define SLOW_ZONE_MM        500     /* Distance at which braking begins (mm) */
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void CollisionDetect(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

#ifdef __cplusplus
}
#endif

#endif /* __COLLISION_DETECT_H */
