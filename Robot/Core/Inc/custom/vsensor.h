#ifndef VSENSOR_H
#define VSENSOR_H

#define VSENSOR_CH1     0
#define VSENSOR_CH2     1

#include "adc.h"

void InitVSensor(void);
void RunVSensor(void);
void VSensorCplt(void);

float GetVolts(uint32_t channel);

#endif
