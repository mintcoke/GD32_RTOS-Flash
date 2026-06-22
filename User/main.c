/*
 * main.c — EtherCAT + RFID 从站主程序入口
 *
 * 硬件平台：GD32F103CBT6 + TR8253 RFID 模块
 * EtherCAT ESC：通过 SPI1 连接
 * RFID 通信：USART0 115200bps + DMA
 *
 * 主循环流程：
 *   1. ECAT_Stack_MainLoop() — 驱动 SSC 协议栈（必须最高频调用）
 *   2. RFID_EcatCmdTask()   — 处理 PLC 发来的 RFID 命令（仅 OP 状态）
 *   3. RFID_Scan()          — 无命令时自动轮询 3 天线标签（仅 OP 状态）
 *   4. RFID_WatchdogCheck() — 检测 RFID 模块是否卡死（所有状态下都运行）
 *   5. ECAT_Monitor()       — 打印 AL 状态变化
 *   6. WDG_Feed()           — 喂硬件看门狗 FWDGT（5s 超时）
 *
 * 注意事项：
 *   - RFID_WatchdogCheck() 在 OP 门控之外运行，因为 RFID 模块
 *     在任何 EtherCAT 状态下都可能卡死，需要及时恢复
 *   - RFID 命令阻塞期间使用 ECAT_KeepAlive() 保活，不会导致掉线
 */
#include "gd32f10x.h"
#include "systick.h"
#include "spi_ecat.h"
#include "timer_ecat.h"
#include "gpio_ecat.h"
#include "ecat_def.h"
#include "SSC-Device.h"
#include "applInterface.h"
#include "GD32Evb.h"
#include "Application.h"
#include "ecat_api.h"
#include "uart.h"
#include "rfid_ecat.h"
#include "wdg_ecat.h"

/*───────────────────────────────────────────────────────────
 * ECAT_Monitor — AL 状态变化日志
 *
 * 仅在 AL 状态发生变化时打印一条，避免串口刷屏。
 *   AL = AL 状态（0x01=INIT, 0x02=PREOP, 0x04=SAFEOP, 0x08=OP）
 *   SC = AL Status Code（错误码，0x001B=SM看门狗, 0x001A=同步错误）
 *   LE = 本地错误码（由 ECAT_StateChange 设置）
 *───────────────────────────────────────────────────────────*/
static void ECAT_Monitor(void)
{
    static uint8_t last_al = 0xFFU;  /* 上一次的 AL 状态，初始为无效值确保首次打印 */
    uint8_t al = ECAT_GetAlState();

    if (al != last_al) {
        uint16_t status_code = ECAT_GetAlStatusCode();
        uint16_t local_err  = ECAT_GetLocalErrorCode();

        printf("[ECAT] AL=0x%02X SC=0x%04X LE=0x%04X\r\n",
               al, status_code, local_err);

        last_al = al;
    }
}

/*───────────────────────────────────────────────────────────
 * main — 程序入口
 *───────────────────────────────────────────────────────────*/
int main(void)
{
    /* ---- 系统初始化 ---- */
    SystemInit();                           /* GD32 时钟配置：72MHz */
    systick_config();                       /* SysTick 1ms 中断，驱动 g_SysTickCnt */
    nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0); /* 4 位抢占优先级，0 位子优先级 */

    /* ---- 外设初始化 ---- */
    UART_Init();                            /* 调试串口 (USART1) */
    MX_GPIO_Init();                         /* GPIO 初始化 (LED, RFID EN 等) */
    MX_SPI1_Init();                         /* SPI1 初始化 (连接 ESC) */
    MX_TIM2_Init();                         /* TIM2 初始化 (ESC PDI 中断或定时) */

    /* ---- EtherCAT 协议栈初始化 ---- */
    ECAT_Stack_Init();                      /* 初始化 ESC + SSC 状态机 */
    ECAT_RegisterPeriodicTask(APPL_UpdateTxPdo); /* 注册周期性任务：每个 PDO 周期更新天线数据 */
    ECAT_RegisterSafeOutput(APPL_SafeOutput);    /* 注册安全输出：SAFE-OP 时调用（当前空实现） */

    /* ---- RFID 模块初始化 ---- */
    RFID_Init();                            /* USART0 + DMA + 天线 GPIO */

    /* ---- 硬件看门狗初始化 ---- */
    WDG_Init();                             /* FWDGT, 5s 超时, 40kHz RC / DIV64 / 重载3125 */

    /* ============================================================
     * 主循环 — 严格按顺序执行，不可随意调换
     *
     * ECAT_Stack_MainLoop() 必须最高频调用，
     * 其他任务在其间隙执行。
     * ============================================================ */
    while (1) {
        /* 1. 驱动 EtherCAT 协议栈（处理 PDO、邮箱、AL 状态机等） */
        ECAT_Stack_MainLoop();

        /* 2. RFID 软件看门狗 — 所有状态下都检查
         *
         * 放在 OP 门控之外的原因：
         *   RFID 模块在 INIT/PRE-OP/SAFE-OP 下也可能卡死，
         *   如果只在 OP 下检查，进入 OP 后第一次扫描就可能
         *   遇到已经卡死的模块，导致首次通信就超时。
         *   全状态检查确保 RFID 模块始终可用。
         */
        if (RFID_WatchdogCheck()) {
            DI(TX_RFID_CMD_STATUS) = RFID_PLC_STATUS_ERROR;   /* 通知主站：RFID 故障 */
            DI(TX_RFID_CMD_RESULT) = RFID_ERR_WD_TIMEOUT;     /* 错误码：看门狗超时 */
        }

        /* 3. OP 状态下执行 RFID 任务 */
        if ((ECAT_GetAlState() & 0x0FU) == 0x08U) {
            /* 优先处理 PLC 命令；无命令时才做自动扫描 */
            if (!RFID_EcatCmdTask()) {
                RFID_Scan();
            }
        }

        /* 4. 打印 AL 状态变化 */
        ECAT_Monitor();

        /* 5. 喂硬件看门狗 — 防止 FWDGT 5s 超时复位 */
        WDG_Feed();
    }
}

/* 错误处理 — 禁用中断后死循环，需外部复位恢复 */
void Error_Handler(void)
{
    __disable_irq();
    while (1) {}
}
