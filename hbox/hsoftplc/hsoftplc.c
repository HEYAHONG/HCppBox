/***************************************************************
 * Name:      hsoftplc.c
 * Purpose:   实现hsoftplc接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hsoftplc.h"
#include "stdlib.h"
#include "stdint.h"

/*
 * 导入配置文件
 */
#include "hdefaults/hdefaults_config.h"


/*
 * 引入hdefaults_tick_get
 */
#include "hdefaults/hdefaults_baseapi.h"

/*
 * 引入hsyscall_gettimeofday
 */
#include "hdefaults/syscall/hsyscall/time/hsyscall_time.h"

/*
 * 引入hgettimeofday
 */
#include "hdefaults/syscall/wrapper/hgettimeofday.h"

/*
 * 引入hstrcmp
 */
#include "hdefaults/libc/wrapper/string/hstrcmp.h"

#if defined(HSOFTPLC)
#include "h3rdparty/3rdparty/matiec_c_header/iec_types_all.h"

/*
 * matiec生成的入口
 */
extern void config_init__(void);
extern void config_run__(unsigned long tick);

/*
 * matiec生成的节拍间隔(ns)
 */
extern unsigned long long common_ticktime__;
extern unsigned long greatest_tick_count__;

/*
 * 当前时间
 */
TIME __CURRENT_TIME;

static void update_current_time(uint64_t tv_sec,size_t tv_nsec)
{
    __CURRENT_TIME.tv_sec=tv_sec;
    __CURRENT_TIME.tv_nsec=tv_nsec;
}

/*
 * 软件PLC需要的变量(动态生成),如果不需要动态生成的变量，可关闭。
 * 实践中，可采用__WEAK预定义部分变量，可兼容动态生成的变量
 */
#ifndef HSOFTPLC_NO_DYNAMIC_LOCATED_VARIABLES

#undef __LOCATED_VAR
#define __LOCATED_VAR(type, name, ...) type __##name;
#include "LOCATED_VARIABLES.h"
#undef __LOCATED_VAR
#define __LOCATED_VAR(type, name, ...) type* name = &__##name;
#include "LOCATED_VARIABLES.h"
#undef __LOCATED_VAR

#endif

#endif // HSOFTPLC

static hsoftplc_callback_t hsoftplc_callback=NULL;

void hsoftplc_init(void)
{
#if defined(HSOFTPLC)

    {
        /*
         * 初始化开始
         */
        hsoftplc_callback_t cb=hsoftplc_callback;
        if(cb!=NULL)
        {
            cb(HSOFTPLC_CALLBACK_TYPE_CONFIG_INIT_BEGIN);
        }
    }

    /*
     * 组态初始化
     */
    config_init__();

    {
        /*
         * 初始化结束
         */
        hsoftplc_callback_t cb=hsoftplc_callback;
        if(cb!=NULL)
        {
            cb(HSOFTPLC_CALLBACK_TYPE_CONFIG_INIT_END);
        }
    }
#endif
}


void hsoftplc_loop(void)
{
#if defined(HSOFTPLC)
#if defined(HSOFTPLC_LOOP_CHECK_TIMEOUT)
    if(HSOFTPLC_LOOP_CHECK_TIMEOUT(common_ticktime__))
    {
#else
    static hdefaults_tick_t last_tick=0;
    if(hdefaults_tick_get()-last_tick > common_ticktime__/1000000)
    {
        last_tick=hdefaults_tick_get();
#endif
        {
            /*
             * 更新__CURRENT_TIME
             */
#if   defined(HSOFTPLC_LOOP_CURRENT_TIME)
            uint64_t tv_sec=0;
            uint64_t tv_nsec=0;
            HSOFTPLC_LOOP_CURRENT_TIME(tv_sec,tv_nsec);
            update_current_time(tv_sec,tv_nsec);
#elif !defined(HDEFAULTS_SYSCALL_NO_IMPLEMENTATION) && !defined(HDEFAULTS_SYSCALL_NO_HGETTIMEOFDAY)
            hgettimeofday_timeval_t tv;
            hgettimeofday_timezone_t tz;
            hgettimeofday(&tv,&tz);
            update_current_time(tv.tv_sec,tv.tv_usec*1000);
#else
            hgettimeofday_timeval_t tv={0};
            {
                /*
                 * 使用hsyscall扩展的系统调用
                 */
                hsyscall_gettimeofday(&tv,NULL);
            }
            update_current_time(tv.tv_sec,tv.tv_usec*1000);
#endif
        }

        {
            /*
             * 运行开始
             */
            hsoftplc_callback_t cb=hsoftplc_callback;
            if(cb!=NULL)
            {
                cb(HSOFTPLC_CALLBACK_TYPE_CONFIG_RUN_BEGIN);
            }
        }

        /*
         * 组态运行
         */
        static unsigned long tick=0;
        if(greatest_tick_count__!=0 && greatest_tick_count__ < tick)
        {
            tick=0;
        }
        config_run__(tick++);

        {
            /*
             * 运行结束
             */
            hsoftplc_callback_t cb=hsoftplc_callback;
            if(cb!=NULL)
            {
                cb(HSOFTPLC_CALLBACK_TYPE_CONFIG_RUN_END);
            }
        }

    }
#endif
}

hsoftplc_callback_t hsoftplc_set_callback(hsoftplc_callback_t cb)
{
    hsoftplc_callback_t ret=hsoftplc_callback;
    hsoftplc_callback=cb;
    return ret;
}

static const struct
{
    const char *  name;
    void *        variable;
}
hsoftplc_located_variables[]=
{
#if defined(HSOFTPLC) && !defined(HSOFTPLC_NO_DYNAMIC_LOCATED_VARIABLES)
#undef __LOCATED_VAR
#define __LOCATED_VAR(type, name, ...) {  #name , (void *) &__##name },
#include "LOCATED_VARIABLES.h"
#undef __LOCATED_VAR
#endif
    {NULL,NULL}
};

void *hsoftplc_get_located_variables(const char *variable_name)
{
    for(size_t i=0; i<sizeof(hsoftplc_located_variables)/sizeof(hsoftplc_located_variables[0]); i++)
    {
        if(hsoftplc_located_variables[i].name!=NULL && hstrcmp(hsoftplc_located_variables[i].name,variable_name)==0)
        {
            return hsoftplc_located_variables[i].variable;
        }
    }
    return NULL;
}

size_t hsoftplc_get_located_all_variables(hsoftplc_located_variable_enum_callback_t cb,void *usr)
{
    size_t ret=0;
    for(size_t i=0; i<sizeof(hsoftplc_located_variables)/sizeof(hsoftplc_located_variables[0]); i++)
    {
        if(hsoftplc_located_variables[i].name!=NULL && hsoftplc_located_variables[i].variable!=NULL )
        {
            if(cb!=NULL)
            {
                cb(hsoftplc_located_variables[i].name,hsoftplc_located_variables[i].variable,usr);
            }
            ret++;
        }
    }
    return ret;
}

bool hsoftplc_get_iec_addr_from_variable_name(hsoftplc_variable_name_t iec_addr,const char * variable_name)
{
    bool ret=false;
    if(iec_addr==NULL || variable_name==NULL)
    {
        return ret;
    }

    if(variable_name[0]!='_' || variable_name[1]!='_' )
    {
        return ret;
    }

    iec_addr[0]='%';

    for(size_t i=0; i< sizeof(hsoftplc_variable_name_t)-1; i++)
    {
        iec_addr[1+i]=variable_name[2+i];
        if(iec_addr[1+i]=='_')
        {
            iec_addr[1+i]='.';
        }
        if(iec_addr[1+i]=='\0')
        {
            break;
        }
    }

    iec_addr[sizeof(hsoftplc_variable_name_t)-1]='\0';

    ret=true;
    return ret;
}

bool hsoftplc_get_variable_name_from_iec_addr(hsoftplc_variable_name_t variable_name,const char *iec_addr)
{
    bool ret=false;
    if(iec_addr==NULL || variable_name==NULL)
    {
        return ret;
    }

    if(iec_addr[0]!='%' )
    {
        return ret;
    }

    variable_name[0]='_';
    variable_name[1]='_';

    for(size_t i=0; i< sizeof(hsoftplc_variable_name_t)-1; i++)
    {
        variable_name[2+i]=iec_addr[1+i];
        if(variable_name[2+i]=='.')
        {
            variable_name[2+i]='_';
        }
        if(variable_name[2+i]=='\0')
        {
            break;
        }
    }

    variable_name[sizeof(hsoftplc_variable_name_t)-1]='\0';

    ret=true;
    return ret;
}

hsoftplc_variable_symbol_t * hsoftplc_parse_variable_symbol(hsoftplc_variable_symbol_t * variable_symbol,const char *variable_name_or_iec_addr)
{
    if(variable_symbol == NULL || variable_name_or_iec_addr==NULL)
    {
        return NULL;
    }
    {
        hsoftplc_variable_symbol_t zero_variable_symbol= {0};
        (*variable_symbol)=zero_variable_symbol;
    }
    size_t base_offset=0;
    if(variable_name_or_iec_addr[0]=='%')
    {
        //IEC地址
        base_offset=1;
    }
    else if(variable_name_or_iec_addr[0]=='_' && variable_name_or_iec_addr[1]=='_' )
    {
        //C语言变量名称
        base_offset=2;
    }
    else
    {
        //错误的参数
        return NULL;
    }
    variable_symbol->variable_location=variable_name_or_iec_addr[base_offset];
    if(variable_symbol->variable_location == '\0')
    {
        return NULL;
    }
    variable_symbol->variable_size=variable_name_or_iec_addr[base_offset+1];
    if(variable_symbol->variable_size == '\0')
    {
        variable_symbol->variable_location = '\0';
        return NULL;
    }
    size_t address_offset=base_offset+2;
    if(variable_symbol->variable_size >= '0' && variable_symbol->variable_size <= '9')
    {
        //省略变量大小,默认为1位
        variable_symbol->variable_size='X';
        address_offset=base_offset+1;
    }

    variable_symbol->variable_address[0]=variable_symbol->buffer;
    variable_symbol->buffer[sizeof(variable_symbol->buffer)-1]='\0';
    for(size_t i=0; i<sizeof(variable_symbol->buffer)-1; i++)
    {
        variable_symbol->buffer[i]=variable_name_or_iec_addr[i+address_offset];
        if(variable_symbol->buffer[i] == '\0')
        {
            break;
        }
        if(variable_symbol->buffer[i] == '.' || variable_symbol->buffer[i] == '_')
        {
            variable_symbol->buffer[i]='\0';
            variable_symbol->buffer[i+1]='\0';
            for(size_t j=0; j<sizeof(variable_symbol->variable_address)/sizeof(variable_symbol->variable_address[0]); j++)
            {
                if(variable_symbol->variable_address[j]==NULL)
                {
                    variable_symbol->variable_address[j]=&variable_symbol->buffer[i+1];
                    break;
                }
            }
        }
    }

    return variable_symbol;
}
