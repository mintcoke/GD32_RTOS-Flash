/*
 * App_Flash.c — Flash 参数存储 (GD32 FMC)
 *
 * 持久化 g_PersistentParams 到 Flash 最后一页。
 * 业务用法见 README：改 ecat_api.h 的 PersistentParams_t 加字段，业务代码
 * 直接读写 g_PersistentParams，改后调 ECAT_SetParamDirty()。
 */

#include "App_Flash.h"
#include "gd32f10x.h"
#include "ecat_api.h"
#include <string.h>

uint8_t g_bConfigDirty = 0;

/* 校验和 = XOR 所有 uint32_t (不含 checksum 字段) */
static uint32_t ComputeChecksum(const PersistentParams_t *p)
{
    uint32_t sum = 0;
    uint32_t wordCnt = (sizeof(PersistentParams_t) - sizeof(p->checksum)) / 4;
    const uint32_t *pWords = (const uint32_t *)p;
    uint32_t i;
    for (i = 0; i < wordCnt; i++) {
        sum ^= pWords[i];
    }
    return sum;
}

/* GD32F103CBT6 128KB: 最后一页 1KB */
#define FLASH_ADDR  0x0801FC00

/* 保存到 Flash。内容未变则跳过(减少擦写磨损)。 */
void App_Save_Params_To_Flash(void)
{
    uint32_t size = sizeof(g_PersistentParams);

    g_PersistentParams.checksum = ComputeChecksum(&g_PersistentParams);

    if (memcmp(&g_PersistentParams, (void*)FLASH_ADDR, size) == 0) {
        g_bConfigDirty = 0;
        return;
    }

    fmc_unlock();
    fmc_flag_clear(FMC_FLAG_BANK0_END | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_PGERR);

    if (fmc_page_erase(FLASH_ADDR) == FMC_READY) {
        fmc_flag_clear(FMC_FLAG_BANK0_END | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_PGERR);
    }

    {
        uint32_t addr = FLASH_ADDR;
        uint32_t cnt  = (size + 3) / 4;
        uint32_t *p   = (uint32_t *)&g_PersistentParams;
        uint32_t i;
        fmc_state_enum prog_ok = FMC_READY;
        for (i = 0; i < cnt; i++) {
            if (fmc_word_program(addr, p[i]) != FMC_READY) {
                prog_ok = FMC_BUSY;
                break;
            }
            fmc_flag_clear(FMC_FLAG_BANK0_END | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_PGERR);
            addr += 4;
        }
        if (prog_ok == FMC_READY) g_bConfigDirty = 0;
    }

    fmc_lock();
}

/* 上电从 Flash 加载。校验失败(Flash 损坏/新结构体)则清零用默认值。 */
void App_Load_Params_From_Flash(void)
{
    if (*(__IO uint32_t*)FLASH_ADDR == 0xFFFFFFFF) {
        return;
    }

    memcpy(&g_PersistentParams, (void*)FLASH_ADDR, sizeof(g_PersistentParams));

    if (g_PersistentParams.checksum == ComputeChecksum(&g_PersistentParams)) {
        /* 校验通过，参数已恢复 */
    } else {
        memset(&g_PersistentParams, 0, sizeof(g_PersistentParams));
    }
}
