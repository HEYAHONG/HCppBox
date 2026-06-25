#include "hbox.h"
#include "hrc.h"
#include "zephyr/autoconf.h"
#include "zephyr/kernel.h"

int main(void)
{
    hcpprt_init();
    while(true)
    {
        hcpprt_loop();
        {
            k_timeout_t timeout={1};
            k_sleep(timeout);
        }
    }
	return 0;
}

/*
 * 主初始化
 */
void  main_init(const hruntime_function_t *func)
{
    {
        const char *banner="";
        if(RCGetHandle("banner")!=NULL)
        {
            banner=(const char *)RCGetHandle("banner");
        }
        printf("\r\nhbox init!\r\n%s",banner);
    }
}
HRUNTIME_INIT_EXPORT(main,255,main_init,NULL);
HRUNTIME_SYMBOL_EXPORT(main_init);

/*
 * 主循环
 */
void  main_loop(const hruntime_function_t *func)
{

}
HRUNTIME_LOOP_EXPORT(main,255,main_loop,NULL);
HRUNTIME_SYMBOL_EXPORT(main_loop);

