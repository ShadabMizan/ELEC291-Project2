#ifndef APP_H
#define APP_H

#include <stdint.h>
#include "main.h"
#include "gpio.h"
#include "adc.h"
#include "usart.h"

void app(void);

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif