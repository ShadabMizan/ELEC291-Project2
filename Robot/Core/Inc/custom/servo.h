#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

void ServoInit(void);
void ServoSetAngle(uint8_t angle);

#endif