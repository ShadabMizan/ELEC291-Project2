#ifndef IR_H
#define IR_H

#include <stdint.h>

#define STOP_CMD    0x12
#define RIGHT_CMD   0x42
#define LEFT_CMD    0x09

void IRTx_Callback(void);
void IRTx(char *msg);

void IRRxInit(void);

uint8_t IR_MessageAvailable(void);
void IR_GetMessage(char *dest);
void IRRx_Callback(void);

#endif
