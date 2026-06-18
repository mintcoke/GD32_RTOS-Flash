/*
 * wdg_ecat.c — 硬件看门狗 (FWDGT) 实现
 *
 * 超时计算：
 *   时钟源 = IRC40K (40kHz 内部 RC)
 *   预分频 = DIV64 → 40000/64 = 625 Hz
 *   重载值 = 3125 → 3125/625 = 5.0 秒
 *
 * FWDGT 启动后无法停止（硬件保证），是系统安全的最后防线。
 * 如果主循环卡死超过 5 秒（例如严重的硬件故障），
 * FWDGT 会触发芯片复位，让系统重新初始化。
 *
 * 喂狗位置：
 *   1. main.c 主循环末尾 — 正常运行时每次循环喂一次
 *   2. ECAT_KeepAlive() — RFID 阻塞等待期间持续喂狗
 */
#include "wdg_ecat.h"
#include "gd32f10x_fwdgt.h"

#define WDG_PRESCALER   FWDGT_PSC_DIV64  /* 40kHz / 64 = 625 Hz */
#define WDG_RELOAD      3125U            /* 3125 / 625 Hz = 5.0 秒 */

/*
 * 初始化 FWDGT
 *
 * 流程：
 *   1. 开启寄存器写访问权限
 *   2. 配置预分频和重载值
 *   3. 立即重载计数器
 *   4. 启动 FWDGT（此后无法停止）
 */
void WDG_Init(void)
{
    /* 开启 FWDGT 寄存器写访问权限 */
    fwdgt_write_enable();

    /* 配置预分频和重载值 */
    fwdgt_prescaler_value_config(WDG_PRESCALER);
    fwdgt_reload_value_config(WDG_RELOAD);

    /* 立即重载计数器，避免启动后立刻超时 */
    fwdgt_counter_reload();

    /* 启动 FWDGT — 之后无法停止 */
    fwdgt_enable();
}

/*
 * 喂狗 — 重载计数器
 *
 * 必须在 5 秒内调用，否则芯片硬复位。
 * 正常主循环约 1ms 一次，远低于 5 秒上限。
 */
void WDG_Feed(void)
{
    fwdgt_counter_reload();
}
