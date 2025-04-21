/***************************************************************
 * Name:      hhmac.c
 * Purpose:   实现hhmac接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hhmac.h"

#define HHMAC_IPAD 0x36
#define HHMAC_OPAD 0x5c

int hhmac_md4_starts(hhmac_md4_context_t *ctx,const uint8_t *key,size_t key_len)
{
    if(ctx==NULL || key == NULL || key_len == 0)
    {
        return -1;
    }
    bool is_key_hash=false;
    memset(ctx,0,sizeof(*ctx));
    if(key_len > sizeof(ctx->buffer))
    {
        //当Key大于一个消息块时，先进行hash
        hmd4_starts(&ctx->ctx);
        hmd4_update(&ctx->ctx,key,key_len);
        hmd4_finish(&ctx->ctx,ctx->hash_result);
        is_key_hash=true;
    }

    {
        //I KEY PAD
        memset(ctx->buffer,HHMAC_IPAD,sizeof(ctx->buffer));
        const uint8_t *key_to_pad=key;
        size_t  key_to_pad_size=key_len;
        if(is_key_hash)
        {
            key_to_pad=ctx->hash_result;
            key_to_pad_size=sizeof(ctx->hash_result);
        }

        for(size_t i=0; i<key_to_pad_size; i++)
        {
            ctx->buffer[i] ^= key_to_pad[i];
        }
    }

    hmd4_starts(&ctx->ctx);
    hmd4_update(&ctx->ctx,ctx->buffer,sizeof(ctx->buffer));

    {
        //O KEY PAD
        memset(ctx->buffer,HHMAC_OPAD,sizeof(ctx->buffer));
        const uint8_t *key_to_pad=key;
        size_t  key_to_pad_size=key_len;
        if(is_key_hash)
        {
            key_to_pad=ctx->hash_result;
            key_to_pad_size=sizeof(ctx->hash_result);
        }

        for(size_t i=0; i<key_to_pad_size; i++)
        {
            ctx->buffer[i] ^= key_to_pad[i];
        }
    }

    return 0;
}

int hhmac_md4_update(hhmac_md4_context_t *ctx,const uint8_t *message,size_t message_len)
{
    if(ctx==NULL)
    {
        return -1;
    }
    if(message!=NULL && message_len > 0)
    {
        hmd4_update(&ctx->ctx,message,message_len);
    }
    return 0;
}

int hhmac_md4_finish(hhmac_md4_context_t  *ctx)
{
    if(ctx==NULL)
    {
        return -1;
    }
    hmd4_finish(&ctx->ctx,ctx->hash_result);
    hmd4_starts(&ctx->ctx);
    hmd4_update(&ctx->ctx,ctx->buffer,sizeof(ctx->buffer));
    hmd4_update(&ctx->ctx,ctx->hash_result,sizeof(ctx->hash_result));
    hmd4_finish(&ctx->ctx,ctx->hash_result);
    return 0;
}


int hhmac_md5_starts(hhmac_md5_context_t *ctx,const uint8_t *key,size_t key_len)
{
    if(ctx==NULL || key == NULL || key_len == 0)
    {
        return -1;
    }
    bool is_key_hash=false;
    memset(ctx,0,sizeof(*ctx));
    if(key_len > sizeof(ctx->buffer))
    {
        //当Key大于一个消息块时，先进行hash
        hmd5_starts(&ctx->ctx);
        hmd5_update(&ctx->ctx,key,key_len);
        hmd5_finish(&ctx->ctx,ctx->hash_result);
        is_key_hash=true;
    }

    {
        //I KEY PAD
        memset(ctx->buffer,HHMAC_IPAD,sizeof(ctx->buffer));
        const uint8_t *key_to_pad=key;
        size_t  key_to_pad_size=key_len;
        if(is_key_hash)
        {
            key_to_pad=ctx->hash_result;
            key_to_pad_size=sizeof(ctx->hash_result);
        }

        for(size_t i=0; i<key_to_pad_size; i++)
        {
            ctx->buffer[i] ^= key_to_pad[i];
        }
    }

    hmd5_starts(&ctx->ctx);
    hmd5_update(&ctx->ctx,ctx->buffer,sizeof(ctx->buffer));

    {
        //O KEY PAD
        memset(ctx->buffer,HHMAC_OPAD,sizeof(ctx->buffer));
        const uint8_t *key_to_pad=key;
        size_t  key_to_pad_size=key_len;
        if(is_key_hash)
        {
            key_to_pad=ctx->hash_result;
            key_to_pad_size=sizeof(ctx->hash_result);
        }

        for(size_t i=0; i<key_to_pad_size; i++)
        {
            ctx->buffer[i] ^= key_to_pad[i];
        }
    }

    return 0;
}

int hhmac_md5_update(hhmac_md5_context_t *ctx,const uint8_t *message,size_t message_len)
{
    if(ctx==NULL)
    {
        return -1;
    }
    if(message!=NULL && message_len > 0)
    {
        hmd5_update(&ctx->ctx,message,message_len);
    }
    return 0;
}

int hhmac_md5_finish(hhmac_md5_context_t  *ctx)
{
    if(ctx==NULL)
    {
        return -1;
    }
    hmd5_finish(&ctx->ctx,ctx->hash_result);
    hmd5_starts(&ctx->ctx);
    hmd5_update(&ctx->ctx,ctx->buffer,sizeof(ctx->buffer));
    hmd5_update(&ctx->ctx,ctx->hash_result,sizeof(ctx->hash_result));
    hmd5_finish(&ctx->ctx,ctx->hash_result);
    return 0;
}


int hhmac_sha1_starts(hhmac_sha1_context_t *ctx,const uint8_t *key,size_t key_len)
{
    if(ctx==NULL || key == NULL || key_len == 0)
    {
        return -1;
    }
    bool is_key_hash=false;
    memset(ctx,0,sizeof(*ctx));
    if(key_len > sizeof(ctx->buffer))
    {
        //当Key大于一个消息块时，先进行hash
        hsha1_starts(&ctx->ctx);
        hsha1_update(&ctx->ctx,key,key_len);
        hsha1_finish(&ctx->ctx,ctx->hash_result);
        is_key_hash=true;
    }

    {
        //I KEY PAD
        memset(ctx->buffer,HHMAC_IPAD,sizeof(ctx->buffer));
        const uint8_t *key_to_pad=key;
        size_t  key_to_pad_size=key_len;
        if(is_key_hash)
        {
            key_to_pad=ctx->hash_result;
            key_to_pad_size=sizeof(ctx->hash_result);
        }

        for(size_t i=0; i<key_to_pad_size; i++)
        {
            ctx->buffer[i] ^= key_to_pad[i];
        }
    }

    hsha1_starts(&ctx->ctx);
    hsha1_update(&ctx->ctx,ctx->buffer,sizeof(ctx->buffer));

    {
        //O KEY PAD
        memset(ctx->buffer,HHMAC_OPAD,sizeof(ctx->buffer));
        const uint8_t *key_to_pad=key;
        size_t  key_to_pad_size=key_len;
        if(is_key_hash)
        {
            key_to_pad=ctx->hash_result;
            key_to_pad_size=sizeof(ctx->hash_result);
        }

        for(size_t i=0; i<key_to_pad_size; i++)
        {
            ctx->buffer[i] ^= key_to_pad[i];
        }
    }

    return 0;
}

int hhmac_sha1_update(hhmac_sha1_context_t *ctx,const uint8_t *message,size_t message_len)
{
    if(ctx==NULL)
    {
        return -1;
    }
    if(message!=NULL && message_len > 0)
    {
        hsha1_update(&ctx->ctx,message,message_len);
    }
    return 0;
}

int hhmac_sha1_finish(hhmac_sha1_context_t  *ctx)
{
    if(ctx==NULL)
    {
        return -1;
    }
    hsha1_finish(&ctx->ctx,ctx->hash_result);
    hsha1_starts(&ctx->ctx);
    hsha1_update(&ctx->ctx,ctx->buffer,sizeof(ctx->buffer));
    hsha1_update(&ctx->ctx,ctx->hash_result,sizeof(ctx->hash_result));
    hsha1_finish(&ctx->ctx,ctx->hash_result);
    return 0;
}

int hhmac_sha2_sha224_starts(hhmac_sha2_sha224_context_t *ctx,const uint8_t *key,size_t key_len)
{
    if(ctx==NULL || key == NULL || key_len == 0)
    {
        return -1;
    }
    bool is_key_hash=false;
    memset(ctx,0,sizeof(*ctx));
    if(key_len > sizeof(ctx->buffer))
    {
        //当Key大于一个消息块时，先进行hash
        hsha2_sha224_starts(&ctx->ctx);
        hsha2_sha224_update(&ctx->ctx,key,key_len);
        hsha2_sha224_finish(&ctx->ctx,ctx->hash_result);
        is_key_hash=true;
    }

    {
        //I KEY PAD
        memset(ctx->buffer,HHMAC_IPAD,sizeof(ctx->buffer));
        const uint8_t *key_to_pad=key;
        size_t  key_to_pad_size=key_len;
        if(is_key_hash)
        {
            key_to_pad=ctx->hash_result;
            key_to_pad_size=sizeof(ctx->hash_result);
        }

        for(size_t i=0; i<key_to_pad_size; i++)
        {
            ctx->buffer[i] ^= key_to_pad[i];
        }
    }

    hsha2_sha224_starts(&ctx->ctx);
    hsha2_sha224_update(&ctx->ctx,ctx->buffer,sizeof(ctx->buffer));

    {
        //O KEY PAD
        memset(ctx->buffer,HHMAC_OPAD,sizeof(ctx->buffer));
        const uint8_t *key_to_pad=key;
        size_t  key_to_pad_size=key_len;
        if(is_key_hash)
        {
            key_to_pad=ctx->hash_result;
            key_to_pad_size=sizeof(ctx->hash_result);
        }

        for(size_t i=0; i<key_to_pad_size; i++)
        {
            ctx->buffer[i] ^= key_to_pad[i];
        }
    }

    return 0;
}

int hhmac_sha2_sha224_update(hhmac_sha2_sha224_context_t *ctx,const uint8_t *message,size_t message_len)
{
    if(ctx==NULL)
    {
        return -1;
    }
    if(message!=NULL && message_len > 0)
    {
        hsha2_sha224_update(&ctx->ctx,message,message_len);
    }
    return 0;
}

int hhmac_sha2_sha224_finish(hhmac_sha2_sha224_context_t  *ctx)
{
    if(ctx==NULL)
    {
        return -1;
    }
    hsha2_sha224_finish(&ctx->ctx,ctx->hash_result);
    hsha2_sha224_starts(&ctx->ctx);
    hsha2_sha224_update(&ctx->ctx,ctx->buffer,sizeof(ctx->buffer));
    hsha2_sha224_update(&ctx->ctx,ctx->hash_result,sizeof(ctx->hash_result));
    hsha2_sha224_finish(&ctx->ctx,ctx->hash_result);
    return 0;
}

int hhmac_sha2_sha256_starts(hhmac_sha2_sha256_context_t *ctx,const uint8_t *key,size_t key_len)
{
    if(ctx==NULL || key == NULL || key_len == 0)
    {
        return -1;
    }
    bool is_key_hash=false;
    memset(ctx,0,sizeof(*ctx));
    if(key_len > sizeof(ctx->buffer))
    {
        //当Key大于一个消息块时，先进行hash
        hsha2_sha256_starts(&ctx->ctx);
        hsha2_sha256_update(&ctx->ctx,key,key_len);
        hsha2_sha256_finish(&ctx->ctx,ctx->hash_result);
        is_key_hash=true;
    }

    {
        //I KEY PAD
        memset(ctx->buffer,HHMAC_IPAD,sizeof(ctx->buffer));
        const uint8_t *key_to_pad=key;
        size_t  key_to_pad_size=key_len;
        if(is_key_hash)
        {
            key_to_pad=ctx->hash_result;
            key_to_pad_size=sizeof(ctx->hash_result);
        }

        for(size_t i=0; i<key_to_pad_size; i++)
        {
            ctx->buffer[i] ^= key_to_pad[i];
        }
    }

    hsha2_sha256_starts(&ctx->ctx);
    hsha2_sha256_update(&ctx->ctx,ctx->buffer,sizeof(ctx->buffer));

    {
        //O KEY PAD
        memset(ctx->buffer,HHMAC_OPAD,sizeof(ctx->buffer));
        const uint8_t *key_to_pad=key;
        size_t  key_to_pad_size=key_len;
        if(is_key_hash)
        {
            key_to_pad=ctx->hash_result;
            key_to_pad_size=sizeof(ctx->hash_result);
        }

        for(size_t i=0; i<key_to_pad_size; i++)
        {
            ctx->buffer[i] ^= key_to_pad[i];
        }
    }

    return 0;
}

int hhmac_sha2_sha256_update(hhmac_sha2_sha256_context_t *ctx,const uint8_t *message,size_t message_len)
{
    if(ctx==NULL)
    {
        return -1;
    }
    if(message!=NULL && message_len > 0)
    {
        hsha2_sha256_update(&ctx->ctx,message,message_len);
    }
    return 0;
}

int hhmac_sha2_sha256_finish(hhmac_sha2_sha256_context_t  *ctx)
{
    if(ctx==NULL)
    {
        return -1;
    }
    hsha2_sha256_finish(&ctx->ctx,ctx->hash_result);
    hsha2_sha256_starts(&ctx->ctx);
    hsha2_sha256_update(&ctx->ctx,ctx->buffer,sizeof(ctx->buffer));
    hsha2_sha256_update(&ctx->ctx,ctx->hash_result,sizeof(ctx->hash_result));
    hsha2_sha256_finish(&ctx->ctx,ctx->hash_result);
    return 0;
}

int hhmac_sha2_sha384_starts(hhmac_sha2_sha384_context_t *ctx,const uint8_t *key,size_t key_len)
{
    if(ctx==NULL || key == NULL || key_len == 0)
    {
        return -1;
    }
    bool is_key_hash=false;
    memset(ctx,0,sizeof(*ctx));
    if(key_len > sizeof(ctx->buffer))
    {
        //当Key大于一个消息块时，先进行hash
        hsha2_sha384_starts(&ctx->ctx);
        hsha2_sha384_update(&ctx->ctx,key,key_len);
        hsha2_sha384_finish(&ctx->ctx,ctx->hash_result);
        is_key_hash=true;
    }

    {
        //I KEY PAD
        memset(ctx->buffer,HHMAC_IPAD,sizeof(ctx->buffer));
        const uint8_t *key_to_pad=key;
        size_t  key_to_pad_size=key_len;
        if(is_key_hash)
        {
            key_to_pad=ctx->hash_result;
            key_to_pad_size=sizeof(ctx->hash_result);
        }

        for(size_t i=0; i<key_to_pad_size; i++)
        {
            ctx->buffer[i] ^= key_to_pad[i];
        }
    }

    hsha2_sha384_starts(&ctx->ctx);
    hsha2_sha384_update(&ctx->ctx,ctx->buffer,sizeof(ctx->buffer));

    {
        //O KEY PAD
        memset(ctx->buffer,HHMAC_OPAD,sizeof(ctx->buffer));
        const uint8_t *key_to_pad=key;
        size_t  key_to_pad_size=key_len;
        if(is_key_hash)
        {
            key_to_pad=ctx->hash_result;
            key_to_pad_size=sizeof(ctx->hash_result);
        }

        for(size_t i=0; i<key_to_pad_size; i++)
        {
            ctx->buffer[i] ^= key_to_pad[i];
        }
    }

    return 0;
}

int hhmac_sha2_sha384_update(hhmac_sha2_sha384_context_t *ctx,const uint8_t *message,size_t message_len)
{
    if(ctx==NULL)
    {
        return -1;
    }
    if(message!=NULL && message_len > 0)
    {
        hsha2_sha384_update(&ctx->ctx,message,message_len);
    }
    return 0;
}

int hhmac_sha2_sha384_finish(hhmac_sha2_sha384_context_t  *ctx)
{
    if(ctx==NULL)
    {
        return -1;
    }
    hsha2_sha384_finish(&ctx->ctx,ctx->hash_result);
    hsha2_sha384_starts(&ctx->ctx);
    hsha2_sha384_update(&ctx->ctx,ctx->buffer,sizeof(ctx->buffer));
    hsha2_sha384_update(&ctx->ctx,ctx->hash_result,sizeof(ctx->hash_result));
    hsha2_sha384_finish(&ctx->ctx,ctx->hash_result);
    return 0;
}

int hhmac_sha2_sha512_starts(hhmac_sha2_sha512_context_t *ctx,const uint8_t *key,size_t key_len)
{
    if(ctx==NULL || key == NULL || key_len == 0)
    {
        return -1;
    }
    bool is_key_hash=false;
    memset(ctx,0,sizeof(*ctx));
    if(key_len > sizeof(ctx->buffer))
    {
        //当Key大于一个消息块时，先进行hash
        hsha2_sha512_starts(&ctx->ctx);
        hsha2_sha512_update(&ctx->ctx,key,key_len);
        hsha2_sha512_finish(&ctx->ctx,ctx->hash_result);
        is_key_hash=true;
    }

    {
        //I KEY PAD
        memset(ctx->buffer,HHMAC_IPAD,sizeof(ctx->buffer));
        const uint8_t *key_to_pad=key;
        size_t  key_to_pad_size=key_len;
        if(is_key_hash)
        {
            key_to_pad=ctx->hash_result;
            key_to_pad_size=sizeof(ctx->hash_result);
        }

        for(size_t i=0; i<key_to_pad_size; i++)
        {
            ctx->buffer[i] ^= key_to_pad[i];
        }
    }

    hsha2_sha512_starts(&ctx->ctx);
    hsha2_sha512_update(&ctx->ctx,ctx->buffer,sizeof(ctx->buffer));

    {
        //O KEY PAD
        memset(ctx->buffer,HHMAC_OPAD,sizeof(ctx->buffer));
        const uint8_t *key_to_pad=key;
        size_t  key_to_pad_size=key_len;
        if(is_key_hash)
        {
            key_to_pad=ctx->hash_result;
            key_to_pad_size=sizeof(ctx->hash_result);
        }

        for(size_t i=0; i<key_to_pad_size; i++)
        {
            ctx->buffer[i] ^= key_to_pad[i];
        }
    }

    return 0;
}

int hhmac_sha2_sha512_update(hhmac_sha2_sha512_context_t *ctx,const uint8_t *message,size_t message_len)
{
    if(ctx==NULL)
    {
        return -1;
    }
    if(message!=NULL && message_len > 0)
    {
        hsha2_sha512_update(&ctx->ctx,message,message_len);
    }
    return 0;
}

int hhmac_sha2_sha512_finish(hhmac_sha2_sha512_context_t  *ctx)
{
    if(ctx==NULL)
    {
        return -1;
    }
    hsha2_sha512_finish(&ctx->ctx,ctx->hash_result);
    hsha2_sha512_starts(&ctx->ctx);
    hsha2_sha512_update(&ctx->ctx,ctx->buffer,sizeof(ctx->buffer));
    hsha2_sha512_update(&ctx->ctx,ctx->hash_result,sizeof(ctx->hash_result));
    hsha2_sha512_finish(&ctx->ctx,ctx->hash_result);
    return 0;
}

int hhmac_sm3_starts(hhmac_sm3_context_t *ctx,const uint8_t *key,size_t key_len)
{
    if(ctx==NULL || key == NULL || key_len == 0)
    {
        return -1;
    }
    bool is_key_hash=false;
    memset(ctx,0,sizeof(*ctx));
    if(key_len > sizeof(ctx->buffer))
    {
        //当Key大于一个消息块时，先进行hash
        hsm3_starts(&ctx->ctx);
        hsm3_update(&ctx->ctx,key,key_len);
        hsm3_finish(&ctx->ctx,ctx->hash_result);
        is_key_hash=true;
    }

    {
        //I KEY PAD
        memset(ctx->buffer,HHMAC_IPAD,sizeof(ctx->buffer));
        const uint8_t *key_to_pad=key;
        size_t  key_to_pad_size=key_len;
        if(is_key_hash)
        {
            key_to_pad=ctx->hash_result;
            key_to_pad_size=sizeof(ctx->hash_result);
        }

        for(size_t i=0; i<key_to_pad_size; i++)
        {
            ctx->buffer[i] ^= key_to_pad[i];
        }
    }

    hsm3_starts(&ctx->ctx);
    hsm3_update(&ctx->ctx,ctx->buffer,sizeof(ctx->buffer));

    {
        //O KEY PAD
        memset(ctx->buffer,HHMAC_OPAD,sizeof(ctx->buffer));
        const uint8_t *key_to_pad=key;
        size_t  key_to_pad_size=key_len;
        if(is_key_hash)
        {
            key_to_pad=ctx->hash_result;
            key_to_pad_size=sizeof(ctx->hash_result);
        }

        for(size_t i=0; i<key_to_pad_size; i++)
        {
            ctx->buffer[i] ^= key_to_pad[i];
        }
    }

    return 0;
}

int hhmac_sm3_update(hhmac_sm3_context_t *ctx,const uint8_t *message,size_t message_len)
{
    if(ctx==NULL)
    {
        return -1;
    }
    if(message!=NULL && message_len > 0)
    {
        hsm3_update(&ctx->ctx,message,message_len);
    }
    return 0;
}

int hhmac_sm3_finish(hhmac_sm3_context_t  *ctx)
{
    if(ctx==NULL)
    {
        return -1;
    }
    hsm3_finish(&ctx->ctx,ctx->hash_result);
    hsm3_starts(&ctx->ctx);
    hsm3_update(&ctx->ctx,ctx->buffer,sizeof(ctx->buffer));
    hsm3_update(&ctx->ctx,ctx->hash_result,sizeof(ctx->hash_result));
    hsm3_finish(&ctx->ctx,ctx->hash_result);
    return 0;
}

int hhmac_ripemd160_starts(hhmac_ripemd160_context_t *ctx,const uint8_t *key,size_t key_len)
{
    if(ctx==NULL || key == NULL || key_len == 0)
    {
        return -1;
    }
    bool is_key_hash=false;
    memset(ctx,0,sizeof(*ctx));
    if(key_len > sizeof(ctx->buffer))
    {
        //当Key大于一个消息块时，先进行hash
        hripemd160_starts(&ctx->ctx);
        hripemd160_update(&ctx->ctx,key,key_len);
        hripemd160_finish(&ctx->ctx,ctx->hash_result);
        is_key_hash=true;
    }

    {
        //I KEY PAD
        memset(ctx->buffer,HHMAC_IPAD,sizeof(ctx->buffer));
        const uint8_t *key_to_pad=key;
        size_t  key_to_pad_size=key_len;
        if(is_key_hash)
        {
            key_to_pad=ctx->hash_result;
            key_to_pad_size=sizeof(ctx->hash_result);
        }

        for(size_t i=0; i<key_to_pad_size; i++)
        {
            ctx->buffer[i] ^= key_to_pad[i];
        }
    }

    hripemd160_starts(&ctx->ctx);
    hripemd160_update(&ctx->ctx,ctx->buffer,sizeof(ctx->buffer));

    {
        //O KEY PAD
        memset(ctx->buffer,HHMAC_OPAD,sizeof(ctx->buffer));
        const uint8_t *key_to_pad=key;
        size_t  key_to_pad_size=key_len;
        if(is_key_hash)
        {
            key_to_pad=ctx->hash_result;
            key_to_pad_size=sizeof(ctx->hash_result);
        }

        for(size_t i=0; i<key_to_pad_size; i++)
        {
            ctx->buffer[i] ^= key_to_pad[i];
        }
    }

    return 0;
}

int hhmac_ripemd160_update(hhmac_ripemd160_context_t *ctx,const uint8_t *message,size_t message_len)
{
    if(ctx==NULL)
    {
        return -1;
    }
    if(message!=NULL && message_len > 0)
    {
        hripemd160_update(&ctx->ctx,message,message_len);
    }
    return 0;
}

int hhmac_ripemd160_finish(hhmac_ripemd160_context_t  *ctx)
{
    if(ctx==NULL)
    {
        return -1;
    }
    hripemd160_finish(&ctx->ctx,ctx->hash_result);
    hripemd160_starts(&ctx->ctx);
    hripemd160_update(&ctx->ctx,ctx->buffer,sizeof(ctx->buffer));
    hripemd160_update(&ctx->ctx,ctx->hash_result,sizeof(ctx->hash_result));
    hripemd160_finish(&ctx->ctx,ctx->hash_result);
    return 0;
}

