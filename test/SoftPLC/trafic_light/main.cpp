#include "hbox.h"
#include "HCPPBox.h"
#include <chrono>
#include <thread>
#include HSOFTPLC_IEC_BASE_TYPE_HEADER

std::map<hsoftplc_database_key_t,hsoftplc_database_value_cache_t> plc_variable_cache;

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
        hsoftplc_get_located_all_variables([](const char *name,void *var,void *usr)
        {
            hsoftplc_database_key_t key=hsoftplc_database_key_get_from_variable_name(name);
            hsoftplc_database_value_cache_init(&plc_variable_cache[key],name,var);
        },NULL);
        hprintf("config init end!\r\n");
        hprintf("\r\n");
    }
    break;
    case HSOFTPLC_CALLBACK_TYPE_CONFIG_RUN_BEGIN:
    {

    }
    break;
    case HSOFTPLC_CALLBACK_TYPE_CONFIG_RUN_END:
    {
        bool is_update=false;
        hsoftplc_get_located_all_variables([](const char *name,void *var,void *usr)
        {
            hsoftplc_variable_name_t iec_addr;
            hsoftplc_get_iec_addr_from_variable_name(iec_addr,name);
            hsoftplc_database_key_t key=hsoftplc_database_key_get_from_variable_name(iec_addr);
            hsoftplc_database_value_cache_update(&plc_variable_cache[key],iec_addr,[](const hsoftplc_database_value_cache_t *cache,const char *variable_name,void *usr)
            {
                if(usr!=NULL)
                {
                    (*(bool *)usr)=true;
                }
                if(cache!=NULL && variable_name!=NULL)
                {
                    hprintf("%-16s=%08X%08X\r\n",variable_name,(uint32_t)(cache->value1 >> 32),(uint32_t)cache->value1);
                }
            },usr);
        },&is_update);
        if(is_update)
        {
            hprintf("tick=%u\r\n",(unsigned)hdefaults_tick_get());
            hprintf("\r\n");
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
