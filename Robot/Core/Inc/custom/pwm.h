#ifndef PWM_H
#define PWM_H

#include <stdint.h>

void InitPWM(void);
void SetDuty(float duty, uint32_t channel);

#endif
