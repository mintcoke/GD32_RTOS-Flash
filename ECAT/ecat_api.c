#include "gd32f10x.h"   /* Must be first — avoids TRUE/FALSE enum conflict with ecat_def.h */
#include "ecat_api.h"
#include "applInterface.h"
#include "SSC-Device.h"
#include "GD32Evb.h"
#include "App_Flash.h"
#include "gd32f10x.h"
#include "systick.h"
#include <string.h>

void (*g_pfnPeriodicTask)(void)          = 0;
void (*g_pfnSafeOutput)(void)            = 0;
void (*g_pfnTxPdoMapping)(UINT16* pData) = 0;
void (*g_pfnRxPdoMapping)(UINT16* pData) = 0;

static ecat_periodic_cb_t    s_pfnUserPeriodic = 0;
static ecat_safe_output_cb_t s_pfnUserSafeOut  = 0;
static sonar_fail_cb_t       s_pfnSonarFail    = 0;

extern UINT16 g_EcatLastAlEvent;
extern UINT16 g_EcatLastAlControl;
extern UINT32 g_EcatAlControlCount;

void APPL_CoeTxPdoMapping(UINT16* pData)
{
    memcpy(pData,       &DIUnit10x6000.u16SubIndex0 + 1, sizeof(DIUnit10x6000.aEntries));
    memcpy(pData + 255, &DIUnit30x6001.u16SubIndex0 + 1, sizeof(DIUnit30x6001.aEntries));
}

void APPL_CoeRxPdoMapping(UINT16* pData)
{
    memcpy(&DOUnit20x7000.u16SubIndex0 + 1, pData,       sizeof(DOUnit20x7000.aEntries));
    memcpy(&DOUnit40x7001.u16SubIndex0 + 1, pData + 255, sizeof(DOUnit40x7001.aEntries));
}

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

void ECAT_RegisterPeriodicTask(ecat_periodic_cb_t cb)
{
    s_pfnUserPeriodic = cb;
}

void ECAT_RegisterSafeOutput(ecat_safe_output_cb_t cb)
{
    s_pfnUserSafeOut = cb;
}

void ECAT_RegisterSonarFailCallback(sonar_fail_cb_t cb)
{
    s_pfnSonarFail = cb;
}

void ECAT_CheckSonarFail(void)
{
    if (s_pfnSonarFail) s_pfnSonarFail();
}

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

    /* Flash save disabled — erase takes ~20ms, triggers PDO watchdog
    if (g_bConfigDirty) {
        App_Save_Params_To_Flash();
    }
    */
}

PersistentParams_t g_PersistentParams;

uint32_t ECAT_GetParamSize(void)
{
    return sizeof(g_PersistentParams);
}

void* ECAT_GetParamPtr(void)
{
    return &g_PersistentParams;
}

void ECAT_SetParamDirty(void)
{
    g_bConfigDirty = 1U;
}

uint8_t ECAT_IsOpRunning(void)
{
    return bEcatOutputUpdateRunning ? 1U : 0U;
}

uint8_t ECAT_GetAlState(void)
{
    return nAlStatus;
}

uint16_t ECAT_GetAlStatusCode(void)
{
    uint16_t code = 0U;

    HW_EscReadWord(code, ESC_AL_STATUS_CODE_OFFSET);
    return code;
}

uint16_t ECAT_GetLocalErrorCode(void)
{
    return u16LocalErrorCode;
}

uint16_t ECAT_GetLastAlEvent(void)
{
    return g_EcatLastAlEvent;
}

uint16_t ECAT_GetLastAlControl(void)
{
    return g_EcatLastAlControl;
}

uint32_t ECAT_GetAlControlCount(void)
{
    return g_EcatAlControlCount;
}

uint32_t ECAT_GetEscIntCount(void)
{
    return g_EscIntCount;
}

uint16_t ECAT_ReadEscAlStatus(void)
{
    uint16_t value = 0U;

    HW_EscReadWord(value, ESC_AL_STATUS_OFFSET);
    return value;
}

uint16_t ECAT_ReadEscEventMask(void)
{
    uint16_t value = 0U;

    HW_EscReadWord(value, ESC_AL_EVENTMASK_OFFSET);
    return value;
}

uint16_t ECAT_ReadEscEepromStatus(void)
{
    uint16_t value = 0U;

    HW_EscReadWord(value, ESC_EEPROM_CONTROL_OFFSET);
    return value;
}

uint16_t ECAT_GetSyncType(void)
{
    /* Implemented in ecatappl.c — has full access to sSyncManOutPar */
    extern uint16_t ECAT_GetSyncType_Impl(void);
    return ECAT_GetSyncType_Impl();
}
