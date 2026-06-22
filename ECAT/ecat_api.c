/*
 * ecat_api.c — EtherCAT 从站 API 接口层实现
 *
 * 应用层与 SSC (EtherCAT Slave Stack Code) 的桥梁。
 * 职责：初始化协议栈、驱动主循环、PDO 数据映射、回调管理、状态查询。
 *
 * 数据流：
 *   主站 ──RxPDO──> ESC ──APPL_CoeRxPdoMapping──> DOUnit (DO(n)宏)
 *   主站 <──TxPDO── ESC <──APPL_CoeTxPdoMapping── DIUnit (DI(n)宏)
 */
#include "gd32f10x.h"   /* 必须第一个包含 — 避免 TRUE/FALSE 与 ecat_def.h 的枚举冲突 */
#include "ecat_api.h"
#include "applInterface.h"
#include "SSC-Device.h"
#include "GD32Evb.h"
#include "App_Flash.h"
#include "gd32f10x.h"
#include "systick.h"
#include "wdg_ecat.h"
#include <string.h>

/* ---- SSC 桥接函数指针 — SSC 协议栈通过这些指针调用应用层 ---- */

void (*g_pfnPeriodicTask)(void)          = 0;  /* 周期性任务（APPL_UpdateTxPdo） */
void (*g_pfnSafeOutput)(void)            = 0;  /* 安全输出（APPL_SafeOutput） */
void (*g_pfnTxPdoMapping)(UINT16* pData) = 0;
void (*g_pfnRxPdoMapping)(UINT16* pData) = 0;

static ecat_periodic_cb_t    s_pfnUserPeriodic = 0;
static ecat_safe_output_cb_t s_pfnUserSafeOut  = 0;

/* ---- PDO 映射 — SSC 在 PDO 交换周期中调用 ---- */

/* TxPDO: 应用数据 → ESC。DIUnit 拷到 ESC 缓冲区，主站即可读到。
 * 分两段：pData[0..254]←DIUnit10x6000(主段), pData[255..509]←DIUnit30x6001(副段)。
 * 先调用户周期回调更新 DI 数据(天线 RSSI/EPC)。 */
void APPL_CoeTxPdoMapping(UINT16* pData)
{
    const uint16_t second_words = (uint16_t)(PDO_TX_UINT16 - 255U);

    if (s_pfnUserPeriodic) {
        s_pfnUserPeriodic();
    }

    memcpy(pData,       &DIUnit10x6000.u16SubIndex0 + 1, sizeof(DIUnit10x6000.aEntries));
    memcpy(pData + 255, &DIUnit30x6001.u16SubIndex0 + 1, (size_t)second_words * sizeof(uint16_t));
}

/* RxPDO: ESC → 应用。主站数据拷到 DOUnit，应用层用 DO(n) 读命令参数。
 * 分两段：DOUnit20x7000←pData[0..254], DOUnit40x7001←pData[255..509]。 */
void APPL_CoeRxPdoMapping(UINT16* pData)
{
    const uint16_t second_words = (uint16_t)(PDO_RX_UINT16 - 255U);

    memcpy(&DOUnit20x7000.u16SubIndex0 + 1, pData,       sizeof(DOUnit20x7000.aEntries));
    memcpy(&DOUnit40x7001.u16SubIndex0 + 1, pData + 255, (size_t)second_words * sizeof(uint16_t));
}

/* ---- 桥接回调 — SSC 经 g_pfn* 调用，转发给用户回调 ---- */

static void Bridge_PeriodicTask(void)
{
    if (s_pfnUserPeriodic) {
        s_pfnUserPeriodic();
    }
}

static void Bridge_SafeOutput(void)
{
    if (s_pfnUserSafeOut) {
        s_pfnUserSafeOut();
    }
}

/* ---- 回调注册 — 在 main() 中调用 ---- */

void ECAT_RegisterPeriodicTask(ecat_periodic_cb_t cb)
{
    s_pfnUserPeriodic = cb;
}

void ECAT_RegisterSafeOutput(ecat_safe_output_cb_t cb)
{
    s_pfnUserSafeOut = cb;
}

/* ---- 协议栈核心 ---- */

/* 初始化协议栈：注册桥接指针 → 初始化 ESC(SPI, 重试3次) → 加载 Flash 参数 → MainInit */
void ECAT_Stack_Init(void)
{
    int retry = 0;

    g_pfnPeriodicTask = Bridge_PeriodicTask;
    g_pfnSafeOutput = Bridge_SafeOutput;
    g_pfnTxPdoMapping = APPL_CoeTxPdoMapping;
    g_pfnRxPdoMapping = APPL_CoeRxPdoMapping;

    while (HW_Init() != 0U && retry < 3) {
        retry++;
        delay_1ms(100U);
    }

    App_Load_Params_From_Flash();

    MainInit();
}

/* 驱动 SSC 主循环。必须在主 while(1) 中高频调用(至少每毫秒一次)，
 * 否则 EtherCAT 通信中断、SM 看门狗超时。本函数不喂硬件看门狗，
 * 主循环中需单独调 WDG_Feed()。
 * 应用层看门狗(ECAT_WD_TIMEOUT_MS>0 时启用)：主站停止发 PDO 超时后调安全输出。 */
void ECAT_Stack_MainLoop(void)
{
#if ECAT_WD_TIMEOUT_MS > 0
    static uint32_t s_WatchdogTimer = 0U;
#endif

    MainLoop();

#if ECAT_WD_TIMEOUT_MS > 0
    if (bEcatOutputUpdateRunning) {
        s_WatchdogTimer = 0U;
    } else {
        s_WatchdogTimer++;
        if (s_WatchdogTimer >= ECAT_WD_TIMEOUT_MS) {
            if (g_pfnSafeOutput) {
                g_pfnSafeOutput();
            }
            s_WatchdogTimer = 0U;
        }
    }
#endif

    /* Flash 保存已禁用 — 擦除耗时约 20ms，会触发 SM 看门狗
    if (g_bConfigDirty) {
        App_Save_Params_To_Flash();
    }
    */
}

/* 阻塞期间保活 = ECAT_Stack_MainLoop() + WDG_Feed()。
 * 用于 RFID 命令等待响应期间(最长 500ms)，主循环被阻塞无法回 while(1) 顶部喂狗。
 * 确保 SSC 持续运行(PDO 映射正常→SM 看门狗不触发) + FWDGT 被喂(5s 不触发)。 */
void ECAT_KeepAlive(void)
{
    ECAT_Stack_MainLoop();
    WDG_Feed();
}

/* ---- 持久化参数 — Flash 读写 ---- */

PersistentParams_t g_PersistentParams;

uint32_t ECAT_GetParamSize(void)
{
    return sizeof(g_PersistentParams);
}

void* ECAT_GetParamPtr(void)
{
    return &g_PersistentParams;
}

/* 标记参数已修改，待写入 Flash（当前 Flash 保存已禁用） */
void ECAT_SetParamDirty(void)
{
    g_bConfigDirty = 1U;
}

/* ---- 状态查询 — 调试和监控用 ---- */

/* AL 状态: 0x01=INIT, 0x02=PRE-OP, 0x04=SAFE-OP, 0x08=OP, 高4位 0x10=Error */
uint8_t ECAT_GetAlState(void)
{
    return nAlStatus;
}

/* AL 状态码(错误码): 0x0000=无错误, 0x001B=SM看门狗, 0x0030=本地错误 */
uint16_t ECAT_GetAlStatusCode(void)
{
    uint16_t code = 0U;

    HW_EscReadWord(code, ESC_AL_STATUS_CODE_OFFSET);
    return code;
}

/* 本地错误码(由应用层 ECAT_StateChange 设置) */
uint16_t ECAT_GetLocalErrorCode(void)
{
    return u16LocalErrorCode;
}
