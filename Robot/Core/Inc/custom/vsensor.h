#ifndef VSENSOR_H
#define VSENSOR_H

#define LEFT_COIL       0
#define RIGHT_COIL      1
#define CENTRE_COIL     2

#include "adc.h"

void InitVSensor(void);
void RunVSensor(void);
void VSensorCplt(void);

float GetVolts(uint32_t channel);

#endif
