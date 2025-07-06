/***************************************************************
 * Name:      hmemorystaticallocator.c
 * Purpose:   实现hmemorystaticallocator接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-07-06
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hmemorystaticallocator.h"
#include "hdefaults.h"

const hmemorystaticallocator_lock_t hmemorystaticallocator_global_lock=
{
    NULL,
    hdefaults_mutex_lock,
    hdefaults_mutex_unlock
};

static inline const hmemorystaticallocator_lock_t * hmemorystaticallocator_lock_get(const hmemorystaticallocator_lock_t *lock)
{
    if(lock==NULL)
    {
        return &hmemorystaticallocator_global_lock;
    }

    return lock;
}

const hmemorystaticallocator_item_t * hmemorystaticallocator_global_table=NULL;

static inline const hmemorystaticallocator_item_t * hmemorystaticallocator_table_get(const hmemorystaticallocator_item_t *table)
{
    if(table==NULL)
    {
        return hmemorystaticallocator_global_table;
    }

    return table;
}

bool hmemorystaticallocator_bitmap_get(const hmemorystaticallocator_bitmap_t *bitmap,size_t index)
{
    if(bitmap==NULL)
    {
        return false;
    }
    size_t byte_offset=index/8;
    size_t bit_offset=index%8;
    if(bitmap->bitmap_size < byte_offset)
    {
        return false;
    }
    bool ret=false;
    const hmemorystaticallocator_lock_t * lock=hmemorystaticallocator_lock_get(bitmap->lock);
    if(lock->mutex_lock!=NULL)
    {
        lock->mutex_lock(lock->usr);
    }

    uint8_t byte=0;
    if(sizeof(byte)==hsoftwarevirtualmemory_read(bitmap->map_table,(uintptr_t)bitmap->bitmap_address+byte_offset,&byte,sizeof(byte)))
    {
        if((byte&(1<<(bit_offset)))!=0)
        {
            ret=true;
        }
    }

    if(lock->mutex_unlock!=NULL)
    {
        lock->mutex_unlock(lock->usr);
    }
    return ret;
}

bool hmemorystaticallocator_bitmap_try_set(const hmemorystaticallocator_bitmap_t *bitmap,size_t index)
{
    if(bitmap==NULL)
    {
        return false;
    }
    size_t byte_offset=index/8;
    size_t bit_offset=index%8;
    if(bitmap->bitmap_size < byte_offset)
    {
        return false;
    }
    bool ret=false;
    const hmemorystaticallocator_lock_t * lock=hmemorystaticallocator_lock_get(bitmap->lock);
    if(lock->mutex_lock!=NULL)
    {
        lock->mutex_lock(lock->usr);
    }

    uint8_t byte=0;
    if(sizeof(byte)==hsoftwarevirtualmemory_read(bitmap->map_table,(uintptr_t)bitmap->bitmap_address+byte_offset,&byte,sizeof(byte)))
    {
        if((byte&(1<<(bit_offset)))==0)
        {
            byte|=(1<<(bit_offset));
            ret=(sizeof(byte)==hsoftwarevirtualmemory_write(bitmap->map_table,(uintptr_t)bitmap->bitmap_address+byte_offset,&byte,sizeof(byte)));
        }
    }

    if(lock->mutex_unlock!=NULL)
    {
        lock->mutex_unlock(lock->usr);
    }
    return ret;
}

bool hmemorystaticallocator_bitmap_try_clear(const hmemorystaticallocator_bitmap_t *bitmap,size_t index)
{
    if(bitmap==NULL)
    {
        return false;
    }
    size_t byte_offset=index/8;
    size_t bit_offset=index%8;
    if(bitmap->bitmap_size < byte_offset)
    {
        return false;
    }
    bool ret=false;
    const hmemorystaticallocator_lock_t * lock=hmemorystaticallocator_lock_get(bitmap->lock);
    if(lock->mutex_lock!=NULL)
    {
        lock->mutex_lock(lock->usr);
    }

    uint8_t byte=0;
    if(sizeof(byte)==hsoftwarevirtualmemory_read(bitmap->map_table,(uintptr_t)bitmap->bitmap_address+byte_offset,&byte,sizeof(byte)))
    {
        if((byte&(1<<(bit_offset)))!=0)
        {
            byte&=(~(1<<(bit_offset)));
            ret=(sizeof(byte)==hsoftwarevirtualmemory_write(bitmap->map_table,(uintptr_t)bitmap->bitmap_address+byte_offset,&byte,sizeof(byte)));
        }
    }

    if(lock->mutex_unlock!=NULL)
    {
        lock->mutex_unlock(lock->usr);
    }
    return ret;
}

const hmemorystaticallocator_item_t * hmemorystaticallocator_get_table_item(const hmemorystaticallocator_item_t * table,const char *name)
{
    const hmemorystaticallocator_item_t *alloc_table=hmemorystaticallocator_table_get(table);
    if(alloc_table==NULL || name==NULL )
    {
        return 0;
    }
    const hmemorystaticallocator_item_t * ret=NULL;
    uintptr_t current_addr=0;                                               /**< 当前地址 */
    const hmemorystaticallocator_item_t *current_item=alloc_table;          /**< 当前项 */
    while(true)
    {
        if(current_item->obj_size==0)
        {
            //表结束
            current_addr=0;
            break;
        }

        uintptr_t address=current_addr;
        if(current_item->address!=0)
        {
            address=current_item->address;
        }

        size_t obj_size=current_item->obj_size;

        size_t obj_count=current_item->obj_count;
        if(current_item->obj_count==0)
        {
            const hmemorystaticallocator_item_t *next_item=(&current_item[1]);
            if(next_item->address!=0 && next_item->address > address)
            {
                obj_count=(next_item->address-address)/obj_size;
            }
        }

        if(current_item->name!=NULL && strcmp(name,current_item->name)==0)
        {
            //成功匹配名称
            ret=current_item;
            break;
        }

        current_addr=address+obj_size*obj_count;

        current_item++;
    }

    return ret;
}

size_t hmemorystaticallocator_get_table_alloc_size(const hmemorystaticallocator_item_t * table)
{
    const hmemorystaticallocator_item_t *alloc_table=hmemorystaticallocator_table_get(table);
    if(alloc_table==NULL)
    {
        return 0;
    }
    size_t ret=0;
    uintptr_t current_addr=0;                                               /**< 当前地址 */
    const hmemorystaticallocator_item_t *current_item=alloc_table;          /**< 当前项 */
    while(true)
    {
        if(current_item->obj_size==0)
        {
            //表结束
            break;
        }

        uintptr_t address=current_addr;
        if(current_item->address!=0)
        {
            address=current_item->address;
        }

        size_t obj_size=current_item->obj_size;

        size_t obj_count=current_item->obj_count;
        if(current_item->obj_count==0)
        {
            const hmemorystaticallocator_item_t *next_item=(&current_item[1]);
            if(next_item->address!=0 && next_item->address > address)
            {
                obj_count=(next_item->address-address)/obj_size;
            }
        }

        current_addr=address+obj_size*obj_count;
        if(current_addr > ret)
        {
            ret=current_addr;
        }


        current_item++;
    }


    return ret;
}

size_t hmemorystaticallocator_get_table_bitmap_size(const hmemorystaticallocator_item_t * table)
{
    const hmemorystaticallocator_item_t *alloc_table=hmemorystaticallocator_table_get(table);
    if(alloc_table==NULL)
    {
        return 0;
    }
    size_t ret=0;
    size_t bitmap_size=0;                                                   /**< 用于存储分配信息的位图大小 */
    uintptr_t current_addr=0;                                               /**< 当前地址 */
    const hmemorystaticallocator_item_t *current_item=alloc_table;          /**< 当前项 */
    while(true)
    {
        if(current_item->obj_size==0)
        {
            //表结束
            break;
        }

        uintptr_t address=current_addr;
        if(current_item->address!=0)
        {
            address=current_item->address;
        }

        size_t obj_size=current_item->obj_size;

        size_t obj_count=current_item->obj_count;
        if(current_item->obj_count==0)
        {
            const hmemorystaticallocator_item_t *next_item=(&current_item[1]);
            if(next_item->address!=0 && next_item->address > address)
            {
                obj_count=(next_item->address-address)/obj_size;
            }
        }

        current_addr=address+obj_size*obj_count;

        if((current_item->flags&(HMEMORYSTATICALLOCATOR_FLAGS_NO_ALLOC))==0)
        {
            //需要位图保存分配情况
            bitmap_size+=obj_count;
        }

        current_item++;
    }

    if(bitmap_size!=0)
    {
        /*
         * 位图的空间(按字节计算)
         */
        ret+=((bitmap_size+7)/8);
    }

    return ret;
}

size_t hmemorystaticallocator_get_table_size(const hmemorystaticallocator_item_t * table)
{
    return hmemorystaticallocator_get_table_alloc_size(table)+hmemorystaticallocator_get_table_bitmap_size(table);
}

uintptr_t hmemorystaticallocator_get_object_address(const hmemorystaticallocator_item_t * table,const char *name)
{
    const hmemorystaticallocator_item_t *alloc_table=hmemorystaticallocator_table_get(table);
    if(alloc_table==NULL || name==NULL )
    {
        return 0;
    }
    uintptr_t current_addr=0;                                               /**< 当前地址 */
    const hmemorystaticallocator_item_t *current_item=alloc_table;          /**< 当前项 */
    while(true)
    {
        if(current_item->obj_size==0)
        {
            //表结束
            current_addr=0;
            break;
        }

        uintptr_t address=current_addr;
        if(current_item->address!=0)
        {
            address=current_item->address;
        }

        size_t obj_size=current_item->obj_size;

        size_t obj_count=current_item->obj_count;
        if(current_item->obj_count==0)
        {
            const hmemorystaticallocator_item_t *next_item=(&current_item[1]);
            if(next_item->address!=0 && next_item->address > address)
            {
                obj_count=(next_item->address-address)/obj_size;
            }
        }

        if(current_item->name!=NULL && strcmp(name,current_item->name)==0)
        {
            //成功匹配名称
            break;
        }

        current_addr=address+obj_size*obj_count;

        current_item++;
    }

    return current_addr;
}

size_t hmemorystaticallocator_get_object_size(const hmemorystaticallocator_item_t * table,const char *name)
{
    const hmemorystaticallocator_item_t *alloc_table=hmemorystaticallocator_table_get(table);
    if(alloc_table==NULL || name==NULL )
    {
        return 0;
    }
    size_t ret=0;
    uintptr_t current_addr=0;                                               /**< 当前地址 */
    const hmemorystaticallocator_item_t *current_item=alloc_table;          /**< 当前项 */
    while(true)
    {
        if(current_item->obj_size==0)
        {
            //表结束
            current_addr=0;
            break;
        }

        uintptr_t address=current_addr;
        if(current_item->address!=0)
        {
            address=current_item->address;
        }

        size_t obj_size=current_item->obj_size;

        size_t obj_count=current_item->obj_count;
        if(current_item->obj_count==0)
        {
            const hmemorystaticallocator_item_t *next_item=(&current_item[1]);
            if(next_item->address!=0 && next_item->address > address)
            {
                obj_count=(next_item->address-address)/obj_size;
            }
        }

        if(current_item->name!=NULL && strcmp(name,current_item->name)==0)
        {
            //成功匹配名称
            ret=obj_size;
            break;
        }

        current_addr=address+obj_size*obj_count;

        current_item++;
    }

    return ret;
}

size_t hmemorystaticallocator_get_object_count(const hmemorystaticallocator_item_t * table,const char *name)
{
    const hmemorystaticallocator_item_t *alloc_table=hmemorystaticallocator_table_get(table);
    if(alloc_table==NULL || name==NULL )
    {
        return 0;
    }
    size_t ret=0;
    uintptr_t current_addr=0;                                               /**< 当前地址 */
    const hmemorystaticallocator_item_t *current_item=alloc_table;          /**< 当前项 */
    while(true)
    {
        if(current_item->obj_size==0)
        {
            //表结束
            current_addr=0;
            break;
        }

        uintptr_t address=current_addr;
        if(current_item->address!=0)
        {
            address=current_item->address;
        }

        size_t obj_size=current_item->obj_size;

        size_t obj_count=current_item->obj_count;
        if(current_item->obj_count==0)
        {
            const hmemorystaticallocator_item_t *next_item=(&current_item[1]);
            if(next_item->address!=0 && next_item->address > address)
            {
                obj_count=(next_item->address-address)/obj_size;
            }
        }

        if(current_item->name!=NULL && strcmp(name,current_item->name)==0)
        {
            //成功匹配名称
            ret=obj_count;
            break;
        }

        current_addr=address+obj_size*obj_count;

        current_item++;
    }

    return ret;
}

uint32_t hmemorystaticallocator_get_object_flags(const hmemorystaticallocator_item_t * table,const char *name)
{
    const hmemorystaticallocator_item_t *alloc_table=hmemorystaticallocator_table_get(table);
    if(alloc_table==NULL || name==NULL )
    {
        return 0;
    }
    uint32_t ret=0;
    uintptr_t current_addr=0;                                               /**< 当前地址 */
    const hmemorystaticallocator_item_t *current_item=alloc_table;          /**< 当前项 */
    while(true)
    {
        if(current_item->obj_size==0)
        {
            //表结束
            current_addr=0;
            break;
        }

        uintptr_t address=current_addr;
        if(current_item->address!=0)
        {
            address=current_item->address;
        }

        size_t obj_size=current_item->obj_size;

        size_t obj_count=current_item->obj_count;
        if(current_item->obj_count==0)
        {
            const hmemorystaticallocator_item_t *next_item=(&current_item[1]);
            if(next_item->address!=0 && next_item->address > address)
            {
                obj_count=(next_item->address-address)/obj_size;
            }
        }

        if(current_item->name!=NULL && strcmp(name,current_item->name)==0)
        {
            //成功匹配名称
            ret=current_item->flags;
            break;
        }

        current_addr=address+obj_size*obj_count;

        current_item++;
    }

    return ret;
}

void * hmemorystaticallocator_alloc_object(const hmemorystaticallocator_item_t * table,const hmemorystaticallocator_bitmap_t *bitmap,const char *name,void *base_address)
{
    const hmemorystaticallocator_item_t *alloc_table=hmemorystaticallocator_table_get(table);
    if(alloc_table==NULL || name==NULL )
    {
        return NULL;
    }
    void *ret=NULL;
    size_t bitmap_offset=0;
    uintptr_t current_addr=0;                                               /**< 当前地址 */
    const hmemorystaticallocator_item_t *current_item=alloc_table;          /**< 当前项 */
    while(true)
    {
        if(current_item->obj_size==0)
        {
            //表结束
            current_addr=0;
            break;
        }

        uintptr_t address=current_addr;
        if(current_item->address!=0)
        {
            address=current_item->address;
        }

        size_t obj_size=current_item->obj_size;

        size_t obj_count=current_item->obj_count;
        if(current_item->obj_count==0)
        {
            const hmemorystaticallocator_item_t *next_item=(&current_item[1]);
            if(next_item->address!=0 && next_item->address > address)
            {
                obj_count=(next_item->address-address)/obj_size;
            }
        }

        if(current_item->name!=NULL && strcmp(name,current_item->name)==0)
        {
            //成功匹配名称
            if((current_item->flags&(HMEMORYSTATICALLOCATOR_FLAGS_NO_ALLOC))==0)
            {
                //匹配到允许分配的项
                for(size_t i=bitmap_offset; i<(bitmap_offset+obj_count); i++)
                {
                    if(hmemorystaticallocator_bitmap_try_set(bitmap,i))
                    {
                        //成功设置位图中的位
                        ret=(void *)((uintptr_t)base_address+address+obj_size*i);
                        break;
                    }
                }
                break;
            }
        }

        current_addr=address+obj_size*obj_count;

        if((current_item->flags&(HMEMORYSTATICALLOCATOR_FLAGS_NO_ALLOC))==0)
        {
            bitmap_offset+=obj_count;
        }

        current_item++;
    }

    return ret;
}

void  hmemorystaticallocator_free_object(const hmemorystaticallocator_item_t * table,const hmemorystaticallocator_bitmap_t *bitmap,const char *name,void *base_address,void *address)
{
    const hmemorystaticallocator_item_t *alloc_table=hmemorystaticallocator_table_get(table);
    if(alloc_table==NULL || name==NULL || ((uintptr_t)address) < ((uintptr_t)base_address))
    {
        return;
    }
    uintptr_t obj_address=(((uintptr_t)address)-((uintptr_t)base_address));
    size_t bitmap_offset=0;
    uintptr_t current_addr=0;                                               /**< 当前地址 */
    const hmemorystaticallocator_item_t *current_item=alloc_table;          /**< 当前项 */
    while(true)
    {
        if(current_item->obj_size==0)
        {
            //表结束
            current_addr=0;
            break;
        }

        uintptr_t address=current_addr;
        if(current_item->address!=0)
        {
            address=current_item->address;
        }

        size_t obj_size=current_item->obj_size;

        size_t obj_count=current_item->obj_count;
        if(current_item->obj_count==0)
        {
            const hmemorystaticallocator_item_t *next_item=(&current_item[1]);
            if(next_item->address!=0 && next_item->address > address)
            {
                obj_count=(next_item->address-address)/obj_size;
            }
        }

        if(current_item->name!=NULL && strcmp(name,current_item->name)==0)
        {
            //成功匹配名称
            if((current_item->flags&(HMEMORYSTATICALLOCATOR_FLAGS_NO_ALLOC))==0)
            {
                //匹配到允许分配的项
                if((obj_address>address)&& (((obj_address-address)%obj_size)==0))
                {
                    size_t index=((obj_address-address)/obj_size);
                    if(index < obj_count)
                    {
                        hmemorystaticallocator_bitmap_try_clear(bitmap,bitmap_offset+index);
                    }
                }
                break;
            }
        }

        current_addr=address+obj_size*obj_count;

        if((current_item->flags&(HMEMORYSTATICALLOCATOR_FLAGS_NO_ALLOC))==0)
        {
            bitmap_offset+=obj_count;
        }

        current_item++;
    }
}

void * hmemorystaticallocator_alloc_anonymous_object(const hmemorystaticallocator_item_t * table,const hmemorystaticallocator_bitmap_t *bitmap,size_t object_size,void *base_address)
{
    const hmemorystaticallocator_item_t *alloc_table=hmemorystaticallocator_table_get(table);
    if(alloc_table==NULL || object_size==0 )
    {
        return NULL;
    }
    void *ret=NULL;
    size_t bitmap_offset=0;
    uintptr_t current_addr=0;                                               /**< 当前地址 */
    const hmemorystaticallocator_item_t *current_item=alloc_table;          /**< 当前项 */
    while(true)
    {
        if(current_item->obj_size==0)
        {
            //表结束
            current_addr=0;
            break;
        }

        uintptr_t address=current_addr;
        if(current_item->address!=0)
        {
            address=current_item->address;
        }

        size_t obj_size=current_item->obj_size;

        size_t obj_count=current_item->obj_count;
        if(current_item->obj_count==0)
        {
            const hmemorystaticallocator_item_t *next_item=(&current_item[1]);
            if(next_item->address!=0 && next_item->address > address)
            {
                obj_count=(next_item->address-address)/obj_size;
            }
        }

        if(current_item->name==NULL && object_size==obj_size)
        {
            //成功匹配大小
            if((current_item->flags&(HMEMORYSTATICALLOCATOR_FLAGS_NO_ALLOC))==0)
            {
                //匹配到允许分配的项
                for(size_t i=bitmap_offset; i<(bitmap_offset+obj_count); i++)
                {
                    if(hmemorystaticallocator_bitmap_try_set(bitmap,i))
                    {
                        //成功设置位图中的位
                        ret=(void *)((uintptr_t)base_address+address+obj_size*i);
                        break;
                    }
                }
                break;
            }
        }

        current_addr=address+obj_size*obj_count;

        if((current_item->flags&(HMEMORYSTATICALLOCATOR_FLAGS_NO_ALLOC))==0)
        {
            bitmap_offset+=obj_count;
        }

        current_item++;
    }

    return ret;
}


void hmemorystaticallocator_free_anonymous_object(const hmemorystaticallocator_item_t * table,const hmemorystaticallocator_bitmap_t *bitmap,size_t object_size,void *base_address,void *address)
{
    const hmemorystaticallocator_item_t *alloc_table=hmemorystaticallocator_table_get(table);
    if(alloc_table==NULL || object_size==0 || ((uintptr_t)address) < ((uintptr_t)base_address))
    {
        return;
    }
    uintptr_t obj_address=(((uintptr_t)address)-((uintptr_t)base_address));
    size_t bitmap_offset=0;
    uintptr_t current_addr=0;                                               /**< 当前地址 */
    const hmemorystaticallocator_item_t *current_item=alloc_table;          /**< 当前项 */
    while(true)
    {
        if(current_item->obj_size==0)
        {
            //表结束
            current_addr=0;
            break;
        }

        uintptr_t address=current_addr;
        if(current_item->address!=0)
        {
            address=current_item->address;
        }

        size_t obj_size=current_item->obj_size;

        size_t obj_count=current_item->obj_count;
        if(current_item->obj_count==0)
        {
            const hmemorystaticallocator_item_t *next_item=(&current_item[1]);
            if(next_item->address!=0 && next_item->address > address)
            {
                obj_count=(next_item->address-address)/obj_size;
            }
        }

        if(current_item->name==NULL && object_size==obj_size)
        {
            //成功匹配大小
            if((current_item->flags&(HMEMORYSTATICALLOCATOR_FLAGS_NO_ALLOC))==0)
            {
                //匹配到允许分配的项
                if((obj_address>address)&& (((obj_address-address)%obj_size)==0))
                {
                    size_t index=((obj_address-address)/obj_size);
                    if(index < obj_count)
                    {
                        hmemorystaticallocator_bitmap_try_clear(bitmap,bitmap_offset+index);
                    }
                }
                break;
            }
        }

        current_addr=address+obj_size*obj_count;

        if((current_item->flags&(HMEMORYSTATICALLOCATOR_FLAGS_NO_ALLOC))==0)
        {
            bitmap_offset+=obj_count;
        }

        current_item++;
    }
}


static const hmemorystaticallocator_item_t hmemorystaticallocator_memory_cache_table[]=
{
    {0          ,1   ,16,NULL,0},
    {16         ,2   ,8 ,NULL,0},
    {32         ,4   ,8 ,NULL,0},
    {64         ,8   ,8 ,NULL,0},
    {128        ,16  ,8 ,NULL,0},
    {256        ,32  ,8 ,NULL,0},
    {512        ,64  ,8 ,NULL,0},
    {1024       ,128 ,8 ,NULL,0},
    {2048       ,256 ,8 ,NULL,0},
    {4096       ,0   ,0 ,NULL,0}                              /**< 分配表结束 */
};
const hmemorystaticallocator_item_t * const hmemorystaticallocator_cache_table=hmemorystaticallocator_memory_cache_table;
