#include "hbox.h"
#include "HCPPBox.h"
#include <chrono>
#include <thread>
#include HSOFTPLC_IEC_BASE_TYPE_HEADER

static bool plc_get_qx0_0(void)
{
    /*
     * 获取%QX0.0
     */
    hsoftplc_variable_name_t variable_name;
    hsoftplc_get_variable_name_from_iec_addr(variable_name,"%QX0.0");
    IEC_BOOL *var=(IEC_BOOL *)hsoftplc_get_located_variables(variable_name);
    if(var!=NULL)
    {
        return (*var)!=0;
    }
    return false;
}
static bool last_qx0_0=false;
static void hsoftplc_callback(hsoftplc_callback_type_t cb_type)
{
    switch(cb_type)
    {
    case HSOFTPLC_CALLBACK_TYPE_CONFIG_INIT_BEGIN:
    {
        {
            /*
             * 枚举变量
             */
            size_t var_count=hsoftplc_get_located_all_variables([](const char *name,void *var,void *usr)
            {
                hsoftplc_variable_name_t iec_addr;
                hsoftplc_variable_name_t variable_name;
                hsoftplc_get_iec_addr_from_variable_name(iec_addr,name);
                hsoftplc_get_variable_name_from_iec_addr(variable_name,iec_addr);
                hprintf("---------------\r\n");
                hprintf("\tvar=%s,iec_addr=%s,addr=0x%p\r\n",variable_name,iec_addr,var);
                hsoftplc_variable_symbol_t variable_symbol;
                hsoftplc_parse_variable_symbol(&variable_symbol,name);
                if(variable_symbol.variable_location!='\0')
                {
                    hprintf("\tlocation=%c,size=%c,address0=%s,address1=%s,address2=%s\r\n",variable_symbol.variable_location,variable_symbol.variable_size,variable_symbol.variable_address[0],variable_symbol.variable_address[1],variable_symbol.variable_address[2]);
                }
                hprintf("---------------\r\n");
            },NULL);
            hprintf("var_count=%d\r\n",(int)var_count);
        }
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
         * 记录%QX0.0
         */
        last_qx0_0=plc_get_qx0_0();
    }
    break;
    case HSOFTPLC_CALLBACK_TYPE_CONFIG_RUN_END:
    {
        /*
         * 判断%QX0.0的值
         */
        if((plc_get_qx0_0())!=last_qx0_0)
        {
            hprintf("%%QX0.0=%-08s tick=%u\r\n",(plc_get_qx0_0())?"true":"false",(unsigned)hdefaults_tick_get());
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
