#ifndef IR_H
#define IR_H

#include <stdint.h>

// For Testing
void IRTxInit(void);
void IRTx(uint8_t msg);
void IRTxCallback(void);

void IRRxInit(void);
void IRUpdateCMD(void);
void IRRxCallback(void);

#endif
