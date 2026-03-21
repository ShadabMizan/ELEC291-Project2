#ifndef IR_H
#define IR_H

#include <stdint.h>

// For Testing
void IRTxInit(void);
void IRTx(uint8_t msg);
void IRTxCallback(void);



void IRRxInit(void);
uint8_t IRRxGet(uint8_t *cmd, uint8_t *val);
void IRRxCallback(void);

#endif
