#ifndef __FLASH_EEPROM_H
#define __FLASH_EEPROM_H

#include "gd32f10x.h"
#include <stdint.h>

/* ====================================================
 * 函数声明
 * ==================================================== */

/**
 * @brief  写入配置数据到片内 Flash (带 RTOS 临界区保护)
 * @param  data: 要写入的 uint32_t 数组指针
 * @param  length: 要写入的数据长度（以 32-bit 为单位）
 */
void Flash_Write_Config(uint32_t *data, uint16_t length);

/**
 * @brief  从片内 Flash 读取配置数据
 * @param  buffer: 用于存放读取数据的 uint32_t 数组指针
 * @param  length: 要读取的数据长度（以 32-bit 为单位）
 */
void Flash_Read_Config(uint32_t *buffer, uint16_t length);
/* 变量声明 */
extern uint8_t need_save_flash;
extern uint32_t sys_config[3];
#endif /* __FLASH_EEPROM_H */
