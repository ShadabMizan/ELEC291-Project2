#include "vsensor.h"
#include "main.h"
#include "adc.h"
#include "stm32l0xx_hal_adc.h"
#include "stm32l0xx_hal_adc_ex.h"

#define NUM_ADC_MEASUREMENTS    2
#define VREF        3.311f

static volatile uint8_t measurements_cplt_flag = 0;

static uint16_t adcbuf[NUM_ADC_MEASUREMENTS] = {0};

void InitVSensor(void) {
    HAL_ADCEx_Calibration_Start(&hadc, ADC_SINGLE_ENDED);
}

void RunVSensor(void) {
    measurements_cplt_flag = 0;
    HAL_ADC_Start_DMA(&hadc, (uint32_t *)adcbuf, NUM_ADC_MEASUREMENTS);

    while (!measurements_cplt_flag);
}

float GetVolts(uint32_t channel) {
    uint16_t adc_val = adcbuf[channel];
    float vADC = (adc_val/4095.0f) * VREF;

    return vADC;
}

void VSensorCplt(void) {
    measurements_cplt_flag = 1;
}