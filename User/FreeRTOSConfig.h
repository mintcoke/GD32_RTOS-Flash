#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/* 1. 引入 GD32 固件库头文件，以获取真实的系统频率 SystemCoreClock */
#include "gd32f10x.h"

/******************************************************************************/
/* 硬件时钟设置 **********************************/
/******************************************************************************/

/* 修改：不再使用死板的 20000000，而是读取 GD32 真实的运行频率 */
#define configCPU_CLOCK_HZ    ( SystemCoreClock )

/******************************************************************************/
/* 调度器行为设置 **********************************/
/******************************************************************************/

/* 修改：改为 1000U，即 1ms 触发一次滴答中断（最常用） */
#define configTICK_RATE_HZ                         ( 1000U )
#define configUSE_PREEMPTION                       1
#define configUSE_TIME_SLICING                     1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION    0
#define configUSE_TICKLESS_IDLE                    0
#define configMAX_PRIORITIES                       5U
#define configMINIMAL_STACK_SIZE                   128U
#define configMAX_TASK_NAME_LEN                    16U

/* 【核心修复】：解决 Tick 宽度报错！直接赋数字 1，强制代表 32 位宽 */
#define configENFORCE_SYSTEM_CALLS_FROM_KERNEL_ONLY  1
#define configTICK_TYPE_WIDTH_IN_BITS                1

#define configIDLE_SHOULD_YIELD                    1
#define configTASK_NOTIFICATION_ARRAY_ENTRIES      1U
#define configQUEUE_REGISTRY_SIZE                  8U
#define configENABLE_BACKWARD_COMPATIBILITY        1
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS    0
#define configSTACK_DEPTH_TYPE                     size_t
#define configMESSAGE_BUFFER_LENGTH_TYPE           size_t
#define configUSE_NEWLIB_REENTRANT                 0

/******************************************************************************/
/* 软件定时器设置 ****************************************/
/******************************************************************************/

#define configUSE_TIMERS                1
#define configTIMER_TASK_PRIORITY       ( configMAX_PRIORITIES - 1U )
#define configTIMER_TASK_STACK_DEPTH    ( configMINIMAL_STACK_SIZE * 2 )
#define configTIMER_QUEUE_LENGTH        10U

/******************************************************************************/
/* 内存分配设置 *************************************/
/******************************************************************************/

/* 修复：关闭静态分配，避免编译器报错找不到 GetIdleTaskMemory 等钩子函数 */
#define configSUPPORT_STATIC_ALLOCATION              0
#define configSUPPORT_DYNAMIC_ALLOCATION             1

/* 修改：将动态堆内存调大到 15KB，原先的 4096 (4KB) 跑五路声纳轮询容易栈溢出 */
#define configTOTAL_HEAP_SIZE                        ( 15 * 1024U )
#define configAPPLICATION_ALLOCATED_HEAP             0
#define configSTACK_ALLOCATION_FROM_SEPARATE_HEAP    0
#define configUSE_MINI_LIST_ITEM                     0

/******************************************************************************/
/* 中断优先级配置 (修复死机隐患) *********************************/
/******************************************************************************/

/* Cortex-M3 专用：原文件全为 0 会导致 FreeRTOS 锁死全局中断，必须修改如下 */
#define configPRIO_BITS                            4 
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY    15
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5

#define configKERNEL_INTERRUPT_PRIORITY          ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
#define configMAX_SYSCALL_INTERRUPT_PRIORITY     ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
#define configMAX_API_CALL_INTERRUPT_PRIORITY    0U

/******************************************************************************/
/* 钩子函数设置 ****************************/
/******************************************************************************/

#define configUSE_IDLE_HOOK                   0
#define configUSE_TICK_HOOK                   0
#define configUSE_MALLOC_FAILED_HOOK          0
#define configUSE_DAEMON_TASK_STARTUP_HOOK    0
#define configCHECK_FOR_STACK_OVERFLOW        0

/******************************************************************************/
/* 运行时间与统计设置 *********************/
/******************************************************************************/

#define configGENERATE_RUN_TIME_STATS           0
#define configUSE_TRACE_FACILITY                0
#define configUSE_STATS_FORMATTING_FUNCTIONS    0

/******************************************************************************/
/* 功能裁剪设置 *************************/
/******************************************************************************/

#define configUSE_TASK_NOTIFICATIONS           1
#define configUSE_MUTEXES                      1
#define configUSE_RECURSIVE_MUTEXES            1
#define configUSE_COUNTING_SEMAPHORES          1
#define configUSE_QUEUE_SETS                   1
#define configUSE_APPLICATION_TASK_TAG         1
#define INCLUDE_vTaskPrioritySet               1
#define INCLUDE_uxTaskPriorityGet              1
#define INCLUDE_vTaskDelete                    1
#define INCLUDE_vTaskSuspend                   1
#define INCLUDE_xResumeFromISR                 1
#define INCLUDE_vTaskDelayUntil                1
#define INCLUDE_vTaskDelay                     1
#define INCLUDE_xTaskGetSchedulerState         1
#define INCLUDE_xTaskGetCurrentTaskHandle      1
#define INCLUDE_uxTaskGetStackHighWaterMark    1
#define INCLUDE_xTaskGetIdleTaskHandle         1
#define INCLUDE_eTaskGetState                  1
#define INCLUDE_xEventGroupSetBitFromISR       1
#define INCLUDE_xTimerPendFunctionCall         1
#define INCLUDE_xTaskAbortDelay                1
#define INCLUDE_xTaskGetHandle                 1
#define INCLUDE_xTaskResumeFromISR             1

/******************************************************************************/
/* 中断接管映射 (极其重要，GD32 裸机切换 RTOS 必备) **********************/
/******************************************************************************/
#define vPortSVCHandler      SVC_Handler
#define xPortPendSVHandler   PendSV_Handler
#define xPortSysTickHandler  SysTick_Handler

#endif /* FREERTOS_CONFIG_H */
