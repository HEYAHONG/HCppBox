/***************************************************************
 * Name:      main.cpp
 * Purpose:   moxiebox基础固件入口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "hruntime.h"
#include "hshell.h"
#include "hrc.h"

void vApplicationIdleHook(void)
{
}

void vApplicationStackOverflowHook(TaskHandle_t xTask,char *pcTaskName)
{
    (void)xTask;
    (void)pcTaskName;

    for(;;)
    {
    }
}

void vApplicationMallocFailedHook(void)
{
    for(;;)
    {
    }
}

static void main_task(void *arg)
{
    (void)arg;

    hruntime_init();

    for(;;)
    {
        hruntime_loop();
        vTaskDelay(1);
    }
}

int main(void)
{
    hruntime_init_lowlevel();

    {
        const char *banner=(const char *)RCGetHandle("banner");
        if(banner!=NULL)
        {
            hshell_printf(NULL,"%s",banner);
        }
    }

    hshell_printf(NULL,"HBox hshell base\r\n");

    if(xTaskCreate(main_task,"main",2048,NULL,2,NULL)!=pdPASS)
    {
        for(;;)
        {
        }
    }

    vTaskStartScheduler();

    for(;;)
    {
    }

    return 0;
}
