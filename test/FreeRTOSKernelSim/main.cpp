#include "hrc.h"
#include "stdio.h"
#include "stdlib.h"
#include "HCPPBox.h"

bool running=true;
extern "C" void vApplicationStackOverflowHook( TaskHandle_t xTask,char * pcTaskName );
void vApplicationStackOverflowHook( TaskHandle_t xTask,char * pcTaskName )
{
    hfprintf(stderr,"%s StackOverflow!\r\n",pcTaskName);
    running=false;
    while(true);
}

void main_task1(void *arg)
{
    while(1)
    {
        vTaskDelay(1000);
        hprintf("task1 loop!\r\n");
    }
}

void main_task2(void *arg)
{
    while(1)
    {
        vTaskDelay(1500);
        hprintf("task2 loop!\r\n");
    }
}



int main()
{
    /*创建任务*/
    xTaskCreate( main_task1, "main_task1",4096, NULL, 1, NULL );
    xTaskCreate( main_task2, "main_task2",4096, NULL, 1, NULL );

    /* Start the scheduler so the tasks start executing. */
    vTaskStartScheduler();
    while(running);//一般不会到这里

    return 0;
}
