﻿#include <inttypes.h>
#include <stdint.h>
#include <thread>
#include "hbox.h"
#include "time.h"
static int hcompiler_test(int argc,const char *argv[]);
static int hdefaults_test(int argc,const char *argv[]);
static int heventloop_test(int argc,const char *argv[]);
static int heventslots_test(int argc,const char *argv[]);
static int heventchain_test(int argc,const char *argv[]);
static int hwatchdog_test(int argc,const char *argv[]);
static int hmemoryheap_test(int argc,const char *argv[]);
static int hobject_test(int argc,const char *argv[]);
static int hringbuf_test(int argc,const char *argv[]);
static int hunicode_test(int argc,const char *argv[]);
static int hstacklesscoroutine_test(int argc,const char *argv[]);
static int hbase64_test(int argc,const char *argv[]);
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
    hunicode_test,
    hstacklesscoroutine_test,
    hbase64_test,
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

static int hunicode_test(int argc,const char *argv[])
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

static int hstacklesscoroutine_test(int argc,const char *argv[])
{
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
    return 0;
}

static int hbase64_test(int argc,const char *argv[])
{
    {
        printf("hbase64 test1:start!\r\n");
        uint8_t text[]="1";
        char text_encode[32]= {0};
        size_t text_encode_len=hbase64_encode(text_encode,sizeof(text_encode),text,strlen((char *)text));
        uint8_t text_decode[32]= {0};
        size_t text_decode_len=hbase64_decode(text_decode,sizeof(text_decode),text_encode,hbase64_get_encoding_length(text_encode));
        printf("text=%s,encode=%s(%d),decode=%s(%d)\r\n",(char *)text,(char *)text_encode,(int)text_encode_len,(char *)text_decode,(int)text_decode_len);
        printf("hbase64 test1:end!\r\n");
    }
    {
        printf("hbase64 test2:start!\r\n");
        uint8_t text[]="12";
        char text_encode[32]= {0};
        size_t text_encode_len=hbase64_encode(text_encode,sizeof(text_encode),text,strlen((char *)text));
        uint8_t text_decode[32]= {0};
        size_t text_decode_len=hbase64_decode(text_decode,sizeof(text_decode),text_encode,hbase64_get_encoding_length(text_encode));
        printf("text=%s,encode=%s(%d),decode=%s(%d)\r\n",(char *)text,(char *)text_encode,(int)text_encode_len,(char *)text_decode,(int)text_decode_len);
        printf("hbase64 test2:end!\r\n");
    }
    {
        printf("hbase64 test3:start!\r\n");
        uint8_t text[]="123";
        char text_encode[32]= {0};
        size_t text_encode_len=hbase64_encode(text_encode,sizeof(text_encode),text,strlen((char *)text));
        uint8_t text_decode[32]= {0};
        size_t text_decode_len=hbase64_decode(text_decode,sizeof(text_decode),text_encode,hbase64_get_encoding_length(text_encode));
        printf("text=%s,encode=%s(%d),decode=%s(%d)\r\n",(char *)text,(char *)text_encode,(int)text_encode_len,(char *)text_decode,(int)text_decode_len);
        printf("hbase64 test3:end!\r\n");
    }
    {
        printf("hbase64 test4:start!\r\n");
        uint8_t text[]="1234";
        char text_encode[32]= {0};
        size_t text_encode_len=hbase64_encode(text_encode,sizeof(text_encode),text,strlen((char *)text));
        uint8_t text_decode[32]= {0};
        size_t text_decode_len=hbase64_decode(text_decode,sizeof(text_decode),text_encode,hbase64_get_encoding_length(text_encode));
        printf("text=%s,encode=%s(%d),decode=%s(%d)\r\n",(char *)text,(char *)text_encode,(int)text_encode_len,(char *)text_decode,(int)text_decode_len);
        printf("hbase64 test4:end!\r\n");
    }
    {
        printf("hbase64 test5:start!\r\n");
        uint8_t text[]="12345";
        char text_encode[32]= {0};
        size_t text_encode_len=hbase64_encode(text_encode,sizeof(text_encode),text,strlen((char *)text));
        uint8_t text_decode[32]= {0};
        size_t text_decode_len=hbase64_decode(text_decode,sizeof(text_decode),text_encode,hbase64_get_encoding_length(text_encode));
        printf("text=%s,encode=%s(%d),decode=%s(%d)\r\n",(char *)text,(char *)text_encode,(int)text_encode_len,(char *)text_decode,(int)text_decode_len);
        printf("hbase64 test5:end!\r\n");
    }
    {
        printf("hbase64 test6:start!\r\n");
        uint8_t text[]="123456";
        char text_encode[32]= {0};
        size_t text_encode_len=hbase64_encode(text_encode,sizeof(text_encode),text,strlen((char *)text));
        uint8_t text_decode[32]= {0};
        size_t text_decode_len=hbase64_decode(text_decode,sizeof(text_decode),text_encode,hbase64_get_encoding_length(text_encode));
        printf("text=%s,encode=%s(%d),decode=%s(%d)\r\n",(char *)text,(char *)text_encode,(int)text_encode_len,(char *)text_decode,(int)text_decode_len);
        printf("hbase64 test6:end!\r\n");
    }
    {
        printf("hbase64 test7:start!\r\n");
        uint8_t text[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        char text_encode[128]= {0};
        size_t text_encode_len=hbase64_encode(text_encode,sizeof(text_encode),text,strlen((char *)text));
        uint8_t text_decode[128]= {0};
        size_t text_decode_len=hbase64_decode(text_decode,sizeof(text_decode),text_encode,hbase64_get_encoding_length(text_encode));
        printf("text=%s,encode=%s(%d),decode=%s(%d)\r\n",(char *)text,(char *)text_encode,(int)text_encode_len,(char *)text_decode,(int)text_decode_len);
        printf("hbase64 test7:end!\r\n");
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
    }
    return 0;
}

static int hcrypto_test(int argc,const char *argv[])
{
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
        //CRC16 Modbus 查表法
        printf("hcrypto crc16/modbus(fast):data=%s,crc=%04X,check %s\r\n",(char *)data,hcrc_crc16_modbus_calculate(data,sizeof(data)-1),hcrc_crc16_modbus_check(data,sizeof(data)-1,0x2750)?"ok":"failed");

    }

    return 0;
}

static void hcpprt_init_entry()
{
    printf("hcpprt_init!\r\n");
}
HCPPRT_INIT_EXPORT(init,hcpprt_init_entry);

