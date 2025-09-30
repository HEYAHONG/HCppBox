#include "hrc.h"
#include "stdio.h"
#include "stdlib.h"
#include "HCPPBox.h"

static bool running=true;
static bool hshell_active=false;
extern "C" void vApplicationStackOverflowHook( TaskHandle_t xTask,char * pcTaskName );
void vApplicationStackOverflowHook( TaskHandle_t xTask,char * pcTaskName )
{
    hfprintf(stderr,"%s StackOverflow!\r\n",pcTaskName);
    running=false;
    while(true);
}


static void main_task1(void *arg)
{
    while(1)
    {
        vTaskDelay(1000);
        if(!hshell_active)
        {
            hprintf("task1 loop!\r\n");
        }
    }
}

static void main_task2(void *arg)
{
    while(1)
    {
        vTaskDelay(1500);
        if(!hshell_active)
        {
            hprintf("task2 loop!\r\n");
        }
    }
}

static void hshell_task(void *arg);

int main()
{
    /*创建任务*/
    xTaskCreate( main_task1, "main_task1",40960, NULL, 1, NULL );
    xTaskCreate( main_task2, "main_task2",40960, NULL, 1, NULL );
    xTaskCreate( hshell_task, "hshell_task2",40960, NULL, 3, NULL );

    /* Start the scheduler so the tasks start executing. */
    vTaskStartScheduler();
    while(running);//一般不会到这里

    return 0;
}

#ifdef HAVE_FCNTL_H
#include "fcntl.h"
#endif // HAVE_FCNTL_H
#ifdef HDEFAULTS_OS_WINDOWS
#include <conio.h>
#endif // HDEFAULTS_OS_WINDOWS

static int getchar_nonblock(void)
{
#ifdef HDEFAULTS_OS_WINDOWS
    if(_kbhit())
    {
        return getchar();
    }
#else
#ifdef HAVE_FCNTL_H
    int old_option = fcntl( STDIN_FILENO, F_GETFL );
    int new_option = old_option | O_NONBLOCK;
    fcntl(STDIN_FILENO, F_SETFL, new_option );
#endif
    int8_t ch=EOF;
    if(EOF!=fread(&ch,sizeof(ch),1,stdin))
    {
        return ch;
    }
#endif
    return EOF;
}
static hshell_context_external_api_t hshell_api=hshell_context_default_external_api();
static int command_quit(int argc,const char *argv[]);
static int command_ps(int argc,const char *argv[]);
static hshell_command_t commands[]=
{
    {
        command_quit,
        "quit",
        "quit program"
    },
    {
        command_ps,
        "ps",
        "show process information"
    }
};

static void hshell_task(void *arg)
{
    //关闭输出缓冲
    setbuf(stdout,NULL);
    setbuf(stdin,NULL);
    hshell_echo_set(NULL,false);
    hshell_command_array_set(NULL,commands,sizeof(commands)/sizeof(commands[0]));
    hshell_api.getchar=getchar_nonblock;
    hshell_external_api_set(NULL,hshell_api);
    printf("press Enter to activate hshell!\r\n");
    while('\n'!=getchar_nonblock())
    {
        vTaskDelay(1);
    }
    hshell_active=true;
    while(running)
    {
        while(hshell_loop(NULL)==0);
        vTaskDelay(1);
    }
}

static int command_quit(int argc,const char *argv[])
{
    running=false;
    vTaskEndScheduler();
    return 0;
}

static int command_ps(int argc,const char *argv[])
{
    hshell_context_t * hshell_ctx=hshell_context_get_from_main_argv(argc,argv);
    char strbuff[4096];
    strbuff[sizeof(strbuff)-1]='\0';
    vTaskList(strbuff);
    hshell_printf(hshell_ctx,"%s\r\n",strbuff);
    return 0;
};



