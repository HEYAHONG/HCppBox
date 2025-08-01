﻿#include <inttypes.h>
#include <stdint.h>
#include <thread>
#include <functional>
#include <string>
#include "hbox.h"
#include "hrc.h"
#include "time.h"
#include H3RDPARTY_ZLIB_HEADER
#include H3RDPARTY_LZ4_HEADER
#include H3RDPARTY_LZ4HC_HEADER
#include H3RDPARTY_LZ4FRAME_HEADER
#include H3RDPARTY_LZ4FILE_HEADER
#include H3RDPARTY_LIBQRENCODE_HEADER
#include H3RDPARTY_ARGTABLE3_HEADER
#include H3RDPARTY_TLSF_HEADER
#include H3RDPARTY_LIBFDT_HEADER

static int hcompiler_test(int argc,const char *argv[]);
static int hdefaults_test(int argc,const char *argv[]);
static int heventloop_test(int argc,const char *argv[]);
static int heventslots_test(int argc,const char *argv[]);
static int heventchain_test(int argc,const char *argv[]);
static int hwatchdog_test(int argc,const char *argv[]);
static int hmemoryheap_test(int argc,const char *argv[]);
static int hobject_test(int argc,const char *argv[]);
static int hringbuf_test(int argc,const char *argv[]);
static int hmemory_test(int argc,const char *argv[]);
static int hlocale_test(int argc,const char *argv[]);
static int hruntime_test(int argc,const char *argv[]);
static int hsimulator_test(int argc,const char *argv[]);
static int h3rdparty_test(int argc,const char *argv[]);
static int huuid_test(int argc,const char *argv[]);
static int hcrypto_test(int argc,const char *argv[]);

static int (*test_cb[])(int,const char *[])=
{
    hcompiler_test,
    hdefaults_test,
    heventloop_test,
    heventslots_test,
    heventchain_test,
    hwatchdog_test,
    hmemoryheap_test,
    hobject_test,
    hringbuf_test,
    hmemory_test,
    hlocale_test,
    hruntime_test,
    hsimulator_test,
    h3rdparty_test,
    huuid_test,
    hcrypto_test
};

int main(int argc,const char *argv[])
{
    //关闭缓冲，便于调试
    setbuf(stdout,NULL);
    for(size_t i=0; i<(sizeof(test_cb)/sizeof(test_cb[0])); i++)
    {
        int ret=0;
        if((ret=test_cb[i](argc,argv))!=0)
        {
            return ret;
        }
    }
    return 0;
}

extern "C" void c_compiler_test();
static int hcompiler_test(int argc,const char *argv[])
{
    c_compiler_test();

    printf("hcompiler: " __DATE__ " " __TIME__ "\r\n");

    printf("hcompiler:C Compiler __SDTC_VERSION__=%d\r\n",(int)hcompiler_get_stdc_version());

    printf("hcompiler:build time %04d/%02d/%02d %02d:%02d:%02d\r\n",hcompiler_get_date_year(),hcompiler_get_date_month(),hcompiler_get_date_day(),hcompiler_get_time_hour(),hcompiler_get_time_minute(),hcompiler_get_time_second());

#ifdef __HAS_CPP11
    printf("hcompiler:C++11\r\n");
#endif // __HAS_CPP11

#ifdef __HAS_CPP14
    printf("hcompiler:C++14\r\n");
#endif // __HAS_CPP14

#ifdef __HAS_CPP17
    printf("hcompiler:C++17\r\n");
#endif // __HAS_CPP17

#ifdef __HAS_CPP20
    printf("hcompiler:C++20\r\n");
#endif // __HAS_CPP20

#ifdef __HAS_CPP23
    printf("hcompiler:C++23\r\n");
#endif // __HAS_CPP23

#ifdef __cplusplus_date
    printf("hcompiler:C++Ver=%d\r\n",(int)__CPP_VERSION);
#endif // __cplusplus_date
    return 0;
}

static int hdefaults_test(int argc,const char *argv[])
{
    {
#ifdef HDEFAULTS_BITS_16_OR_8
        printf("hdefaults:16/8 bits environment!\r\n");
#endif // HDEFAULTS_BITS_16_OR_8
#ifdef HDEFAULTS_BITS_32
        printf("hdefaults:32 bits environment!\r\n");
#endif // HDEFAULTS_BITS_32
#ifdef HDEFAULTS_BITS_64
        printf("hdefaults:64 bits environment!\r\n");
#endif // HDEFAULTS_BITS_64
#ifdef HDEFAULTS_BITS_ABOVE_64
        printf("hdefaults: above 64 bits environment!\r\n");
#endif // HDEFAULTS_BITS_ABOVE_64
    }

    {
        hdefaults_tick_t start=hdefaults_tick_get();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        hdefaults_tick_t end=hdefaults_tick_get();
        printf("hdefaults_test:tick start=%" PRIu64 ",tick end=%" PRIu64 ",tick interval=20ms\r\n",(uint64_t)start,(uint64_t)end);
    }

    hdefaults_mutex_lock(NULL);
    hdefaults_mutex_unlock(NULL);
    void *ptr=hdefaults_malloc(1024,NULL);
    if(ptr==NULL)
    {
        printf("hdefaults_test:1st error\r\n");
        return -1;
    }
    printf("hdefaults_test:1st pointer addr=0x%" PRIX64 "\r\n",(uint64_t)(intptr_t)ptr);
    hdefaults_free(ptr,NULL);
    ptr=hdefaults_malloc(1024,NULL);
    if(ptr==NULL)
    {
        printf("hdefaults_test:2nd error\r\n");
        return -1;
    }
    printf("hdefaults_test:2nd pointer addr=0x%" PRIX64 "\r\n",(uint64_t)(intptr_t)ptr);
    hdefaults_free(ptr,NULL);

    {
        //usercall测试
        {
            hdefaults_tick_t tick=0;
            hdefaults_usercall(HDEFAULTS_USERCALL_NUMBER_TICK,&tick);
            printf("hdefaults usercall tick:number=%d,tick=%d\r\n",(int)HDEFAULTS_USERCALL_NUMBER_TICK,(int)tick);
        }
        {
            void *mem=NULL;
            hdefaults_usercall(HDEFAULTS_USERCALL_NUMBER_MALLOC,&mem,1024,NULL);
            printf("hdefaults usercall malloc:addr=0x%" PRIX64 "\r\n",(uint64_t)(intptr_t)ptr);
            int ret_value=0;
            hdefaults_usercall(HDEFAULTS_USERCALL_NUMBER_FREE,&ret_value,mem,NULL);
        }

        {
            int ret_value=0;
            hdefaults_usercall(HDEFAULTS_USERCALL_NUMBER_GLOCK,&ret_value,NULL);
            hdefaults_usercall(HDEFAULTS_USERCALL_NUMBER_GUNLOCK,&ret_value,NULL);
        }
    }

    {
        hgettimeofday_timeval_t tv= {0};
        hgettimeofday_timezone_t tz= {0};
        if(hgettimeofday(&tv,&tz)==0)
        {
            printf("hdefaults hgettimeofday: tv_sec=%" PRIu64 "\ttv_usec=%" PRIu64 "\ttz_minuteswest=%d\ttz_dsttime=%d\r\n",tv.tv_sec,tv.tv_usec,tz.tz_minuteswest,tz.tz_dsttime);
        }
    }

    {
        uint8_t random_buffer[32]= {0};
        if(hgetrandom(random_buffer,sizeof(random_buffer),0) > 0)
        {
            printf("hdefaults hgetrandom:");
            for(size_t i=0; i<sizeof(random_buffer); i++)
            {
                printf("%02X",(int)random_buffer[i]);
            }
            printf("\r\n");
        }
    }

    {
        //Windows下的操作
#if defined(HDEFAULTS_OS_WINDOWS) || defined(HDEFAULTS_OS_CYGWIN)
        const char *crt_name="msvcrt.dll";
#elif   defined(HDEFAULTS_OS_LINUX) && defined(HDEFAULTS_BITS_64)
        const char *crt_name="/lib/x86_64-linux-gnu/libc.so.6";
#elif   defined(HDEFAULTS_OS_FREEBSD)
        const char *crt_name="/lib/libc.so.7";
#else
        const char *crt_name="dummy";
#endif
        void * handle=hdlopen(crt_name,HRTLD_LAZY);
        if(handle!=NULL)
        {
            printf("hdefaults libdl:open crt(%s) success!\r\n",crt_name);
            printf("hdefaults libdl:printf addr=%p\r\n",hdlsym(handle,"printf"));
            hdlclose(handle);
        }
        else
        {
            printf("hdefaults libdl:open crt(%s) failed!\r\n",crt_name);
        }

    }

    {
        printf("hdefaults libdl:symbol_table printf=%p\r\n",hdlsym(NULL,"printf"));
        printf("hdefaults libdl:symbol_table register!\r\n");
        static const hruntime_symbol_t symbol_table[]=
        {
            {
                "printf",(uintptr_t)&hprintf
            }
        };
        hruntime_symbol_dynamic_table_register(symbol_table,sizeof(symbol_table)/sizeof(symbol_table[0]));
        printf("hdefaults libdl:symbol_table printf=%p\r\n",hdlsym(NULL,"printf"));
        printf("hdefaults libdl:symbol_table unregister!\r\n");
        hruntime_symbol_dynamic_table_unregister(symbol_table,sizeof(symbol_table)/sizeof(symbol_table[0]));
        printf("hdefaults libdl:symbol_table printf=%p\r\n",hdlsym(NULL,"printf"));
    }

    return 0;
}
static int heventloop_test(int argc,const char *argv[])
{
    //创建eventloop
    heventloop_t *loop=heventloop_new(NULL);
    if(loop==NULL)
    {
        printf("heventloop_test:create error\r\n");
        return -1;
    }
    printf("heventloop_test:pool addr=0x%" PRIX64 "\r\n",(uint64_t)(intptr_t)loop);

    //添加两个事件(使用无捕获的lambda函数)
    printf("heventloop_test:add events\r\n");
    heventloop_add_event(loop,NULL,[](void *)
    {
        printf("heventloop_test:event\r\n");
    },NULL);
    heventloop_add_event_ex1(loop,NULL,[](void*,heventloop_t *m_loop)
    {
        printf("heventloop_test:event ex\r\n");
        printf("heventloop_test:pool addr=0x%" PRIX64 "\r\n",(uint64_t)(intptr_t)m_loop);
    },NULL);

    printf("heventloop_test:process events(1)\r\n");
    heventloop_process_event(loop);

    printf("heventloop_test:process events(2)\r\n");
    heventloop_process_event(loop);


    //添加两个事件(使用无捕获的lambda函数)
    printf("heventloop_test:add events\r\n");
    heventloop_add_event_ex1(loop,NULL,[](void*,heventloop_t *m_loop)
    {
        printf("heventloop_test:event ex\r\n");
        printf("heventloop_test:pool addr=0x%" PRIX64 "\r\n",(uint64_t)(intptr_t)m_loop);
    },NULL);
    heventloop_add_event(loop,NULL,[](void *)
    {
        printf("heventloop_test:event\r\n");
    },NULL);

    printf("heventloop_test:process events(3)\r\n");
    heventloop_process_event(loop);

    //释放eventloop(正常使用时一般不用释放)
    heventloop_free(loop);

    {
        //C++测试
        hloop m_loop;

        //添加两个事件(使用无捕获的lambda函数)
        printf("hloop_test:add events\r\n");
        m_loop.add_event([](void *)
        {
            printf("hloop_test:event\r\n");
        },NULL,NULL);
        m_loop.add_event_ex1([](void*,heventloop_t *m_loop)
        {
            printf("hloop_test:event ex\r\n");
            printf("hloop_test:pool addr=0x%" PRIX64 "\r\n",(uint64_t)(intptr_t)m_loop);
        },NULL,NULL);

        printf("hloop_test:process events(1)\r\n");
        m_loop.process_event();

        printf("hloop_test:process events(2)\r\n");
        m_loop.process_event();


        //添加两个事件(使用无捕获的lambda函数)
        printf("hloop_test:add events\r\n");
        m_loop.add_event_ex1([](void*,heventloop_t *m_loop)
        {
            printf("hloop_test:event ex\r\n");
            printf("hloop_test:pool addr=0x%" PRIX64 "\r\n",(uint64_t)(intptr_t)m_loop);
        },NULL,NULL);
        m_loop.add_event([](void *)
        {
            printf("hloop_test:event\r\n");
        },NULL,NULL);

        printf("hloop_test:process events(3)\r\n");
        m_loop.process_event();


    }
    return 0;
}
static int heventslots_test(int argc,const char *argv[])
{
    //创建eventslots
    heventslots_t *slots=heventslots_new(NULL);
    if(slots==NULL)
    {
        printf("heventslots_test:create error\r\n");
        return -1;
    }
    printf("heventslots_test:slots addr=0x%" PRIX64 "\r\n",(uint64_t)(intptr_t)slots);


    //添加槽函数
    printf("heventslots_test:add slots\r\n");
    uint32_t slot1=heventslots_register_slot(slots,NULL,[](void *signal,void *usr)
    {
        printf("heventslots_test:slot1,signal:%s\r\n",signal!=NULL?((const char *)signal):"");
    },NULL);
    uint32_t slot2=heventslots_register_slot(slots,NULL,[](void *signal,void *usr)
    {
        printf("heventslots_test:slot2,signal:%s\r\n",signal!=NULL?((const char *)signal):"");
    },NULL);
    uint32_t slot3=heventslots_register_slot(slots,NULL,[](void *signal,void *usr)
    {
        printf("heventslots_test:slot3,signal:%s\r\n",signal!=NULL?((const char *)signal):"");
    },NULL);
    uint32_t slot4=heventslots_register_slot(slots,NULL,[](void *signal,void *usr)
    {
        printf("heventslots_test:slot4,signal:%s\r\n",signal!=NULL?((const char *)signal):"");
    },NULL);
    uint32_t slot5=heventslots_register_slot(slots,NULL,[](void *signal,void *usr)
    {
        printf("heventslots_test:slot5,signal:%s\r\n",signal!=NULL?((const char *)signal):"");
    },NULL);

    //发送信号
    printf("heventslots_test:emit signal 1\r\n");
    heventslots_emit_signal(slots,(char *)"1");
    printf("heventslots_test:emit signal 2\r\n");
    heventslots_emit_signal(slots,(char *)"2");
    printf("heventslots_test:emit signal 3\r\n");
    heventslots_emit_signal(slots,(char *)"3");

    //删除slot3
    printf("heventslots_test:remove slot3\r\n");
    heventslots_unregister_slot(slots,slot3);

    //发送信号
    printf("heventslots_test:emit signal 1\r\n");
    heventslots_emit_signal(slots,(char *)"1");
    printf("heventslots_test:emit signal 2\r\n");
    heventslots_emit_signal(slots,(char *)"2");
    printf("heventslots_test:emit signal 3\r\n");
    heventslots_emit_signal(slots,(char *)"3");

    //释放evnetslots
    heventslots_free(slots);

    {
        //C++测试
        hslots m_slots;

        //添加槽函数
        printf("hslots_test:add slots\r\n");
        uint32_t slot1=m_slots.register_slot([](void *signal,void *usr)
        {
            printf("hslots_test:slot1,signal:%s\r\n",signal!=NULL?((const char *)signal):"");
        });
        uint32_t slot2=m_slots.register_slot([](void *signal,void *usr)
        {
            printf("hslots_test:slot2,signal:%s\r\n",signal!=NULL?((const char *)signal):"");
        });
        uint32_t slot3=m_slots.register_slot([](void *signal,void *usr)
        {
            printf("hslots_test:slot3,signal:%s\r\n",signal!=NULL?((const char *)signal):"");
        });
        uint32_t slot4=m_slots.register_slot([](void *signal,void *usr)
        {
            printf("hslots_test:slot4,signal:%s\r\n",signal!=NULL?((const char *)signal):"");
        });
        uint32_t slot5=m_slots.register_slot([](void *signal,void *usr)
        {
            printf("hslots_test:slot5,signal:%s\r\n",signal!=NULL?((const char *)signal):"");
        });

        //发送信号
        printf("hslots_test:emit signal 1\r\n");
        m_slots.emit_signal((char *)"1");
        printf("hslots_test:emit signal 2\r\n");
        m_slots.emit_signal((char *)"2");
        printf("hslots_test:emit signal 3\r\n");
        m_slots.emit_signal((char *)"3");

        //删除slot3
        printf("hslots_test:remove slot3\r\n");
        m_slots.unregister_slot(slot3);

        //发送信号
        printf("hslots_test:emit signal 1\r\n");
        m_slots.emit_signal((char *)"1");
        printf("hslots_test:emit signal 2\r\n");
        m_slots.emit_signal((char *)"2");
        printf("hslots_test:emit signal 3\r\n");
        m_slots.emit_signal((char *)"3");

    }


    return 0;
}
static int heventchain_test(int argc,const char *argv[])
{
    //创建eventchain
    heventchain_t *chain=heventchain_new(NULL);
    if(chain==NULL)
    {
        printf("heventchain_test:create error\r\n");
        return -1;
    }
    printf("heventchain_test:chain addr=0x%" PRIX64 "\r\n",(uint64_t)(intptr_t)chain);

    //添加chain
    printf("heventchain_test:add chain\r\n");
    heventchain_install_hook(chain,1,NULL,[](void *para,void *usr)->bool
    {
        printf("heventchain_test:hook 1\r\n");
        if(para!=NULL)
        {
            int paraval=*(int*)para;
            if(paraval==1)
            {
                return true;
            }
        }
        return false;
    },NULL);
    heventchain_install_hook(chain,5,NULL,[](void *para,void *usr)->bool
    {
        printf("heventchain_test:hook 5\r\n");
        if(para!=NULL)
        {
            int paraval=*(int*)para;
            if(paraval==5)
            {
                return true;
            }
        }
        return false;
    },NULL);
    heventchain_install_hook(chain,4,NULL,[](void *para,void *usr)->bool
    {
        printf("heventchain_test:hook 4\r\n");
        if(para!=NULL)
        {
            int paraval=*(int*)para;
            if(paraval==4)
            {
                return true;
            }
        }
        return false;
    },NULL);
    heventchain_install_hook(chain,2,NULL,[](void *para,void *usr)->bool
    {
        printf("heventchain_test:hook 2\r\n");
        if(para!=NULL)
        {
            int paraval=*(int*)para;
            if(paraval==2)
            {
                return true;
            }
        }
        return false;
    },NULL);
    heventchain_install_hook(chain,3,NULL,[](void *para,void *usr)->bool
    {
        printf("heventchain_test:hook 3\r\n");
        if(para!=NULL)
        {
            int paraval=*(int*)para;
            if(paraval==3)
            {
                return true;
            }
        }
        return false;
    },NULL);

    //启动事件链
    {
        printf("heventchain_test:start chain 0\r\n");
        int val=0;
        heventchain_start(chain,&val);
    }
    {
        printf("heventchain_test:start chain 1\r\n");
        int val=1;
        heventchain_start(chain,&val);
    }
    {
        printf("heventchain_test:start chain 2\r\n");
        int val=2;
        heventchain_start(chain,&val);
    }
    {
        printf("heventchain_test:start chain 3\r\n");
        int val=3;
        heventchain_start(chain,&val);
    }
    {
        printf("heventchain_test:start chain 4\r\n");
        int val=4;
        heventchain_start(chain,&val);
    }
    {
        printf("heventchain_test:start chain 5\r\n");
        int val=5;
        heventchain_start(chain,&val);
    }



    //释放eventchain
    heventchain_free(chain);

    {
        printf("hchain_test:add chain\r\n");
        //C++测试
        hchain m_chain;
        m_chain.install_hook([](void *para,void *usr)->bool
        {
            printf("hchain_test:hook 1\r\n");
            if(para!=NULL)
            {
                int paraval=*(int*)para;
                if(paraval==1)
                {
                    return true;
                }
            }
            return false;
        },NULL,1);
        m_chain.install_hook([](void *para,void *usr)->bool
        {
            printf("hchain_test:hook 5\r\n");
            if(para!=NULL)
            {
                int paraval=*(int*)para;
                if(paraval==5)
                {
                    return true;
                }
            }
            return false;
        },NULL,5);
        m_chain.install_hook([](void *para,void *usr)->bool
        {
            printf("hchain_test:hook 4\r\n");
            if(para!=NULL)
            {
                int paraval=*(int*)para;
                if(paraval==4)
                {
                    return true;
                }
            }
            return false;
        },NULL,4);
        m_chain.install_hook([](void *para,void *usr)->bool
        {
            printf("hchain_test:hook 2\r\n");
            if(para!=NULL)
            {
                int paraval=*(int*)para;
                if(paraval==2)
                {
                    return true;
                }
            }
            return false;
        },NULL,2);
        m_chain.install_hook([](void *para,void *usr)->bool
        {
            printf("hchain_test:hook 3\r\n");
            if(para!=NULL)
            {
                int paraval=*(int*)para;
                if(paraval==3)
                {
                    return true;
                }
            }
            return false;
        },NULL,3);

        //启动事件链
        {
            printf("hchain_test:start chain 0\r\n");
            int val=0;
            m_chain.start(&val);
        }
        {
            printf("hchain_test:start chain 1\r\n");
            int val=1;
            m_chain.start(&val);
        }
        {
            printf("hchain_test:start chain 2\r\n");
            int val=2;
            m_chain.start(&val);
        }
        {
            printf("hevent_test:start chain 3\r\n");
            int val=3;
            m_chain.start(&val);
        }
        {
            printf("hchain_test:start chain 4\r\n");
            int val=4;
            m_chain.start(&val);
        }
        {
            printf("hchain_test:start chain 5\r\n");
            int val=5;
            m_chain.start(&val);
        }
    }

    return 0;
}
static int hwatchdog_test(int argc,const char *argv[])
{
    return 0;
}
uint8_t memory_heap[1024]= {0};
static int hmemoryheap_test(int argc,const char *argv[])
{
    printf("hmemoryheap_test:memory addr=0x%" PRIX64 "\r\n",(uint64_t)(intptr_t)memory_heap);
    hmemoryheap_pool_t *pool=hmemoryheap_pool_format_with_default_lock(NULL,memory_heap,sizeof(memory_heap));
    if(pool==NULL)
    {
        printf("hmemoryheap_test:error");
        return -1;
    }
    {
        size_t total_size=0;
        size_t free_size=0;
        hmemoryheap_pool_get_info(pool,&total_size,&free_size);
        printf("hmemoryheap_test:total=%" PRIu64 ",free=%" PRIu64 "\r\n",(uint64_t)total_size,(uint64_t)free_size);
    }

    //单个指针分配
    printf("hmemoryheap_test::test1\r\n");
    {
        void *ptr=hmemoryheap_pool_malloc(pool,1);
        if(ptr==NULL)
        {
            printf("hmemoryheap_test::error\r\n");
            return -1;
        }
        printf("hmemoryheap_test:1st pointer addr=0x%" PRIX64 "\r\n",(uint64_t)(intptr_t)ptr);
        {
            size_t total_size=0;
            size_t free_size=0;
            hmemoryheap_pool_get_info(pool,&total_size,&free_size);
            printf("hmemoryheap_test:total=%" PRIu64 ",free=%" PRIu64 "\r\n",(uint64_t)total_size,(uint64_t)free_size);
        }
        hmemoryheap_pool_free(pool,ptr);
        {
            size_t total_size=0;
            size_t free_size=0;
            hmemoryheap_pool_get_info(pool,&total_size,&free_size);
            printf("hmemoryheap_test:after free total=%" PRIu64 ",free=%" PRIu64 "\r\n",(uint64_t)total_size,(uint64_t)free_size);
        }
        ptr=hmemoryheap_pool_malloc(pool,1);
        if(ptr==NULL)
        {
            printf("hmemoryheap_test::error\r\n");
            return -1;
        }
        printf("hmemoryheap_test:2nd pointer addr=0x%" PRIX64 "\r\n",(uint64_t)(intptr_t)ptr);
        {
            size_t total_size=0;
            size_t free_size=0;
            hmemoryheap_pool_get_info(pool,&total_size,&free_size);
            printf("hmemoryheap_test:total=%" PRIu64 ",free=%" PRIu64 "\r\n",(uint64_t)total_size,(uint64_t)free_size);
        }
        hmemoryheap_pool_free(pool,ptr);
        {
            size_t total_size=0;
            size_t free_size=0;
            hmemoryheap_pool_get_info(pool,&total_size,&free_size);
            printf("hmemoryheap_test:after free total=%" PRIu64 ",free=%" PRIu64 "\r\n",(uint64_t)total_size,(uint64_t)free_size);
        }
    }

    printf("hmemoryheap_test::test2\r\n");
    {
        void *ptr1=hmemoryheap_pool_malloc(pool,1);
        if(ptr1==NULL)
        {
            printf("hmemoryheap_test::error\r\n");
            return -1;
        }
        printf("hmemoryheap_test:ptr1 pointer addr=0x%" PRIX64 "\r\n",(uint64_t)(intptr_t)ptr1);
        {
            size_t total_size=0;
            size_t free_size=0;
            hmemoryheap_pool_get_info(pool,&total_size,&free_size);
            printf("hmemoryheap_test:total=%" PRIu64 ",free=%" PRIu64 "\r\n",(uint64_t)total_size,(uint64_t)free_size);
        }

        void *ptr2=hmemoryheap_pool_malloc(pool,2);
        if(ptr1==NULL)
        {
            printf("hmemoryheap_test::error\r\n");
            return -1;
        }
        printf("hmemoryheap_test:ptr2 pointer addr=0x%" PRIX64 "\r\n",(uint64_t)(intptr_t)ptr2);
        {
            size_t total_size=0;
            size_t free_size=0;
            hmemoryheap_pool_get_info(pool,&total_size,&free_size);
            printf("hmemoryheap_test:total=%" PRIu64 ",free=%" PRIu64 "\r\n",(uint64_t)total_size,(uint64_t)free_size);
        }

        void *ptr3=hmemoryheap_pool_malloc(pool,3);
        if(ptr3==NULL)
        {
            printf("hmemoryheap_test::error\r\n");
            return -1;
        }
        printf("hmemoryheap_test:ptr3 pointer addr=0x%" PRIX64 "\r\n",(uint64_t)(intptr_t)ptr3);
        {
            size_t total_size=0;
            size_t free_size=0;
            hmemoryheap_pool_get_info(pool,&total_size,&free_size);
            printf("hmemoryheap_test:total=%" PRIu64 ",free=%" PRIu64 "\r\n",(uint64_t)total_size,(uint64_t)free_size);
        }

        void *ptr4=hmemoryheap_pool_malloc(pool,4);
        if(ptr4==NULL)
        {
            printf("hmemoryheap_test::error\r\n");
            return -1;
        }
        printf("hmemoryheap_test:ptr4 pointer addr=0x%" PRIX64 "\r\n",(uint64_t)(intptr_t)ptr4);
        {
            size_t total_size=0;
            size_t free_size=0;
            hmemoryheap_pool_get_info(pool,&total_size,&free_size);
            printf("hmemoryheap_test:total=%" PRIu64 ",free=%" PRIu64 "\r\n",(uint64_t)total_size,(uint64_t)free_size);
        }

        void *ptr5=hmemoryheap_pool_malloc(pool,5);
        if(ptr1==NULL)
        {
            printf("hmemoryheap_test::error\r\n");
            return -1;
        }
        printf("hmemoryheap_test:ptr5 pointer addr=0x%" PRIX64 "\r\n",(uint64_t)(intptr_t)ptr5);
        {
            size_t total_size=0;
            size_t free_size=0;
            hmemoryheap_pool_get_info(pool,&total_size,&free_size);
            printf("hmemoryheap_test:total=%" PRIu64 ",free=%" PRIu64 "\r\n",(uint64_t)total_size,(uint64_t)free_size);
        }

        void *ptr6=hmemoryheap_pool_malloc(pool,6);
        if(ptr1==NULL)
        {
            printf("hmemoryheap_test::error\r\n");
            return -1;
        }
        printf("hmemoryheap_test:ptr6 pointer addr=0x%" PRIX64 "\r\n",(uint64_t)(intptr_t)ptr6);
        {
            size_t total_size=0;
            size_t free_size=0;
            hmemoryheap_pool_get_info(pool,&total_size,&free_size);
            printf("hmemoryheap_test:total=%" PRIu64 ",free=%" PRIu64 "\r\n",(uint64_t)total_size,(uint64_t)free_size);
        }

        void *ptr7=hmemoryheap_pool_malloc(pool,7);
        if(ptr7==NULL)
        {
            printf("hmemoryheap_test::error\r\n");
            return -1;
        }
        printf("hmemoryheap_test:ptr7 pointer addr=0x%" PRIX64 "\r\n",(uint64_t)(intptr_t)ptr7);
        {
            size_t total_size=0;
            size_t free_size=0;
            hmemoryheap_pool_get_info(pool,&total_size,&free_size);
            printf("hmemoryheap_test:total=%" PRIu64 ",free=%" PRIu64 "\r\n",(uint64_t)total_size,(uint64_t)free_size);
        }

        void *ptr8=hmemoryheap_pool_malloc(pool,8);
        if(ptr8==NULL)
        {
            printf("hmemoryheap_test::error\r\n");
            return -1;
        }
        printf("hmemoryheap_test:ptr8 pointer addr=0x%" PRIX64 "\r\n",(uint64_t)(intptr_t)ptr8);
        {
            size_t total_size=0;
            size_t free_size=0;
            hmemoryheap_pool_get_info(pool,&total_size,&free_size);
            printf("hmemoryheap_test:total=%" PRIu64 ",free=%" PRIu64 "\r\n",(uint64_t)total_size,(uint64_t)free_size);
        }




        hmemoryheap_pool_free(pool,ptr1);
        {
            size_t total_size=0;
            size_t free_size=0;
            hmemoryheap_pool_get_info(pool,&total_size,&free_size);
            printf("hmemoryheap_test:after free ptr1 total=%" PRIu64 ",free=%" PRIu64 "\r\n",(uint64_t)total_size,(uint64_t)free_size);
        }

        hmemoryheap_pool_free(pool,ptr2);
        {
            size_t total_size=0;
            size_t free_size=0;
            hmemoryheap_pool_get_info(pool,&total_size,&free_size);
            printf("hmemoryheap_test:after free ptr2 total=%" PRIu64 ",free=%" PRIu64 "\r\n",(uint64_t)total_size,(uint64_t)free_size);
        }

        hmemoryheap_pool_free(pool,ptr3);
        {
            size_t total_size=0;
            size_t free_size=0;
            hmemoryheap_pool_get_info(pool,&total_size,&free_size);
            printf("hmemoryheap_test:after free ptr3 total=%" PRIu64 ",free=%" PRIu64 "\r\n",(uint64_t)total_size,(uint64_t)free_size);
        }

        hmemoryheap_pool_free(pool,ptr4);
        {
            size_t total_size=0;
            size_t free_size=0;
            hmemoryheap_pool_get_info(pool,&total_size,&free_size);
            printf("hmemoryheap_test:after free ptr4 total=%" PRIu64 ",free=%" PRIu64 "\r\n",(uint64_t)total_size,(uint64_t)free_size);
        }

        hmemoryheap_pool_free(pool,ptr5);
        {
            size_t total_size=0;
            size_t free_size=0;
            hmemoryheap_pool_get_info(pool,&total_size,&free_size);
            printf("hmemoryheap_test:after free ptr5 total=%" PRIu64 ",free=%" PRIu64 "\r\n",(uint64_t)total_size,(uint64_t)free_size);
        }

        hmemoryheap_pool_free(pool,ptr6);
        {
            size_t total_size=0;
            size_t free_size=0;
            hmemoryheap_pool_get_info(pool,&total_size,&free_size);
            printf("hmemoryheap_test:after free ptr6 total=%" PRIu64 ",free=%" PRIu64 "\r\n",(uint64_t)total_size,(uint64_t)free_size);
        }

        hmemoryheap_pool_free(pool,ptr7);
        {
            size_t total_size=0;
            size_t free_size=0;
            hmemoryheap_pool_get_info(pool,&total_size,&free_size);
            printf("hmemoryheap_test:after free ptr7 total=%" PRIu64 ",free=%" PRIu64 "\r\n",(uint64_t)total_size,(uint64_t)free_size);
        }

        hmemoryheap_pool_free(pool,ptr8);
        {
            size_t total_size=0;
            size_t free_size=0;
            hmemoryheap_pool_get_info(pool,&total_size,&free_size);
            printf("hmemoryheap_test:after free ptr8 total=%" PRIu64 ",free=%" PRIu64 "\r\n",(uint64_t)total_size,(uint64_t)free_size);
        }

    }


    return 0;
}
static int hobject_test(int argc,const char *argv[])
{
    printf("hobject_test:start\r\n");
    {

        {
            //对象初始化(格式1)
            hobject_simple_ptr_t  obj=HOBJECT_SIMPLE_PTR_INITIALIZER;
        }
        {
            //对象初始化(格式2,C++,指针)
            hobject_simple_ptr_t *obj=new hobject_simple_ptr_t(HOBJECT_SIMPLE_PTR_INITIALIZER);

            //释放内存
            hobject_cleanup(hobject_simple_ptr_base(obj));
            free(obj);
        }
        {
            //对象初始化(格式3,C,指针)
            hobject_simple_ptr_t *obj=(hobject_simple_ptr_t*)malloc(sizeof(hobject_simple_ptr_t));
            (*obj)=HOBJECT_SIMPLE_PTR_INITIALIZER;

            //释放内存
            hobject_cleanup(hobject_simple_ptr_base(obj));
            free(obj);
        }
    }
    {

        {
            //对象初始化(格式1)
            hobject_array_ptr_t  obj=HOBJECT_ARRAY_PTR_INITIALIZER;
        }
        {
            //对象初始化(格式2,C++,指针)
            hobject_array_ptr_t *obj=new hobject_array_ptr_t(HOBJECT_ARRAY_PTR_INITIALIZER);

            //释放内存
            hobject_cleanup(hobject_array_ptr_base(obj));
            free(obj);
        }
        {
            //对象初始化(格式3,C,指针)
            hobject_array_ptr_t *obj=(hobject_array_ptr_t*)malloc(sizeof(hobject_array_ptr_t));
            (*obj)=HOBJECT_ARRAY_PTR_INITIALIZER;

            //释放内存
            hobject_cleanup(hobject_array_ptr_base(obj));
            free(obj);
        }
    }
    {

        {
            //对象初始化(格式1)
            hobject_managed_ptr_t  obj=HOBJECT_MANAGED_PTR_INITIALIZER;
        }
        {
            //对象初始化(格式2,C++,指针)
            hobject_managed_ptr_t *obj=new hobject_managed_ptr_t(HOBJECT_MANAGED_PTR_INITIALIZER);

            //释放内存
            hobject_cleanup(hobject_managed_ptr_base(obj));
            free(obj);
        }
        {
            //对象初始化(格式3,C,指针)
            hobject_managed_ptr_t *obj=(hobject_managed_ptr_t*)malloc(sizeof(hobject_managed_ptr_t));
            (*obj)=HOBJECT_MANAGED_PTR_INITIALIZER;

            //释放内存
            hobject_cleanup(hobject_managed_ptr_base(obj));
            free(obj);
        }
    }
    {

        {
            //对象初始化(格式1)
            hobject_managed_array_ptr_t  obj=HOBJECT_MANAGED_ARRAY_PTR_INITIALIZER;
        }
        {
            //对象初始化(格式2,C++,指针)
            hobject_managed_array_ptr_t *obj=new hobject_managed_array_ptr_t(HOBJECT_MANAGED_ARRAY_PTR_INITIALIZER);

            //释放内存
            hobject_cleanup(hobject_managed_array_ptr_base(obj));
            free(obj);
        }
        {
            //对象初始化(格式3,C,指针)
            hobject_managed_array_ptr_t *obj=(hobject_managed_array_ptr_t*)malloc(sizeof(hobject_managed_array_ptr_t));
            (*obj)=HOBJECT_MANAGED_ARRAY_PTR_INITIALIZER;

            //释放内存
            hobject_cleanup(hobject_managed_array_ptr_base(obj));
            free(obj);
        }
    }
    {

        {
            //对象初始化(格式1)
            hobject_double_t  obj=HOBJECT_DOUBLE_INITIALIZER;
        }
        {
            //对象初始化(格式2,C++,指针)
            hobject_double_t *obj=new hobject_double_t(HOBJECT_DOUBLE_INITIALIZER);

            //释放内存
            hobject_cleanup(hobject_double_base(obj));
            free(obj);
        }
        {
            //对象初始化(格式3,C,指针)
            hobject_double_t *obj=(hobject_double_t*)malloc(sizeof(hobject_double_t));
            (*obj)=HOBJECT_DOUBLE_INITIALIZER;

            //释放内存
            hobject_cleanup(hobject_double_base(obj));
            free(obj);
        }
    }
    {

        {
            //对象初始化(格式1)
            hobject_int8_t  obj=HOBJECT_INT8_INITIALIZER;
        }
        {
            //对象初始化(格式2,C++,指针)
            hobject_int8_t *obj=new hobject_int8_t(HOBJECT_INT8_INITIALIZER);

            //释放内存
            hobject_cleanup(hobject_int8_base(obj));
            free(obj);
        }
        {
            //对象初始化(格式3,C,指针)
            hobject_int8_t *obj=(hobject_int8_t*)malloc(sizeof(hobject_int8_t));
            (*obj)=HOBJECT_INT8_INITIALIZER;

            //释放内存
            hobject_cleanup(hobject_int8_base(obj));
            free(obj);
        }
    }
    {

        {
            //对象初始化(格式1)
            hobject_uint8_t  obj=HOBJECT_UINT8_INITIALIZER;
        }
        {
            //对象初始化(格式2,C++,指针)
            hobject_uint8_t *obj=new hobject_uint8_t(HOBJECT_UINT8_INITIALIZER);

            //释放内存
            hobject_cleanup(hobject_uint8_base(obj));
            free(obj);
        }
        {
            //对象初始化(格式3,C,指针)
            hobject_uint8_t *obj=(hobject_uint8_t*)malloc(sizeof(hobject_uint8_t));
            (*obj)=HOBJECT_UINT8_INITIALIZER;

            //释放内存
            hobject_cleanup(hobject_uint8_base(obj));
            free(obj);
        }
    }
    {

        {
            //对象初始化(格式1)
            hobject_int32_t  obj=HOBJECT_INT32_INITIALIZER;
        }
        {
            //对象初始化(格式2,C++,指针)
            hobject_int32_t *obj=new hobject_int32_t(HOBJECT_INT8_INITIALIZER);

            //释放内存
            hobject_cleanup(hobject_int32_base(obj));
            free(obj);
        }
        {
            //对象初始化(格式3,C,指针)
            hobject_int32_t *obj=(hobject_int32_t*)malloc(sizeof(hobject_int32_t));
            (*obj)=HOBJECT_INT32_INITIALIZER;

            //释放内存
            hobject_cleanup(hobject_int32_base(obj));
            free(obj);
        }
    }
    {

        {
            //对象初始化(格式1)
            hobject_uint32_t  obj=HOBJECT_UINT32_INITIALIZER;
        }
        {
            //对象初始化(格式2,C++,指针)
            hobject_uint32_t *obj=new hobject_uint32_t(HOBJECT_UINT32_INITIALIZER);

            //释放内存
            hobject_cleanup(hobject_uint32_base(obj));
            free(obj);
        }
        {
            //对象初始化(格式3,C,指针)
            hobject_uint32_t *obj=(hobject_uint32_t*)malloc(sizeof(hobject_uint32_t));
            (*obj)=HOBJECT_UINT32_INITIALIZER;

            //释放内存
            hobject_cleanup(hobject_uint32_base(obj));
            free(obj);
        }
    }
    {

        {
            //对象初始化(格式1)
            hobject_int64_t  obj=HOBJECT_INT64_INITIALIZER;
        }
        {
            //对象初始化(格式2,C++,指针)
            hobject_int64_t *obj=new hobject_int64_t(HOBJECT_INT8_INITIALIZER);

            //释放内存
            hobject_cleanup(hobject_int64_base(obj));
            free(obj);
        }
        {
            //对象初始化(格式3,C,指针)
            hobject_int64_t *obj=(hobject_int64_t*)malloc(sizeof(hobject_int64_t));
            (*obj)=HOBJECT_INT64_INITIALIZER;

            //释放内存
            hobject_cleanup(hobject_int64_base(obj));
            free(obj);
        }
    }
    {

        {
            //对象初始化(格式1)
            hobject_uint64_t  obj=HOBJECT_UINT64_INITIALIZER;
        }
        {
            //对象初始化(格式2,C++,指针)
            hobject_uint64_t *obj=new hobject_uint64_t(HOBJECT_UINT64_INITIALIZER);

            //释放内存
            hobject_cleanup(hobject_uint64_base(obj));
            free(obj);
        }
        {
            //对象初始化(格式3,C,指针)
            hobject_uint64_t *obj=(hobject_uint64_t*)malloc(sizeof(hobject_uint64_t));
            (*obj)=HOBJECT_UINT64_INITIALIZER;

            //释放内存
            hobject_cleanup(hobject_uint64_base(obj));
            free(obj);
        }
    }
    {
        {
            //对象初始化(格式1,C,指针),末尾32字节用于存储字符串
            hobject_struct_t *obj=(hobject_struct_t*)malloc(sizeof(hobject_struct_t)+32);
#ifndef _MSC_VER
            (*obj)=HOBJECT_STRUCT_INITIALIZER;
#else
            memset(obj,0,sizeof(*obj));//MSVC不支持长度为0的初始化,使用memset替代
#endif

            //存储字符串
            memcpy((void *)obj->o_struct,"struct test",strlen("struct test"));

            //释放内存
            hobject_cleanup(hobject_struct_base(obj));
            free(obj);
        }
    }
    {
        {
            //对象初始化(格式1,C,指针),末尾32字节用于存储字符串
            hobject_managed_struct_t *obj=(hobject_managed_struct_t*)malloc(sizeof(hobject_managed_struct_t)+32);
#ifndef  _MSC_VER
            (*obj)=HOBJECT_MANAGED_STRUCT_INITIALIZER;
#else
            memset(obj,0,sizeof(*obj));//MSVC不支持长度为0的初始化,使用memset替代
#endif

            //存储字符串
            memcpy((void *)obj->o_struct,"struct test",strlen("struct test"));

            //释放内存
            hobject_cleanup(hobject_managed_struct_base(obj));
            free(obj);
        }
    }
    {
        hobject_int16_t obj=HOBJECT_INT16_INITIALIZER;
        printf("hobject_test: obj addr=%08X,data addr=%08X,offset=%08X,get_obj_by_data=%08X\r\n",(uint32_t)(uintptr_t)&obj,(uint32_t)(uintptr_t)&obj.data,(uint32_t)GET_STRUCT_MEMBER_OFFSET(hobject_int16_t,data),(uint32_t)(uintptr_t)GET_STRUCT_PTR_BY_MEMBER_PTR(&obj.data,hobject_int16_t,data));
    }

    {
        {
            hstaticobjectext obj(5);
            printf("hobject_test: obj=%08X,hobject_base=%08X,this=%08X\r\n",(int)(intptr_t)&obj,(int)(intptr_t)(hobject_base_t *)obj,(int)(intptr_t)obj.get_this(obj));
            printf("hobject_test: data=%d\r\n",*(int *)obj);
        }
        {
            int i=5;
            hstaticobjectext obj(i);
            printf("hobject_test: obj=%08X,hobject_base=%08X,this=%08X\r\n",(int)(intptr_t)&obj,(int)(intptr_t)(hobject_base_t *)obj,(int)(intptr_t)obj.get_this(obj));
            printf("hobject_test: data=%d\r\n",*(int *)obj);
        }
        {
            hstaticobject obj(5);
            printf("hobject_test: obj=%08X,hobject_base=%08X,this=%08X\r\n",(int)(intptr_t)&obj,(int)(intptr_t)(hobject_base_t *)obj,(int)(intptr_t)obj.get_this(obj));
        }
        {
            int i=5;
            hstaticobject obj(i);
            printf("hobject_test: obj=%08X,hobject_base=%08X,this=%08X\r\n",(int)(intptr_t)&obj,(int)(intptr_t)(hobject_base_t *)obj,(int)(intptr_t)obj.get_this(obj));
        }
    }
    {
        {
            hdymaicobj obj(5);
        }
        {
            typedef struct
            {
                int int_val;
                double double_val;
            } data_t;
            data_t data = { 5,6 };
            hdymaicobj<data_t> obj(data);
            obj->int_val=10;
            obj->double_val=12;
        }
    }
    printf("hobject_test:end\r\n");
    return 0;
}

static int hringbuf_test(int argc,const char *argv[])
{
    uint8_t buffer[256]= {0};
    hringbuf_t *ring=hringbuf_get(buffer,sizeof(buffer));
    {
        uint8_t buff[256]= {0};
        printf("hringbuf_test:test 1\r\n");
        hringbuf_input(ring,(uint8_t *)"Hello",strlen("Hello"));
        hringbuf_output_no_clear(ring,buff,sizeof(buff));
        printf("hringbuf_test(no_clear):%s\r\n",buff);
        hringbuf_output(ring,buff,sizeof(buff));
        printf("hringbuf_test:%s\r\n",buff);
        hringbuf_input(ring,(uint8_t *)"ringbuf",strlen("ringbuf"));
        hringbuf_output_no_clear(ring,buff,sizeof(buff));
        printf("hringbuf_test(no_clear):%s\r\n",buff);
        hringbuf_output(ring,buff,sizeof(buff));
        printf("hringbuf_test:%s\r\n",buff);
    }
    {
        uint8_t buff[256]= {0};
        printf("hringbuf_test:test 2\r\n");
        hringbuf_input(ring,(uint8_t *)"Hello",strlen("Hello"));
        hringbuf_input(ring,(uint8_t *)" ",strlen(" "));
        hringbuf_input(ring,(uint8_t *)"ringbuf",strlen("ringbuf"));
        hringbuf_output(ring,buff,sizeof(buff));
        printf("hringbuf_test:%s\r\n",buff);
    }

    {
        //C++测试
        hfifo m_fifo;
        m_fifo.set_buffer(buffer,sizeof(buffer));
        {
            uint8_t buff[256]= {0};
            printf("hfifo_test:test 1\r\n");
            m_fifo.write((uint8_t *)"Hello",strlen("Hello"));
            m_fifo.read(buff,sizeof(buff),false);
            printf("hfifo_test(no_clear):%s\r\n",buff);
            m_fifo.read(buff,sizeof(buff));
            printf("hfifo_test:%s\r\n",buff);
            m_fifo.write((uint8_t *)"ringbuf",strlen("ringbuf"));
            m_fifo.read(buff,sizeof(buff),false);
            printf("hfifo_test(no_clear):%s\r\n",buff);
            m_fifo.read(buff,sizeof(buff));
            printf("hfifo_test:%s\r\n",buff);
        }
        {
            uint8_t buff[256]= {0};
            printf("hfifo_test:test 2\r\n");
            m_fifo.write((uint8_t *)"Hello",strlen("Hello"));
            m_fifo.write((uint8_t *)" ",strlen(" "));
            m_fifo.write((uint8_t *)"ringbuf",strlen("ringbuf"));
            m_fifo.read(buff,sizeof(buff));
            printf("hfifo_test:%s\r\n",buff);
        }


    }
    return 0;
}

static int hmemory_test(int argc,const char *argv[])
{
    {
        uint8_t buffer[256]= {0};
        hsoftwarevirtualmemory_ringbuf_t ring=hsoftwarevirtualmemory_ringbuf_get(NULL,(uintptr_t)buffer,sizeof(buffer));
        {
            uint8_t buff[256]= {0};
            printf("hsoftwarevirtualmemory_ringbuf_test:test 1\r\n");
            hsoftwarevirtualmemory_ringbuf_input(ring,(uint8_t *)"Hello",strlen("Hello"));
            hsoftwarevirtualmemory_ringbuf_output_no_clear(ring,buff,sizeof(buff));
            printf("hsoftwarevirtualmemory_ringbuf_test(no_clear):%s\r\n",buff);
            hsoftwarevirtualmemory_ringbuf_output(ring,buff,sizeof(buff));
            printf("hsoftwarevirtualmemory_ringbuf_test:%s\r\n",buff);
            hsoftwarevirtualmemory_ringbuf_input(ring,(uint8_t *)"ringbuf",strlen("ringbuf"));
            hsoftwarevirtualmemory_ringbuf_output_no_clear(ring,buff,sizeof(buff));
            printf("hsoftwarevirtualmemory_ringbuf_test(no_clear):%s\r\n",buff);
            hsoftwarevirtualmemory_ringbuf_output(ring,buff,sizeof(buff));
            printf("hsoftwarevirtualmemory_ringbuf_test:%s\r\n",buff);
        }
        {
            uint8_t buff[256]= {0};
            printf("hsoftwarevirtualmemory_ringbuf_test:test 2\r\n");
            hsoftwarevirtualmemory_ringbuf_input(ring,(uint8_t *)"Hello",strlen("Hello"));
            hsoftwarevirtualmemory_ringbuf_input(ring,(uint8_t *)" ",strlen(" "));
            hsoftwarevirtualmemory_ringbuf_input(ring,(uint8_t *)"ringbuf",strlen("ringbuf"));
            hsoftwarevirtualmemory_ringbuf_output(ring,buff,sizeof(buff));
            printf("hsoftwarevirtualmemory_ringbuf_test:%s\r\n",buff);
        }
    }

    {
        printf("hmemorystaticallocator: cache alloc len=%d,bitmap=%d\r\n",(int)hmemorystaticallocator_get_table_alloc_size(hmemorystaticallocator_cache_table),(int)hmemorystaticallocator_get_table_bitmap_size(hmemorystaticallocator_cache_table));
    }
    return 0;
}


static int hlocale_test(int argc,const char *argv[])
{
    {
        printf("hlocale:%s\r\n",hlocale_locale_get());
        printf("hlocale:UTF-8 = %s\r\n",hlocale_charset_is_utf8()?"true":"false");
        printf("hlocale:GB2312 = %s\r\n",hlocale_charset_is_gb2312()?"true":"false");
    }
    {
        const char *const ascii_test_string="testtest";
        const char *const utf8_test_string="test测试";
        const wchar_t *const wchar_test_string=L"test测试";
        {
            printf("hunicode_test:%s %s ascii string,len=%d\r\n",ascii_test_string,hunicode_cchar_string_is_ascii(ascii_test_string)?"is":"is not",(int)hunicode_cchar_string_length(ascii_test_string));
            printf("hunicode_test:%s %s ascii string\r\n",utf8_test_string,hunicode_cchar_string_is_ascii(utf8_test_string)?"is":"is not");
            printf("hunicode_test:%s %s utf8 string,len=%d\r\n",ascii_test_string,hunicode_cchar_string_is_utf8(ascii_test_string)?"is":"is not",(int)hunicode_cchar_utf8_string_length(ascii_test_string));
            printf("hunicode_test:%s %s utf8 string,len=%d\r\n",utf8_test_string,hunicode_cchar_string_is_utf8(utf8_test_string)?"is":"is not",(int)hunicode_cchar_utf8_string_length(utf8_test_string));
            printf("hunicode_test:wchar string len=%d\r\n",(int)hunicode_wchar_t_string_length(wchar_test_string));
        }
        {
            //测试wchar_t转换
            hunicode_char_t unicode_string[32]= {0};
            hunicode_char_from_wchar_string(unicode_string,sizeof(unicode_string)/sizeof(unicode_string[0]),wchar_test_string);
            wchar_t wchar_string[32]= {0};
            hunicode_char_string_to_wchar(wchar_string,sizeof(wchar_string)/sizeof(wchar_string[0]),unicode_string);
            printf("hunicode_test:wchar_t convert %s\r\n",wcscmp(wchar_string,wchar_test_string)==0?"ok":"failed");
        }
        {
            //测试UTF-8转换
            hunicode_char_t unicode_string[32]= {0};
            hunicode_char_from_utf8_string(unicode_string,sizeof(unicode_string)/sizeof(unicode_string[0]),utf8_test_string);
            char utf8_string[32]= {0};
            hunicode_char_string_to_utf8(utf8_string,sizeof(utf8_string)/sizeof(utf8_string[0]),unicode_string);
            printf("hunicode_test:utf8 convert %s\r\n",strcmp(utf8_string,utf8_test_string)==0?"ok":"failed");
        }

        {
            //测试GB2312转换
            hunicode_char_t unicode_string[32]= {0};
            hunicode_char_from_utf8_string(unicode_string,sizeof(unicode_string)/sizeof(unicode_string[0]),utf8_test_string);
            char gb2312_string[sizeof(unicode_string)*2]= {0};
            hgb2312_string_from_unicode(gb2312_string,sizeof(gb2312_string)/sizeof(gb2312_string[0]),unicode_string,sizeof(unicode_string)/sizeof(unicode_string[0]));
            hunicode_char_t unicode_string2[sizeof(unicode_string)]= {0};
            hgb2312_string_to_unicode(unicode_string2,sizeof(unicode_string2)/sizeof(unicode_string2[0]),gb2312_string,sizeof(gb2312_string)/sizeof(gb2312_string[0]));
            printf("hgb2312_test:%s\r\n",gb2312_string);
            printf("hgb2312_test:gb2312 string convert %s\r\n",memcmp(unicode_string,unicode_string2,sizeof(unicode_string))==0?"ok":"failed");
        }
    }
    return 0;
}

HSTACKLESSCOROUTINE_DECLARE_COROUTINE(co1_c)
HSTACKLESSCOROUTINE_DECLARE_COROUTINE(co1_cpp)
HSTACKLESSCOROUTINE_DECLARE_COROUTINE_GROUP(main)
HSTACKLESSCOROUTINE_GROUP_BLOCK_START(main)
HSTACKLESSCOROUTINE_GROUP_BLOCK_ITEM(co1_c)
HSTACKLESSCOROUTINE_GROUP_BLOCK_ITEM(co1_cpp)
HSTACKLESSCOROUTINE_GROUP_BLOCK_END(main)
static void hstacklesscoroutine2_co1(hstacklesscoroutine2_scheduler_t *scheduler,hstacklesscoroutine2_ccb_t *ccb,void *usr)
{
    static size_t i=0;
    while(i++<3)
    {
        printf("hstacklesscoroutine2 co1:%d!\r\n",(int)i);
        hstacklesscoroutine2_delay(scheduler,ccb,1);
    }
}
static void hstacklesscoroutine2_co2(hstacklesscoroutine2_scheduler_t *scheduler,hstacklesscoroutine2_ccb_t *ccb,void *usr)
{
    static size_t i=0;
    static bool is_await=false;
    if(!is_await)
    {
        is_await=true;
        hstacklesscoroutine2_await(scheduler,ccb,hstacklesscoroutine_awaiter2_init([](hstacklesscoroutine2_scheduler_t * sch,hstacklesscoroutine2_ccb_t *ccb,hstacklesscoroutine2_awaiter_t *awaiter) -> bool
        {
            if(awaiter==NULL)
            {
                return true;
            }
            uintptr_t & usr=*(uintptr_t *)&awaiter->usr;
            if(usr > 1000)
            {
                return true;
            }
            usr++;
            return false;
        },(void *)(uintptr_t)0));
    }
    while(i++<4)
    {
        printf("hstacklesscoroutine2 co2:%d!\r\n",(int)i);
        hstacklesscoroutine2_delay(scheduler,ccb,1);
    }
}
static void hstacklesscoroutine2_co3(hstacklesscoroutine2_scheduler_t *scheduler,hstacklesscoroutine2_ccb_t *ccb,void *usr)
{
    static size_t i=0;
    while(i++<5)
    {
        printf("hstacklesscoroutine2 co3:%d!\r\n",(int)i);
        hstacklesscoroutine2_delay(scheduler,ccb,1);
    }
}

static void hstacklesscoroutine2_co4(hstacklesscoroutine2_scheduler_t *scheduler,hstacklesscoroutine2_ccb_t *ccb,void *usr)
{
    static size_t i=0;
    HSTACKLESSCOROUTINE2_BLOCK_START(ccb)
    HSTACKLESSCOROUTINE2_BLOCK_LABEL(ccb,1)
    printf("hstacklesscoroutine2 co4:step 1!\r\n");
    HSTACKLESSCOROUTINE2_BLOCK_POINT(ccb)
    printf("hstacklesscoroutine2 co4:step 2!\r\n");
    HSTACKLESSCOROUTINE2_BLOCK_POINT(ccb)
    printf("hstacklesscoroutine2 co4:step 3!\r\n");
    if(i++ < 2)
    {
        hstacklesscoroutine2_delay(scheduler,ccb,1);
    }
    HSTACKLESSCOROUTINE2_BLOCK_END(ccb)

    HSTACKLESSCOROUTINE2_BLOCK_START(ccb)
    printf("hstacklesscoroutine2 co4:step 4!\r\n");
    HSTACKLESSCOROUTINE2_BLOCK_POINT(ccb)
    printf("hstacklesscoroutine2 co4:step 5!\r\n");
    HSTACKLESSCOROUTINE2_BLOCK_POINT(ccb)
    printf("hstacklesscoroutine2 co4:step 6!\r\n");
    if(i++ < 4)
    {
        hstacklesscoroutine2_delay(scheduler,ccb,1);
    }
    if(i++ <= 5)
    {
        HSTACKLESSCOROUTINE2_BLOCK_GOTO(scheduler,ccb,1)
    }
    HSTACKLESSCOROUTINE2_BLOCK_END(ccb)
}

static int hruntime_test(int argc,const char *argv[])
{
    {
        //COFF测试
        const char *coff_list[]=
        {
            "runtime/coff/helloworld.i386.obj",
            "runtime/coff/helloworld.i386.msvc.obj",
            "runtime/coff/helloworld.x86_64.obj",
            "runtime/coff/helloworld.x86_64.msvc.obj",
            "runtime/coff/helloworld.aarch64.obj",
            "runtime/coff/helloworld.aarch64.msvc.obj"
        };
        for(size_t i=0; i<sizeof(coff_list)/sizeof(coff_list[0]); i++)
        {
            const char * RC_PATH=coff_list[i];
            printf("hcoff fileheader:%s\r\n",RC_PATH);
            const uint8_t * helloworld_obj=RCGetHandle(RC_PATH);
            size_t helloworld_obj_size=RCGetSize(RC_PATH);
            hcoff_fileheader_t hdr= {0};
            if(hcoff_fileheader_read(&hdr,helloworld_obj,helloworld_obj_size))
            {
                //文件头读取成功
                printf("hcoff fileheader:magic=%08X,nscns=%d,timdat=%d,symptr=%08X,nsyms=%d,opthdr=%d,flags=%08X\r\n",(int)hdr.f_magic,(int)hdr.f_nscns,(int)hdr.f_timdat,(int)hdr.f_symptr,(int)hdr.f_nsyms,(int)hdr.f_opthdr,(int)hdr.f_flags);
                printf("hcoff fileheader section:section_offset=%08X,section_count=%d\r\n",(int)hcoff_fileheader_section_offset_get(&hdr),(int)hcoff_fileheader_section_count_get(&hdr));
                printf("hcoff fileheader relocatable_object_file:%s\r\n",hcoff_fileheader_is_relocatable_object_file(&hdr)?"true":"false");
                hcoff_file_input_t input_file= {0};
                hcoff_file_input_init(&input_file,[](hcoff_file_input_t *input,uintptr_t address,void *buffer,size_t buffer_length) -> size_t
                {
                    size_t ret=0;
                    if(input == NULL || input->usr==NULL || buffer == NULL )
                    {
                        return ret;
                    }
                    const uint8_t *data=(const uint8_t *)input->usr;
                    memcpy(buffer,&data[address],buffer_length);
                    ret=buffer_length;
                    return ret;

                },(void *)helloworld_obj);
                for(size_t i=0; i < hcoff_fileheader_section_count_get(&hdr) ; i++)
                {
                    hcoff_sectionheader_t sectionheader= {0};
                    if(hcoff_sectionheader_read(&sectionheader,i,&input_file))
                    {
                        char section_name[64]= {0};
                        printf("hcoff sectionheader:name=%s,index=%d\r\n",(const char *)(hcoff_sectionheader_name_read(&sectionheader,&input_file,section_name,sizeof(section_name))!=NULL?section_name:""),(int)i);
                        printf("hcoff sectionheader:rawdata addr=%08X,length=%d\r\n",(int)sectionheader.s_scnptr,(int)sectionheader.s_size);
                        {
                            //通过名称判断类型
                            if(hcoff_sectionheader_name_match(HCOFF_SECTIONHEADER_S_NAME_TEXT,&sectionheader,&input_file))
                            {
                                printf("hcoff sectionheader:type=%s\r\n",HCOFF_SECTIONHEADER_S_NAME_TEXT);
                            }
                            if(hcoff_sectionheader_name_match(HCOFF_SECTIONHEADER_S_NAME_RDATA,&sectionheader,&input_file))
                            {
                                printf("hcoff sectionheader:type=%s\r\n",HCOFF_SECTIONHEADER_S_NAME_RDATA);
                            }
                            if(hcoff_sectionheader_name_match(HCOFF_SECTIONHEADER_S_NAME_PDATA,&sectionheader,&input_file))
                            {
                                printf("hcoff sectionheader:type=%s\r\n",HCOFF_SECTIONHEADER_S_NAME_PDATA);
                            }
                            if(hcoff_sectionheader_name_match(HCOFF_SECTIONHEADER_S_NAME_XDATA,&sectionheader,&input_file))
                            {
                                printf("hcoff sectionheader:type=%s\r\n",HCOFF_SECTIONHEADER_S_NAME_XDATA);
                            }
                            if(hcoff_sectionheader_name_match(HCOFF_SECTIONHEADER_S_NAME_SDATA,&sectionheader,&input_file))
                            {
                                printf("hcoff sectionheader:type=%s\r\n",HCOFF_SECTIONHEADER_S_NAME_SDATA);
                            }
                            if(hcoff_sectionheader_name_match(HCOFF_SECTIONHEADER_S_NAME_DATA,&sectionheader,&input_file))
                            {
                                printf("hcoff sectionheader:type=%s\r\n",HCOFF_SECTIONHEADER_S_NAME_DATA);
                            }
                            if(hcoff_sectionheader_name_match(HCOFF_SECTIONHEADER_S_NAME_BSS,&sectionheader,&input_file))
                            {
                                printf("hcoff sectionheader:type=%s\r\n",HCOFF_SECTIONHEADER_S_NAME_BSS);
                            }
                        }
                        if(sectionheader.s_relptr != 0 && sectionheader.s_nreloc !=0)
                        {
                            hcoff_section_relocation_t relocation= {0};
                            for(size_t j=0; j<sectionheader.s_nreloc; j++)
                            {
                                if(hcoff_section_relocation_read(&relocation,j,&sectionheader,&input_file))
                                {
                                    const char *type_name="";
                                    {
                                        if(hdr.f_magic==HCOFF_FILEHEADER_F_MAGIC_I386)
                                        {
                                            type_name=(const char *)hcoff_i386_section_relocation_info_get(&relocation)->name;
                                        }
                                        if(hdr.f_magic==HCOFF_FILEHEADER_F_MAGIC_AMD64)
                                        {
                                            type_name=(const char *)hcoff_amd64_section_relocation_info_get(&relocation)->name;
                                        }
                                        if(hdr.f_magic==HCOFF_FILEHEADER_F_MAGIC_ARM64)
                                        {
                                            type_name=(const char *)hcoff_aarch64_section_relocation_info_get(&relocation)->name;
                                        }
                                    }
                                    printf("hcoff section relocation:vaddr=%08X,symndx=%d,type=%s(%d)\r\n",(int)relocation.r_vaddr,(int)relocation.r_symndx,type_name,(int)relocation.r_type);
                                }
                            }
                        }
                    }
                }
                for(size_t i=0; i<hdr.f_nsyms; i++)
                {
                    if(hcoff_symbol_is_symbol(i,&input_file))
                    {
                        hcoff_symbol_entry_t symbol_entry;
                        if(hcoff_symbol_entry_read(&symbol_entry,i,&input_file))
                        {
                            char namebuf[64]= {0};
                            hcoff_symbol_entry_name_read(&symbol_entry,&input_file,namebuf,sizeof(namebuf));
                            printf("hcoff symbol(index=%06d):%-32s(%08X),section=%d,type=%02X,sclass=%d\r\n",(int)i,namebuf,(int)symbol_entry.e_value,(int)(int16_t)symbol_entry.e_scnum,(int)symbol_entry.e_type,(int)symbol_entry.e_sclass);
                        }
                    }
                }
            }
        }
#if defined(_M_IX86) || defined(__i386__) || defined(i386) || defined(__i686__) || defined(i686)
#if defined(HDEFAULTS_OS_UNIX) || defined(HDEFAULTS_OS_WINDOWS)
        {
            //i386架构下才支持测试加载i386可重定位对象
            printf("hcoff:current arch=i386\r\n");
            const char *coff_list[]=
            {
                "runtime/coff/helloworld.i386.obj",
                "runtime/coff/helloworld.i386.msvc.obj",
            };
            for(size_t i=0; i<sizeof(coff_list)/sizeof(coff_list[0]); i++)
            {
                const char * RC_PATH=coff_list[i];
                printf("hcoff:%s\r\n",RC_PATH);
                const uint8_t * helloworld_obj=RCGetHandle(RC_PATH);
                size_t helloworld_obj_size=RCGetSize(RC_PATH);
                hcoff_file_input_t input_file= {0};
                hcoff_file_input_init(&input_file,[](hcoff_file_input_t *input,uintptr_t address,void *buffer,size_t buffer_length) -> size_t
                {
                    size_t ret=0;
                    if(input == NULL || input->usr==NULL || buffer == NULL )
                    {
                        return ret;
                    }
                    const uint8_t *data=(const uint8_t *)input->usr;
                    memcpy(buffer,&data[address],buffer_length);
                    ret=buffer_length;
                    return ret;

                },(void *)helloworld_obj);
                const hcoff_i386_relocatable_t * obj=hcoff_i386_relocatable_new(&input_file,hruntime_symbol_find);
                if(obj!=NULL)
                {
                    printf("hcoff:load ok\r\n");
                    {
                        //访问信息
                        for(size_t i=0; i<obj->symbol_table_size; i++)
                        {
                            if(strcmp("coff_info",obj->symbol_table[i].symbol_name)==0 || strcmp("_coff_info",obj->symbol_table[i].symbol_name)==0 )
                            {
                                typedef struct
                                {
                                    const char *name;
                                } coff_info_t;
                                coff_info_t *info=(coff_info_t *)obj->symbol_table[i].symbol_addr;
                                if(info!=NULL && info->name!=NULL)
                                {
                                    printf("hcoff: module name=%s\r\n",info->name);
                                }
                                break;
                            }
                        }
                    }
                    {
                        //访问数据
                        for(size_t i=0; i<obj->symbol_table_size; i++)
                        {
                            if(strcmp("coff_data",obj->symbol_table[i].symbol_name)==0 || strcmp("_coff_data",obj->symbol_table[i].symbol_name)==0 )
                            {
                                typedef struct
                                {
                                    int refcnt;
                                } coff_data_t;
                                coff_data_t *data=(coff_data_t *)obj->symbol_table[i].symbol_addr;
                                if(data!=NULL )
                                {
                                    printf("hcoff: module refcnt=%d\r\n",data->refcnt);
                                }
                                break;
                            }
                        }
                    }
                    {
                        //进入
                        for(size_t i=0; i<obj->symbol_table_size; i++)
                        {
                            if(strcmp("coff_entry",obj->symbol_table[i].symbol_name)==0 || strcmp("_coff_entry",obj->symbol_table[i].symbol_name)==0 )
                            {
                                typedef int (*coff_entry_t)();
                                coff_entry_t entry=(coff_entry_t)obj->symbol_table[i].symbol_addr;
                                if(entry!=NULL)
                                {
                                    printf("hcoff: module enter,ret=%d\r\n",(entry)());
                                }
                                break;
                            }
                        }
                    }
                    {
                        //读取数据
                        for(size_t i=0; i<obj->symbol_table_size; i++)
                        {
                            if(strcmp("coff_data",obj->symbol_table[i].symbol_name)==0 || strcmp("_coff_data",obj->symbol_table[i].symbol_name)==0 )
                            {
                                typedef struct
                                {
                                    int refcnt;
                                } coff_data_t;
                                coff_data_t *data=(coff_data_t *)obj->symbol_table[i].symbol_addr;
                                if(data!=NULL )
                                {
                                    printf("hcoff: module refcnt=%d\r\n",data->refcnt);
                                }
                                break;
                            }
                        }

                    }
                    {
                        //退出
                        for(size_t i=0; i<obj->symbol_table_size; i++)
                        {
                            if(strcmp("coff_exit",obj->symbol_table[i].symbol_name)==0 || strcmp("_coff_exit",obj->symbol_table[i].symbol_name)==0 )
                            {
                                typedef int (*coff_exit_t)();
                                coff_exit_t module_exit=(coff_exit_t )obj->symbol_table[i].symbol_addr;
                                if(module_exit!=NULL)
                                {
                                    printf("hcoff: module exit,ret=%d\r\n",module_exit());
                                }
                                break;
                            }
                        }
                    }
                    hcoff_i386_relocatable_delete(obj);
                }
            }

        }
#endif
#elif defined(_M_X64) || defined(__x86_64) || defined(__x86_64__)
#if defined(HDEFAULTS_OS_UNIX) || defined(HDEFAULTS_OS_WINDOWS)
        {
            //x86_64架构下才支持测试加载x86_64可重定位对象
            printf("hcoff:current arch=amd64\r\n");
            const char *coff_list[]=
            {
                "runtime/coff/helloworld.x86_64.obj",
                "runtime/coff/helloworld.x86_64.msvc.obj",
            };
            for(size_t i=0; i<sizeof(coff_list)/sizeof(coff_list[0]); i++)
            {
                const char * RC_PATH=coff_list[i];
                printf("hcoff:%s\r\n",RC_PATH);
                const uint8_t * helloworld_obj=RCGetHandle(RC_PATH);
                size_t helloworld_obj_size=RCGetSize(RC_PATH);
                hcoff_file_input_t input_file= {0};
                hcoff_file_input_init(&input_file,[](hcoff_file_input_t *input,uintptr_t address,void *buffer,size_t buffer_length) -> size_t
                {
                    size_t ret=0;
                    if(input == NULL || input->usr==NULL || buffer == NULL )
                    {
                        return ret;
                    }
                    const uint8_t *data=(const uint8_t *)input->usr;
                    memcpy(buffer,&data[address],buffer_length);
                    ret=buffer_length;
                    return ret;

                },(void *)helloworld_obj);
                const hcoff_amd64_relocatable_t * obj=hcoff_amd64_relocatable_new(&input_file,hruntime_symbol_find);
                if(obj!=NULL)
                {
                    printf("hcoff:load ok\r\n");
                    {
                        //访问信息
                        for(size_t i=0; i<obj->symbol_table_size; i++)
                        {
                            if(strcmp("coff_info",obj->symbol_table[i].symbol_name)==0 || strcmp("_coff_info",obj->symbol_table[i].symbol_name)==0 )
                            {
                                typedef struct
                                {
                                    const char *name;
                                } coff_info_t;
                                coff_info_t *info=(coff_info_t *)obj->symbol_table[i].symbol_addr;
                                if(info!=NULL && info->name!=NULL)
                                {
                                    printf("hcoff: module name=%s\r\n",info->name);
                                }
                                break;
                            }
                        }
                    }
                    {
                        //访问数据
                        for(size_t i=0; i<obj->symbol_table_size; i++)
                        {
                            if(strcmp("coff_data",obj->symbol_table[i].symbol_name)==0 || strcmp("_coff_data",obj->symbol_table[i].symbol_name)==0 )
                            {
                                typedef struct
                                {
                                    int refcnt;
                                } coff_data_t;
                                coff_data_t *data=(coff_data_t *)obj->symbol_table[i].symbol_addr;
                                if(data!=NULL )
                                {
                                    printf("hcoff: module refcnt=%d\r\n",data->refcnt);
                                }
                                break;
                            }
                        }
                    }
                    {
                        //设置hprintf指针
                        /*
                         *  注意:在x64下，由于Windows与非Windows下的调用约定的差异,跨ABI调用函数会导致传参问题。因此外部函数调用的测试仅支持在Cygwin或者MSYS2 MSYS下测试
                         */
#if defined(HDEFAULTS_OS_CYGWIN) || defined(HDEFAULTS_OS_WINDOWS)
                        for(size_t i=0; i<obj->symbol_table_size; i++)
                        {
                            if(strcmp("_hprintf",obj->symbol_table[i].symbol_name)==0 || strcmp("hprintf",obj->symbol_table[i].symbol_name)==0 )
                            {
                                //设置函数指针
                                typedef int (*hprintf_t)(const char *format,...);
                                hprintf_t* data=(hprintf_t*)obj->symbol_table[i].symbol_addr;
                                (*data)=(hprintf_t)hprintf;
                                break;
                            }
                        }
#endif

                    }
                    {
                        //进入
                        for(size_t i=0; i<obj->symbol_table_size; i++)
                        {
                            if(strcmp("coff_entry",obj->symbol_table[i].symbol_name)==0 || strcmp("_coff_entry",obj->symbol_table[i].symbol_name)==0 )
                            {
                                typedef int (*coff_entry_t)();
                                coff_entry_t entry=(coff_entry_t)obj->symbol_table[i].symbol_addr;
                                if(entry!=NULL)
                                {
                                    printf("hcoff: module enter,ret=%d\r\n",(entry)());
                                }
                                break;
                            }
                        }
                    }
                    {
                        //读取数据
                        for(size_t i=0; i<obj->symbol_table_size; i++)
                        {
                            if(strcmp("coff_data",obj->symbol_table[i].symbol_name)==0 || strcmp("_coff_data",obj->symbol_table[i].symbol_name)==0 )
                            {
                                typedef struct
                                {
                                    int refcnt;
                                } coff_data_t;
                                coff_data_t *data=(coff_data_t *)obj->symbol_table[i].symbol_addr;
                                if(data!=NULL )
                                {
                                    printf("hcoff: module refcnt=%d\r\n",data->refcnt);
                                }
                                break;
                            }
                        }

                    }
                    {
                        //退出
                        for(size_t i=0; i<obj->symbol_table_size; i++)
                        {
                            if(strcmp("coff_exit",obj->symbol_table[i].symbol_name)==0 || strcmp("_coff_exit",obj->symbol_table[i].symbol_name)==0 )
                            {
                                typedef int (*coff_exit_t)();
                                coff_exit_t module_exit=(coff_exit_t )obj->symbol_table[i].symbol_addr;
                                if(module_exit!=NULL)
                                {
                                    printf("hcoff: module exit,ret=%d\r\n",module_exit());
                                }
                                break;
                            }
                        }
                    }
                    hcoff_amd64_relocatable_delete(obj);
                }
            }
        }
#endif
#elif defined(_M_ARM64) || defined(__aarch64__) || defined(__AARCH64EL__)
#if defined(HDEFAULTS_OS_UNIX)
        {

            printf("hcoff:current arch=aarch64\r\n");
            const char *coff_list[]=
            {
                "runtime/coff/helloworld.aarch64.obj",
                "runtime/coff/helloworld.aarch64.msvc.obj"
            };
            for(size_t i=0; i<sizeof(coff_list)/sizeof(coff_list[0]); i++)
            {
                const char * RC_PATH=coff_list[i];
                printf("hcoff:%s\r\n",RC_PATH);
                const uint8_t * helloworld_obj=RCGetHandle(RC_PATH);
                size_t helloworld_obj_size=RCGetSize(RC_PATH);
                hcoff_file_input_t input_file= {0};
                hcoff_file_input_init(&input_file,[](hcoff_file_input_t *input,uintptr_t address,void *buffer,size_t buffer_length) -> size_t
                {
                    size_t ret=0;
                    if(input == NULL || input->usr==NULL || buffer == NULL )
                    {
                        return ret;
                    }
                    const uint8_t *data=(const uint8_t *)input->usr;
                    memcpy(buffer,&data[address],buffer_length);
                    ret=buffer_length;
                    return ret;

                },(void *)helloworld_obj);
                const hcoff_aarch64_relocatable_t * obj=hcoff_aarch64_relocatable_new(&input_file,hruntime_symbol_find);
                if(obj!=NULL)
                {
                    printf("hcoff:load ok\r\n");
                    {
                        //访问信息
                        for(size_t i=0; i<obj->symbol_table_size; i++)
                        {
                            if(strcmp("coff_info",obj->symbol_table[i].symbol_name)==0 || strcmp("_coff_info",obj->symbol_table[i].symbol_name)==0 )
                            {
                                typedef struct
                                {
                                    const char *name;
                                } coff_info_t;
                                coff_info_t *info=(coff_info_t *)obj->symbol_table[i].symbol_addr;
                                if(info!=NULL && info->name!=NULL)
                                {
                                    printf("hcoff: module name=%s\r\n",info->name);
                                }
                                break;
                            }
                        }
                    }
                    {
                        //访问数据
                        for(size_t i=0; i<obj->symbol_table_size; i++)
                        {
                            if(strcmp("coff_data",obj->symbol_table[i].symbol_name)==0 || strcmp("_coff_data",obj->symbol_table[i].symbol_name)==0 )
                            {
                                typedef struct
                                {
                                    int refcnt;
                                } coff_data_t;
                                coff_data_t *data=(coff_data_t *)obj->symbol_table[i].symbol_addr;
                                if(data!=NULL )
                                {
                                    printf("hcoff: module refcnt=%d\r\n",data->refcnt);
                                }
                                break;
                            }
                        }
                    }
                    {
                        //设置hprintf指针
                        for(size_t i=0; i<obj->symbol_table_size; i++)
                        {
                            if(strcmp("_hprintf",obj->symbol_table[i].symbol_name)==0 || strcmp("hprintf",obj->symbol_table[i].symbol_name)==0 )
                            {
                                //设置函数指针
                                typedef int (*hprintf_t)(const char *format,...);
                                hprintf_t* data=(hprintf_t*)obj->symbol_table[i].symbol_addr;
                                (*data)=(hprintf_t)hprintf;
                                break;
                            }
                        }
                    }
                    {
                        //进入
                        for(size_t i=0; i<obj->symbol_table_size; i++)
                        {
                            if(strcmp("coff_entry",obj->symbol_table[i].symbol_name)==0 || strcmp("_coff_entry",obj->symbol_table[i].symbol_name)==0 )
                            {
                                typedef int (*coff_entry_t)();
                                coff_entry_t entry=(coff_entry_t)obj->symbol_table[i].symbol_addr;
                                if(entry!=NULL)
                                {
                                    printf("hcoff: module enter,ret=%d\r\n",(entry)());
                                }
                                break;
                            }
                        }
                    }
                    {
                        //读取数据
                        for(size_t i=0; i<obj->symbol_table_size; i++)
                        {
                            if(strcmp("coff_data",obj->symbol_table[i].symbol_name)==0 || strcmp("_coff_data",obj->symbol_table[i].symbol_name)==0 )
                            {
                                typedef struct
                                {
                                    int refcnt;
                                } coff_data_t;
                                coff_data_t *data=(coff_data_t *)obj->symbol_table[i].symbol_addr;
                                if(data!=NULL )
                                {
                                    printf("hcoff: module refcnt=%d\r\n",data->refcnt);
                                }
                                break;
                            }
                        }

                    }
                    {
                        //退出
                        for(size_t i=0; i<obj->symbol_table_size; i++)
                        {
                            if(strcmp("coff_exit",obj->symbol_table[i].symbol_name)==0 || strcmp("_coff_exit",obj->symbol_table[i].symbol_name)==0 )
                            {
                                typedef int (*coff_exit_t)();
                                coff_exit_t module_exit=(coff_exit_t )obj->symbol_table[i].symbol_addr;
                                if(module_exit!=NULL)
                                {
                                    printf("hcoff: module exit,ret=%d\r\n",module_exit());
                                }
                                break;
                            }
                        }
                    }
                    {
                        //读取数据
                        for(size_t i=0; i<obj->symbol_table_size; i++)
                        {
                            if(strcmp("coff_data",obj->symbol_table[i].symbol_name)==0 || strcmp("_coff_data",obj->symbol_table[i].symbol_name)==0 )
                            {
                                typedef struct
                                {
                                    int refcnt;
                                } coff_data_t;
                                coff_data_t *data=(coff_data_t *)obj->symbol_table[i].symbol_addr;
                                if(data!=NULL )
                                {
                                    printf("hcoff: module refcnt=%d\r\n",data->refcnt);
                                }
                                break;
                            }
                        }

                    }
                    hcoff_aarch64_relocatable_delete(obj);
                }
            }
        }
#endif
#endif
    }
    {
        //无栈协程测试
        {
            //版本1
            printf("hstacklesscoroutine:display coroutine!\r\n");
            {
                auto enum_cb=[](hstacklesscoroutine_entry_t entry)
                {
                    printf("hstacklesscoroutine:%08X\r\n",(int)(intptr_t)entry);
                };
                HSTACKLESSCOROUTINE_GROUP_FOREACH(main,enum_cb)
            }
            printf("hstacklesscoroutine_test1:start!\r\n");
            do
            {
                HSTACKLESSCOROUTINE_GROUP_ENTRY(main,NULL);
            }
            while(!hstacklesscoroutine_is_finished(HSTACKLESSCOROUTINE_GET_GLOBAL_CCB(co1_c)) || !hstacklesscoroutine_is_finished(HSTACKLESSCOROUTINE_GET_GLOBAL_CCB(co1_cpp)));
            printf("hstacklesscoroutine_test1:end!\r\n");
            printf("hstacklesscoroutine_test2:start!\r\n");
            hstacklesscoroutine_coroutine_restart(HSTACKLESSCOROUTINE_GET_GLOBAL_CCB(co1_c));
            hstacklesscoroutine_coroutine_restart(HSTACKLESSCOROUTINE_GET_GLOBAL_CCB(co1_cpp));
            do
            {
                HSTACKLESSCOROUTINE_ENTRY(co1_c);
                HSTACKLESSCOROUTINE_ENTRY(co1_cpp);
                if(hstacklesscoroutine_is_suspend(HSTACKLESSCOROUTINE_GET_GLOBAL_CCB(co1_c)))
                {
                    if(!hstacklesscoroutine_is_await(HSTACKLESSCOROUTINE_GET_GLOBAL_CCB(co1_c)))
                    {
                        hstacklesscoroutine_coroutine_resume(HSTACKLESSCOROUTINE_GET_GLOBAL_CCB(co1_c));
                    }
                }
                else
                {
                    hstacklesscoroutine_coroutine_suspend(HSTACKLESSCOROUTINE_GET_GLOBAL_CCB(co1_c));
                }
            }
            while(!hstacklesscoroutine_is_finished(HSTACKLESSCOROUTINE_GET_GLOBAL_CCB(co1_c)) || !hstacklesscoroutine_is_finished(HSTACKLESSCOROUTINE_GET_GLOBAL_CCB(co1_cpp)));
            printf("hstacklesscoroutine_test2:end!\r\n");
        }
        {
            //版本2
            uint32_t ccb_buffer1[1024]= {0};
            {
                hstacklesscoroutine2_ccb_t *ccb=hstacklesscoroutine2_ccb_init((void *)ccb_buffer1,sizeof(ccb_buffer1));
                hstacklesscoroutine2_ccb_set(ccb,hstacklesscoroutine2_task_init(hstacklesscoroutine2_co1,NULL));
                hstacklesscoroutine2_scheduler_ccb_register(NULL,ccb);
            }
            uint32_t ccb_buffer2[1024]= {0};
            {
                hstacklesscoroutine2_ccb_t *ccb=hstacklesscoroutine2_ccb_init((void *)ccb_buffer2,sizeof(ccb_buffer2));
                hstacklesscoroutine2_ccb_set(ccb,hstacklesscoroutine2_task_init(hstacklesscoroutine2_co2,NULL));
                hstacklesscoroutine2_scheduler_ccb_register(NULL,ccb);
            }
            uint32_t ccb_buffer3[1024]= {0};
            {
                hstacklesscoroutine2_ccb_t *ccb=hstacklesscoroutine2_ccb_init((void *)ccb_buffer3,sizeof(ccb_buffer3));
                hstacklesscoroutine2_ccb_set(ccb,hstacklesscoroutine2_task_init(hstacklesscoroutine2_co3,NULL));
                hstacklesscoroutine2_scheduler_ccb_register(NULL,ccb);
            }

            uint32_t ccb_buffer4[1024]= {0};
            {
                hstacklesscoroutine2_ccb_t *ccb=hstacklesscoroutine2_ccb_init((void *)ccb_buffer4,sizeof(ccb_buffer4));
                hstacklesscoroutine2_ccb_set(ccb,hstacklesscoroutine2_task_init(hstacklesscoroutine2_co4,NULL));
                hstacklesscoroutine2_scheduler_ccb_register(NULL,ccb);
            }

#ifdef HSTACKLESSCOROUTINE2_BARE_MACHINE
            printf("hstacklesscoroutine2:bare machine!\r\n");
#endif // HSTACKLESSCOROUTINE2_BARE_MACHINE

            //启动调度器
            hstacklesscoroutine2_scheduler_start(NULL);
        }
    }
    return 0;
}




HS_RISC_V_OPCODES_INSN_DECLARE(add);
HS_RISC_V_OPCODES_CSR_DECLARE(misa);
static int hsimulator_test(int argc,const char *argv[])
{
    {
        //测试PIO FIFO
        hs_rp_pio_sm_fifo_t fifo;
        hs_rp_pio_sm_fifo_init(&fifo);
        {
            //0个数据
            {
                uint32_t data=0;
                while(hs_rp_pio_sm_fifo_pull(&fifo,&data))
                {
                    printf("hsimulator hs_rp_pio_sm_fifo 5:data=%d\r\n",(int)data);
                }
            }
        }

        {
            //1个数据
            hs_rp_pio_sm_fifo_push(&fifo,1);
            {
                uint32_t data=0;
                while(hs_rp_pio_sm_fifo_pull(&fifo,&data))
                {
                    printf("hsimulator hs_rp_pio_sm_fifo 1:data=%d\r\n",(int)data);
                }
            }
        }

        {
            //2个数据
            hs_rp_pio_sm_fifo_push(&fifo,1);
            hs_rp_pio_sm_fifo_push(&fifo,2);
            {
                uint32_t data=0;
                while(hs_rp_pio_sm_fifo_pull(&fifo,&data))
                {
                    printf("hsimulator hs_rp_pio_sm_fifo 2:data=%d\r\n",(int)data);
                }
            }
        }

        {
            //3个数据
            hs_rp_pio_sm_fifo_push(&fifo,1);
            hs_rp_pio_sm_fifo_push(&fifo,2);
            hs_rp_pio_sm_fifo_push(&fifo,3);
            {
                uint32_t data=0;
                while(hs_rp_pio_sm_fifo_pull(&fifo,&data))
                {
                    printf("hsimulator hs_rp_pio_sm_fifo 3:data=%d\r\n",(int)data);
                }
            }
        }

        {
            //4个数据
            hs_rp_pio_sm_fifo_push(&fifo,1);
            hs_rp_pio_sm_fifo_push(&fifo,2);
            hs_rp_pio_sm_fifo_push(&fifo,3);
            hs_rp_pio_sm_fifo_push(&fifo,4);
            {
                uint32_t data=0;
                while(hs_rp_pio_sm_fifo_pull(&fifo,&data))
                {
                    printf("hsimulator hs_rp_pio_sm_fifo 4:data=%d\r\n",(int)data);
                }
            }
        }

        {
            //5个数据
            hs_rp_pio_sm_fifo_push(&fifo,1);
            hs_rp_pio_sm_fifo_push(&fifo,2);
            hs_rp_pio_sm_fifo_push(&fifo,3);
            hs_rp_pio_sm_fifo_push(&fifo,4);
            hs_rp_pio_sm_fifo_push(&fifo,5);
            {
                uint32_t data=0;
                while(hs_rp_pio_sm_fifo_pull(&fifo,&data))
                {
                    printf("hsimulator hs_rp_pio_sm_fifo 5:data=%d\r\n",(int)data);
                }
            }
        }

        {
            //6个数据
            hs_rp_pio_sm_fifo_push(&fifo,1);
            hs_rp_pio_sm_fifo_push(&fifo,2);
            hs_rp_pio_sm_fifo_push(&fifo,3);
            hs_rp_pio_sm_fifo_push(&fifo,4);
            hs_rp_pio_sm_fifo_push(&fifo,5);
            hs_rp_pio_sm_fifo_push(&fifo,6);
            {
                uint32_t data=0;
                while(hs_rp_pio_sm_fifo_pull(&fifo,&data))
                {
                    printf("hsimulator hs_rp_pio_sm_fifo 6:data=%d\r\n",(int)data);
                }
            }
        }

        {
            //6个数据(中间读取两个)
            hs_rp_pio_sm_fifo_push(&fifo,1);
            hs_rp_pio_sm_fifo_push(&fifo,2);
            hs_rp_pio_sm_fifo_push(&fifo,3);
            {
                uint32_t data=0;
                {
                    if(hs_rp_pio_sm_fifo_pull(&fifo,&data))
                    {
                        printf("hsimulator hs_rp_pio_sm_fifo 6(2):data=%d\r\n",(int)data);
                    }
                }
                {
                    if(hs_rp_pio_sm_fifo_pull(&fifo,&data))
                    {
                        printf("hsimulator hs_rp_pio_sm_fifo 6(2):data=%d\r\n",(int)data);
                    }
                }
            }
            hs_rp_pio_sm_fifo_push(&fifo,4);
            hs_rp_pio_sm_fifo_push(&fifo,5);
            hs_rp_pio_sm_fifo_push(&fifo,6);
            {
                uint32_t data=0;
                while(hs_rp_pio_sm_fifo_pull(&fifo,&data))
                {
                    printf("hsimulator hs_rp_pio_sm_fifo 6(2):data=%d\r\n",(int)data);
                }
            }
        }
    }

    {

        hs_rp_pio_t pio;
        printf("hsimulator hs_rp_pio sm_size=%d,pio_size=%d,pio_sm_size=%d\r\n",(int)hs_rp_pio_sm_size(),(int)sizeof(hs_rp_pio_t),(int)sizeof(pio.sm));
    }

    {
        printf("hsimulator hs_rp_pio_hello_pio start!\r\n");
        hs_rp_pio_hello_pio_t pio;
        hs_rp_pio_hello_pio_init(&pio,[](hs_rp_pio_hello_pio_t *sm,hs_rp_pio_sm_hello_pio_io_opt_t opt,uint32_t *val,void *usr)->bool
        {
            bool ret=true;
            switch(opt)
            {
            case HS_RP_PIO_SM_HELLO_PIO_IO_RESET://IO复位，无参数
            {
                printf("hsimulator hs_rp_pio_hello_pio reset!\r\n");
            }
            break;
            case HS_RP_PIO_SM_HELLO_PIO_IO_WRITE_PINS:
            {
                printf("hsimulator hs_rp_pio_hello_pio pins=%08X!\r\n",(int)(*val));
            }
            break;
            default:
            {

            }
            break;
            }
            return ret;
        },NULL);



        printf("hsimulator hs_rp_pio_hello_pio 40 ticks !\r\n");
        hs_rp_pio_hello_pio_tick(&pio,40);
        printf("hsimulator hs_rp_pio_hello_pio push tx_fifo 0xFFFFFFFF!\r\n");
        hs_rp_pio_hello_pio_push(&pio,0xFFFFFFFF);
        printf("hsimulator hs_rp_pio_hello_pio push tx_fifo 0xFFFFFFFF!\r\n");
        hs_rp_pio_hello_pio_push(&pio,0xFFFFFFFF);
        printf("hsimulator hs_rp_pio_hello_pio push tx_fifo 0x0!\r\n");
        hs_rp_pio_hello_pio_push(&pio,0x0);
        printf("hsimulator hs_rp_pio_hello_pio push tx_fifo 0xFFFFFFFF!\r\n");
        hs_rp_pio_hello_pio_push(&pio,0xFFFFFFFF);
        printf("hsimulator hs_rp_pio_hello_pio 40 ticks !\r\n");
        hs_rp_pio_hello_pio_tick(&pio,40);
        printf("hsimulator hs_rp_pio_hello_pio end!\r\n");
    }

    {
        // 打印mcs_51_core大小。
        printf("hsimulator hs_mcs_51_core_size=%d,HS_MCS_51_CORE_SIZE=%d!\r\n",(int)hs_mcs_51_core_size(),(int)HS_MCS_51_CORE_SIZE());
    }

    {
        //测试MCS-51 Core,足够存下hs_mcs_51_core_t
        uint8_t mcs_51[4096]= {0};
        static hs_mcs_51_serial_t   mcs_51_uart;
        static hs_mcs_51_rom_t      mcs_51_rom=HS_MCS_51_ROM_INITIALIZER;
        static hs_mcs_51_ram_model_huge_t mcs_51_ram;
        static hs_mcs_51_pin_t    mcs_51_pin;
        static hs_mcs_51_timer_t  mcs_51_timer;
        hs_mcs_51_serial_init(&mcs_51_uart,[](hs_mcs_51_serial_t *serial,hs_mcs_51_serial_io_t io_type,uint16_t *data) -> bool
        {
            if(io_type==HS_MCS_51_SERIAL_IO_TRANSMIT)
            {
                putchar((uint8_t)(*data));
            }
            return true;
        },NULL);
        hs_mcs_51_pin_init(&mcs_51_pin,[](hs_mcs_51_pin_t *pin,hs_mcs_51_pin_io_t io_type,hs_mcs_51_pin_port_t port,uint8_t pinnum) -> void
        {
            if(io_type==HS_MCS_51_PIN_IO_PIN_CHANGE)
            {
                bool value=hs_mcs_51_pin_port_pin_get((hs_mcs_51_core_t *)pin->usr,pin,port,pinnum);
                printf("hsimulator mcs_51_pin port=%d pinnum=%d value=%s\r\n",static_cast<int>(port),(int)pinnum,value?"HIGH":"LOW");
            }
        },mcs_51);
        hs_mcs_51_timer_init(&mcs_51_timer);
        hs_mcs_51_core_t *core=hs_mcs_51_core_init(mcs_51,[](hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr)->bool
        {
            //RAM操作(注意：此操作前不应当有外设操作)
            hs_mcs_51_ram_model_huge_bus_io(core,opt,address,data,length,usr,&mcs_51_ram);
            //ROM操作
            hs_mcs_51_rom_bus_io(core,opt,address,data,length,usr,&mcs_51_rom);
            //处理串口外设
            hs_mcs_51_serial_bus_io(core,opt,address,data,length,usr,&mcs_51_uart);
            //处理PIN
            hs_mcs_51_pin_bus_io(core,opt,address,data,length,usr,&mcs_51_pin);
            //处理定时器
            hs_mcs_51_timer_bus_io(core,opt,address,data,length,usr,&mcs_51_timer);
            return true;
        }
        ,mcs_51);

        {
            printf("hsimulator mcs_51_core(helloworld) loading!\r\n");
            mcs_51_rom=hs_mcs_51_rom_helloworld;
            printf("hsimulator mcs_51_core(helloworld) instruction_count=%d,instruction_type_count=%d!\r\n",(int)hs_mcs_51_disassembly_code_instruction_count(mcs_51_rom.code,mcs_51_rom.len),(int)hs_mcs_51_disassembly_code_instruction_type_count(mcs_51_rom.code,mcs_51_rom.len));
            printf("hsimulator mcs_51_core(helloworld) start!\r\n");
            hs_mcs_51_core_tick(core,10000);
            {
                const char * serialport_str="SerialPort Ok!\n";
                while((*serialport_str)!='\0')
                {
                    if(hs_mcs_51_serial_status_dataready_set(core,&mcs_51_uart,((*serialport_str))))
                    {
                        //等待内核处理数据
                        hs_mcs_51_core_tick(core,100);
                        serialport_str++;
                    }
                    else
                    {
                        //等待内核处理数据
                        hs_mcs_51_core_tick(core,1);
                    }
                }
                //内核处理最后一个数据
                hs_mcs_51_core_tick(core,10000);
            }
            printf("hsimulator mcs_51_core(helloworld) end!\r\n");
            printf("hsimulator mcs_51_serial mode=%d,baud=%d!\r\n",(int)hs_mcs_51_serial_config_mode_get(core,&mcs_51_uart),(int)hs_mcs_51_serial_config_baud_get(core,&mcs_51_uart));

        }

        {
            printf("hsimulator mcs_51_core(helloworld_stdio) loading!\r\n");
            hs_mcs_51_core_reset(core);
            mcs_51_rom=hs_mcs_51_rom_helloworld_stdio;
            printf("hsimulator mcs_51_core(helloworld_stdio) instruction_count=%d,instruction_type_count=%d!\r\n",(int)hs_mcs_51_disassembly_code_instruction_count(mcs_51_rom.code,mcs_51_rom.len),(int)hs_mcs_51_disassembly_code_instruction_type_count(mcs_51_rom.code,mcs_51_rom.len));
            printf("hsimulator mcs_51_core(helloworld_stdio) start!\r\n");
            hs_mcs_51_core_tick(core,10000);
            {
                const char * serialport_str="SerialPort Ok!\n";
                while((*serialport_str)!='\0')
                {
                    if(hs_mcs_51_serial_status_dataready_set(core,&mcs_51_uart,((*serialport_str))))
                    {
                        //等待内核处理数据
                        hs_mcs_51_core_tick(core,100);
                        serialport_str++;
                    }
                    else
                    {
                        //等待内核处理数据
                        hs_mcs_51_core_tick(core,1);
                    }
                }
                //内核处理最后一个数据
                hs_mcs_51_core_tick(core,10000);
            }
            printf("hsimulator mcs_51_core(helloworld_stdio) end!\r\n");
            printf("hsimulator mcs_51_serial mode=%d,baud=%d!\r\n",(int)hs_mcs_51_serial_config_mode_get(core,&mcs_51_uart),(int)hs_mcs_51_serial_config_baud_get(core,&mcs_51_uart));

        }

        {
            //测试intelhex读取
            const uint8_t *helloworld_ihx=RCGetHandle("simulator/mcs_51/rom/helloworld/helloworld.ihx");
            size_t helloworld_ihx_len=RCGetSize("simulator/mcs_51/rom/helloworld/helloworld.ihx");
            if(helloworld_ihx!=NULL)
            {
                hintelhex_reader_t reader;
                uint8_t code[64*1024];
                memset(code,0xFF,sizeof(code));
                hintelhex_reader_init(&reader,[](hintelhex_reader_t *reader,hintelhex_reader_operate_t op,uint32_t address,const uint8_t *data,size_t data_len)
                {
                    uint8_t *code=(uint8_t *)reader->usr;
                    if(op==HINTELHEX_READER_OPERATE_DATA)
                    {
                        if(address+data_len < 0x10000)
                        {
                            memcpy(&code[address],data,data_len);
                        }
                    }
                    if(op==HINTELHEX_READER_OPERATE_END_OF_FILE)
                    {
                        printf("hsimulator mcs51 ihx(helloworld.ihx) load:end of file!\r\n");
                        if(memcmp(hs_mcs_51_rom_helloworld.code,code,hs_mcs_51_rom_helloworld.len)==0)
                        {
                            printf("hsimulator mcs51 ihx(helloworld.ihx) load:ok\r\n");
                        }
                    }
                },code);
                printf("hsimulator mcs51 ihx(helloworld.ihx) load:start!\r\n");
                hintelhex_reader_input(&reader,(const char *)helloworld_ihx,helloworld_ihx_len);
                printf("hsimulator mcs51 ihx(helloworld.ihx) load:end!\r\n");
            }
        }
        {
            //测试intelhex写入
            hintelhex_writer_t writer;
            hintelhex_writer_init(&writer,[](hintelhex_writer_t *writer,const char *line,size_t line_length)
            {
                printf("hsimulator mcs51 ihx(helloworld.ihx) store:%s\r\n",line);
            },NULL);
            printf("hsimulator mcs51 ihx(helloworld.ihx) store:start!\r\n");
            hintelhex_writer_write_data(&writer,0,(const uint8_t *)hs_mcs_51_rom_helloworld.code,hs_mcs_51_rom_helloworld.len);
            hintelhex_writer_write_end_of_file(&writer);
            printf("hsimulator mcs51 ihx(helloworld.ihx) store:end!\r\n");
        }
    }

    {
        //测试RISC-V opcodes组件
        hs_risc_v_opcodes_insn_t insn=HS_RISC_V_OPCODES_INSN_NAME(add);
        printf("hsimulator risc-v opcodes insn add name=%s,mask=%08X,match=%08X\r\n",insn.name,insn.mask,insn.match);

        hs_risc_v_opcodes_csr_t csr=HS_RISC_V_OPCODES_CSR_NAME(misa);
        printf("hsimulator risc-v opcodes csr misa name=%s,addr=%08X\r\n",csr.name,(int)csr.addr);
    }
    {
        //测试RISC-V Common组件
        uint8_t bytes[4]= {0x44,0x33,0x22,0x11};
        hs_risc_v_common_memory_word_t word;
        memcpy(word.bytes,bytes,sizeof(bytes));
        printf("hsimulator risc-v common value(before fix)=%08X!\r\n",(int)word.value);
        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(word);
        printf("hsimulator risc-v common value(after  fix)=%08X!\r\n",(int)word.value);
        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(word);
        printf("hsimulator risc-v common bytes=%02X%02X%02X%02X!\r\n",(int)word.bytes[0],(int)word.bytes[1],(int)word.bytes[2],(int)word.bytes[3]);
    }

    {
        // 打印hs_risc_v_core_rv32大小。
        printf("hsimulator hs_risc_v_core_rv32_size=%d,HS_RISC_V_CORE_RV32_SIZE=%d!\r\n",(int)hs_risc_v_core_rv32_size(),(int)HS_RISC_V_CORE_RV32_SIZE());
    }

    return 0;
}

//导入main_entry消息定义及实现
#include "pbinc/main_entry.pb.h"
#include "pbinc/main_entry.pb.c"

//测试paho.mqtt.embedded-c头文件引入功能
#include H3RDPARTY_PAHO_MQTT_EMBEDDED_C_PACKET_HEADER

static int h3rdparty_test(int argc,const char *argv[])
{
    printf("h3rdparty_test:start!\r\n");

    h3rdparty_init();

    {
        //cJSON测试
        printf("h3rdparty_test:cJSON start!\r\n");
        cJSON * root=cJSON_Parse("{\"cJSON\":\"Testing\"}");
        if(root!=NULL)
        {
            cJSON_AddBoolToObject(root,"bool",true);
            cJSON_AddNullToObject(root,"null");
            cJSON_AddNumberToObject(root,"number",12345);
            cJSON_AddStringToObject(root,"string","ok");
            {
                char buffer[4096];
                buffer[0]='\0';
                if(cJSON_PrintPreallocated(root,buffer,sizeof(buffer),true))
                {
                    printf("%s\r\n",buffer);
                }
            }
            cJSON_Delete(root);
        }
        printf("h3rdparty_test:cJSON end!\r\n");
    }

    {
        printf("h3rdparty_test:nanopb start!\r\n");
        {
            uint8_t buffer[4096]= {0};
            //数据长度
            size_t buffer_length=0;
            {
                //消息编码（序列化）
                printf("h3rdparty_test:nanopb encode!\r\n");
                MainEntry message=MainEntry_init_zero;
                message.argc=argc;
                message.argv.funcs.encode=[] (pb_ostream_t *stream, const pb_field_t *field, void * const *arg)->bool
                {
                    const char **argv=(const char **)*arg;
                    if(stream!=NULL && field->tag == MainEntry_argv_tag)
                    {
                        for(size_t i=0; argv[i]!=NULL; i++)
                        {
                            if (!pb_encode_tag_for_field(stream, field))
                            {
                                return false;
                            }
                            if(!pb_encode_string(stream,(const pb_byte_t *)argv[i],strlen(argv[i])))
                            {
                                return false;
                            }
                        }
                    }
                    return true;
                };
                message.argv.arg=(void *)argv;
                pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
                if(pb_encode(&stream, MainEntry_fields, &message))
                {
                    buffer_length=stream.bytes_written;
                }
            }
            {
                for(size_t i=0; i<buffer_length; i++)
                {
                    if(i!=0 && (i%16) == 0)
                    {
                        printf("\r\n");
                    }
                    printf("%02X ",(int)buffer[i]);
                }
                printf("\r\n");
            }
            {
                //消息解码（反序列化）
                printf("h3rdparty_test:nanopb decode!\r\n");
                MainEntry message=MainEntry_init_zero;
                message.argc=argc;
                message.argv.funcs.decode=[] (pb_istream_t *stream, const pb_field_t *field, void **arg)->bool
                {
                    if(stream!=NULL && field->tag == MainEntry_argv_tag)
                    {
                        uint8_t buff[4096]= {0};
                        if(sizeof(buff) < stream->bytes_left)
                        {
                            return false;
                        }
                        pb_read(stream,buff,stream->bytes_left);
                        printf("argv:%s\r\n",(char *)buff);
                    }
                    return true;
                };
                pb_istream_t stream = pb_istream_from_buffer(buffer, buffer_length);
                if(pb_decode(&stream, MainEntry_fields, &message))
                {
                    printf("argc=%d\r\n",message.argc);
                }
            }
        }
        printf("h3rdparty_test:nanopb end!\r\n");
    }

    {
        const char * data="12346678901203349454558656767676976767677000000000000000000000000000000000000000000000000000000000sdefdefrfrgr";
        uint8_t data_compress[256]= {0};
        printf("h3rdparty_test:zlib compress test!\r\n");
        bool is_compress_success=false;
        unsigned long data_compress_length=sizeof(data_compress);
        {
            if(Z_OK==compress2(data_compress,&data_compress_length,(const uint8_t *)data,strlen(data),Z_BEST_COMPRESSION))
            {
                printf("h3rdparty_test:zlib compress success!data_compress_length=%d\r\n",(int)data_compress_length);
                is_compress_success=true;
            }
        }
        uint8_t data_uncompress[256]= {0};
        unsigned long data_uncompress_length=sizeof(data_uncompress);
        if(is_compress_success)
        {
            if(Z_OK==uncompress(data_uncompress,&data_uncompress_length,(const uint8_t *)data_compress,data_uncompress_length))
            {
                printf("h3rdparty_test:zlib uncompress success!data_uncompress_length=%d\r\n",(int)data_uncompress_length);
            }
        }

        if(data_uncompress_length==strlen(data) && memcmp(data,data_uncompress,data_uncompress_length)==0)
        {
            printf("h3rdparty_test:zlib compress test OK!\r\n");
        }
        else
        {
            printf("h3rdparty_test:zlib compress test Failed!\r\n");
        }
        memset(data_compress,0,sizeof(data_compress));
        memset(data_uncompress,0,sizeof(data_uncompress));

        //测试LZ4
        is_compress_success=false;
        data_compress_length=sizeof(data_compress);
        data_uncompress_length=sizeof(data_uncompress);
        {
            if(0!=(data_compress_length=LZ4_compress_default((const char *)data,(char *)data_compress,strlen(data),data_compress_length)))
            {
                printf("h3rdparty_test:lz4 compress success!data_compress_length=%d\r\n",(int)data_compress_length);
                is_compress_success=true;
            }
        }
        if(is_compress_success)
        {
            if(0!=(data_uncompress_length=LZ4_decompress_safe((const char *)data_compress,(char *)data_uncompress,data_compress_length,data_uncompress_length)))
            {
                printf("h3rdparty_test:lz4 uncompress success!data_compress_length=%d\r\n",(int)data_uncompress_length);
                is_compress_success=true;
            }
        }
        if(data_uncompress_length==strlen(data) && memcmp(data,data_uncompress,data_uncompress_length)==0)
        {
            printf("h3rdparty_test:lz4 compress test OK!\r\n");
        }
        else
        {
            printf("h3rdparty_test:lz4 compress test Failed!\r\n");
        }

    }

    printf("h3rdparty test:end!\r\n");
    return 0;
}

static int huuid_test(int argc,const char *argv[])
{
    {
        huuid_short16_t ble_uuid=0xFFF0;
        huuid_t simple_uuid0= {0};
        huuid_uuid_ble_set(simple_uuid0,ble_uuid);
        huuid_string_t uuid0_string= {0},uuid0_string_upper= {0},uuid0_string_lower= {0};
        huuid_unparse(uuid0_string,simple_uuid0);
        huuid_unparse_upper(uuid0_string_upper,simple_uuid0);
        huuid_unparse_lower(uuid0_string_lower,simple_uuid0);
        printf("huuid ble uuid(%04X):%s\tupper %s\tlower %s\r\n",(int)huuid_uuid_ble_get(simple_uuid0),uuid0_string,uuid0_string_upper,uuid0_string_lower);
    }
    {
        //测试uuid格式化
        srand(time(NULL));
        for(size_t i=0; i<10; i++)
        {
            huuid_t uuid= {0};
            for(size_t j=0; j<sizeof(uuid); j++)
            {
                uuid[j]=rand()%0x100;
            }
            {
                //随机uuid
                huuid_random_uuid_format(uuid);
                huuid_string_t uuid_string= {0};
                huuid_unparse(uuid_string,uuid);
                printf("huuid uuid_random:%s\r\n",uuid_string);
                {
                    //测试解析
                    huuid_t uuid1= {0};
                    huuid_parse(uuid1,uuid_string);
                    huuid_string_t uuid1_string= {0};
                    huuid_unparse(uuid1_string,uuid1);
                    printf("huuid uuid parse:%s\r\n",uuid1_string);
                }
            }
            {
                //时间顺序+随机 uuid
                uint64_t unix_ts_ms=time(NULL)*1000; /**< 实际应用中应当使用getimeofday可获取毫秒数的函数，此处仅用于测试 */
                huuid_t uuid1;
                huuid_copy(uuid1,uuid);
                huuid_time_ordered_random_uuid_format(uuid1,unix_ts_ms);
                huuid_string_t uuid_string= {0};
                huuid_unparse(uuid_string,uuid1);
                printf("huuid uuid_time_ordered_random:%s\r\n",uuid_string);
            }
            {
                //自定义uuid
                huuid_t uuid1;
                huuid_copy(uuid1,uuid);
                huuid_custom_uuid_format(uuid1);
                huuid_string_t uuid_string= {0};
                huuid_unparse(uuid_string,uuid1);
                printf("huuid uuid_custom:%s\r\n",uuid_string);
            }
        }

        {
            huuid_node_t node= {0};
            for(size_t j=0; j<sizeof(node); j++)
            {
                node[j]=rand()%0x100;
            }
            huuid_node_format(node);
            uint64_t unix_ts_us =   time(NULL)*1000000; /**< 实际应用中应当使用getimeofday可获取毫秒数的函数，此处仅用于测试 */
            uint16_t clock_seq  =   0xf4f;/**< 此处需要实际的值，此处仅用于测试*/
            {
                huuid_t uuid;
                huuid_time_uuid_format(uuid,huuid_timestamp_convert(unix_ts_us),clock_seq,node);
                huuid_string_t uuid_string;
                huuid_unparse(uuid_string,uuid);
                printf("huuid uuid v1:%s\r\n",uuid_string);
            }
            {
                huuid_t uuid;
                huuid_time_be_uuid_format(uuid,huuid_timestamp_convert(unix_ts_us),clock_seq,node);
                huuid_string_t uuid_string;
                huuid_unparse(uuid_string,uuid);
                printf("huuid uuid v6:%s\r\n",uuid_string);
            }
        }

        {
            //uuid V3
            huuid_t uuid;
            const char *name="a";
            huuid_md5_uuid_generate(uuid,(const uint8_t *)name,strlen(name),huuid_url_uuid);
            huuid_string_t uuid_string;
            huuid_unparse(uuid_string,uuid);
            printf("huuid uuid v3:name=%s,uuid=%s\r\n",name,uuid_string);
        }

        {
            //uuid V5
            huuid_t uuid;
            const char *name="a";
            huuid_sha1_uuid_generate(uuid,(const uint8_t *)name,strlen(name),huuid_url_uuid);
            huuid_string_t uuid_string;
            huuid_unparse(uuid_string,uuid);
            printf("huuid uuid v5:name=%s,uuid=%s\r\n",name,uuid_string);
        }
    }
    return 0;
}

static int hcrypto_test(int argc,const char *argv[])
{
    {
        //大数测试
        huint128_t value[8];
        {
            huint128_load_uint32(&value[0],0x11);
            huint128_load_uint32(&value[1],0x0e);
            huint128_power(&value[2],&value[3],&value[4],&value[5],&value[0],&value[1]);
            uint32_t a=0;
            huint128_store_uint32(&value[0],&a);
            uint32_t b=0;
            huint128_store_uint32(&value[1],&b);
            printf("hcrypto uint128:%08X pow %08X=%08X%08X\r\n",(int)a,(int)b,(int)value[5].val[1],(int)value[5].val[0]);
            huint128_copy(&value[0],&value[5]);
            huint128_load_uint32(&value[1],0xFEC00000U);
            uint32_t c=0;
            huint128_store_uint32(&value[1],&c);
            huint128_div(&value[2],&value[3],&value[4],&value[5],&value[0],&value[1]);
            printf("hcrypto uint128:%08X pow %08X mod %08X=%08X%08X\r\n",a,b,c,(int)value[2].val[1],(int)value[2].val[0]);
        }
        {
            //专用的幂取模
            huint128_load_uint32(&value[0],0x11);
            huint128_load_uint32(&value[1],0x0e);
            huint128_load_uint32(&value[2],0xFEC00000U);
            huint128_power_mod(&value[3],&value[4],&value[5],&value[6],&value[7],&value[0],&value[1],&value[2]);
            uint32_t a=0;
            huint128_store_uint32(&value[0],&a);
            uint32_t b=0;
            huint128_store_uint32(&value[1],&b);
            uint32_t c=0;
            huint128_store_uint32(&value[2],&c);
            printf("hcrypto uint128:%08X pow %08X mod %08X=%08X%08X\r\n",a,b,c,(int)value[7].val[1],(int)value[7].val[0]);
        }
        {
            {
                huint128_load_uint32(&value[0],0xFFFF234D);
                huint128_load_uint32(&value[1],0xD0D);
                huint128_div(&value[2],&value[3],&value[4],&value[5],&value[0],&value[1]);
                uint32_t a=0;
                huint128_store_uint32(&value[0],&a);
                uint32_t b=0;
                huint128_store_uint32(&value[1],&b);
                printf("hcrypto uint128:%08X/%08X=%08X%08X,%08X%%%08X=%08X%08X\r\n",(int)a,(int)b,(int)value[5].val[1],(int)value[5].val[0],(int)a,(int)b,(int)value[2].val[1],(int)value[2].val[0]);
            }
            {
                huint128_load_uint64(&value[0],0xFFFFFFFFFFFF234DULL);
                huint128_load_uint64(&value[1],0xD0D000000);
                huint128_div(&value[2],&value[3],&value[4],&value[5],&value[0],&value[1]);
                uint64_t a=0;
                huint128_store_uint64(&value[0],&a);
                uint64_t b=0;
                huint128_store_uint64(&value[1],&b);
                printf("hcrypto uint128:%016" PRIX64 "/%016" PRIX64 "=%08X%08X,%016" PRIX64 "%%%016" PRIX64 "=%08X%08X\r\n",a,b,(int)value[5].val[1],(int)value[5].val[0],a,b,(int)value[2].val[1],(int)value[2].val[0]);
            }
        }

        {
            huint8224_t num[8];
            {
                const uint8_t data[]=
                {
                    0xa3, 0xfb, 0xf0, 0x7d, 0xf3, 0xfa, 0x2f, 0xde,
                    0x4f, 0x37, 0x6c, 0xa2, 0x3e, 0x82, 0x73, 0x70,
                    0x41, 0x60, 0x5d, 0x9f, 0x4f, 0x4f, 0x57, 0xbd,
                    0x8c, 0xff, 0x2c, 0x1d, 0x4b, 0x79, 0x55, 0xec,
                    0x2a, 0x97, 0x94, 0x8b, 0xd3, 0x72, 0x29, 0x15,
                    0xc8, 0xf3, 0xd3, 0x37, 0xf7, 0xd3, 0x70, 0x05,
                    0x0e, 0x9e, 0x96, 0xd6, 0x47, 0xb7, 0xc3, 0x9f,
                    0x56, 0xe0, 0x31, 0xca, 0x5e, 0xb6, 0x25, 0x0d,
                    0x40, 0x42, 0xe0, 0x27, 0x85, 0xec, 0xec, 0xfa,
                    0x4b, 0x4b, 0xb5, 0xe8, 0xea, 0xd0, 0x44, 0x0e,
                    0x20, 0xb6, 0xe8, 0xdb, 0x09, 0xd8, 0x81, 0xa7,
                    0xc6, 0x13, 0x2f, 0x42, 0x0e, 0x52, 0x79, 0x50,
                    0x42, 0xbd, 0xfa, 0x77, 0x73, 0xd8, 0xa9, 0x05,
                    0x14, 0x47, 0xb3, 0x29, 0x1c, 0xe1, 0x41, 0x1c,
                    0x68, 0x04, 0x65, 0x55, 0x2a, 0xa6, 0xc4, 0x05,
                    0xb7, 0x76, 0x4d, 0x5e, 0x87, 0xbe, 0xa8, 0x5a,
                    0xd0, 0x0f, 0x84, 0x49, 0xed, 0x8f, 0x72, 0xd0,
                    0xd6, 0x62, 0xab, 0x05, 0x26, 0x91, 0xca, 0x66,
                    0x42, 0x4b, 0xc8, 0x6d, 0x2d, 0xf8, 0x0e, 0xa4,
                    0x1f, 0x43, 0xab, 0xf9, 0x37, 0xd3, 0x25, 0x9d,
                    0xc4, 0xb2, 0xd0, 0xdf, 0xb4, 0x8a, 0x6c, 0x91,
                    0x39, 0xdd, 0xd7, 0xf7, 0x69, 0x66, 0xe9, 0x28,
                    0xe6, 0x35, 0x55, 0x3b, 0xa7, 0x6c, 0x5c, 0x87,
                    0x9d, 0x7b, 0x35, 0xd4, 0x9e, 0xb2, 0xe6, 0x2b,
                    0x08, 0x71, 0xcd, 0xac, 0x63, 0x89, 0x39, 0xe2,
                    0x5e, 0x8a, 0x1e, 0x0e, 0xf9, 0xd5, 0x28, 0x0f,
                    0xa8, 0xca, 0x32, 0x8b, 0x35, 0x1c, 0x3c, 0x76,
                    0x59, 0x89, 0xcb, 0xcf, 0x3d, 0xaa, 0x8b, 0x6c,
                    0xcc, 0x3a, 0xaf, 0x9f, 0x39, 0x79, 0xc9, 0x2b,
                    0x37, 0x20, 0xfc, 0x88, 0xdc, 0x95, 0xed, 0x84,
                    0xa1, 0xbe, 0x05, 0x9c, 0x64, 0x99, 0xb9, 0xfd,
                    0xa2, 0x36, 0xe7, 0xe8, 0x18, 0xb0, 0x4b, 0x0b,
                    0xc3, 0x9c, 0x1e, 0x87, 0x6b, 0x19, 0x3b, 0xfe,
                    0x55, 0x69, 0x75, 0x3f, 0x88, 0x12, 0x8c, 0xc0,
                    0x8a, 0xaa, 0x9b, 0x63, 0xd1, 0xa1, 0x6f, 0x80,
                    0xef, 0x25, 0x54, 0xd7, 0x18, 0x9c, 0x41, 0x1f,
                    0x58, 0x69, 0xca, 0x52, 0xc5, 0xb8, 0x3f, 0xa3,
                    0x6f, 0xf2, 0x16, 0xb9, 0xc1, 0xd3, 0x00, 0x62,
                    0xbe, 0xbc, 0xfd, 0x2d, 0xc5, 0xbc, 0xe0, 0x91,
                    0x19, 0x34, 0xfd, 0xa7, 0x9a, 0x86, 0xf6, 0xe6,
                    0x98, 0xce, 0xd7, 0x59, 0xc3, 0xff, 0x9b, 0x64,
                    0x77, 0x33, 0x8f, 0x3d, 0xa4, 0xf9, 0xcd, 0x85,
                    0x14, 0xea, 0x99, 0x82, 0xcc, 0xaf, 0xb3, 0x41,
                    0xb2, 0x38, 0x4d, 0xd9, 0x02, 0xf3, 0xd1, 0xab,
                    0x7a, 0xc6, 0x1d, 0xd2, 0x9c, 0x6f, 0x21, 0xba,
                    0x5b, 0x86, 0x2f, 0x37, 0x30, 0xe3, 0x7c, 0xfd,
                    0xc4, 0xfd, 0x80, 0x6c, 0x22, 0xf2, 0x21
                };
                {
                    huint8224_load(&num[0],data,sizeof(data));
                    uint8_t data_store[sizeof(data)]= {0};
                    huint8224_store(&num[0],data_store,sizeof(data_store));
                    printf("hcrypto uint load and store %s\r\n",memcmp(data,data_store,sizeof(data))==0?"ok":"failed");
                }
                {
                    huint8224_load_be(&num[0],data,sizeof(data));
                    uint8_t data_store[sizeof(data)]= {0};
                    huint8224_store_be(&num[0],data_store,sizeof(data_store));
                    printf("hcrypto uint load_be and store_be %s\r\n",memcmp(data,data_store,sizeof(data))==0?"ok":"failed");
                }

            }
        }
    }
    {
        const char *testdata="01234567890ABCabc~12345@#$%测试123";
        char encoding[1024]= {0};
        hbase16_encode_with_null_terminator(encoding,sizeof(encoding),(uint8_t *)testdata,strlen(testdata));
        printf("hcrypto base16:data=%s,encoding=%s\r\n",testdata,encoding);
        uint8_t decode[1024]= {0};
        hbase16_decode(decode,sizeof(decode),encoding,strlen(encoding));
        printf("hcrypto base16:decode=%s,encoding=%s\r\n",decode,encoding);
        printf("hcrypto base16: %s\r\n",strcmp((char *)testdata,(char *)decode)==0?"ok":"failed");
    }
    {
        {
            printf("hcrypto base64 test1:start!\r\n");
            uint8_t text[]="1";
            char text_encode[32]= {0};
            size_t text_encode_len=hbase64_encode(text_encode,sizeof(text_encode),text,strlen((char *)text));
            uint8_t text_decode[32]= {0};
            size_t text_decode_len=hbase64_decode(text_decode,sizeof(text_decode),text_encode,hbase64_get_encoding_length(text_encode));
            printf("text=%s,encode=%s(%d),decode=%s(%d)\r\n",(char *)text,(char *)text_encode,(int)text_encode_len,(char *)text_decode,(int)text_decode_len);
            printf("hcrypto base64 test: %s\r\n",(strcmp((char *)text,(char *)text_decode)==0)?"ok":"failed");
            printf("hcrypto base64 test1:end!\r\n");
        }
        {
            printf("hcrypto base64 test2:start!\r\n");
            uint8_t text[]="12";
            char text_encode[32]= {0};
            size_t text_encode_len=hbase64_encode(text_encode,sizeof(text_encode),text,strlen((char *)text));
            uint8_t text_decode[32]= {0};
            size_t text_decode_len=hbase64_decode(text_decode,sizeof(text_decode),text_encode,hbase64_get_encoding_length(text_encode));
            printf("text=%s,encode=%s(%d),decode=%s(%d)\r\n",(char *)text,(char *)text_encode,(int)text_encode_len,(char *)text_decode,(int)text_decode_len);
            printf("hcrypto base64 test: %s\r\n",(strcmp((char *)text,(char *)text_decode)==0)?"ok":"failed");
            printf("hcrypto base64 test2:end!\r\n");
        }
        {
            printf("hcrypto base64 test3:start!\r\n");
            uint8_t text[]="123";
            char text_encode[32]= {0};
            size_t text_encode_len=hbase64_encode(text_encode,sizeof(text_encode),text,strlen((char *)text));
            uint8_t text_decode[32]= {0};
            size_t text_decode_len=hbase64_decode(text_decode,sizeof(text_decode),text_encode,hbase64_get_encoding_length(text_encode));
            printf("text=%s,encode=%s(%d),decode=%s(%d)\r\n",(char *)text,(char *)text_encode,(int)text_encode_len,(char *)text_decode,(int)text_decode_len);
            printf("hcrypto base64 test: %s\r\n",(strcmp((char *)text,(char *)text_decode)==0)?"ok":"failed");
            printf("hcrypto base64 test3:end!\r\n");
        }
        {
            printf("hcrypto base64 test4:start!\r\n");
            uint8_t text[]="1234";
            char text_encode[32]= {0};
            size_t text_encode_len=hbase64_encode(text_encode,sizeof(text_encode),text,strlen((char *)text));
            uint8_t text_decode[32]= {0};
            size_t text_decode_len=hbase64_decode(text_decode,sizeof(text_decode),text_encode,hbase64_get_encoding_length(text_encode));
            printf("text=%s,encode=%s(%d),decode=%s(%d)\r\n",(char *)text,(char *)text_encode,(int)text_encode_len,(char *)text_decode,(int)text_decode_len);
            printf("hcrypto base64 test: %s\r\n",(strcmp((char *)text,(char *)text_decode)==0)?"ok":"failed");
            printf("hcrypto base64 test4:end!\r\n");
        }
        {
            printf("hcrypto base64 test5:start!\r\n");
            uint8_t text[]="12345";
            char text_encode[32]= {0};
            size_t text_encode_len=hbase64_encode(text_encode,sizeof(text_encode),text,strlen((char *)text));
            uint8_t text_decode[32]= {0};
            size_t text_decode_len=hbase64_decode(text_decode,sizeof(text_decode),text_encode,hbase64_get_encoding_length(text_encode));
            printf("text=%s,encode=%s(%d),decode=%s(%d)\r\n",(char *)text,(char *)text_encode,(int)text_encode_len,(char *)text_decode,(int)text_decode_len);
            printf("hcrypto base64 test: %s\r\n",(strcmp((char *)text,(char *)text_decode)==0)?"ok":"failed");
            printf("hcrypto base64 test5:end!\r\n");
        }
        {
            printf("hcrypto base64 test6:start!\r\n");
            uint8_t text[]="123456";
            char text_encode[32]= {0};
            size_t text_encode_len=hbase64_encode(text_encode,sizeof(text_encode),text,strlen((char *)text));
            uint8_t text_decode[32]= {0};
            size_t text_decode_len=hbase64_decode(text_decode,sizeof(text_decode),text_encode,hbase64_get_encoding_length(text_encode));
            printf("text=%s,encode=%s(%d),decode=%s(%d)\r\n",(char *)text,(char *)text_encode,(int)text_encode_len,(char *)text_decode,(int)text_decode_len);
            printf("hcrypto base64 test: %s\r\n",(strcmp((char *)text,(char *)text_decode)==0)?"ok":"failed");
            printf("hcrypto base64 test6:end!\r\n");
        }
        {
            printf("hcrypto base64 test7:start!\r\n");
            uint8_t text[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
            char text_encode[128]= {0};
            size_t text_encode_len=hbase64_encode(text_encode,sizeof(text_encode),text,strlen((char *)text));
            uint8_t text_decode[128]= {0};
            size_t text_decode_len=hbase64_decode(text_decode,sizeof(text_decode),text_encode,hbase64_get_encoding_length(text_encode));
            printf("text=%s,encode=%s(%d),decode=%s(%d)\r\n",(char *)text,(char *)text_encode,(int)text_encode_len,(char *)text_decode,(int)text_decode_len);
            printf("hcrypto base64 test: %s\r\n",(strcmp((char *)text,(char *)text_decode)==0)?"ok":"failed");
            printf("hcrypto base64 test7:end!\r\n");
        }
    }
    {
        /*
         * 可使用 openssl asn1parse --inform pem --in emqx/client-key.pem查看pem内容
         */
        const uint8_t *key_pem=RCGetHandle("emqx/client-key.pem");
        if(key_pem!=NULL)
        {
            std::string pem((const char *)key_pem);
            auto Replace=[](std::string& strBig, const std::string& strsrc, const std::string& strdst)
            {
                std::string::size_type pos = 0;
                std::string::size_type srclen = strsrc.size();
                std::string::size_type dstlen = strdst.size();

                while ((pos = strBig.find(strsrc, pos)) != std::string::npos)
                {
                    strBig.replace(pos, srclen, strdst);
                    pos += dstlen;
                }
            };
            Replace(pem,"\r","");
            auto GetLine=[](std::string &data) -> std::string
            {
                std::string::size_type pos=data.find("\n");
                std::string ret=data.substr(0,pos);
                if(pos!=std::string::npos)
                {
                    data=data.substr(pos+1);
                }
                else
                {
                    data.clear();
                }
                return ret;
            };
            std::string base64_pem;
            bool  is_begin=false;
            do
            {
                std::string Line=GetLine(pem);
                if(Line.find("END RSA PRIVATE KEY")!= std::string::npos)
                {
                    is_begin=false;
                }
                if(is_begin)
                {
                    base64_pem+=Line;
                }
                if(Line.find("BEGIN RSA PRIVATE KEY")!= std::string::npos)
                {
                    is_begin=true;
                }

            }
            while(!pem.empty());
            uint8_t bin_pem[4096]= {0};
            size_t len=hbase64_decode(bin_pem,sizeof(bin_pem),(const char *)base64_pem.c_str(),base64_pem.length());
            printf("hcrypto asn1:input length=%d\r\n",(int)len);
            /*
             * RSAPrivateKey (PKCS#1)
             *
             *  RSAPrivateKey ::= SEQUENCE {
             *      version           Version,
             *      modulus           INTEGER,  -- n
             *      publicExponent    INTEGER,  -- e
             *      privateExponent   INTEGER,  -- d
             *      prime1            INTEGER,  -- p
             *      prime2            INTEGER,  -- q
             *      exponent1         INTEGER,  -- d mod (p-1)
             *      exponent2         INTEGER,  -- d mod (q-1)
             *      coefficient       INTEGER,  -- (inverse of q) mod p
             *      otherPrimeInfos   OtherPrimeInfos OPTIONAL
             *  }
             */
            hasn1_ber_parse([](void *usr,size_t depth,size_t index,const hasn1_ber_type_t *type,const hasn1_ber_value_t *value)
            {
                printf("hcrypto asn1:depth=%d,index=%d,class=%02X,p_c=%02X,tag=%d,value length=%d\r\n",(int)depth,(int)index,(int)hasn1_ber_type_class_get(type),(int)hasn1_ber_type_p_c_get(type),(int)hasn1_ber_type_tag_get(type),(int)value->length);
                if(hasn1_ber_type_p_c_get(type)==HASN1_BER_TYPE_PRIMITIVE)
                {
                    printf("hcrypto asn1:");
                    for(size_t i=0; i<value->length; i++)
                    {
                        printf("%02X",(int)value->value[i]);
                    }
                    printf("\r\n");

                }
            },NULL,0,bin_pem,len);
        }
    }
    {
        uint8_t key[16];
        {
            printf("hcrypto aes key(aes%d):",(int)sizeof(key)*8);
            for(size_t i=0; i<sizeof(key); i++)
            {
                key[i]=rand()%0x100;
                printf("%02X",(int)key[i]);
            }
            printf("\r\n");
        }
        uint8_t data[40];
        {
            printf("hcrypto aes data(aes%d):",(int)sizeof(key)*8);
            for(size_t i=0; i<sizeof(data); i++)
            {
                data[i]=rand()%0x100;
                printf("%02X",(int)data[i]);
            }
            printf("\r\n");
        }
        uint8_t data_encrypt[sizeof(data)+HAES_BLOCK_SIZE]= {0};
        {
            haes_key_t aes_key;
            haes_set_encrypt_key(key,sizeof(key)*8,&aes_key);
            uint8_t inv[HAES_BLOCK_SIZE]= {0};
            haes_cbc_encrypt(data,data_encrypt,sizeof(data),&aes_key,inv,true);
        }
        {
            printf("hcrypto aes data(encrypt):");
            for(size_t i=0; i<sizeof(data_encrypt); i++)
            {
                printf("%02X",(int)data_encrypt[i]);
            }
            printf("\r\n");
        }
        uint8_t data_decrypt[sizeof(data)+HAES_BLOCK_SIZE]= {0};
        {
            haes_key_t aes_key;
            haes_set_decrypt_key(key,sizeof(key)*8,&aes_key);
            uint8_t inv[HAES_BLOCK_SIZE]= {0};
            haes_cbc_encrypt(data_encrypt,data_decrypt,sizeof(data),&aes_key,inv,false);
        }
        {
            printf("hcrypto aes data(decrypt):");
            for(size_t i=0; i<sizeof(data_decrypt); i++)
            {
                printf("%02X",(int)data_decrypt[i]);
            }
            printf("\r\n");
        }
        printf("hcrypto aes%d %s\r\n",(int)sizeof(key)*8,(memcmp(data,data_decrypt,sizeof(data))==0)?"ok":"failed");
    }


    {
        uint8_t key[24];
        {
            printf("hcrypto aes key(aes%d):",(int)sizeof(key)*8);
            for(size_t i=0; i<sizeof(key); i++)
            {
                key[i]=rand()%0x100;
                printf("%02X",(int)key[i]);
            }
            printf("\r\n");
        }
        uint8_t data[40];
        {
            printf("hcrypto aes data(aes%d):",(int)sizeof(key)*8);
            for(size_t i=0; i<sizeof(data); i++)
            {
                data[i]=rand()%0x100;
                printf("%02X",(int)data[i]);
            }
            printf("\r\n");
        }
        uint8_t data_encrypt[sizeof(data)+HAES_BLOCK_SIZE]= {0};
        {
            haes_key_t aes_key;
            haes_set_encrypt_key(key,sizeof(key)*8,&aes_key);
            uint8_t inv[HAES_BLOCK_SIZE]= {0};
            haes_cbc_encrypt(data,data_encrypt,sizeof(data),&aes_key,inv,true);
        }
        {
            printf("hcrypto aes data(encrypt):");
            for(size_t i=0; i<sizeof(data_encrypt); i++)
            {
                printf("%02X",(int)data_encrypt[i]);
            }
            printf("\r\n");
        }
        uint8_t data_decrypt[sizeof(data)+HAES_BLOCK_SIZE]= {0};
        {
            haes_key_t aes_key;
            haes_set_decrypt_key(key,sizeof(key)*8,&aes_key);
            uint8_t inv[HAES_BLOCK_SIZE]= {0};
            haes_cbc_encrypt(data_encrypt,data_decrypt,sizeof(data),&aes_key,inv,false);
        }
        {
            printf("hcrypto aes data(decrypt):");
            for(size_t i=0; i<sizeof(data_decrypt); i++)
            {
                printf("%02X",(int)data_decrypt[i]);
            }
            printf("\r\n");
        }
        printf("hcrypto aes%d %s\r\n",(int)sizeof(key)*8,(memcmp(data,data_decrypt,sizeof(data))==0)?"ok":"failed");
    }

    {
        uint8_t key[32];
        {
            printf("hcrypto aes key(aes%d):",(int)sizeof(key)*8);
            for(size_t i=0; i<sizeof(key); i++)
            {
                key[i]=rand()%0x100;
                printf("%02X",(int)key[i]);
            }
            printf("\r\n");
        }
        uint8_t data[40];
        {
            printf("hcrypto aes data(aes%d):",(int)sizeof(key)*8);
            for(size_t i=0; i<sizeof(data); i++)
            {
                data[i]=rand()%0x100;
                printf("%02X",(int)data[i]);
            }
            printf("\r\n");
        }
        uint8_t data_encrypt[sizeof(data)+HAES_BLOCK_SIZE]= {0};
        {
            haes_key_t aes_key;
            haes_set_encrypt_key(key,sizeof(key)*8,&aes_key);
            uint8_t inv[HAES_BLOCK_SIZE]= {0};
            haes_cbc_encrypt(data,data_encrypt,sizeof(data),&aes_key,inv,true);
        }
        {
            printf("hcrypto aes data(encrypt):");
            for(size_t i=0; i<sizeof(data_encrypt); i++)
            {
                printf("%02X",(int)data_encrypt[i]);
            }
            printf("\r\n");
        }
        uint8_t data_decrypt[sizeof(data)+HAES_BLOCK_SIZE]= {0};
        {
            haes_key_t aes_key;
            haes_set_decrypt_key(key,sizeof(key)*8,&aes_key);
            uint8_t inv[HAES_BLOCK_SIZE]= {0};
            haes_cbc_encrypt(data_encrypt,data_decrypt,sizeof(data),&aes_key,inv,false);
        }
        {
            printf("hcrypto aes data(decrypt):");
            for(size_t i=0; i<sizeof(data_decrypt); i++)
            {
                printf("%02X",(int)data_decrypt[i]);
            }
            printf("\r\n");
        }
        printf("hcrypto aes%d %s\r\n",(int)sizeof(key)*8,(memcmp(data,data_decrypt,sizeof(data))==0)?"ok":"failed");
    }

    {
        uint8_t key[16];
        {
            printf("hcrypto sm4 key(sm4):");
            for(size_t i=0; i<sizeof(key); i++)
            {
                key[i]=rand()%0x100;
                printf("%02X",(int)key[i]);
            }
            printf("\r\n");
        }
        uint8_t data[40];
        {
            printf("hcrypto sm4 data(sm4):");
            for(size_t i=0; i<sizeof(data); i++)
            {
                data[i]=rand()%0x100;
                printf("%02X",(int)data[i]);
            }
            printf("\r\n");
        }
        uint8_t data_encrypt[sizeof(data)+HAES_BLOCK_SIZE]= {0};
        {
            hsm4_key_t sm4_key;
            hsm4_set_encrypt_key(key,&sm4_key);
            uint8_t inv[HSM4_BLOCK_SIZE]= {0};
            hsm4_cbc_encrypt(data,data_encrypt,sizeof(data),&sm4_key,inv,true);
        }
        {
            printf("hcrypto sm4 data(encrypt):");
            for(size_t i=0; i<sizeof(data_encrypt); i++)
            {
                printf("%02X",(int)data_encrypt[i]);
            }
            printf("\r\n");
        }
        uint8_t data_decrypt[sizeof(data)+HAES_BLOCK_SIZE]= {0};
        {
            hsm4_key_t sm4_key;
            hsm4_set_decrypt_key(key,&sm4_key);
            uint8_t inv[HSM4_BLOCK_SIZE]= {0};
            hsm4_cbc_encrypt(data_encrypt,data_decrypt,sizeof(data),&sm4_key,inv,false);
        }
        {
            printf("hcrypto sm4 data(decrypt):");
            for(size_t i=0; i<sizeof(data_decrypt); i++)
            {
                printf("%02X",(int)data_decrypt[i]);
            }
            printf("\r\n");
        }
        printf("hcrypto sm4 %s\r\n",(memcmp(data,data_decrypt,sizeof(data))==0)?"ok":"failed");
    }

    {
        //RC4测试
        const char * key="1234567891234567";
        uint8_t data[12]="ABCDEFGHJ";
        uint8_t data_encrypt[sizeof(data)]= {0};
        uint8_t data_decrypt[sizeof(data)]= {0};
        printf("hcrypto rc4:key=%s,data=%s\r\n",(const char *)key,(const char *)data);
        {
            //加密
            hrc4_context_t rc4= {0};
            hrc4_set_key(&rc4,(const uint8_t *)key,strlen(key));
            hrc4_rc4(&rc4,sizeof(data),data,data_encrypt);
        }
        {
            //打印加密后的数据
            printf("hcrypto rc4:");
            for(size_t i=0; i<strlen((const char *)data); i++)
            {
                printf("%02X",(int)data_encrypt[i]);
            }
            printf("\r\n");

        }
        {
            //解密
            hrc4_context_t rc4= {0};
            hrc4_set_key(&rc4,(const uint8_t *)key,strlen(key));
            hrc4_rc4(&rc4,sizeof(data),data_encrypt,data_decrypt);
        }
        printf("hcrypto rc4 %s\r\n",(memcmp(data,data_decrypt,sizeof(data))==0)?"ok":"failed");
    }

    {
        static const uint8_t test_keys[2][32] =
        {
            {
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
            },
            {
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
            }
        };

        static const uint8_t test_nonces[2][12] =
        {
            {
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00
            },
            {
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x02
            }
        };

        static const uint32_t test_counters[2] =
        {
            0U,
            1U
        };

        static const uint8_t test_input[2][375] =
        {
            {
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
            },
            {
                0x41, 0x6e, 0x79, 0x20, 0x73, 0x75, 0x62, 0x6d,
                0x69, 0x73, 0x73, 0x69, 0x6f, 0x6e, 0x20, 0x74,
                0x6f, 0x20, 0x74, 0x68, 0x65, 0x20, 0x49, 0x45,
                0x54, 0x46, 0x20, 0x69, 0x6e, 0x74, 0x65, 0x6e,
                0x64, 0x65, 0x64, 0x20, 0x62, 0x79, 0x20, 0x74,
                0x68, 0x65, 0x20, 0x43, 0x6f, 0x6e, 0x74, 0x72,
                0x69, 0x62, 0x75, 0x74, 0x6f, 0x72, 0x20, 0x66,
                0x6f, 0x72, 0x20, 0x70, 0x75, 0x62, 0x6c, 0x69,
                0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x61,
                0x73, 0x20, 0x61, 0x6c, 0x6c, 0x20, 0x6f, 0x72,
                0x20, 0x70, 0x61, 0x72, 0x74, 0x20, 0x6f, 0x66,
                0x20, 0x61, 0x6e, 0x20, 0x49, 0x45, 0x54, 0x46,
                0x20, 0x49, 0x6e, 0x74, 0x65, 0x72, 0x6e, 0x65,
                0x74, 0x2d, 0x44, 0x72, 0x61, 0x66, 0x74, 0x20,
                0x6f, 0x72, 0x20, 0x52, 0x46, 0x43, 0x20, 0x61,
                0x6e, 0x64, 0x20, 0x61, 0x6e, 0x79, 0x20, 0x73,
                0x74, 0x61, 0x74, 0x65, 0x6d, 0x65, 0x6e, 0x74,
                0x20, 0x6d, 0x61, 0x64, 0x65, 0x20, 0x77, 0x69,
                0x74, 0x68, 0x69, 0x6e, 0x20, 0x74, 0x68, 0x65,
                0x20, 0x63, 0x6f, 0x6e, 0x74, 0x65, 0x78, 0x74,
                0x20, 0x6f, 0x66, 0x20, 0x61, 0x6e, 0x20, 0x49,
                0x45, 0x54, 0x46, 0x20, 0x61, 0x63, 0x74, 0x69,
                0x76, 0x69, 0x74, 0x79, 0x20, 0x69, 0x73, 0x20,
                0x63, 0x6f, 0x6e, 0x73, 0x69, 0x64, 0x65, 0x72,
                0x65, 0x64, 0x20, 0x61, 0x6e, 0x20, 0x22, 0x49,
                0x45, 0x54, 0x46, 0x20, 0x43, 0x6f, 0x6e, 0x74,
                0x72, 0x69, 0x62, 0x75, 0x74, 0x69, 0x6f, 0x6e,
                0x22, 0x2e, 0x20, 0x53, 0x75, 0x63, 0x68, 0x20,
                0x73, 0x74, 0x61, 0x74, 0x65, 0x6d, 0x65, 0x6e,
                0x74, 0x73, 0x20, 0x69, 0x6e, 0x63, 0x6c, 0x75,
                0x64, 0x65, 0x20, 0x6f, 0x72, 0x61, 0x6c, 0x20,
                0x73, 0x74, 0x61, 0x74, 0x65, 0x6d, 0x65, 0x6e,
                0x74, 0x73, 0x20, 0x69, 0x6e, 0x20, 0x49, 0x45,
                0x54, 0x46, 0x20, 0x73, 0x65, 0x73, 0x73, 0x69,
                0x6f, 0x6e, 0x73, 0x2c, 0x20, 0x61, 0x73, 0x20,
                0x77, 0x65, 0x6c, 0x6c, 0x20, 0x61, 0x73, 0x20,
                0x77, 0x72, 0x69, 0x74, 0x74, 0x65, 0x6e, 0x20,
                0x61, 0x6e, 0x64, 0x20, 0x65, 0x6c, 0x65, 0x63,
                0x74, 0x72, 0x6f, 0x6e, 0x69, 0x63, 0x20, 0x63,
                0x6f, 0x6d, 0x6d, 0x75, 0x6e, 0x69, 0x63, 0x61,
                0x74, 0x69, 0x6f, 0x6e, 0x73, 0x20, 0x6d, 0x61,
                0x64, 0x65, 0x20, 0x61, 0x74, 0x20, 0x61, 0x6e,
                0x79, 0x20, 0x74, 0x69, 0x6d, 0x65, 0x20, 0x6f,
                0x72, 0x20, 0x70, 0x6c, 0x61, 0x63, 0x65, 0x2c,
                0x20, 0x77, 0x68, 0x69, 0x63, 0x68, 0x20, 0x61,
                0x72, 0x65, 0x20, 0x61, 0x64, 0x64, 0x72, 0x65,
                0x73, 0x73, 0x65, 0x64, 0x20, 0x74, 0x6f
            }
        };

        static const uint8_t test_output[2][375] =
        {
            {
                0x76, 0xb8, 0xe0, 0xad, 0xa0, 0xf1, 0x3d, 0x90,
                0x40, 0x5d, 0x6a, 0xe5, 0x53, 0x86, 0xbd, 0x28,
                0xbd, 0xd2, 0x19, 0xb8, 0xa0, 0x8d, 0xed, 0x1a,
                0xa8, 0x36, 0xef, 0xcc, 0x8b, 0x77, 0x0d, 0xc7,
                0xda, 0x41, 0x59, 0x7c, 0x51, 0x57, 0x48, 0x8d,
                0x77, 0x24, 0xe0, 0x3f, 0xb8, 0xd8, 0x4a, 0x37,
                0x6a, 0x43, 0xb8, 0xf4, 0x15, 0x18, 0xa1, 0x1c,
                0xc3, 0x87, 0xb6, 0x69, 0xb2, 0xee, 0x65, 0x86
            },
            {
                0xa3, 0xfb, 0xf0, 0x7d, 0xf3, 0xfa, 0x2f, 0xde,
                0x4f, 0x37, 0x6c, 0xa2, 0x3e, 0x82, 0x73, 0x70,
                0x41, 0x60, 0x5d, 0x9f, 0x4f, 0x4f, 0x57, 0xbd,
                0x8c, 0xff, 0x2c, 0x1d, 0x4b, 0x79, 0x55, 0xec,
                0x2a, 0x97, 0x94, 0x8b, 0xd3, 0x72, 0x29, 0x15,
                0xc8, 0xf3, 0xd3, 0x37, 0xf7, 0xd3, 0x70, 0x05,
                0x0e, 0x9e, 0x96, 0xd6, 0x47, 0xb7, 0xc3, 0x9f,
                0x56, 0xe0, 0x31, 0xca, 0x5e, 0xb6, 0x25, 0x0d,
                0x40, 0x42, 0xe0, 0x27, 0x85, 0xec, 0xec, 0xfa,
                0x4b, 0x4b, 0xb5, 0xe8, 0xea, 0xd0, 0x44, 0x0e,
                0x20, 0xb6, 0xe8, 0xdb, 0x09, 0xd8, 0x81, 0xa7,
                0xc6, 0x13, 0x2f, 0x42, 0x0e, 0x52, 0x79, 0x50,
                0x42, 0xbd, 0xfa, 0x77, 0x73, 0xd8, 0xa9, 0x05,
                0x14, 0x47, 0xb3, 0x29, 0x1c, 0xe1, 0x41, 0x1c,
                0x68, 0x04, 0x65, 0x55, 0x2a, 0xa6, 0xc4, 0x05,
                0xb7, 0x76, 0x4d, 0x5e, 0x87, 0xbe, 0xa8, 0x5a,
                0xd0, 0x0f, 0x84, 0x49, 0xed, 0x8f, 0x72, 0xd0,
                0xd6, 0x62, 0xab, 0x05, 0x26, 0x91, 0xca, 0x66,
                0x42, 0x4b, 0xc8, 0x6d, 0x2d, 0xf8, 0x0e, 0xa4,
                0x1f, 0x43, 0xab, 0xf9, 0x37, 0xd3, 0x25, 0x9d,
                0xc4, 0xb2, 0xd0, 0xdf, 0xb4, 0x8a, 0x6c, 0x91,
                0x39, 0xdd, 0xd7, 0xf7, 0x69, 0x66, 0xe9, 0x28,
                0xe6, 0x35, 0x55, 0x3b, 0xa7, 0x6c, 0x5c, 0x87,
                0x9d, 0x7b, 0x35, 0xd4, 0x9e, 0xb2, 0xe6, 0x2b,
                0x08, 0x71, 0xcd, 0xac, 0x63, 0x89, 0x39, 0xe2,
                0x5e, 0x8a, 0x1e, 0x0e, 0xf9, 0xd5, 0x28, 0x0f,
                0xa8, 0xca, 0x32, 0x8b, 0x35, 0x1c, 0x3c, 0x76,
                0x59, 0x89, 0xcb, 0xcf, 0x3d, 0xaa, 0x8b, 0x6c,
                0xcc, 0x3a, 0xaf, 0x9f, 0x39, 0x79, 0xc9, 0x2b,
                0x37, 0x20, 0xfc, 0x88, 0xdc, 0x95, 0xed, 0x84,
                0xa1, 0xbe, 0x05, 0x9c, 0x64, 0x99, 0xb9, 0xfd,
                0xa2, 0x36, 0xe7, 0xe8, 0x18, 0xb0, 0x4b, 0x0b,
                0xc3, 0x9c, 0x1e, 0x87, 0x6b, 0x19, 0x3b, 0xfe,
                0x55, 0x69, 0x75, 0x3f, 0x88, 0x12, 0x8c, 0xc0,
                0x8a, 0xaa, 0x9b, 0x63, 0xd1, 0xa1, 0x6f, 0x80,
                0xef, 0x25, 0x54, 0xd7, 0x18, 0x9c, 0x41, 0x1f,
                0x58, 0x69, 0xca, 0x52, 0xc5, 0xb8, 0x3f, 0xa3,
                0x6f, 0xf2, 0x16, 0xb9, 0xc1, 0xd3, 0x00, 0x62,
                0xbe, 0xbc, 0xfd, 0x2d, 0xc5, 0xbc, 0xe0, 0x91,
                0x19, 0x34, 0xfd, 0xa7, 0x9a, 0x86, 0xf6, 0xe6,
                0x98, 0xce, 0xd7, 0x59, 0xc3, 0xff, 0x9b, 0x64,
                0x77, 0x33, 0x8f, 0x3d, 0xa4, 0xf9, 0xcd, 0x85,
                0x14, 0xea, 0x99, 0x82, 0xcc, 0xaf, 0xb3, 0x41,
                0xb2, 0x38, 0x4d, 0xd9, 0x02, 0xf3, 0xd1, 0xab,
                0x7a, 0xc6, 0x1d, 0xd2, 0x9c, 0x6f, 0x21, 0xba,
                0x5b, 0x86, 0x2f, 0x37, 0x30, 0xe3, 0x7c, 0xfd,
                0xc4, 0xfd, 0x80, 0x6c, 0x22, 0xf2, 0x21
            }
        };

        static const size_t test_lengths[2] =
        {
            64U,
            375U
        };

        for(size_t i=0; i<sizeof(test_keys)/sizeof(test_keys[0]); i++)
        {
            printf("hcrypto chacha20: index=%d\r\n",(int)i);
            uint8_t encrypt[512]= {0};
            uint8_t decrypt[512]= {0};
            hchacha20_crypt(test_keys[i],test_nonces[i],test_counters[i],test_lengths[i],test_input[i],encrypt);
            hchacha20_crypt(test_keys[i],test_nonces[i],test_counters[i],test_lengths[i],encrypt,decrypt);
            printf("hcrypto chacha20: encrypt %s decrypt %s\r\n",memcmp(test_output[i],encrypt,test_lengths[i])==0?"ok":"failed",memcmp(test_input[i],decrypt,test_lengths[i])==0?"ok":"failed");
        }
    }


    {
        //BCC校验测试
        uint8_t data[]="1234567890ABC";
        printf("hcrypto bcc:data=%s,bcc=%02X,check %s\r\n",(char *)data,hbcc_calculate(data,sizeof(data)),hbcc_check(data,sizeof(data),0x41)?"ok":"failed");
    }

    {
        //LRC校验测试
        uint8_t data[]="1234567890ABC";
        printf("hcrypto lrc:data=%s,lrc=%02X,check %s\r\n",(char *)data,hlrc_calculate(data,sizeof(data)),hlrc_check(data,sizeof(data),0x2D)?"ok":"failed");
    }

    {

        uint8_t data[]="1234567890ABC";
        //CRC8校验测试(不含末尾\0字符)
        printf("hcrypto crc8:data=%s,crc8=%02X,check %s\r\n",(char *)data,hcrc_crc8_calculate(NULL,data,sizeof(data)-1),hcrc_crc8_check(NULL,data,sizeof(data)-1,0xA0)?"ok":"failed");
        printf("hcrypto crc8/itu:data=%s,crc8=%02X,check %s\r\n",(char *)data,hcrc_crc8_calculate(&hcrc_crc8_itu,data,sizeof(data)-1),hcrc_crc8_check(&hcrc_crc8_itu,data,sizeof(data)-1,0xF5)?"ok":"failed");
        printf("hcrypto crc8/rohc:data=%s,crc8=%02X,check %s\r\n",(char *)data,hcrc_crc8_calculate(&hcrc_crc8_rohc,data,sizeof(data)-1),hcrc_crc8_check(&hcrc_crc8_rohc,data,sizeof(data)-1,0x71)?"ok":"failed");
        printf("hcrypto crc8/maxim:data=%s,crc8=%02X,check %s\r\n",(char *)data,hcrc_crc8_calculate(&hcrc_crc8_maxim,data,sizeof(data)-1),hcrc_crc8_check(&hcrc_crc8_maxim,data,sizeof(data)-1,0x5A)?"ok":"failed");
        //CRC16校验测试(不含末尾\0字符)
        printf("hcrypto crc16/ibm:data=%s,crc=%04X,check %s\r\n",(char *)data,hcrc_crc16_calculate(&hcrc_crc16_ibm,data,sizeof(data)-1),hcrc_crc16_check(&hcrc_crc16_ibm,data,sizeof(data)-1,0xCC53)?"ok":"failed");
        printf("hcrypto crc16/maxim:data=%s,crc=%04X,check %s\r\n",(char *)data,hcrc_crc16_calculate(&hcrc_crc16_maxim,data,sizeof(data)-1),hcrc_crc16_check(&hcrc_crc16_maxim,data,sizeof(data)-1,0x33AC)?"ok":"failed");
        printf("hcrypto crc16/usb:data=%s,crc=%04X,check %s\r\n",(char *)data,hcrc_crc16_calculate(&hcrc_crc16_usb,data,sizeof(data)-1),hcrc_crc16_check(&hcrc_crc16_usb,data,sizeof(data)-1,0xD8AF)?"ok":"failed");
        printf("hcrypto crc16/modbus:data=%s,crc=%04X,check %s\r\n",(char *)data,hcrc_crc16_calculate(&hcrc_crc16_modbus,data,sizeof(data)-1),hcrc_crc16_check(&hcrc_crc16_modbus,data,sizeof(data)-1,0x2750)?"ok":"failed");
        printf("hcrypto crc16/ccitt:data=%s,crc=%04X,check %s\r\n",(char *)data,hcrc_crc16_calculate(&hcrc_crc16_ccitt,data,sizeof(data)-1),hcrc_crc16_check(&hcrc_crc16_ccitt,data,sizeof(data)-1,0x98FB)?"ok":"failed");
        printf("hcrypto crc16/ccitt_flase:data=%s,crc=%04X,check %s\r\n",(char *)data,hcrc_crc16_calculate(&hcrc_crc16_ccitt_false,data,sizeof(data)-1),hcrc_crc16_check(&hcrc_crc16_ccitt_false,data,sizeof(data)-1,0x188B)?"ok":"failed");
        printf("hcrypto crc16/x25:data=%s,crc=%04X,check %s\r\n",(char *)data,hcrc_crc16_calculate(&hcrc_crc16_x25,data,sizeof(data)-1),hcrc_crc16_check(&hcrc_crc16_x25,data,sizeof(data)-1,0x5710)?"ok":"failed");
        printf("hcrypto crc16/xmodem:data=%s,crc=%04X,check %s\r\n",(char *)data,hcrc_crc16_calculate(&hcrc_crc16_xmodem,data,sizeof(data)-1),hcrc_crc16_check(&hcrc_crc16_xmodem,data,sizeof(data)-1,0x3087)?"ok":"failed");
        printf("hcrypto crc16/dnp:data=%s,crc=%04X,check %s\r\n",(char *)data,hcrc_crc16_calculate(&hcrc_crc16_dnp,data,sizeof(data)-1),hcrc_crc16_check(&hcrc_crc16_dnp,data,sizeof(data)-1,0x9779)?"ok":"failed");
        //CRC32校验测试(不含末尾\0字符)
        printf("hcrypto crc32:data=%s,crc=%08X,check %s\r\n",(char *)data,hcrc_crc32_calculate(NULL,data,sizeof(data)-1),hcrc_crc32_check(NULL,data,sizeof(data)-1,0x5F28EC9B)?"ok":"failed");
        printf("hcrypto crc32/mpeg-2:data=%s,crc=%08X,check %s\r\n",(char *)data,hcrc_crc32_calculate(&hcrc_crc32_mpeg_2,data,sizeof(data)-1),hcrc_crc32_check(&hcrc_crc32_mpeg_2,data,sizeof(data)-1,0x5B3E29FC)?"ok":"failed");
        //CRC8 查表法
        printf("hcrypto crc8(fast):data=%s,crc8=%02X,check %s\r\n",(char *)data,hcrc_crc8_fast_calculate(data,sizeof(data)-1),hcrc_crc8_fast_check(data,sizeof(data)-1,0xA0)?"ok":"failed");
        //CRC16 Modbus 查表法
        printf("hcrypto crc16/modbus(fast):data=%s,crc=%04X,check %s\r\n",(char *)data,hcrc_crc16_modbus_calculate(data,sizeof(data)-1),hcrc_crc16_modbus_check(data,sizeof(data)-1,0x2750)?"ok":"failed");
        //CRC32 查表法
        printf("hcrypto crc32(fast):data=%s,crc=%08X,check %s\r\n",(char *)data,hcrc_crc32_fast_calculate(data,sizeof(data)-1),hcrc_crc32_fast_check(data,sizeof(data)-1,0x5F28EC9B)?"ok":"failed");
    }

    {
        //MD4测试
        uint8_t md4_test_buf[7][81] =
        {
            { "" },
            { "a" },
            { "abc" },
            { "message digest" },
            { "abcdefghijklmnopqrstuvwxyz" },
            { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" },
            { "12345678901234567890123456789012345678901234567890123456789012345678901234567890" }
        };
        const size_t md4_test_buflen[sizeof(md4_test_buf)/sizeof(md4_test_buf[0])] =
        {
            0, 1, 3, 14, 26, 62, 80
        };
        const hmd4_md4_t md4_test_sum[sizeof(md4_test_buf)/sizeof(md4_test_buf[0])] =
        {
            { 0x31, 0xd6, 0xcf, 0xe0, 0xd1, 0x6a, 0xe9, 0x31, 0xb7, 0x3c, 0x59, 0xd7, 0xe0, 0xc0, 0x89, 0xc0 },
            { 0xbd, 0xe5, 0x2c, 0xb3, 0x1d, 0xe3, 0x3e, 0x46, 0x24, 0x5e, 0x05, 0xfb, 0xdb, 0xd6, 0xfb, 0x24 },
            { 0xa4, 0x48, 0x01, 0x7a, 0xaf, 0x21, 0xd8, 0x52, 0x5f, 0xc1, 0x0a, 0xe8, 0x7a, 0xa6, 0x72, 0x9d },
            { 0xd9, 0x13, 0x0a, 0x81, 0x64, 0x54, 0x9f, 0xe8, 0x18, 0x87, 0x48, 0x06, 0xe1, 0xc7, 0x01, 0x4b },
            { 0xd7, 0x9e, 0x1c, 0x30, 0x8a, 0xa5, 0xbb, 0xcd, 0xee, 0xa8, 0xed, 0x63, 0xdf, 0x41, 0x2d, 0xa9 },
            { 0x04, 0x3f, 0x85, 0x82, 0xf2, 0x41, 0xdb, 0x35, 0x1c, 0xe6, 0x27, 0xe1, 0x53, 0xe7, 0xf0, 0xe4 },
            { 0xe3, 0x3b, 0x4d, 0xdc, 0x9c, 0x38, 0xf2, 0x19, 0x9c, 0x3e, 0x7b, 0x16, 0x4f, 0xcc, 0x05, 0x36 }
        };
        for(size_t i=0; i<(sizeof(md4_test_buf)/sizeof(md4_test_buf[0])); i++)
        {
            hmd4_md4_t md4= {0};
            hmd4_md4(md4_test_buf[i],md4_test_buflen[i],md4);
            printf("hcrypto md4:data=%s\r\n",(char *)md4_test_buf[i]);
            printf("hcrypto md4:");
            for(size_t i=0; i<sizeof(md4); i++)
            {
                printf("%02X",md4[i]);
            }
            printf("\t%s\r\n",(memcmp(md4,md4_test_sum[i],sizeof(md4))==0)?"ok":"failed");
        }
    }

    {
        //MD5测试
        uint8_t md5_test_buf[7][81] =
        {
            { "" },
            { "a" },
            { "abc" },
            { "message digest" },
            { "abcdefghijklmnopqrstuvwxyz" },
            { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" },
            { "12345678901234567890123456789012345678901234567890123456789012345678901234567890" }
        };
        const size_t md5_test_buflen[sizeof(md5_test_buf)/sizeof(md5_test_buf[0])] =
        {
            0, 1, 3, 14, 26, 62, 80
        };
        const hmd5_md5_t md5_test_sum[sizeof(md5_test_buf)/sizeof(md5_test_buf[0])] =
        {
            { 0xD4, 0x1D, 0x8C, 0xD9, 0x8F, 0x00, 0xB2, 0x04,0xE9, 0x80, 0x09, 0x98, 0xEC, 0xF8, 0x42, 0x7E },
            { 0x0C, 0xC1, 0x75, 0xB9, 0xC0, 0xF1, 0xB6, 0xA8,0x31, 0xC3, 0x99, 0xE2, 0x69, 0x77, 0x26, 0x61 },
            { 0x90, 0x01, 0x50, 0x98, 0x3C, 0xD2, 0x4F, 0xB0,0xD6, 0x96, 0x3F, 0x7D, 0x28, 0xE1, 0x7F, 0x72 },
            { 0xF9, 0x6B, 0x69, 0x7D, 0x7C, 0xB7, 0x93, 0x8D,0x52, 0x5A, 0x2F, 0x31, 0xAA, 0xF1, 0x61, 0xD0 },
            { 0xC3, 0xFC, 0xD3, 0xD7, 0x61, 0x92, 0xE4, 0x00,0x7D, 0xFB, 0x49, 0x6C, 0xCA, 0x67, 0xE1, 0x3B },
            { 0xD1, 0x74, 0xAB, 0x98, 0xD2, 0x77, 0xD9, 0xF5,0xA5, 0x61, 0x1C, 0x2C, 0x9F, 0x41, 0x9D, 0x9F },
            { 0x57, 0xED, 0xF4, 0xA2, 0x2B, 0xE3, 0xC9, 0x55,0xAC, 0x49, 0xDA, 0x2E, 0x21, 0x07, 0xB6, 0x7A }
        };
        for(size_t i=0; i<(sizeof(md5_test_buf)/sizeof(md5_test_buf[0])); i++)
        {
            hmd5_md5_t md5= {0};
            hmd5_md5(md5_test_buf[i],md5_test_buflen[i],md5);
            printf("hcrypto md5:data=%s\r\n",(char *)md5_test_buf[i]);
            printf("hcrypto md5:");
            for(size_t i=0; i<sizeof(md5); i++)
            {
                printf("%02X",md5[i]);
            }
            printf("\t%s\r\n",(memcmp(md5,md5_test_sum[i],sizeof(md5))==0)?"ok":"failed");
        }
    }

    {
        uint8_t sha1_test_buf[3][1024] =
        {
            { "abc" },
            { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq" },
            { "" } //不在此存储，单独写1000个a
        };
        static size_t sha1_test_buflen[sizeof(sha1_test_buf)/sizeof(sha1_test_buf[0])] =
        {
            3, 56, 1000
        };
        {
            for(size_t i=0; i<sha1_test_buflen[2]; i++)
            {
                sha1_test_buf[2][i]='a';
            }
        }
        const uint8_t sha1_test_sum[sizeof(sha1_test_buf)/sizeof(sha1_test_buf[0])][20] =
        {
            {
                0xA9, 0x99, 0x3E, 0x36, 0x47, 0x06, 0x81, 0x6A, 0xBA, 0x3E,0x25, 0x71, 0x78, 0x50, 0xC2, 0x6C, 0x9C, 0xD0, 0xD8, 0x9D
            },
            {
                0x84, 0x98, 0x3E, 0x44, 0x1C, 0x3B, 0xD2, 0x6E, 0xBA, 0xAE,0x4A, 0xA1, 0xF9, 0x51, 0x29, 0xE5, 0xE5, 0x46, 0x70, 0xF1
            },
            {
                0x34, 0xAA, 0x97, 0x3C, 0xD4, 0xC4, 0xDA, 0xA4, 0xF6, 0x1E,0xEB, 0x2B, 0xDB, 0xAD, 0x27, 0x31, 0x65, 0x34, 0x01, 0x6F
            }
        };

        for(size_t i=0; i<(sizeof(sha1_test_buf)/sizeof(sha1_test_buf[0])); i++)
        {
            if(i<2)
            {
                hsha1_sha_t sha1= {0};
                hsha1_sha(sha1_test_buf[i],sha1_test_buflen[i],sha1);
                printf("hcrypto sha1:data=%s\r\n",(char *)sha1_test_buf[i]);
                printf("hcrypto sha1:");
                for(size_t i=0; i<sizeof(sha1); i++)
                {
                    printf("%02X",sha1[i]);
                }
                printf("\t%s\r\n",(memcmp(sha1,sha1_test_sum[i],sizeof(sha1))==0)?"ok":"failed");
            }
            else
            {
                hsha1_context_t ctx= {0};
                hsha1_sha_t sha1= {0};
                hsha1_starts(&ctx);
                for(size_t i=0; i<1000; i++)
                {
                    hsha1_update(&ctx,sha1_test_buf[2],sha1_test_buflen[2]);
                }
                hsha1_finish(&ctx,sha1);
                printf("hcrypto sha1:data (1000000 * 'a')\r\n");
                printf("hcrypto sha1:");
                for(size_t i=0; i<sizeof(sha1); i++)
                {
                    printf("%02X",sha1[i]);
                }
                printf("\t%s\r\n",(memcmp(sha1,sha1_test_sum[i],sizeof(sha1))==0)?"ok":"failed");
            }
        }

        const hsha2_sha224_t sha224_test_sum[] =
        {
            {
                0x23, 0x09, 0x7D, 0x22, 0x34, 0x05, 0xD8, 0x22,0x86, 0x42, 0xA4, 0x77, 0xBD, 0xA2, 0x55, 0xB3, 0x2A, 0xAD, 0xBC, 0xE4, 0xBD, 0xA0, 0xB3, 0xF7, 0xE3, 0x6C, 0x9D, 0xA7
            },
            {
                0x75, 0x38, 0x8B, 0x16, 0x51, 0x27, 0x76, 0xCC,0x5D, 0xBA, 0x5D, 0xA1, 0xFD, 0x89, 0x01, 0x50, 0xB0, 0xC6, 0x45, 0x5C, 0xB4, 0xF5, 0x8B, 0x19,0x52, 0x52, 0x25, 0x25
            },
            {
                0x20, 0x79, 0x46, 0x55, 0x98, 0x0C, 0x91, 0xD8,0xBB, 0xB4, 0xC1, 0xEA, 0x97, 0x61, 0x8A, 0x4B, 0xF0, 0x3F, 0x42, 0x58, 0x19, 0x48, 0xB2, 0xEE, 0x4E, 0xE7, 0xAD, 0x67
            }
        };
        for(size_t i=0; i<(sizeof(sha1_test_buf)/sizeof(sha1_test_buf[0])); i++)
        {
            if(i<2)
            {
                hsha2_sha224_t sha2= {0};
                hsha2_sha224(sha1_test_buf[i],sha1_test_buflen[i],sha2);
                printf("hcrypto sha2 sha224:data=%s\r\n",(char *)sha1_test_buf[i]);
                printf("hcrypto sha2 sha224:");
                for(size_t i=0; i<sizeof(sha2); i++)
                {
                    printf("%02X",sha2[i]);
                }
                printf("\t%s\r\n",(memcmp(sha2,sha224_test_sum[i],sizeof(sha2))==0)?"ok":"failed");
            }
            else
            {
                hsha2_sha224_context_t ctx= {0};
                hsha2_sha224_t sha2= {0};
                hsha2_sha224_starts(&ctx);
                for(size_t i=0; i<1000; i++)
                {
                    hsha2_sha224_update(&ctx,sha1_test_buf[2],sha1_test_buflen[2]);
                }
                hsha2_sha224_finish(&ctx,sha2);
                printf("hcrypto sha2 sha224:data (1000000 * 'a')\r\n");
                printf("hcrypto sha2 sha224:");
                for(size_t i=0; i<sizeof(sha2); i++)
                {
                    printf("%02X",sha2[i]);
                }
                printf("\t%s\r\n",(memcmp(sha2,sha224_test_sum[i],sizeof(sha2))==0)?"ok":"failed");
            }
        }

        const hsha2_sha256_t sha256_test_sum[] =
        {
            {
                0xBA, 0x78, 0x16, 0xBF, 0x8F, 0x01, 0xCF, 0xEA, 0x41, 0x41, 0x40, 0xDE, 0x5D, 0xAE, 0x22, 0x23, 0xB0, 0x03, 0x61, 0xA3, 0x96, 0x17, 0x7A, 0x9C, 0xB4, 0x10, 0xFF, 0x61, 0xF2, 0x00, 0x15, 0xAD
            },
            {
                0x24, 0x8D, 0x6A, 0x61, 0xD2, 0x06, 0x38, 0xB8, 0xE5, 0xC0, 0x26, 0x93, 0x0C, 0x3E, 0x60, 0x39, 0xA3, 0x3C, 0xE4, 0x59, 0x64, 0xFF, 0x21, 0x67, 0xF6, 0xEC, 0xED, 0xD4, 0x19, 0xDB, 0x06, 0xC1
            },
            {
                0xCD, 0xC7, 0x6E, 0x5C, 0x99, 0x14, 0xFB, 0x92, 0x81, 0xA1, 0xC7, 0xE2, 0x84, 0xD7, 0x3E, 0x67, 0xF1, 0x80, 0x9A, 0x48, 0xA4, 0x97, 0x20, 0x0E, 0x04, 0x6D, 0x39, 0xCC, 0xC7, 0x11, 0x2C, 0xD0
            }
        };

        for(size_t i=0; i<(sizeof(sha1_test_buf)/sizeof(sha1_test_buf[0])); i++)
        {
            if(i<2)
            {
                hsha2_sha256_t sha2= {0};
                hsha2_sha256(sha1_test_buf[i],sha1_test_buflen[i],sha2);
                printf("hcrypto sha2 sha256:data=%s\r\n",(char *)sha1_test_buf[i]);
                printf("hcrypto sha2 sha256:");
                for(size_t i=0; i<sizeof(sha2); i++)
                {
                    printf("%02X",sha2[i]);
                }
                printf("\t%s\r\n",(memcmp(sha2,sha256_test_sum[i],sizeof(sha2))==0)?"ok":"failed");
            }
            else
            {
                hsha2_sha256_context_t ctx= {0};
                hsha2_sha256_t sha2= {0};
                hsha2_sha256_starts(&ctx);
                for(size_t i=0; i<1000; i++)
                {
                    hsha2_sha256_update(&ctx,sha1_test_buf[2],sha1_test_buflen[2]);
                }
                hsha2_sha256_finish(&ctx,sha2);
                printf("hcrypto sha2 sha256:data (1000000 * 'a')\r\n");
                printf("hcrypto sha2 sha256:");
                for(size_t i=0; i<sizeof(sha2); i++)
                {
                    printf("%02X",sha2[i]);
                }
                printf("\t%s\r\n",(memcmp(sha2,sha256_test_sum[i],sizeof(sha2))==0)?"ok":"failed");
            }
        }

        const hsha2_sha384_t sha384_test_sum[] =
        {
            {
                0xCB, 0x00, 0x75, 0x3F, 0x45, 0xA3, 0x5E, 0x8B,
                0xB5, 0xA0, 0x3D, 0x69, 0x9A, 0xC6, 0x50, 0x07,
                0x27, 0x2C, 0x32, 0xAB, 0x0E, 0xDE, 0xD1, 0x63,
                0x1A, 0x8B, 0x60, 0x5A, 0x43, 0xFF, 0x5B, 0xED,
                0x80, 0x86, 0x07, 0x2B, 0xA1, 0xE7, 0xCC, 0x23,
                0x58, 0xBA, 0xEC, 0xA1, 0x34, 0xC8, 0x25, 0xA7
            },
            {
                0x09, 0x33, 0x0C, 0x33, 0xF7, 0x11, 0x47, 0xE8,
                0x3D, 0x19, 0x2F, 0xC7, 0x82, 0xCD, 0x1B, 0x47,
                0x53, 0x11, 0x1B, 0x17, 0x3B, 0x3B, 0x05, 0xD2,
                0x2F, 0xA0, 0x80, 0x86, 0xE3, 0xB0, 0xF7, 0x12,
                0xFC, 0xC7, 0xC7, 0x1A, 0x55, 0x7E, 0x2D, 0xB9,
                0x66, 0xC3, 0xE9, 0xFA, 0x91, 0x74, 0x60, 0x39
            },
            {
                0x9D, 0x0E, 0x18, 0x09, 0x71, 0x64, 0x74, 0xCB,
                0x08, 0x6E, 0x83, 0x4E, 0x31, 0x0A, 0x4A, 0x1C,
                0xED, 0x14, 0x9E, 0x9C, 0x00, 0xF2, 0x48, 0x52,
                0x79, 0x72, 0xCE, 0xC5, 0x70, 0x4C, 0x2A, 0x5B,
                0x07, 0xB8, 0xB3, 0xDC, 0x38, 0xEC, 0xC4, 0xEB,
                0xAE, 0x97, 0xDD, 0xD8, 0x7F, 0x3D, 0x89, 0x85
            }
        };

        //替换test_buf第2条内部的数据
        strcpy((char *)sha1_test_buf[1],"abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu");
        sha1_test_buflen[1]=112;

        for(size_t i=0; i<(sizeof(sha1_test_buf)/sizeof(sha1_test_buf[0])); i++)
        {
            if(i<2)
            {
                hsha2_sha384_t sha2= {0};
                hsha2_sha384(sha1_test_buf[i],sha1_test_buflen[i],sha2);
                printf("hcrypto sha2 sha384:data=%s\r\n",(char *)sha1_test_buf[i]);
                printf("hcrypto sha2 sha384:");
                for(size_t i=0; i<sizeof(sha2); i++)
                {
                    printf("%02X",sha2[i]);
                }
                printf("\t%s\r\n",(memcmp(sha2,sha384_test_sum[i],sizeof(sha2))==0)?"ok":"failed");
            }
            else
            {
                hsha2_sha384_context_t ctx= {0};
                hsha2_sha384_t sha2= {0};
                hsha2_sha384_starts(&ctx);
                for(size_t i=0; i<1000; i++)
                {
                    hsha2_sha384_update(&ctx,sha1_test_buf[2],sha1_test_buflen[2]);
                }
                hsha2_sha384_finish(&ctx,sha2);
                printf("hcrypto sha2 sha384:data (1000000 * 'a')\r\n");
                printf("hcrypto sha2 sha384:");
                for(size_t i=0; i<sizeof(sha2); i++)
                {
                    printf("%02X",sha2[i]);
                }
                printf("\t%s\r\n",(memcmp(sha2,sha384_test_sum[i],sizeof(sha2))==0)?"ok":"failed");
            }
        }

        const hsha2_sha512_t sha512_test_sum[] =
        {
            {
                0xDD, 0xAF, 0x35, 0xA1, 0x93, 0x61, 0x7A, 0xBA,
                0xCC, 0x41, 0x73, 0x49, 0xAE, 0x20, 0x41, 0x31,
                0x12, 0xE6, 0xFA, 0x4E, 0x89, 0xA9, 0x7E, 0xA2,
                0x0A, 0x9E, 0xEE, 0xE6, 0x4B, 0x55, 0xD3, 0x9A,
                0x21, 0x92, 0x99, 0x2A, 0x27, 0x4F, 0xC1, 0xA8,
                0x36, 0xBA, 0x3C, 0x23, 0xA3, 0xFE, 0xEB, 0xBD,
                0x45, 0x4D, 0x44, 0x23, 0x64, 0x3C, 0xE8, 0x0E,
                0x2A, 0x9A, 0xC9, 0x4F, 0xA5, 0x4C, 0xA4, 0x9F
            },
            {
                0x8E, 0x95, 0x9B, 0x75, 0xDA, 0xE3, 0x13, 0xDA,
                0x8C, 0xF4, 0xF7, 0x28, 0x14, 0xFC, 0x14, 0x3F,
                0x8F, 0x77, 0x79, 0xC6, 0xEB, 0x9F, 0x7F, 0xA1,
                0x72, 0x99, 0xAE, 0xAD, 0xB6, 0x88, 0x90, 0x18,
                0x50, 0x1D, 0x28, 0x9E, 0x49, 0x00, 0xF7, 0xE4,
                0x33, 0x1B, 0x99, 0xDE, 0xC4, 0xB5, 0x43, 0x3A,
                0xC7, 0xD3, 0x29, 0xEE, 0xB6, 0xDD, 0x26, 0x54,
                0x5E, 0x96, 0xE5, 0x5B, 0x87, 0x4B, 0xE9, 0x09
            },
            {
                0xE7, 0x18, 0x48, 0x3D, 0x0C, 0xE7, 0x69, 0x64,
                0x4E, 0x2E, 0x42, 0xC7, 0xBC, 0x15, 0xB4, 0x63,
                0x8E, 0x1F, 0x98, 0xB1, 0x3B, 0x20, 0x44, 0x28,
                0x56, 0x32, 0xA8, 0x03, 0xAF, 0xA9, 0x73, 0xEB,
                0xDE, 0x0F, 0xF2, 0x44, 0x87, 0x7E, 0xA6, 0x0A,
                0x4C, 0xB0, 0x43, 0x2C, 0xE5, 0x77, 0xC3, 0x1B,
                0xEB, 0x00, 0x9C, 0x5C, 0x2C, 0x49, 0xAA, 0x2E,
                0x4E, 0xAD, 0xB2, 0x17, 0xAD, 0x8C, 0xC0, 0x9B
            }
        };

        for(size_t i=0; i<(sizeof(sha1_test_buf)/sizeof(sha1_test_buf[0])); i++)
        {
            if(i<2)
            {
                hsha2_sha512_t sha2= {0};
                hsha2_sha512(sha1_test_buf[i],sha1_test_buflen[i],sha2);
                printf("hcrypto sha2 sha512:data=%s\r\n",(char *)sha1_test_buf[i]);
                printf("hcrypto sha2 sha512:");
                for(size_t i=0; i<sizeof(sha2); i++)
                {
                    printf("%02X",sha2[i]);
                }
                printf("\t%s\r\n",(memcmp(sha2,sha512_test_sum[i],sizeof(sha2))==0)?"ok":"failed");
            }
            else
            {
                hsha2_sha512_context_t ctx= {0};
                hsha2_sha512_t sha2= {0};
                hsha2_sha512_starts(&ctx);
                for(size_t i=0; i<1000; i++)
                {
                    hsha2_sha512_update(&ctx,sha1_test_buf[2],sha1_test_buflen[2]);
                }
                hsha2_sha512_finish(&ctx,sha2);
                printf("hcrypto sha2 sha512:data (1000000 * 'a')\r\n");
                printf("hcrypto sha2 sha512:");
                for(size_t i=0; i<sizeof(sha2); i++)
                {
                    printf("%02X",sha2[i]);
                }
                printf("\t%s\r\n",(memcmp(sha2,sha512_test_sum[i],sizeof(sha2))==0)?"ok":"failed");
            }
        }

        const char sha3_test_buf[2][32]=
        {
            "",
            "abc"
        };
        {
            const hsha3_sha224_t sha3_224_test_sum[] =
            {
                { /* "" */
                    0x6B, 0x4E, 0x03, 0x42, 0x36, 0x67, 0xDB, 0xB7,
                    0x3B, 0x6E, 0x15, 0x45, 0x4F, 0x0E, 0xB1, 0xAB,
                    0xD4, 0x59, 0x7F, 0x9A, 0x1B, 0x07, 0x8E, 0x3F,
                    0x5B, 0x5A, 0x6B, 0xC7
                },
                { /* "abc" */
                    0xE6, 0x42, 0x82, 0x4C, 0x3F, 0x8C, 0xF2, 0x4A,
                    0xD0, 0x92, 0x34, 0xEE, 0x7D, 0x3C, 0x76, 0x6F,
                    0xC9, 0xA3, 0xA5, 0x16, 0x8D, 0x0C, 0x94, 0xAD,
                    0x73, 0xB4, 0x6F, 0xDF
                }
            };

            for(size_t i=0; i<(sizeof(sha3_test_buf)/sizeof(sha3_test_buf[0])); i++)
            {
                if(i<2)
                {
                    hsha3_sha224_t sha3= {0};
                    hsha3_sha224((const uint8_t *)sha3_test_buf[i],strlen(sha3_test_buf[i]),sha3);
                    printf("hcrypto sha3 sha224:data=%s\r\n",(char *)sha3_test_buf[i]);
                    printf("hcrypto sha3 sha224:");
                    for(size_t i=0; i<sizeof(sha3); i++)
                    {
                        printf("%02X",sha3[i]);
                    }
                    printf("\t%s\r\n",(memcmp(sha3,sha3_224_test_sum[i],sizeof(sha3))==0)?"ok":"failed");
                }
            }
        }

        {
            const hsha3_sha256_t sha3_256_test_sum[] =
            {
                { /* "" */
                    0xA7, 0xFF, 0xC6, 0xF8, 0xBF, 0x1E, 0xD7, 0x66,
                    0x51, 0xC1, 0x47, 0x56, 0xA0, 0x61, 0xD6, 0x62,
                    0xF5, 0x80, 0xFF, 0x4D, 0xE4, 0x3B, 0x49, 0xFA,
                    0x82, 0xD8, 0x0A, 0x4B, 0x80, 0xF8, 0x43, 0x4A
                },
                { /* "abc" */
                    0x3A, 0x98, 0x5D, 0xA7, 0x4F, 0xE2, 0x25, 0xB2,
                    0x04, 0x5C, 0x17, 0x2D, 0x6B, 0xD3, 0x90, 0xBD,
                    0x85, 0x5F, 0x08, 0x6E, 0x3E, 0x9D, 0x52, 0x5B,
                    0x46, 0xBF, 0xE2, 0x45, 0x11, 0x43, 0x15, 0x32
                }
            };

            for(size_t i=0; i<(sizeof(sha3_test_buf)/sizeof(sha3_test_buf[0])); i++)
            {
                if(i<2)
                {
                    hsha3_sha256_t sha3= {0};
                    hsha3_sha256((const uint8_t *)sha3_test_buf[i],strlen(sha3_test_buf[i]),sha3);
                    printf("hcrypto sha3 sha256:data=%s\r\n",(char *)sha3_test_buf[i]);
                    printf("hcrypto sha3 sha256:");
                    for(size_t i=0; i<sizeof(sha3); i++)
                    {
                        printf("%02X",sha3[i]);
                    }
                    printf("\t%s\r\n",(memcmp(sha3,sha3_256_test_sum[i],sizeof(sha3))==0)?"ok":"failed");
                }
            }
        }

        {
            const hsha3_sha384_t sha3_384_test_sum[] =
            {
                { /* "" */
                    0x0C, 0x63, 0xA7, 0x5B, 0x84, 0x5E, 0x4F, 0x7D,
                    0x01, 0x10, 0x7D, 0x85, 0x2E, 0x4C, 0x24, 0x85,
                    0xC5, 0x1A, 0x50, 0xAA, 0xAA, 0x94, 0xFC, 0x61,
                    0x99, 0x5E, 0x71, 0xBB, 0xEE, 0x98, 0x3A, 0x2A,
                    0xC3, 0x71, 0x38, 0x31, 0x26, 0x4A, 0xDB, 0x47,
                    0xFB, 0x6B, 0xD1, 0xE0, 0x58, 0xD5, 0xF0, 0x04
                },
                { /* "abc" */
                    0xEC, 0x01, 0x49, 0x82, 0x88, 0x51, 0x6F, 0xC9,
                    0x26, 0x45, 0x9F, 0x58, 0xE2, 0xC6, 0xAD, 0x8D,
                    0xF9, 0xB4, 0x73, 0xCB, 0x0F, 0xC0, 0x8C, 0x25,
                    0x96, 0xDA, 0x7C, 0xF0, 0xE4, 0x9B, 0xE4, 0xB2,
                    0x98, 0xD8, 0x8C, 0xEA, 0x92, 0x7A, 0xC7, 0xF5,
                    0x39, 0xF1, 0xED, 0xF2, 0x28, 0x37, 0x6D, 0x25
                }
            };

            for(size_t i=0; i<(sizeof(sha3_test_buf)/sizeof(sha3_test_buf[0])); i++)
            {
                if(i<2)
                {
                    hsha3_sha384_t sha3= {0};
                    hsha3_sha384((const uint8_t *)sha3_test_buf[i],strlen(sha3_test_buf[i]),sha3);
                    printf("hcrypto sha3 sha384:data=%s\r\n",(char *)sha3_test_buf[i]);
                    printf("hcrypto sha3 sha384:");
                    for(size_t i=0; i<sizeof(sha3); i++)
                    {
                        printf("%02X",sha3[i]);
                    }
                    printf("\t%s\r\n",(memcmp(sha3,sha3_384_test_sum[i],sizeof(sha3))==0)?"ok":"failed");
                }
            }
        }

        {
            const hsha3_sha512_t sha3_512_test_sum[] =
            {
                { /* "" */
                    0xA6, 0x9F, 0x73, 0xCC, 0xA2, 0x3A, 0x9A, 0xC5,
                    0xC8, 0xB5, 0x67, 0xDC, 0x18, 0x5A, 0x75, 0x6E,
                    0x97, 0xC9, 0x82, 0x16, 0x4F, 0xE2, 0x58, 0x59,
                    0xE0, 0xD1, 0xDC, 0xC1, 0x47, 0x5C, 0x80, 0xA6,
                    0x15, 0xB2, 0x12, 0x3A, 0xF1, 0xF5, 0xF9, 0x4C,
                    0x11, 0xE3, 0xE9, 0x40, 0x2C, 0x3A, 0xC5, 0x58,
                    0xF5, 0x00, 0x19, 0x9D, 0x95, 0xB6, 0xD3, 0xE3,
                    0x01, 0x75, 0x85, 0x86, 0x28, 0x1D, 0xCD, 0x26
                },
                { /* "abc" */
                    0xB7, 0x51, 0x85, 0x0B, 0x1A, 0x57, 0x16, 0x8A,
                    0x56, 0x93, 0xCD, 0x92, 0x4B, 0x6B, 0x09, 0x6E,
                    0x08, 0xF6, 0x21, 0x82, 0x74, 0x44, 0xF7, 0x0D,
                    0x88, 0x4F, 0x5D, 0x02, 0x40, 0xD2, 0x71, 0x2E,
                    0x10, 0xE1, 0x16, 0xE9, 0x19, 0x2A, 0xF3, 0xC9,
                    0x1A, 0x7E, 0xC5, 0x76, 0x47, 0xE3, 0x93, 0x40,
                    0x57, 0x34, 0x0B, 0x4C, 0xF4, 0x08, 0xD5, 0xA5,
                    0x65, 0x92, 0xF8, 0x27, 0x4E, 0xEC, 0x53, 0xF0
                }
            };

            for(size_t i=0; i<(sizeof(sha3_test_buf)/sizeof(sha3_test_buf[0])); i++)
            {
                if(i<2)
                {
                    hsha3_sha512_t sha3= {0};
                    hsha3_sha512((const uint8_t *)sha3_test_buf[i],strlen(sha3_test_buf[i]),sha3);
                    printf("hcrypto sha3 sha512:data=%s\r\n",(char *)sha3_test_buf[i]);
                    printf("hcrypto sha3 sha512:");
                    for(size_t i=0; i<sizeof(sha3); i++)
                    {
                        printf("%02X",sha3[i]);
                    }
                    printf("\t%s\r\n",(memcmp(sha3,sha3_512_test_sum[i],sizeof(sha3))==0)?"ok":"failed");
                }
            }
        }

        {
            const hsha3_shake128_t sha3_test_sum[] =
            {
                { /* "" */
                    0x7f,0x9c,0x2b,0xa4,0xe8,0x8f,0x82,0x7d,0x61,0x60,0x45,0x50,0x76,0x05,0x85,0x3e
                },
                { /* "abc" */
                    0x58,0x81,0x09,0x2d,0xd8,0x18,0xbf,0x5c,0xf8,0xa3,0xdd,0xb7,0x93,0xfb,0xcb,0xa7
                }
            };

            for(size_t i=0; i<(sizeof(sha3_test_buf)/sizeof(sha3_test_buf[0])); i++)
            {
                if(i<2)
                {
                    hsha3_shake128_t sha3= {0};
                    hsha3_shake128((const uint8_t *)sha3_test_buf[i],strlen(sha3_test_buf[i]),sha3,sizeof(sha3));
                    printf("hcrypto sha3 shake128:data=%s\r\n",(char *)sha3_test_buf[i]);
                    printf("hcrypto sha3 shake128:");
                    for(size_t i=0; i<sizeof(sha3); i++)
                    {
                        printf("%02X",sha3[i]);
                    }
                    printf("\t%s\r\n",(memcmp(sha3,sha3_test_sum[i],sizeof(sha3))==0)?"ok":"failed");
                    uint8_t shake[2*sizeof(sha3)]= {0};
                    hsha3_shake128((const uint8_t *)sha3_test_buf[i],strlen(sha3_test_buf[i]),shake,sizeof(shake));
                    printf("hcrypto sha3 shake128(more output):");
                    for(size_t i=0; i<sizeof(shake); i++)
                    {
                        printf("%02X",shake[i]);
                    }
                    printf("\r\n");

                }
            }
        }

        {
            const hsha3_shake256_t sha3_test_sum[] =
            {
                { /* "" */
                    0x46,0xb9,0xdd,0x2b,0x0b,0xa8,0x8d,0x13,0x23,0x3b,0x3f,0xeb,0x74,0x3e,0xeb,0x24,0x3f,0xcd,0x52,0xea,0x62,0xb8,0x1b,0x82,0xb5,0x0c,0x27,0x64,0x6e,0xd5,0x76,0x2f
                },
                { /* "abc" */
                    0x48,0x33,0x66,0x60,0x13,0x60,0xa8,0x77,0x1c,0x68,0x63,0x08,0x0c,0xc4,0x11,0x4d,0x8d,0xb4,0x45,0x30,0xf8,0xf1,0xe1,0xee,0x4f,0x94,0xea,0x37,0xe7,0x8b,0x57,0x39
                }
            };

            for(size_t i=0; i<(sizeof(sha3_test_buf)/sizeof(sha3_test_buf[0])); i++)
            {
                if(i<2)
                {
                    hsha3_shake256_t sha3= {0};
                    hsha3_shake256((const uint8_t *)sha3_test_buf[i],strlen(sha3_test_buf[i]),sha3,sizeof(sha3));
                    printf("hcrypto sha3 shake256:data=%s\r\n",(char *)sha3_test_buf[i]);
                    printf("hcrypto sha3 shake256:");
                    for(size_t i=0; i<sizeof(sha3); i++)
                    {
                        printf("%02X",sha3[i]);
                    }
                    printf("\t%s\r\n",(memcmp(sha3,sha3_test_sum[i],sizeof(sha3))==0)?"ok":"failed");
                    uint8_t shake[2*sizeof(sha3)]= {0};
                    hsha3_shake256((const uint8_t *)sha3_test_buf[i],strlen(sha3_test_buf[i]),shake,sizeof(shake));
                    printf("hcrypto sha3 shake256(more output):");
                    for(size_t i=0; i<sizeof(shake); i++)
                    {
                        printf("%02X",shake[i]);
                    }
                    printf("\r\n");
                }
            }
        }

    }

    {
        const uint8_t ripemd160_test_str[][81] =
        {
            { "" },
            { "a" },
            { "abc" },
            { "message digest" },
            { "abcdefghijklmnopqrstuvwxyz" },
            { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq" },
            { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" },
            { "12345678901234567890123456789012345678901234567890123456789012345678901234567890" },
        };

        const size_t ripemd160_test_strlen[sizeof(ripemd160_test_str)/sizeof(ripemd160_test_str[0])] =
        {
            0, 1, 3, 14, 26, 56, 62, 80
        };

        const uint8_t ripemd160_test_md[sizeof(ripemd160_test_str)/sizeof(ripemd160_test_str[0])][20] =
        {
            {
                0x9c, 0x11, 0x85, 0xa5, 0xc5, 0xe9, 0xfc, 0x54, 0x61, 0x28, 0x08, 0x97, 0x7e, 0xe8, 0xf5, 0x48, 0xb2, 0x25, 0x8d, 0x31
            },
            {
                0x0b, 0xdc, 0x9d, 0x2d, 0x25, 0x6b, 0x3e, 0xe9, 0xda, 0xae, 0x34, 0x7b, 0xe6, 0xf4, 0xdc, 0x83, 0x5a, 0x46, 0x7f, 0xfe
            },
            {
                0x8e, 0xb2, 0x08, 0xf7, 0xe0, 0x5d, 0x98, 0x7a, 0x9b, 0x04, 0x4a, 0x8e, 0x98, 0xc6, 0xb0, 0x87, 0xf1, 0x5a, 0x0b, 0xfc
            },
            {
                0x5d, 0x06, 0x89, 0xef, 0x49, 0xd2, 0xfa, 0xe5, 0x72, 0xb8, 0x81, 0xb1, 0x23, 0xa8, 0x5f, 0xfa, 0x21, 0x59, 0x5f, 0x36
            },
            {
                0xf7, 0x1c, 0x27, 0x10, 0x9c, 0x69, 0x2c, 0x1b, 0x56, 0xbb, 0xdc, 0xeb, 0x5b, 0x9d, 0x28, 0x65, 0xb3, 0x70, 0x8d, 0xbc
            },
            {
                0x12, 0xa0, 0x53, 0x38, 0x4a, 0x9c, 0x0c, 0x88, 0xe4, 0x05, 0xa0, 0x6c, 0x27, 0xdc, 0xf4, 0x9a, 0xda, 0x62, 0xeb, 0x2b
            },
            {
                0xb0, 0xe2, 0x0b, 0x6e, 0x31, 0x16, 0x64, 0x02, 0x86, 0xed, 0x3a, 0x87, 0xa5, 0x71, 0x30, 0x79, 0xb2, 0x1f, 0x51, 0x89
            },
            {
                0x9b, 0x75, 0x2e, 0x45, 0x57, 0x3d, 0x4b, 0x39, 0xf4, 0xdb, 0xd3, 0x32, 0x3c, 0xab, 0x82, 0xbf, 0x63, 0x32, 0x6b, 0xfb
            },
        };

        for(size_t i=0; i<(sizeof(ripemd160_test_str)/sizeof(ripemd160_test_str[0])); i++)
        {
            hripemd160_ripemd160_t ripemd160= {0};
            hripemd160_ripemd160(ripemd160_test_str[i],ripemd160_test_strlen[i],ripemd160);
            printf("hcrypto ripemd160:data=%s\r\n",(char *)ripemd160_test_str[i]);
            printf("hcrypto ripemd160:");
            for(size_t i=0; i<sizeof(ripemd160); i++)
            {
                printf("%02X",ripemd160[i]);
            }
            printf("\t%s\r\n",(memcmp(ripemd160,ripemd160_test_md[i],sizeof(ripemd160))==0)?"ok":"failed");
        }
    }

    {
        const uint8_t sm3_test_str[][81] =
        {
            { "" },
            { "a" },
            { "abc" },
            { "message digest" },
            { "abcdefghijklmnopqrstuvwxyz" },
            { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq" },
            { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" },
            { "12345678901234567890123456789012345678901234567890123456789012345678901234567890" },
        };

        const size_t sm3_test_strlen[sizeof(sm3_test_str)/sizeof(sm3_test_str[0])] =
        {
            0, 1, 3, 14, 26, 56, 62, 80
        };

        const hsm3_sm3_t sm3_test_md[sizeof(sm3_test_str)/sizeof(sm3_test_str[0])] =
        {
            {
                0x1a, 0xb2, 0x1d, 0x83, 0x55, 0xcf, 0xa1, 0x7f, 0x8e, 0x61, 0x19, 0x48, 0x31, 0xe8, 0x1a, 0x8f, 0x22, 0xbe, 0xc8, 0xc7, 0x28, 0xfe, 0xfb, 0x74, 0x7e, 0xd0, 0x35, 0xeb, 0x50, 0x82, 0xaa, 0x2b
            },
            {
                0x62, 0x34, 0x76, 0xac, 0x18, 0xf6, 0x5a, 0x29, 0x09, 0xe4, 0x3c, 0x7f, 0xec, 0x61, 0xb4, 0x9c, 0x7e, 0x76, 0x4a, 0x91, 0xa1, 0x8c, 0xcb, 0x82, 0xf1, 0x91, 0x7a, 0x29, 0xc8, 0x6c, 0x5e, 0x88
            },
            {
                0x66, 0xc7, 0xf0, 0xf4, 0x62, 0xee, 0xed, 0xd9, 0xd1, 0xf2, 0xd4, 0x6b, 0xdc, 0x10, 0xe4, 0xe2, 0x41, 0x67, 0xc4, 0x87, 0x5c, 0xf2, 0xf7, 0xa2, 0x29, 0x7d, 0xa0, 0x2b, 0x8f, 0x4b, 0xa8, 0xe0
            },
            {
                0xc5, 0x22, 0xa9, 0x42, 0xe8, 0x9b, 0xd8, 0x0d, 0x97, 0xdd, 0x66, 0x6e, 0x7a, 0x55, 0x31, 0xb3, 0x61, 0x88, 0xc9, 0x81, 0x71, 0x49, 0xe9, 0xb2, 0x58, 0xdf, 0xe5, 0x1e, 0xce, 0x98, 0xed, 0x77
            },
            {
                0xb8, 0x0f, 0xe9, 0x7a, 0x4d, 0xa2, 0x4a, 0xfc, 0x27, 0x75, 0x64, 0xf6, 0x6a, 0x35, 0x9e, 0xf4, 0x40, 0x46, 0x2a, 0xd2, 0x8d, 0xcc, 0x6d, 0x63, 0xad, 0xb2, 0x4d, 0x5c, 0x20, 0xa6, 0x15, 0x95
            },
            {
                0x63, 0x9b, 0x6c, 0xc5, 0xe6, 0x4d, 0x9e, 0x37, 0xa3, 0x90, 0xb1, 0x92, 0xdf, 0x4f, 0xa1, 0xea, 0x07, 0x20, 0xab, 0x74, 0x7f, 0xf6, 0x92, 0xb9, 0xf3, 0x8c, 0x4e, 0x66, 0xad, 0x7b, 0x8c, 0x05
            },
            {
                0x29, 0x71, 0xd1, 0x0c, 0x88, 0x42, 0xb7, 0x0c, 0x97, 0x9e, 0x55, 0x06, 0x34, 0x80, 0xc5, 0x0b, 0xac, 0xff, 0xd9, 0x0e, 0x98, 0xe2, 0xe6, 0x0d, 0x25, 0x12, 0xab, 0x8a, 0xbf, 0xdf, 0xce, 0xc5
            },
            {
                0xad, 0x81, 0x80, 0x53, 0x21, 0xf3, 0xe6, 0x9d, 0x25, 0x12, 0x35, 0xbf, 0x88, 0x6a, 0x56, 0x48, 0x44, 0x87, 0x3b, 0x56, 0xdd, 0x7d, 0xde, 0x40, 0x0f, 0x05, 0x5b, 0x7d, 0xde, 0x39, 0x30, 0x7a
            },
        };

        for(size_t i=0; i<(sizeof(sm3_test_str)/sizeof(sm3_test_str[0])); i++)
        {
            hsm3_sm3_t sm3= {0};
            hsm3_sm3(sm3_test_str[i],sm3_test_strlen[i],sm3);
            printf("hcrypto sm3:data=%s\r\n",(char *)sm3_test_str[i]);
            printf("hcrypto sm3:");
            for(size_t i=0; i<sizeof(sm3); i++)
            {
                printf("%02X",sm3[i]);
            }
            printf("\t%s\r\n",(memcmp(sm3,sm3_test_md[i],sizeof(sm3))==0)?"ok":"failed");
        }
    }

    {
        const char *key="1234567890";
        const char *message="123455666";
        {
            hhmac_md4_context_t ctx;
            hhmac_md4_starts(&ctx,(const uint8_t *)key,strlen(key));
            hhmac_md4_update(&ctx,(const uint8_t *)message,strlen(message));
            hhmac_md4_finish(&ctx);
            printf("hcrypto hmac-md4:key=%s,message=%s,result=",key,message);
            for(size_t i=0; i<sizeof(ctx.hash_result); i++)
            {
                printf("%02X",ctx.hash_result[i]);
            }
            printf("\r\n");
        }
        {
            hhmac_md5_context_t ctx;
            hhmac_md5_starts(&ctx,(const uint8_t *)key,strlen(key));
            hhmac_md5_update(&ctx,(const uint8_t *)message,strlen(message));
            hhmac_md5_finish(&ctx);
            printf("hcrypto hmac-md5:key=%s,message=%s,result=",key,message);
            for(size_t i=0; i<sizeof(ctx.hash_result); i++)
            {
                printf("%02X",ctx.hash_result[i]);
            }
            printf("\r\n");
        }
        {
            hhmac_sha1_context_t ctx;
            hhmac_sha1_starts(&ctx,(const uint8_t *)key,strlen(key));
            hhmac_sha1_update(&ctx,(const uint8_t *)message,strlen(message));
            hhmac_sha1_finish(&ctx);
            printf("hcrypto hmac-sha1:key=%s,message=%s,result=",key,message);
            for(size_t i=0; i<sizeof(ctx.hash_result); i++)
            {
                printf("%02X",ctx.hash_result[i]);
            }
            printf("\r\n");
        }
        {
            hhmac_sha2_sha224_context_t ctx;
            hhmac_sha2_sha224_starts(&ctx,(const uint8_t *)key,strlen(key));
            hhmac_sha2_sha224_update(&ctx,(const uint8_t *)message,strlen(message));
            hhmac_sha2_sha224_finish(&ctx);
            printf("hcrypto hmac-sha2_sha224:key=%s,message=%s,result=",key,message);
            for(size_t i=0; i<sizeof(ctx.hash_result); i++)
            {
                printf("%02X",ctx.hash_result[i]);
            }
            printf("\r\n");
        }
        {
            hhmac_sha2_sha256_context_t ctx;
            hhmac_sha2_sha256_starts(&ctx,(const uint8_t *)key,strlen(key));
            hhmac_sha2_sha256_update(&ctx,(const uint8_t *)message,strlen(message));
            hhmac_sha2_sha256_finish(&ctx);
            printf("hcrypto hmac-sha2_sha256:key=%s,message=%s,result=",key,message);
            for(size_t i=0; i<sizeof(ctx.hash_result); i++)
            {
                printf("%02X",ctx.hash_result[i]);
            }
            printf("\r\n");
        }
        {
            hhmac_sha2_sha384_context_t ctx;
            hhmac_sha2_sha384_starts(&ctx,(const uint8_t *)key,strlen(key));
            hhmac_sha2_sha384_update(&ctx,(const uint8_t *)message,strlen(message));
            hhmac_sha2_sha384_finish(&ctx);
            printf("hcrypto hmac-sha2_sha384:key=%s,message=%s,result=",key,message);
            for(size_t i=0; i<sizeof(ctx.hash_result); i++)
            {
                printf("%02X",ctx.hash_result[i]);
            }
            printf("\r\n");
        }
        {
            hhmac_sha2_sha512_context_t ctx;
            hhmac_sha2_sha512_starts(&ctx,(const uint8_t *)key,strlen(key));
            hhmac_sha2_sha512_update(&ctx,(const uint8_t *)message,strlen(message));
            hhmac_sha2_sha512_finish(&ctx);
            printf("hcrypto hmac-sha2_sha512:key=%s,message=%s,result=",key,message);
            for(size_t i=0; i<sizeof(ctx.hash_result); i++)
            {
                printf("%02X",ctx.hash_result[i]);
            }
            printf("\r\n");
        }

        {
            hhmac_sm3_context_t ctx;
            hhmac_sm3_starts(&ctx,(const uint8_t *)key,strlen(key));
            hhmac_sm3_update(&ctx,(const uint8_t *)message,strlen(message));
            hhmac_sm3_finish(&ctx);
            printf("hcrypto hmac-sm3:key=%s,message=%s,result=",key,message);
            for(size_t i=0; i<sizeof(ctx.hash_result); i++)
            {
                printf("%02X",ctx.hash_result[i]);
            }
            printf("\r\n");
        }

        {
            hhmac_ripemd160_context_t ctx;
            hhmac_ripemd160_starts(&ctx,(const uint8_t *)key,strlen(key));
            hhmac_ripemd160_update(&ctx,(const uint8_t *)message,strlen(message));
            hhmac_ripemd160_finish(&ctx);
            printf("hcrypto hmac-ripemd160:key=%s,message=%s,result=",key,message);
            for(size_t i=0; i<sizeof(ctx.hash_result); i++)
            {
                printf("%02X",ctx.hash_result[i]);
            }
            printf("\r\n");
        }
    }

    {
        {
            const  hpoly1305_mac_t mac_result= {0xdd,0xb9,0xda,0x7d,0xdd,0x5e,0x52,0x79,0x27,0x30,0xed,0x5c,0xda,0x5f,0x90,0xa4};
            hpoly1305_key_t key= {0};
            {
                printf("hcrypto hpoly1305:key=");
                for (size_t i = 0; i < sizeof(key); i++)
                {
                    key[i] = (uint8_t)(i + 221);
                    printf("%02X",(int)key[i]);
                }
                printf("\r\n");
            }
            hpoly1305_context_t ctx;
            hpoly1305_starts(&ctx,key);
            uint8_t msg[73]= {0};
            {
                printf("hcrypto hpoly1305:msg=");
                for (size_t i = 0; i < sizeof(msg); i++)
                {
                    msg[i] = (uint8_t)(i + 121);
                    printf("%02X",(int)msg[i]);
                }
                printf("\r\n");
            }
            hpoly1305_update(&ctx,msg,sizeof(msg));
            hpoly1305_mac_t mac= {0};
            hpoly1305_finish(&ctx,mac);
            {
                printf("hcrypto hpoly1305:mac=");
                for (size_t i = 0; i < sizeof(mac); i++)
                {
                    printf("%02X",(int)mac[i]);
                }
                printf("\r\n");
            }
            printf("hcrypto hpoly1305:%s\r\n",memcmp(mac_result,mac,sizeof(mac))==0?"ok":"failed");
        }

        {
            //RFC 7539 参考的例子（Test Vector #4）
            const  hpoly1305_mac_t mac_result= {0x45,0x41,0x66,0x9a,0x7e,0xaa,0xee,0x61,0xe7,0x08,0xdc,0x7c,0xbc,0xc5,0xeb,0x62};
            hpoly1305_key_t key= {0x1c,0x92,0x40,0xa5,0xeb,0x55,0xd3,0x8a,0xf3,0x33,0x88,0x86,0x04,0xf6,0xb5,0xf0,0x47,0x39,0x17,0xc1,0x40,0x2b,0x80,0x09,0x9d,0xca,0x5c,0xbc,0x20,0x70,0x75,0xc0};
            {
                printf("hcrypto hpoly1305:key=");
                for (size_t i = 0; i < sizeof(key); i++)
                {
                    printf("%02X",(int)key[i]);
                }
                printf("\r\n");
            }
            hpoly1305_context_t ctx;
            hpoly1305_starts(&ctx,key);
            uint8_t msg[]= "'Twas brillig, and the slithy toves\nDid gyre and gimble in the wabe:\nAll mimsy were the borogoves,\nAnd the mome raths outgrabe.";
            {
                printf("hcrypto hpoly1305:msg=");
                for (size_t i = 0; i < sizeof(msg)-1; i++)
                {
                    printf("%02X",(int)msg[i]);
                }
                printf("\r\n");
            }
            hpoly1305_update(&ctx,msg,sizeof(msg)-1);
            hpoly1305_mac_t mac= {0};
            hpoly1305_finish(&ctx,mac);
            {
                printf("hcrypto hpoly1305:mac=");
                for (size_t i = 0; i < sizeof(mac); i++)
                {
                    printf("%02X",(int)mac[i]);
                }
                printf("\r\n");
            }
            printf("hcrypto hpoly1305:%s\r\n",memcmp(mac_result,mac,sizeof(mac))==0?"ok":"failed");
        }
    }

    return 0;
}

static void hcpprt_init_entry()
{
    printf("hcpprt_init!\r\n");
}
HCPPRT_INIT_EXPORT(init,hcpprt_init_entry);

