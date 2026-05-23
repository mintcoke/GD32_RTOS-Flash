#ifndef __SONAR_H
#define __SONAR_H

#include "gd32f10x.h"

/* 寄存器地址 (根据规格书) */
#define REG_PROCESS_VAL_H 	0x01  // 处理值高位(滤波后)
#define REG_PROCESS_VAL_L 	0x00  // 处理值低位
#define REG_REALTIME_VAL_H 	0x01 	// 实时值高位
#define REG_REALTIME_VAL_L 	0x01 	// 实时值低位

/* 函数声明 */
uint16_t Modbus_CRC16(uint8_t *puchMsg, uint16_t usDataLen);
void Sonar_Send_Read_Cmd(uint8_t addr);
int32_t Sonar_Parse_Data(uint8_t *buf, uint16_t len, uint8_t expect_addr);
void vSonarPollingTask(void *pvParameters);

#endif /* __SONAR_H */
