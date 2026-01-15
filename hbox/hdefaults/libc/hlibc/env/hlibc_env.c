/***************************************************************
 * Name:      hlibc_env.c
 * Purpose:   实现hlibc_env接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hlibc_env.h"
#include "hdefaults.h"
#include "stdlib.h"
#include "hmemory.h"

#if defined(HDEFAULTS_OS_UNIX) || defined(HDEFAULTS_OS_WINDOWS)
#ifndef HLIBC_ENV_ITEM_COUNT
#define HLIBC_ENV_ITEM_COUNT      (4096)
#endif // HLIBC_ENV_ITEM_COUNT
#ifndef HLIBC_ENV_BLOCK_SIZE
#define HLIBC_ENV_BLOCK_SIZE      (32768)
#endif // HLIBC_ENV_BLOCK_SIZE
#endif

#ifndef HLIBC_ENV_ITEM_COUNT
#define HLIBC_ENV_ITEM_COUNT      (0)
#endif // HLIBC_ENV_ITEM_COUNT
#if      (HLIBC_ENV_ITEM_COUNT) < (4)
#undef   HLIBC_ENV_ITEM_COUNT
#define  HLIBC_ENV_ITEM_COUNT     (4)
#endif

#ifndef HLIBC_ENV_BLOCK_SIZE
#define HLIBC_ENV_BLOCK_SIZE      (0)
#endif // HLIBC_ENV_BLOCK_SIZE
#if      (HLIBC_ENV_BLOCK_SIZE) < (512)
#undef   HLIBC_ENV_BLOCK_SIZE
#define  HLIBC_ENV_BLOCK_SIZE     (512)
#endif

static char * hlibc_env_item[HLIBC_ENV_ITEM_COUNT+1]= {0};
/*
 * 用于单独存储环境变量的块
 */
static uint64_t hlibc_env_block[(HLIBC_ENV_BLOCK_SIZE+sizeof(uint64_t)-1)/sizeof(uint64_t)]= {0};
static hmemoryheap_pool_t * hlibc_env_block_heap_ptr=NULL;
static hmemoryheap_pool_t * hlibc_env_block_heap(void)
{
    if(hlibc_env_block_heap_ptr==NULL)
    {
        hlibc_env_block_heap_ptr=hmemoryheap_pool_format_with_default_lock(NULL,(uint8_t *)hlibc_env_block,sizeof(hlibc_env_block));
    }
    return hlibc_env_block_heap_ptr;
}

static char * hlibc_env_block_heap_alloc(size_t nBytes)
{
    return (char *)hmemoryheap_pool_malloc(hlibc_env_block_heap(),nBytes);
}

static void hlibc_env_block_heap_free(void *env_string)
{
    if(hmemoryheap_is_ptr_in_pool(hlibc_env_block_heap(),env_string))
    {
        hmemoryheap_pool_free(hlibc_env_block_heap(),env_string);
    }
}

void hlibc_env_init(void)
{
#ifndef HLIBC_ENV_NO_INIT

#if defined(HDEFAULTS_OS_UNIX) || defined(HDEFAULTS_LIBC_NEWLIB) ||defined(HDEFAULTS_LIBC_PICOLIBC)
    {
        /*
         * 复制原有环境变量指针，注意：应用中若有改变环境变量的行为，必须完全使用或者完全不使用hlibc_env的函数
         */
        extern char **environ;
        char ** env_strings=environ;
        size_t item_index=0;
        bool all_copy_ok=true;
        while(env_strings!=NULL && (*env_strings)!=NULL)
        {
            if(item_index < HLIBC_ENV_ITEM_COUNT)
            {
                hlibc_env_item[item_index]=(*env_strings);
                env_strings++;
                item_index++;
            }
            else
            {
                all_copy_ok=false;
            }
        }
#if  !defined(HDEFAULTS_OS_UNIX)
        if(all_copy_ok)
        {
            /*
             * 重新设置environ
             */
            environ=hlibc_env_item;
        }
#endif
    }
#endif

#endif // HLIBC_ENV_NO_INIT
}

static bool hlibc_env_name_check(const char *name)
{
    if(name==NULL)
    {
        return false;
    }
    while((*name)!='\0')
    {
        if((*name)=='=')
        {
            return false;
        }
        name++;
    }
    return true;
}

static bool  hlibc_env_name_compare(const char *name,const char *env_string)
{
    if(name==NULL || env_string==NULL)
    {
        return false;
    }
    while((*name)!='\0' && (*env_string)!='\0')
    {
        if((*name)!=(*env_string))
        {
            return false;
        }
        name++;
        env_string++;
    }
    if((*env_string)=='=')
    {
        return true;
    }
    return false;
}

char * hlibc_env_getenv(const char *name)
{
    char *env_string=NULL;
    if(hlibc_env_name_check(name))
    {
        hdefaults_mutex_lock(NULL);
        for(size_t i=0; i< HLIBC_ENV_ITEM_COUNT; i++)
        {
            if(hlibc_env_item[i]==NULL)
            {
                break;
            }
            if( hlibc_env_name_compare(name,hlibc_env_item[i]))
            {
                env_string=hlibc_env_item[i];
                break;
            }
        }
        hdefaults_mutex_unlock(NULL);
    }
    return &env_string[hstrlen(name)+1];
}

int hlibc_env_unsetenv(const char *envname)
{
    if(envname==NULL || !hlibc_env_name_check(envname))
    {
        return -1;
    }

    {
        hdefaults_mutex_lock(NULL);
        bool do_delete=false;
        for(size_t i=0; i< HLIBC_ENV_ITEM_COUNT; i++)
        {
            if(hlibc_env_item[i]==NULL)
            {
                break;
            }
            if( hlibc_env_name_compare(envname,hlibc_env_item[i]))
            {
                hlibc_env_block_heap_free(hlibc_env_item[i]);
                do_delete=true;
            }
            if(do_delete)
            {
                hlibc_env_item[i]=hlibc_env_item[i+1];
            }
        }
        hdefaults_mutex_unlock(NULL);
    }

    return 0;
}

int hlibc_env_setenv(const char *envname, const char *envval, int overwrite)
{
    if(envname==NULL || !hlibc_env_name_check(envname) || envval == NULL)
    {
        return -1;
    }
    {
        char *env_value=hlibc_env_getenv(envname);
        if(env_value!=NULL)
        {
            if(overwrite!=0)
            {
                hlibc_env_unsetenv(envname);
            }
            else
            {
                /*
                 * 视为成功
                 */
                return 0;
            }
        }
    }

    size_t env_string_len=hstrlen(envname)+1+hstrlen(envval)+1;
    char * env_string=hlibc_env_block_heap_alloc(env_string_len);
    if(env_string==NULL)
    {
        /*
         * 创建环境变量字符串失败
         */
        return -1;
    }
    env_string[env_string_len-1]='\0';
    {
        /*
         * 复制数据
         */
        char *str=env_string;
        while((*envname)!='\0')
        {
            (*str)=(*envname);
            envname++;
            str++;
        }
        (*str)='=';
        str++;
        while((*envval)!='\0')
        {
            (*str)=(*envval);
            envval++;
            str++;
        }
    }

    {
        hdefaults_mutex_lock(NULL);
        bool append_ok=false;
        for(size_t i=0; i< HLIBC_ENV_ITEM_COUNT; i++)
        {
            if(hlibc_env_item[i]==NULL)
            {
                append_ok=true;
                hlibc_env_item[i]=env_string;
                break;
            }
        }
        hdefaults_mutex_unlock(NULL);
        if(!append_ok)
        {
            /*
             * 未添加环境变量
             */
            hlibc_env_block_heap_free(env_string);
            return -1;
        }
    }


    return 0;
}
