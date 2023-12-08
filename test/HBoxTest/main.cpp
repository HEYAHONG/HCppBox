
#include <inttypes.h>
#include <stdint.h>
#include "hcompiler.h"
#include "hdefaults.h"
#include "heventloop.h"
#include "heventslots.h"
#include "heventchain.h"
#include "hwatchdog.h"
#include "hmemoryheap.h"
#include "hobject.h"

static int hcompiler_test(int argc,const char *argv[]);
static int hdefaults_test(int argc,const char *argv[]);
static int heventloop_test(int argc,const char *argv[]);
static int heventslots_test(int argc,const char *argv[]);
static int heventchain_test(int argc,const char *argv[]);
static int hwatchdog_test(int argc,const char *argv[]);
static int hmemoryheap_test(int argc,const char *argv[]);
static int hobject_test(int argc,const char *argv[]);

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
};

int main(int argc,const char *argv[])
{
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


static int hcompiler_test(int argc,const char *argv[])
{
    return 0;
}

static int hdefaults_test(int argc,const char *argv[])
{
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
    return 0;
}
static int heventslots_test(int argc,const char *argv[])
{
    return 0;
}
static int heventchain_test(int argc,const char *argv[])
{
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
    return 0;
}
