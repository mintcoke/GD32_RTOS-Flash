#ifndef Application_H
#define Application_H

#include "ecat_api.h"
#include "gd32f10x.h"

void DO_LED_Ctrl(void);
void DO_LED_Off(void);
uint8_t RFID_EcatCmdTask(void);

#endif
