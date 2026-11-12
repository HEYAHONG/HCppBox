#include "stdio.h"
#include "stdlib.h"
#include "hbox.h"

extern "C" void vApplicationStackOverflowHook( TaskHandle_t xTask,char * pcTaskName );
void vApplicationStackOverflowHook( TaskHandle_t xTask,char * pcTaskName )
{
    hshell_printf(NULL,"%s StackOverflow!\r\n",pcTaskName);
    while(true);
}

extern "C" void vApplicationIdleHook( void );
void vApplicationIdleHook( void )
{
    /*
     * 空闲任务钩子
     */
}

void main_task(void *arg)
{
    hshell_printf(NULL,"main task enter!\r\n");

    hruntime_init();

    while(true)
    {
        hruntime_loop();
        vTaskDelay(1);
    }
}

int main()
{
    hshell_printf(NULL,"main enter!\r\n");

    hruntime_init_lowlevel();

    xTaskCreate( main_task, "main_task",4096/sizeof(StackType_t), NULL, 2, NULL );

    hshell_printf(NULL,"start scheduler!\r\n");
    vTaskStartScheduler();

    hshell_printf(NULL,"main leave!\r\n");

    return 0;
}

/*
 * 主初始化
 */
void  main_init(const hruntime_function_t *func)
{
    //注册命令
    HSHELL_COMMANDS_REGISTER(NULL);
}
HRUNTIME_INIT_EXPORT(main,255,main_init,NULL);
HRUNTIME_SYMBOL_EXPORT(main_init);

/*
 * 主循环
 */
void  main_loop(const hruntime_function_t *func)
{
    /*
     * 运行默认shell
     */
    hshell_loop(NULL);
}
HRUNTIME_LOOP_EXPORT(main,255,main_loop,NULL);
HRUNTIME_SYMBOL_EXPORT(main_loop);
