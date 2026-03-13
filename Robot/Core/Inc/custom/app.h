#ifndef APP_H
#define APP_H

#include <stdint.h>
#include "adc.h"

void app(void);

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif