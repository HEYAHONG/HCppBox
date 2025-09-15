#include "stdio.h"
#include "stdlib.h"
#include "hbox.h"
#include "opensbi_port.h"


int main()
{
    hruntime_init_lowlevel();

    hruntime_init();

    while(true)
    {
        hruntime_loop();
    }
}

/*
 * 主初始化
 */
void  main_init(const hruntime_function_t *func)
{
    //注册命令
    HSHELL_COMMANDS_REGISTER(NULL);
    hshell_printf(NULL,"HBox Init(fdt addrress=%p)!\r\n",sbi_entry_para_data.a1);
}
HRUNTIME_INIT_EXPORT(main,255,main_init,NULL);
HRUNTIME_SYMBOL_EXPORT(main_init);

/*
 * 主循环
 */
void  main_loop(const hruntime_function_t *func)
{
    while(EOF!=hshell_loop(NULL));
}
HRUNTIME_LOOP_EXPORT(main,255,main_loop,NULL);
HRUNTIME_SYMBOL_EXPORT(main_loop);
