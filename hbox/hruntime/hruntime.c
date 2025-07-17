/***************************************************************
 * Name:      hruntime.c
 * Purpose:   实现hruntime接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hruntime.h"
#include "h3rdparty.h"
#include "hevent.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"

enum
{
    HRUNTIME_INTERNAL_FLAG_LOWLEVEL_INIT_DONE=0,
    HRUNTIME_INTERNAL_FLAG_INIT_DONE,
    HRUNTIME_INTERNAL_FLAG_LOOP_BEGIN,
    HRUNTIME_INTERNAL_FLAG_LOOP_END,
    HRUNTIME_INTERNAL_FLAG_END
};

static uint8_t hruntime_internal_flag[(((size_t)HRUNTIME_INTERNAL_FLAG_END)+7)/8]= {0};

static void hruntime_internal_flag_set(size_t flag)
{
    size_t flag_byte=flag/8;
    size_t flag_bit=flag%8;
    if(flag_byte < sizeof(hruntime_internal_flag))
    {
        hruntime_internal_flag[flag_byte] |= (1 << flag_bit);
    }
}

static void hruntime_internal_flag_clear(size_t flag)
{
    size_t flag_byte=flag/8;
    size_t flag_bit=flag%8;
    if(flag_byte < sizeof(hruntime_internal_flag))
    {
        hruntime_internal_flag[flag_byte] &= (~(1 << flag_bit));
    }
}

static bool hruntime_internal_flag_is_set(size_t flag)
{
    bool ret=false;
    size_t flag_byte=flag/8;
    size_t flag_bit=flag%8;
    if(flag_byte < sizeof(hruntime_internal_flag))
    {
        ret=(0!=(hruntime_internal_flag[flag_byte] & (1 << flag_bit)));
    }
    return ret;
}


void hruntime_init_lowlevel()
{
    if(hruntime_init_lowlevel_done())
    {
        return;
    }

    /*
     * 初始化第三方库
     */
    h3rdparty_init();

    //标记初始化完成
    hruntime_internal_flag_set(HRUNTIME_INTERNAL_FLAG_LOWLEVEL_INIT_DONE);
}

bool hruntime_init_lowlevel_done(void)
{
    return hruntime_internal_flag_is_set(HRUNTIME_INTERNAL_FLAG_LOWLEVEL_INIT_DONE);
}

void hruntime_init()
{
    if(!hruntime_init_lowlevel_done())
    {
        hruntime_init_lowlevel();
    }

    if(hruntime_init_done())
    {
        return;
    }

#ifdef HRUNTIME_USING_INIT_SECTION
    HRUNTIME_INIT_INVOKE();
#endif // HRUNTIME_USING_INIT_SECTION

    /*
     * 系统初始化完成
     */
    {
        heventslots_t *slots_init=heventslots_get_slots_from_table(HEVENTSLOTS_SYSTEM_SLOTS_INIT);
        if(slots_init!=NULL)
        {
            heventslots_emit_signal(slots_init,NULL);
        }
    }

    //标记初始化完成
    hruntime_internal_flag_set(HRUNTIME_INTERNAL_FLAG_INIT_DONE);

}

bool hruntime_init_done(void)
{
    return hruntime_internal_flag_is_set(HRUNTIME_INTERNAL_FLAG_INIT_DONE);
}

void hruntime_loop()
{
    if(!hruntime_internal_flag_is_set(HRUNTIME_INTERNAL_FLAG_INIT_DONE))
    {
        hruntime_init();
    }


    hruntime_internal_flag_clear(HRUNTIME_INTERNAL_FLAG_LOOP_END);
    hruntime_internal_flag_set(HRUNTIME_INTERNAL_FLAG_LOOP_BEGIN);

#ifdef HRUNTIME_USING_LOOP_SECTION
    HRUNTIME_LOOP_INVOKE();
#endif // HRUNTIME_USING_LOOP_SECTION

    /*
     * 系统循环
     */
    {
        heventslots_t *slots_loop=heventslots_get_slots_from_table(HEVENTSLOTS_SYSTEM_SLOTS_LOOP);
        if(slots_loop!=NULL)
        {
            heventslots_emit_signal(slots_loop,NULL);
        }
    }

    /*
     * 工作队列
     */
    {
        heventloop_t *loop_workqueue=heventloop_get_loop_from_table(HEVENTLOOP_SYSTEM_LOOP_WORKQUEUE);
        if(loop_workqueue!=NULL)
        {
            heventloop_process_event(loop_workqueue);
        }
    }

#if !defined(HDEFAULTS_SYSCALL_NO_IMPLEMENTATION) && !defined(HDEFAULTS_SYSCALL_NO_HGETTIMEOFDAY) && !defined(HGETTIMEOFDAY)
    /*
     * 调用一次hgettimeofday检查内部变量是否溢出
     */
    {
        hgettimeofday_timeval_t tv;
        hgettimeofday_timezone_t tz;
        hgettimeofday(&tv,&tz);
    }
#endif


    hruntime_internal_flag_clear(HRUNTIME_INTERNAL_FLAG_LOOP_BEGIN);
    hruntime_internal_flag_set(HRUNTIME_INTERNAL_FLAG_LOOP_END);

}

bool hruntime_loop_begin(void)
{
    return hruntime_internal_flag_is_set(HRUNTIME_INTERNAL_FLAG_LOOP_BEGIN);
}

bool hruntime_loop_end(void)
{
    return hruntime_internal_flag_is_set(HRUNTIME_INTERNAL_FLAG_LOOP_END);
}

void hruntime_function_array_invoke(const hruntime_function_t *array_base,size_t array_size)
{
    if(array_base==NULL || array_size == 0)
    {
        return;
    }

    size_t priority_min=0;
    size_t priority_max=0;
    for(size_t i=0; i<array_size; i++)
    {
        if(array_base[i].priority < priority_min)
        {
            priority_min=array_base[i].priority;
        }
        if(array_base[i].priority > priority_max)
        {
            priority_max=array_base[i].priority;
        }
    }

    for(size_t i=priority_min; priority_min<=priority_max;)
    {
        for(size_t j=0; j<array_size; j++)
        {
            if(array_base[j].priority==i)
            {
                if(array_base[j].entry!=NULL)
                {
                    array_base[j].entry(&array_base[j]);
                }
            }
        }

        if(i==priority_max)
        {
            break;
        }

        {
            size_t priority=priority_max;
            for(size_t j=0; j<array_size; j++)
            {
                if(array_base[j].priority > i && array_base[j].priority < priority )
                {
                    priority=array_base[j].priority;
                }
            }
            i=priority;
        }
    }

}

#if defined(HRUNTIME_SYMBOL_SCAN) || defined(HRUNTIME_NO_SYMBOL_TABLE)
#ifdef HRUNTIME_USING_SYMBOL_TABLE
#undef HRUNTIME_USING_SYMBOL_TABLE
#endif // HRUNTIME_USING_SYMBOL_TABLE
#endif // HRUNTIME_SYMBOL_SCAN

#ifdef HRUNTIME_USING_SYMBOL_TABLE

#include "symbol/hbox_symbol.c"
#include "symbol/hbox_h3rdparty_symbol.c"
#include "symbol/libc_symbol.c"

const struct
{
    const hruntime_symbol_t *   array_base;
    size_t                      array_size;
}
hruntime_symbol_array_list[]=
{
    {
        hruntime_hbox_symbol,
        sizeof(hruntime_hbox_symbol)/sizeof(hruntime_hbox_symbol[0])
    },
    {
        hruntime_hbox_h3rdparty_symbol,
        sizeof(hruntime_hbox_h3rdparty_symbol)/sizeof(hruntime_hbox_h3rdparty_symbol[0])
    },
    {
        hruntime_libc_symbol,
        sizeof(hruntime_libc_symbol)/sizeof(hruntime_libc_symbol[0])
    },
    {
        //结束
        NULL,
        0
    }
};

#endif

const hruntime_symbol_t *hruntime_symbol_find(const char *name)
{
    const hruntime_symbol_t *ret=NULL;
    if(name==NULL || strlen(name) == 0)
    {
        return ret;
    }

#ifdef HRUNTIME_USING_SYMBOL_DYNAMIC_TABLE
    if(ret==NULL)
    {
        ret=hruntime_symbol_dynamic_find(name);
    }
#endif // HRUNTIME_USING_SYMBOL_DYNAMIC_TABLE

#ifdef HRUNTIME_USING_SYMBOL_TABLE
    if(ret==NULL)
    {
        for(size_t i=0; i<sizeof(hruntime_symbol_array_list)/sizeof(hruntime_symbol_array_list[0]); i++)
        {
            const hruntime_symbol_t *   array_base=hruntime_symbol_array_list[i].array_base;
            size_t                      array_size=hruntime_symbol_array_list[i].array_size;
            if(array_base!=NULL && array_size!=0)
            {
                for(size_t i=0; i<array_size; i++)
                {
                    if(array_base[i].symbol_name!=NULL && strcmp(array_base[i].symbol_name,name)==0)
                    {
                        ret=&array_base[i];
                        break;
                    }
                }
            }
            if(ret!=NULL)
            {
                break;
            }
        }
    }
#endif

    if(ret==NULL)
    {
        const hruntime_symbol_t *   array_base=NULL;
        size_t                      array_size=0;
#ifdef HRUNTIME_USING_SYMBOL_SECTION
#if defined(HCOMPILER_ARMCC) || defined(HCOMPILER_ARMCLANG)
        {
            array_base=(hruntime_symbol_t *)&HRuntimeLoop$$Base;
            array_size=(((uintptr_t)(hruntime_symbol_t *)&HRuntimeLoop$$Limit)-((uintptr_t)(hruntime_symbol_t *)&HRuntimeLoop$$Base))/sizeof(hruntime_symbol_t);
        }
#elif  defined(HCOMPILER_GCC) || defined(HCOMPILER_CLANG)
        {
            array_base=__hruntime_symbol_start;
            array_size=(((uintptr_t)__hruntime_symbol_end)-((uintptr_t)__hruntime_symbol_start))/sizeof(hruntime_symbol_t);
        }
#endif
#endif // HRUNTIME_USING_SYMBOL_SECTION
        if(array_base!=NULL && array_size!=0)
        {
            for(size_t i=0; i<array_size; i++)
            {
                if(array_base[i].symbol_name!=NULL && strcmp(array_base[i].symbol_name,name)==0)
                {
                    ret=&array_base[i];
                    break;
                }
            }
        }

    }
    return ret;
}

static hdoublylinkedlist_head_t hruntime_symbol_dynamic_table_list_head= {0};
typedef struct
{
    hdoublylinkedlist_head_t list_head;
    const hruntime_symbol_t *table_start;
    size_t table_size;
} hruntime_symbol_dynamic_table_list_item_t;
bool hruntime_symbol_dynamic_table_register(const hruntime_symbol_t *table_start,size_t table_size)
{
    bool ret=false;
    if(table_start==NULL || table_size == 0)
    {
        return ret;
    }

    hruntime_symbol_dynamic_table_list_item_t *item=(hruntime_symbol_dynamic_table_list_item_t *)hdefaults_malloc(sizeof(hruntime_symbol_dynamic_table_list_item_t),NULL);
    if(item==NULL)
    {
        return ret;
    }

    item->table_size=table_size;
    item->table_start=table_start;

    hdefaults_mutex_lock(NULL);
    ret=true;
    if(hdoublylinkedlist_is_empty(&hruntime_symbol_dynamic_table_list_head))
    {
        hdoublylinkedlist_init(&hruntime_symbol_dynamic_table_list_head);
        //当头为空,直接添加到链表头上
        hdoublylinkedlist_add_back(&hruntime_symbol_dynamic_table_list_head,&item->list_head);
        //由于仅有一个项，应当初始化为空
        hdoublylinkedlist_init(&item->list_head);
    }
    else
    {
        //获取真正的链表头
        hdoublylinkedlist_head_t *list_head=hruntime_symbol_dynamic_table_list_head.next;
        //添加当前项到链表
        hdoublylinkedlist_add_front(list_head,&item->list_head);
    }
    hdefaults_mutex_unlock(NULL);
    if(!ret)
    {
        hdefaults_free(item,NULL);
    }
    return ret;
}

bool hruntime_symbol_dynamic_table_unregister(const hruntime_symbol_t *table_start,size_t table_size)
{
    bool ret=false;
    if(table_start==NULL || table_size == 0)
    {
        return ret;
    }

    if(hdoublylinkedlist_is_empty(&hruntime_symbol_dynamic_table_list_head))
    {
        return ret;
    }

    //获取真正的链表头
    hdoublylinkedlist_head_t *list_head=hruntime_symbol_dynamic_table_list_head.next;
    hruntime_symbol_dynamic_table_list_item_t *item=NULL;
    hdefaults_mutex_lock(NULL);
    HDOUBLYLINKEDLIST_FOREACH(list_head,list_item)
    {
        const hruntime_symbol_dynamic_table_list_item_t * temp=GET_STRUCT_PTR_BY_MEMBER_PTR(list_item,hruntime_symbol_dynamic_table_list_item_t,list_head);
        if(temp !=NULL && (temp->table_start==table_start && temp->table_size==table_size))
        {
            item=(hruntime_symbol_dynamic_table_list_item_t *)temp;
            break;
        }
    }

    if(item!=NULL)
    {
        if(list_head==&item->list_head)
        {
            //当前项是链表头
            if(hdoublylinkedlist_is_empty(list_head))
            {
                //重新恢复为无链表项的状态
                hdoublylinkedlist_init(&hruntime_symbol_dynamic_table_list_head);
            }
            else
            {
                //使用下一项作为新的链表头
                hruntime_symbol_dynamic_table_list_head.next=list_head->next;
                hruntime_symbol_dynamic_table_list_head.prev=list_head->next;
            }
        }
        hdoublylinkedlist_remove(&item->list_head);
        hdefaults_free(item,NULL);
        item=NULL;
        ret=true;
    }

    hdefaults_mutex_unlock(NULL);
    return ret;
}

const hruntime_symbol_t *hruntime_symbol_dynamic_find(const char *name)
{
    const hruntime_symbol_t * ret=NULL;
    if(name==NULL || strlen(name) == 0)
    {
        return ret;
    }

    if(hdoublylinkedlist_is_empty(&hruntime_symbol_dynamic_table_list_head))
    {
        return ret;
    }

    //获取真正的链表头
    hdoublylinkedlist_head_t *list_head=hruntime_symbol_dynamic_table_list_head.next;
    hdefaults_mutex_lock(NULL);
    HDOUBLYLINKEDLIST_FOREACH(list_head,list_item)
    {
        const hruntime_symbol_dynamic_table_list_item_t * temp=GET_STRUCT_PTR_BY_MEMBER_PTR(list_item,hruntime_symbol_dynamic_table_list_item_t,list_head);
        if(temp!=NULL && temp->table_start!=NULL && temp->table_size!=0)
        {
            for(size_t i=0; i< temp->table_size; i++)
            {
                if(temp->table_start[i].symbol_name!=NULL && strcmp(name,temp->table_start[i].symbol_name)==0)
                {
                    ret=&temp->table_start[i];
                    /*
                     * 即便已查找到符号，仍然需要继续查找，以最后一个查找到的符号为准
                     */
                }
            }
        }
    }
    hdefaults_mutex_unlock(NULL);
    return ret;
}
