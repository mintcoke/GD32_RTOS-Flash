/*
 * ecat_api.c — EtherCAT 从站 API 接口层实现
 *
 * 本文件是应用层与 SSC (EtherCAT Slave Stack Code) 之间的桥梁实现。
 * 主要职责：
 *   1. 初始化 EtherCAT 协议栈
 *   2. 驱动协议栈主循环
 *   3. PDO 数据映射（应用数据 ↔ ESC 缓冲区）
 *   4. 回调注册与管理
 *   5. 状态查询接口
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

/* ============================================================
 * SSC 桥接函数指针 — SSC 协议栈通过这些指针调用应用层
 * ============================================================ */

void (*g_pfnPeriodicTask)(void)          = 0;  /* 周期性任务（DO_LED_Ctrl） */
void (*g_pfnSafeOutput)(void)            = 0;  /* 安全输出（DO_LED_Off） */
void (*g_pfnTxPdoMapping)(UINT16* pData) = 0;  /* TxPDO 映射函数 */
void (*g_pfnRxPdoMapping)(UINT16* pData) = 0;  /* RxPDO 映射函数 */

/* 用户回调函数指针（通过 ECAT_Register* 注册） */
static ecat_periodic_cb_t    s_pfnUserPeriodic = 0;  /* 用户周期性回调 */
static ecat_safe_output_cb_t s_pfnUserSafeOut  = 0;  /* 用户安全输出回调 */
static sonar_fail_cb_t       s_pfnSonarFail    = 0;  /* 超声波故障回调（未使用） */

/* ============================================================
 * PDO 映射函数 — SSC 在 PDO 交换周期中调用
 * ============================================================ */

/*
 * TxPDO 映射：应用数据 → ESC 发送缓冲区
 *
 * 把 DIUnit 结构体数据拷贝到 ESC 的 TxPDO 缓冲区，主站即可读到。
 * 同时调用用户注册的周期性回调（更新天线数据到 DI）。
 *
 * PDO 分两段传输：
 *   pData[0..254]   ← DIUnit10x6000 (主段 255 words)
 *   pData[255..509] ← DIUnit30x6001 (副段 255 words)
 */
void APPL_CoeTxPdoMapping(UINT16* pData)
{
    const uint16_t second_words = (uint16_t)(PDO_TX_UINT16 - 255U);

    /* 先调用用户回调，更新 DI 数据（天线 RSSI/EPC 等） */
    if (s_pfnUserPeriodic) {
        s_pfnUserPeriodic();
    }

    /* 把 DIUnit 数据拷贝到 ESC TxPDO 缓冲区 */
    memcpy(pData,       &DIUnit10x6000.u16SubIndex0 + 1, sizeof(DIUnit10x6000.aEntries));
    memcpy(pData + 255, &DIUnit30x6001.u16SubIndex0 + 1, (size_t)second_words * sizeof(uint16_t));
}

/*
 * RxPDO 映射：ESC 接收缓冲区 → 应用数据
 *
 * 把主站发来的 RxPDO 数据从 ESC 缓冲区拷贝到 DOUnit 结构体，
 * 应用层通过 DO(n) 宏读取命令参数。
 *
 * RxPDO 也分两段：
 *   DOUnit20x7000 ← pData[0..254]   (主段 255 words)
 *   DOUnit40x7001 ← pData[255..509] (副段 255 words)
 */
void APPL_CoeRxPdoMapping(UINT16* pData)
{
    const uint16_t second_words = (uint16_t)(PDO_RX_UINT16 - 255U);

    memcpy(&DOUnit20x7000.u16SubIndex0 + 1, pData,       sizeof(DOUnit20x7000.aEntries));
    memcpy(&DOUnit40x7001.u16SubIndex0 + 1, pData + 255, (size_t)second_words * sizeof(uint16_t));
}

/* ============================================================
 * SSC 桥接回调 — SSC 通过 g_pfn* 调用，内部转发给用户回调
 * ============================================================ */

/* SSC 周期性任务入口（在 ECAT_Application 中被调用） */
static void Bridge_PeriodicTask(void)
{
    if (s_pfnUserPeriodic) {
        s_pfnUserPeriodic();
    }
}

/* SSC 安全输出入口（进入 SAFE-OP 时被调用） */
static void Bridge_SafeOutput(void)
{
    if (s_pfnUserSafeOut) {
        s_pfnUserSafeOut();
    }
}

/* ============================================================
 * 回调注册函数 — 在 main() 中调用
 * ============================================================ */

/* 注册周期性任务回调（当前注册的是 DO_LED_Ctrl） */
void ECAT_RegisterPeriodicTask(ecat_periodic_cb_t cb)
{
    s_pfnUserPeriodic = cb;
}

/* 注册安全输出回调（当前注册的是 DO_LED_Off） */
void ECAT_RegisterSafeOutput(ecat_safe_output_cb_t cb)
{
    s_pfnUserSafeOut = cb;
}

/* 注册超声波故障回调（历史遗留，未使用） */
void ECAT_RegisterSonarFailCallback(sonar_fail_cb_t cb)
{
    s_pfnSonarFail = cb;
}

void ECAT_CheckSonarFail(void)
{
    if (s_pfnSonarFail) s_pfnSonarFail();
}

/* ============================================================
 * EtherCAT 协议栈核心函数
 * ============================================================ */

/*
 * 初始化 EtherCAT 协议栈
 *
 * 流程：
 *   1. 注册桥接函数指针（SSC 通过这些指针调用应用层）
 *   2. 初始化 ESC 硬件（SPI 通信），最多重试 3 次
 *   3. 从 Flash 加载持久化参数
 *   4. 调用 MainInit() 初始化 SSC 状态机
 *
 * 在 main() 中于所有外设初始化完成后调用
 */
void ECAT_Stack_Init(void)
{
    int retry = 0;

    /* 注册桥接函数，让 SSC 能调用应用层 */
    g_pfnPeriodicTask = Bridge_PeriodicTask;
    g_pfnSafeOutput = Bridge_SafeOutput;
    g_pfnTxPdoMapping = APPL_CoeTxPdoMapping;
    g_pfnRxPdoMapping = APPL_CoeRxPdoMapping;

    /* 初始化 ESC 硬件（SPI1 连接 ET1100/ET1200） */
    while (HW_Init() != 0U && retry < 3) {
        retry++;
        delay_1ms(100U);
    }

    /* 从 Flash 加载持久化参数 */
    App_Load_Params_From_Flash();

    /* 初始化 SSC 状态机（INIT 状态） */
    MainInit();
}

/*
 * 驱动 SSC 协议栈主循环
 *
 * 每次调用处理一轮：
 *   - 处理 ESC 中断事件（AL 事件、邮箱等）
 *   - 处理 AL 状态机转换
 *   - 执行 PDO 输入/输出映射
 *   - 调用 ECAT_Application（执行周期性任务）
 *
 * 必须在主 while(1) 中高频调用（至少每毫秒一次），
 * 否则 EtherCAT 通信会中断、SM 看门狗会超时。
 *
 * 注意：本函数不喂硬件看门狗，主循环中需单独调 WDG_Feed()
 */
void ECAT_Stack_MainLoop(void)
{
#if ECAT_WD_TIMEOUT_MS > 0
    static uint32_t s_WatchdogTimer = 0U;
#endif

    MainLoop();

#if ECAT_WD_TIMEOUT_MS > 0
    /* 应用层看门狗：如果主站停止发送 PDO 数据，超时后调用安全输出回调 */
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



/*
 * 阻塞期间保活函数
 *
 * = ECAT_Stack_MainLoop() + WDG_Feed()
 *
 * 用途：RFID 命令等待响应期间（最长 500ms），主循环被阻塞，
 *       不能回到 while(1) 顶部执行 WDG_Feed()。
 *       如果只调 ECAT_Stack_MainLoop()，硬件看门狗可能超时。
 *       使用本函数确保两件事都不中断：
 *         1. SSC 协议栈持续运行（PDO 映射正常 → SM 看门狗不触发）
 *         2. FWDGT 被喂狗（5s 硬件看门狗不触发）
 *
 * 调用位置：rfid_wait_frame()、RFID_Inventory()、
 *           RFID_WatchdogCheck()、RFID_EcatDelayMs()
 */
void ECAT_KeepAlive(void)
{
    ECAT_Stack_MainLoop();
    WDG_Feed();
}

/* ============================================================
 * 持久化参数 — Flash 读写
 * ============================================================ */

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

/* ============================================================
 * 状态查询函数 — 用于调试和监控
 * ============================================================ */

/* 读取 AL 状态（来自 SSC 内部变量，非 ESC 寄存器）
 * 返回值: 0x01=INIT, 0x02=PRE-OP, 0x04=SAFE-OP, 0x08=OP
 * 高4位 0x10=Error flag */
uint8_t ECAT_GetAlState(void)
{
    return nAlStatus;
}

/* 从 ESC 寄存器读取 AL 状态码（错误码）
 * 常见值: 0x0000=无错误, 0x001B=SM看门狗, 0x0030=本地错误 */
uint16_t ECAT_GetAlStatusCode(void)
{
    uint16_t code = 0U;

    HW_EscReadWord(code, ESC_AL_STATUS_CODE_OFFSET);
    return code;
}

/* 读取本地错误码（由应用层 ECAT_StateChange 设置） */
uint16_t ECAT_GetLocalErrorCode(void)
{
    return u16LocalErrorCode;
}
