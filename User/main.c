#include "gd32f10x.h"
#include <stdio.h>
#include <string.h>
#include "uart.h"
#include "sonar.h"
#include "FreeRTOS.h"
#include "task.h"
#include "flash_eeprom.h"

int main(void)
{
    // 系统底层初始化
    nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
	UART_Init();

    printf("System Init OK! Starting FreeRTOS...\r\n");
    xTaskCreate(vSonarPollingTask, "SonarTask", 512, NULL, 2, NULL);

    // 启动任务调度器
    vTaskStartScheduler();
		
    // 正常情况下永远不会执行到这里
    while(1) {
    }
}
