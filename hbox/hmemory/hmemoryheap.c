/***************************************************************
 * Name:      hmemoryheap.c
 * Purpose:   实现hmemoryheap接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2023-07-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include "hdefaults.h"
#include "hmemoryheap.h"

#if __STDC_VERSION__ >= 199901L
//C99,MSVC不使用此方法判断

#ifndef HAVE_UINTPTR_T
#define HAVE_UINTPTR_T 1
#endif // HAVE_UINTPTR_T

#endif // __STDC_VERSION__

#ifdef _MSC_VER
//MSVC

#ifndef HAVE_UINTPTR_T
#define HAVE_UINTPTR_T 1
#endif // HAVE_UINTPTR_T

#endif // _MSC_VER

struct hmemoryheap_pool
{
    //魔数,用于标记是否初始化过。未初始化过的内存池不可用。
    uint32_t magic;
    //总大小(表示块所占空间的总大小,不包含struct hmemoryheap_pool所占大小)
    uint32_t totalsize;
    //空闲大小,空闲大小并不能决定是否能成功分配,要看是否有足够大的空闲块
    uint32_t freesize;
    //用户参数
    void * usr;
    //锁,参数为用户参数。
    void (*mutex_lock)(void *);
    //解锁,参数为用户参数。
    void (*mutex_unlock)(void *);
    //异常回调,如OOM。
    void (*onexception)(struct hmemoryheap_pool *pool,hmemoryheap_exception_type_t excetion);
} ;

typedef struct
{
    //下一块内存块相对于hmemoryheap_pool指针的偏移字节数
    uint32_t next_block_offset:31;
    //当前块是否是空闲块。1等于空闲。
    uint32_t blockfree:1;

} hmemoryheap_pool_block_t;

/** \brief 将大小扩大为对齐大小
 *
 * \param size uint64_t 对齐前的大小
 * \return uint64_t 对齐后的大小
 *
 */
static uint64_t hmemoryheap_expand_size_to_aligned_size(uint64_t size)
{
    uint64_t ret=size/HMEMORYHEAP_ALIGNED_SIZE*HMEMORYHEAP_ALIGNED_SIZE;

    if(size%HMEMORYHEAP_ALIGNED_SIZE!=0)
    {
        ret+=HMEMORYHEAP_ALIGNED_SIZE;
    }

    return ret;
}


/** \brief 将大小缩小为对齐大小
 *
 * \param size uint64_t 对齐前的大小
 * \return uint64_t 对齐后的大小
 *
 */
static uint64_t hmemoryheap_shrink_size_to_aligned_size(uint64_t size)
{
    return size/HMEMORYHEAP_ALIGNED_SIZE*HMEMORYHEAP_ALIGNED_SIZE;
}


/** \brief 指针大小,若出现警告则应当调整此值，一般是4（32位系统）或8（64位系统）
 *
 *
 */
#ifdef HMEMORYHEAP_PTR_SIZE
#define PTR_SIZE  HMEMORYHEAP_PTR_SIZE
#else
#define PTR_SIZE 4
#endif // HMEMORYHEAP_PTR_SIZE


static uint64_t hmemoryheap_ptr_to_number(void *ptr)
{
#ifdef HAVE_UINTPTR_T
    {
        return (uintptr_t)ptr;
    }
#else
#if PTR_SIZE == 4
    {
        return (uint32_t)ptr;
    }
#else
    {
        return (uint64_t)ptr;
    }
#endif // VOID_PTR_SIZE
#endif // HAVE_UINTPTR_T
}

static void * hmemoryheap_number_to_ptr(uint64_t number)
{
#ifdef HAVE_UINTPTR_T
    {
        return (void *)(uintptr_t)number;
    }
#else
#if PTR_SIZE == 4
    {
        return (void *)(uint32_t)number;
    }
#else
    {
        return (void *)number;
    }
#endif // PTR_SIZE
#endif // HAVE_UINTPTR_T
}

/** \brief 获取hmemoryheap_pool(结构体)占用大小
 *
 * \return uint64_t hmemoryheap_pool(结构体)占用大小（字节数）
 *
 */
static uint64_t hmemoryheap_get_hmemoryheap_pool_size()
{
    return hmemoryheap_expand_size_to_aligned_size(sizeof(struct  hmemoryheap_pool));
}

/** \brief 获取hmemoryheap_pool_block_t(结构体)占用大小
 *
 * \return uint64_t hmemoryheap_pool_block_t(结构体)占用大小
 *
 */
static uint64_t hmemoryheap_get_hmemoryheap_pool_block_size()
{
    return hmemoryheap_expand_size_to_aligned_size(sizeof(hmemoryheap_pool_block_t));
}

hmemoryheap_pool_t *hmemoryheap_pool_format(void *usr,void (*mutex_lock)(void *),void (*mutex_unlock)(void *),uint8_t *pool_store,size_t pool_store_size)
{
    if(pool_store==NULL || pool_store_size == 0)
    {
        //必须指定数组及大小
        return NULL;
    }

    //起始地址
    uint64_t pool_start_addr=hmemoryheap_ptr_to_number(pool_store);
    //结束地址
    uint64_t pool_end_addr=pool_start_addr+pool_store_size;

    {
        //丢弃数组头尾的非对齐部分
        pool_start_addr=hmemoryheap_expand_size_to_aligned_size(pool_start_addr);
        pool_end_addr=hmemoryheap_shrink_size_to_aligned_size(pool_end_addr);
    }

    uint32_t pool_size=pool_end_addr-pool_start_addr;

    if((pool_end_addr <= pool_start_addr) || (pool_size < (hmemoryheap_get_hmemoryheap_pool_size()+hmemoryheap_get_hmemoryheap_pool_block_size())))
    {
        //数组太小
        return NULL;
    }

    if(pool_size > 0x80000000)
    {
        //数组太大,调整大小
        pool_size=hmemoryheap_shrink_size_to_aligned_size(0x7FFFFFFF);
    }

    pool_end_addr=pool_start_addr+pool_size;

    struct hmemoryheap_pool *pool=(struct hmemoryheap_pool *)hmemoryheap_number_to_ptr(pool_start_addr);

    if(pool->magic == HMEMORYHEAP_MAGIC_NUMBER)
    {
        //已初始化
        return pool;
    }

    pool->usr=usr;
    pool->mutex_lock=mutex_lock;
    pool->mutex_unlock=mutex_unlock;
    pool->magic=HMEMORYHEAP_MAGIC_NUMBER;
    pool->totalsize=pool_size-hmemoryheap_get_hmemoryheap_pool_size();
    pool->freesize=pool->totalsize;
    pool->onexception=NULL;

    {
        //初始化第一块内存为空闲
        uint64_t first_block_addr=pool_start_addr+hmemoryheap_get_hmemoryheap_pool_size();
        hmemoryheap_pool_block_t * block=(hmemoryheap_pool_block_t *)hmemoryheap_number_to_ptr(first_block_addr);
        block->next_block_offset=pool_size;
        block->blockfree=1;
    }

    return pool;
}

hmemoryheap_pool_t *hmemoryheap_pool_format_with_default_lock(void *usr,uint8_t *pool_store,size_t pool_store_size)
{
    return hmemoryheap_pool_format(usr,hdefaults_mutex_lock,hdefaults_mutex_unlock,pool_store,pool_store_size);
}

void * hmemoryheap_pool_get_usr_ptr(hmemoryheap_pool_t *pool)
{
    if(pool==NULL || pool->magic!=HMEMORYHEAP_MAGIC_NUMBER)
    {
        return NULL;
    }

    return pool->usr;
}

void hmemoryheap_pool_get_info(hmemoryheap_pool_t *pool,size_t *total_size,size_t *free_size)
{
    if(pool==NULL || pool->magic!=HMEMORYHEAP_MAGIC_NUMBER)
    {
        if(total_size!=NULL)
        {
            (*total_size)=0;
        }
        if(free_size!=NULL)
        {
            (*free_size)=0;
        }
        return;
    }

    if(total_size!=NULL)
    {
        (*total_size)=pool->totalsize;
    }
    if(free_size!=NULL)
    {
        (*free_size)=pool->freesize;
    }

}

void  hmemoryheap_pool_set_exception(hmemoryheap_pool_t *pool,void (*onexception)(struct hmemoryheap_pool *pool,hmemoryheap_exception_type_t excetion))
{
    if(pool!=NULL)
    {
        pool->onexception=onexception;
    }
}

void * hmemoryheap_pool_malloc(hmemoryheap_pool_t *pool,size_t nbytes)
{
    if(pool==NULL || pool->magic!=HMEMORYHEAP_MAGIC_NUMBER)
    {
        //池不存在或未初始化
        return NULL;
    }

    uint64_t pool_start_addr=hmemoryheap_ptr_to_number(pool);
    uint64_t pool_end_addr=pool_start_addr+pool->totalsize+hmemoryheap_get_hmemoryheap_pool_size();
    size_t wantedsize=hmemoryheap_expand_size_to_aligned_size(nbytes);
    if(wantedsize > pool->freesize || wantedsize == 0)
    {
        //剩余空间不够或申请空间为0
        return NULL;
    }
    hmemoryheap_pool_block_t * block;
    {
        uint64_t first_block_addr=pool_start_addr+hmemoryheap_get_hmemoryheap_pool_size();
        block=(hmemoryheap_pool_block_t *)hmemoryheap_number_to_ptr(first_block_addr);
    }

    void *ret=NULL;
    //加锁
    if(pool->mutex_lock!=NULL)
    {
        pool->mutex_lock(pool->usr);
    }

    while(true)
    {
        uint64_t block_start_addr=hmemoryheap_ptr_to_number(block);
        uint64_t next_block_start_addr=block->next_block_offset+pool_start_addr;
        if(block->blockfree==1)
        {
            //空闲块
            while(true)
            {
                //合并空闲内存块
                if(next_block_start_addr>=pool_end_addr)
                {
                    //已到池末尾
                    break;
                }
                if(next_block_start_addr < block_start_addr)
                {
                    //不可能出现的情况,下一个块必须在当前块之后
                    break;
                }
                hmemoryheap_pool_block_t * next_block=(hmemoryheap_pool_block_t *)hmemoryheap_number_to_ptr(next_block_start_addr);
                if(next_block->blockfree == 1)
                {
                    //下一个是空闲块,合并块
                    block->next_block_offset=next_block->next_block_offset;
                    next_block_start_addr=block->next_block_offset+pool_start_addr;
                }
                else
                {
                    break;
                }
            }

            if(next_block_start_addr < block_start_addr)
            {
                //不可能出现的情况,下一个块必须在当前块之后
                if(pool->onexception!=NULL)
                {
                    pool->onexception(pool,HMEMORYHEAP_EXCEPTION_POOL_BROKEN);
                }
                break;
            }

            if((next_block_start_addr > block_start_addr)&&((next_block_start_addr-block_start_addr) >= (hmemoryheap_get_hmemoryheap_pool_block_size()+wantedsize)))
            {
                //空闲空间够用
                if(((next_block_start_addr-block_start_addr) > (hmemoryheap_get_hmemoryheap_pool_block_size()*2+wantedsize)))
                {
                    block->blockfree=0;
                    ret=hmemoryheap_number_to_ptr(block_start_addr+hmemoryheap_get_hmemoryheap_pool_block_size());
                    uint64_t new_block_start_addr=block_start_addr+hmemoryheap_get_hmemoryheap_pool_block_size()+wantedsize;
                    pool->freesize-=(new_block_start_addr-block_start_addr);
                    //创建新块
                    hmemoryheap_pool_block_t * new_block=(hmemoryheap_pool_block_t *)hmemoryheap_number_to_ptr(new_block_start_addr);
                    new_block->blockfree=1;
                    new_block->next_block_offset=block->next_block_offset;
                    block->next_block_offset=(new_block_start_addr-pool_start_addr);
                    break;
                }
                else
                {
                    //剩余空间不够创建块结构体,不能创建新块
                    block->blockfree=0;
                    pool->freesize-=(next_block_start_addr-block_start_addr);
                    ret=hmemoryheap_number_to_ptr(block_start_addr+hmemoryheap_get_hmemoryheap_pool_block_size());
                    break;
                }
            }
        }
        if(next_block_start_addr>=pool_end_addr)
        {
            //已到池末尾
            break;
        }
        block=(hmemoryheap_pool_block_t *)hmemoryheap_number_to_ptr(next_block_start_addr);
    }

    //解锁
    if(pool->mutex_unlock!=NULL)
    {
        pool->mutex_unlock(pool->usr);
    }

    //清空已申请的内存块
    if(ret!=NULL)
    {
        memset(ret,0,nbytes);
    }
    else
    {
        if(pool->onexception!=NULL)
        {
            pool->onexception(pool,HMEMORYHEAP_EXCEPTION_OOM);
        }
    }

    return ret;
}

bool hmemoryheap_is_ptr_in_pool(hmemoryheap_pool_t *pool,void *ptr)
{
    if(pool==NULL || pool->magic!=HMEMORYHEAP_MAGIC_NUMBER)
    {
        //池不存在或未初始化
        return false;
    }

    uint64_t pool_start_addr=hmemoryheap_ptr_to_number(pool);
    uint64_t pool_end_addr=pool_start_addr+pool->totalsize+hmemoryheap_get_hmemoryheap_pool_size();
    uint64_t ptr_addr=hmemoryheap_ptr_to_number(ptr);
    if(ptr_addr > pool_start_addr && ptr_addr < pool_end_addr)
    {
        return true;
    }
    return false;
}


void hmemoryheap_pool_free(hmemoryheap_pool_t *pool,void *ptr)
{
    if(!hmemoryheap_is_ptr_in_pool(pool,ptr))
    {
        return;
    }

    uint64_t pool_start_addr=hmemoryheap_ptr_to_number(pool);
    uint64_t pool_end_addr=pool_start_addr+pool->totalsize+hmemoryheap_get_hmemoryheap_pool_size();
    uint64_t ptr_addr=hmemoryheap_ptr_to_number(ptr);
    hmemoryheap_pool_block_t * block;
    {
        uint64_t first_block_addr=pool_start_addr+hmemoryheap_get_hmemoryheap_pool_size();
        block=(hmemoryheap_pool_block_t *)hmemoryheap_number_to_ptr(first_block_addr);
    }

    bool is_free=false;

    //加锁
    if(pool->mutex_lock!=NULL)
    {
        pool->mutex_lock(pool->usr);
    }

    while(true)
    {
        uint64_t block_start_addr=hmemoryheap_ptr_to_number(block);
        uint64_t next_block_start_addr=block->next_block_offset+pool_start_addr;

        if(next_block_start_addr < block_start_addr)
        {
            //不可能出现的情况,下一个块必须在当前块之后
            if(pool->onexception!=NULL)
            {
                pool->onexception(pool,HMEMORYHEAP_EXCEPTION_POOL_BROKEN);
            }
            break;
        }

        if(ptr_addr == (block_start_addr+hmemoryheap_get_hmemoryheap_pool_block_size()))
        {
            //已找到指针所在的块
            block->blockfree=1;
            pool->freesize+=(next_block_start_addr-block_start_addr);
            is_free=true;
            break;
        }

        if(next_block_start_addr>=pool_end_addr)
        {
            //已到池末尾
            break;
        }
        block=(hmemoryheap_pool_block_t *)hmemoryheap_number_to_ptr(next_block_start_addr);
    }

    //解锁
    if(pool->mutex_unlock!=NULL)
    {
        pool->mutex_unlock(pool->usr);
    }

    if(!is_free)
    {
        if(pool->onexception!=NULL)
        {
            pool->onexception(pool,HMEMORYHEAP_EXCEPTION_DOUBLE_FREE);
        }
    }

}


static hmemoryheap_pool_t *default_pool=NULL;


#ifdef USING_HMEMORYHEAP
#ifndef HMEMORYHEAP_DEFAULT_POOL_SIZE
#define HMEMORYHEAP_DEFAULT_POOL_SIZE 256
#endif // HMEMORYHEAP_DEFAULT_POOL_SIZE
#endif // USING_HMEMORYHEAP

#if HMEMORYHEAP_DEFAULT_POOL_SIZE >= 256
static uint8_t pool_store[HMEMORYHEAP_DEFAULT_POOL_SIZE]= {0};
static bool g_is_defalut_pool_init=false;
static void check_default_pool()
{
    if(!g_is_defalut_pool_init)
    {
        if(default_pool==NULL)
        {
            default_pool=hmemoryheap_pool_format_with_default_lock(NULL,pool_store,sizeof(pool_store));
        }
        g_is_defalut_pool_init=true;
    }
}
#endif // HMEMORYHEAP_DEFAULT_POOL_SIZE

void hmemoryheap_set_defalut_pool(hmemoryheap_pool_t *pool)
{
    if(pool!=NULL)
    {
        if(pool->magic==HMEMORYHEAP_MAGIC_NUMBER)
        {
            default_pool=pool;
        }
    }
}

void hmemoryheap_get_defalut_pool(hmemoryheap_pool_t **pool_ptr)
{
#if HMEMORYHEAP_DEFAULT_POOL_SIZE >= 256
    check_default_pool();
#endif // HMEMORYHEAP_DEFAULT_POOL_SIZE
    if(pool_ptr!=NULL)
    {
        (*pool_ptr)=default_pool;
    }
}

void hmemoryheap_get_info(size_t *total_size,size_t *free_size)
{
#if HMEMORYHEAP_DEFAULT_POOL_SIZE >= 256
    check_default_pool();
#endif // HMEMORYHEAP_DEFAULT_POOL_SIZE
    hmemoryheap_pool_get_info(default_pool,total_size,free_size);
}

bool hmemoryheap_is_ptr_in_default_pool(void *ptr)
{
#if HMEMORYHEAP_DEFAULT_POOL_SIZE >= 256
    check_default_pool();
#endif // HMEMORYHEAP_DEFAULT_POOL_SIZE
    if(default_pool!=NULL && ptr!=NULL)
    {
        return hmemoryheap_is_ptr_in_pool(default_pool,ptr);
    }
    return false;
}

void *hmemoryheap_malloc(size_t nbytes)
{
#if HMEMORYHEAP_DEFAULT_POOL_SIZE >= 256
    check_default_pool();
#endif // HMEMORYHEAP_DEFAULT_POOL_SIZE
    if(default_pool!=NULL)
    {
        return hmemoryheap_pool_malloc(default_pool,nbytes);
    }
    return NULL;
}

void hmemoryheap_free(void *ptr)
{
#if HMEMORYHEAP_DEFAULT_POOL_SIZE >= 256
    check_default_pool();
#endif // HMEMORYHEAP_DEFAULT_POOL_SIZE
    if(default_pool!=NULL)
    {
        hmemoryheap_pool_free(default_pool,ptr);
    }
}
