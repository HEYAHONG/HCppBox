/***************************************************************
 * Name:      heventchain.c
 * Purpose:   实现heventchain接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2023-07-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include "heventchain.h"
#include "hdefaults.h"
#include "hmemory.h"

typedef struct heventchain_hook
{
    //用户参数
    void *usr;
    //钩子函数,第一个参数为传入参数,第二个参数为用户参数
    bool (*hook)(void *,void *);
    //钩子释放,第一个参数为用户参数
    void (*onfree)(void *);
    //下一个钩子
    struct heventchain_hook *next;
    //钩子id
    uint32_t id;
    //优先级
    uint32_t priority;
} heventchain_hook_t;

struct heventchain
{
    //用户参数
    void *usr;
    hdefaults_api_table_t api;
    //钩子单向链表指针
    heventchain_hook_t *hook_start;
    //下一个钩子id
    uint32_t hook_next_id;
    //标志位
    struct
    {
        uint32_t has_internal_heap:1;
    };
};

size_t heventchain_with_internal_heap_min_size(void)
{
    return sizeof(heventchain_t)+2*sizeof(heventchain_hook_t)+64;//足够两个钩子
}

static void *heventchain_internal_heap_mem_alloc(size_t nbytes,void *usr)
{
    return hmemoryheap_pool_malloc((hmemoryheap_pool_t *)usr,nbytes);
}
static void heventchain_internal_heap_mem_free(void * ptr,void *usr)
{
    hmemoryheap_pool_free((hmemoryheap_pool_t *)usr,ptr);
}

heventchain_t * heventchain_with_internal_heap_init(void *usr,void (*mutex_lock)(void *),void (*mutex_unlock)(void *),void *mem,size_t length)
{
    if(mem==NULL || length < heventchain_with_internal_heap_min_size())
    {
        return NULL;
    }
    hmemoryheap_pool_t *pool=hmemoryheap_pool_format(usr,mutex_lock,mutex_unlock,(uint8_t *)mem,length);
    heventchain_t *chain=heventchain_new_with_memmang_and_lock(pool,heventchain_internal_heap_mem_alloc,heventchain_internal_heap_mem_free,mutex_lock,mutex_unlock);
    if(chain!=NULL)
    {
        chain->has_internal_heap=1;
    }
    return chain;
}

heventchain_t * heventchain_new_with_memmang_and_lock(void *usr,void *(*mem_alloc)(size_t,void *),void (*mem_free)(void *,void *),void (*mutex_lock)(void *),void (*mutex_unlock)(void *))
{
    struct heventchain *chain=NULL;
    if(mem_alloc!=NULL)
    {
        chain=(struct heventchain *)mem_alloc(sizeof(struct heventchain),usr);
    }
    else
    {
        chain=(struct heventchain *)hdefaults_get_api_table()->mem_alloc(sizeof(struct heventchain),usr);
    }

    if(chain==NULL)
    {
        return NULL;
    }

    memset(chain,0,sizeof(struct heventchain));

    chain->api=(*hdefaults_get_api_table());
    chain->usr=usr;
    chain->api.mem_alloc=mem_alloc;
    chain->api.mem_free=mem_free;
    chain->api.mutex_lock=mutex_lock;
    chain->api.mutex_unlock=mutex_unlock;
    chain->hook_start=NULL;
    chain->hook_next_id=1;

    return chain;
}
heventchain_t * heventchain_new_with_memmang(void *usr,void *(*mem_alloc)(size_t,void *),void (*mem_free)(void *,void *))
{
    return heventchain_new_with_memmang_and_lock(usr,mem_alloc,mem_free,hdefaults_mutex_lock,hdefaults_mutex_unlock);
}
heventchain_t * heventchain_new_with_lock(void *usr,void (*mutex_lock)(void *),void (*mutex_unlock)(void *))
{
    return heventchain_new_with_memmang_and_lock(usr,hdefaults_malloc,hdefaults_free,mutex_lock,mutex_unlock);
}
heventchain_t * heventchain_new(void *usr)
{
    return heventchain_new_with_memmang_and_lock(usr,hdefaults_malloc,hdefaults_free,hdefaults_mutex_lock,hdefaults_mutex_unlock);
}

void * heventchain_get_usr_ptr(heventchain_t *chain)
{
    if(chain==NULL)
    {
        return NULL;
    }
    if(chain->has_internal_heap)
    {
        if(chain->usr!=NULL)
        {
            return hmemoryheap_pool_get_usr_ptr((hmemoryheap_pool_t *)chain->usr);
        }
    }
    else
    {
        return chain->usr;
    }
    return NULL;
}

void heventchain_free(heventchain_t *chain)
{
    if(chain==NULL)
    {
        return;
    }

    heventchain_uninstall_all_hook(chain);

    if(chain->api.mem_free!=NULL)
    {
        chain->api.mem_free(chain,chain->usr);
    }
    else
    {
        hdefaults_get_api_table()->mem_free(chain,chain->usr);
    }
}

bool heventchain_start(heventchain_t *chain,void *parameter)
{
    bool ret=false;

    if(chain==NULL)
    {
        return ret;
    }

    //加锁
    if(chain->api.mutex_lock!=NULL)
    {
        chain->api.mutex_lock(chain->usr);
    }

    heventchain_hook_t *hook=chain->hook_start;

    while(hook!=NULL)
    {
        if(hook->hook!=NULL)
        {
            ret=hook->hook(parameter,hook->usr);
            if(ret)
            {
                //成功处理,不再向后传递
                break;
            }
        }
        hook=hook->next;
    }

    //解锁
    if(chain->api.mutex_unlock!=NULL)
    {
        chain->api.mutex_unlock(chain->usr);
    }


    return ret;
}

uint32_t heventchain_install_hook(heventchain_t *chain,uint32_t priority,void *hook_usr,bool (*onhook)(void *,void *),void (*onfree)(void *))
{
    if(chain==NULL)
    {
        return 0;
    }

    heventchain_hook_t *hook=NULL;

    if(chain->api.mem_alloc!=NULL)
    {
        hook=(heventchain_hook_t *)chain->api.mem_alloc(sizeof(heventchain_hook_t),chain->usr);
    }
    else
    {
        hook=(heventchain_hook_t *)hdefaults_get_api_table()->mem_alloc(sizeof(heventchain_hook_t),chain->usr);
    }

    if(hook==NULL)
    {
        return 0;
    }

    hook->usr=hook_usr;
    hook->hook=onhook;
    hook->onfree=onfree;
    hook->id=chain->hook_next_id++;
    hook->priority=priority;
    hook->next=NULL;

    if(hook->id==0)
    {
        hook->id=chain->hook_next_id++;
    }

    //加锁
    if(chain->api.mutex_lock!=NULL)
    {
        chain->api.mutex_lock(chain->usr);
    }

    heventchain_hook_t *chain_hook=chain->hook_start;

    if(chain_hook!=NULL)
    {
        if(hook->priority<=chain_hook->priority)
        {
            //优先级小于相等，插入链表
            hook->next=chain_hook;
            chain->hook_start=hook;
            chain_hook=NULL;
        }
    }
    else
    {
        chain->hook_start=hook;
    }

    while(chain_hook!=NULL)
    {
        if(chain_hook->next==NULL)
        {
            chain_hook->next=hook;
            break;
        }
        if(hook->priority<=chain_hook->next->priority)
        {
            //优先级小于相等，插入链表
            hook->next=chain_hook->next;
            chain_hook->next=hook;
            break;
        }
        chain_hook=chain_hook->next;
    }


    //解锁
    if(chain->api.mutex_unlock!=NULL)
    {
        chain->api.mutex_unlock(chain->usr);
    }

    return hook->id;
}

void heventchain_uninstall_hook(heventchain_t *chain,uint32_t id)
{
    if(chain==NULL)
    {
        return;
    }

    //加锁
    if(chain->api.mutex_lock!=NULL)
    {
        chain->api.mutex_lock(chain->usr);
    }

    heventchain_hook_t *chain_hook=chain->hook_start;

    if(chain_hook!=NULL && chain_hook->id==id)
    {
        heventchain_hook_t *hook=chain_hook->next;
        if(chain_hook->onfree!=NULL)
        {
            chain_hook->onfree(chain_hook->usr);
        }
        if(chain->api.mem_free!=NULL)
        {
            chain->api.mem_free(chain_hook,chain->usr);
        }
        else
        {
            hdefaults_get_api_table()->mem_free(chain_hook,chain->usr);
        }
        chain->hook_start=hook;
        chain_hook=NULL;
    }

    while(chain_hook!=NULL)
    {
        if(chain_hook->next==NULL)
        {
            break;
        }

        if(chain_hook->next->id==id)
        {
            heventchain_hook_t *hook=chain_hook->next;
            chain_hook->next=chain_hook->next->next;
            if(hook->onfree!=NULL)
            {
                hook->onfree(hook->usr);
            }
            if(chain->api.mem_free!=NULL)
            {
                chain->api.mem_free(hook,chain->usr);
            }
            else
            {
                hdefaults_get_api_table()->mem_free(hook,chain->usr);
            }
            break;
        }

        chain_hook=chain_hook->next;
    }


    //解锁
    if(chain->api.mutex_unlock!=NULL)
    {
        chain->api.mutex_unlock(chain->usr);
    }

}

void heventchain_uninstall_all_hook(heventchain_t *chain)
{
    if(chain==NULL)
    {
        return;
    }

    //加锁
    if(chain->api.mutex_lock!=NULL)
    {
        chain->api.mutex_lock(chain->usr);
    }

    heventchain_hook_t *hook=chain->hook_start;
    chain->hook_start=NULL;
    while(hook!=NULL)
    {
        heventchain_hook_t *next_hook=hook->next;
        if(hook->onfree!=NULL)
        {
            hook->onfree(hook->usr);
        }
        if(chain->api.mem_free!=NULL)
        {
            chain->api.mem_free(hook,chain->usr);
        }
        else
        {
            hdefaults_get_api_table()->mem_free(hook,chain->usr);
        }
        hook=next_hook;
    }

    chain->hook_next_id=1;

    //解锁
    if(chain->api.mutex_unlock!=NULL)
    {
        chain->api.mutex_unlock(chain->usr);
    }

}

static heventchain_t * heventchain_table[HEVENTCHAIN_SYSTEM_CHAIN_COUNT+(HEVENTCHAIN_USER_CHAIN_COUNT)]= {0};
heventchain_t *heventchain_get_chain_from_table(int id)
{
    if(id < 0 || id >= (sizeof(heventchain_table)/sizeof(heventchain_table[0])))
    {
        return NULL;
    }
    heventchain_t * ret=NULL;
    hdefaults_get_api_table()->mutex_lock(NULL);
    ret=heventchain_table[id];
    hdefaults_get_api_table()->mutex_unlock(NULL);
    return ret;
}

void heventchain_set_chain_to_table(int id,heventchain_t *chain)
{
    if(id < 0 || id >= (sizeof(heventchain_table)/sizeof(heventchain_table[0])))
    {
        return;
    }
    hdefaults_get_api_table()->mutex_lock(NULL);
    if(heventchain_table[id]!=NULL)
    {
        if(heventchain_table[id]->has_internal_heap==0)
        {
            heventchain_free(heventchain_table[id]);
        }
        heventchain_table[id]=NULL;
    }
    if(chain!=NULL)
    {
        heventchain_table[id]=chain;
    }
    else
    {
        heventchain_table[id]=heventchain_new(NULL);
    }
    hdefaults_get_api_table()->mutex_unlock(NULL);
}
