#include "hbox.h"
#include "HCPPBox.h"
#include <chrono>
#include <thread>
#include <iec_types_all.h>

/*
 * QX0.0
 */
extern "C" BOOL * __QX0_0;
static BOOL last_qx0_0;

static void hsoftplc_callback(hsoftplc_callback_type_t cb_type)
{
    switch(cb_type)
    {
    case HSOFTPLC_CALLBACK_TYPE_CONFIG_INIT_BEGIN:
    {
        hprintf("config init begin!\r\n");
    }
    break;
    case HSOFTPLC_CALLBACK_TYPE_CONFIG_INIT_END:
    {
         hprintf("config init end!\r\n");
    }
    break;
    case HSOFTPLC_CALLBACK_TYPE_CONFIG_RUN_BEGIN:
    {
        /*
         * 记录QX0.0的值
         */
        last_qx0_0=(*__QX0_0);
    }
    break;
    case HSOFTPLC_CALLBACK_TYPE_CONFIG_RUN_END:
    {
        /*
         * 判断QX0.0的值
         */
        if((*__QX0_0)!=last_qx0_0)
        {
            hprintf("QX0.0=%-08s tick=%u\r\n",(*__QX0_0)?"true":"false",(unsigned)hdefaults_tick_get());
        }
    }
    break;
    }
}

int main(int argc,char *argv[])
{

    /*
     * 设置软件PLC回调
     */
    hsoftplc_set_callback(hsoftplc_callback);

    hcpprt_init();
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        hcpprt_loop();
    }
	return 0;
}
