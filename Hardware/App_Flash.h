/*
 * App_Flash.h — Flash 参数存储接口
 * 内部模块，业务工程师无需关心。由 ecat_api 自动调用。
 *
 * 持久化流程:
 *   Load: Flash → g_PersistentParams → 校验 checksum → 恢复
 *   Save: g_PersistentParams → 计算 checksum → Flash
 */

#ifndef __APP_FLASH_H
#define __APP_FLASH_H

#include <stdint.h>

extern uint8_t g_bConfigDirty;

void App_Load_Params_From_Flash(void);
void App_Save_Params_To_Flash(void);

#endif /* __APP_FLASH_H */
