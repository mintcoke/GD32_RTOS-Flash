#ifndef Application_H
#define Application_H

#include "ecat_api.h"
#include "gd32f10x.h"

void APPL_UpdateTxPdo(void);
void APPL_SafeOutput(void);
uint8_t RFID_EcatCmdTask(void);

#endif
