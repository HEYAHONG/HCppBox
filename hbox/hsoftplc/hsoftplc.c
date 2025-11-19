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
 * 引入hgettimeofday
 */
#include "hdefaults/syscall/wrapper/hgettimeofday.h"

#if defined(HSOFTPLC)
#include "h3rdparty/3rdparty/matiec_c_header/iec_types_all.h"

/*
 * matiec生成的入口
 */
extern void config_init__(void);
extern void config_run__(unsigned long tick);

/*
 * matiec生成的节拍间隔(us)
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
    static hdefaults_tick_t last_tick=0;
    if(hdefaults_tick_get()-last_tick > common_ticktime__/1000000)
    {
        last_tick=hdefaults_tick_get();

        {
            /*
             * 更新__CURRENT_TIME
             */
            hgettimeofday_timeval_t tv;
            hgettimeofday_timezone_t tz;
            hgettimeofday(&tv,&tz);
            update_current_time(tv.tv_sec,tv.tv_usec*1000);
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
