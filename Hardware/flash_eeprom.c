#include "flash_eeprom.h"
#include "FreeRTOS.h"
#include "task.h"

//128KB 闪存的芯片 (如 GD32F103CBT6)，倒数第一页的起始地址
#define FLASH_SAVE_ADDR  ((uint32_t)0x0801FC00) 

// 假设我们需要保存 3 个系统配置参数
uint32_t sys_config[3] = {0};
uint8_t need_save_flash = 0;   // 标志位：是否需要保存

/*!
    \brief      写入数据到 Flash (带 RTOS 临界区保护)
*/

void Flash_Write_Config(uint32_t *data, uint16_t length)
{
    // 判断调度器是否已经启动
    BaseType_t scheduler_running = (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED);

    if (scheduler_running) {
        taskENTER_CRITICAL(); // 调度器已启动，使用RTOS临界区保护
    } else {
        __disable_irq();      // 调度器未启动，直接使用裸机底层关闭中断
    }

    fmc_unlock(); // 解锁 Flash
    fmc_flag_clear(FMC_FLAG_BANK0_END | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_PGERR);

    fmc_page_erase(FLASH_SAVE_ADDR);
    fmc_flag_clear(FMC_FLAG_BANK0_END | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_PGERR);

    for (uint16_t i = 0; i < length; i++) {
        fmc_word_program(FLASH_SAVE_ADDR + (i * 4), data[i]);
        fmc_flag_clear(FMC_FLAG_BANK0_END | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_PGERR);
    }

    fmc_lock(); // 上锁

    if (scheduler_running) {
        taskEXIT_CRITICAL(); // 恢复RTOS临界区
    } else {
        __enable_irq();      // 恢复裸机中断
    }
}

/*!
    \brief      从 Flash 读取数据 (读取不需要保护，直接读即可)
*/
void Flash_Read_Config(uint32_t *buffer, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++) {
        buffer[i] = *(__IO uint32_t*)(FLASH_SAVE_ADDR + (i * 4));
    }
}
