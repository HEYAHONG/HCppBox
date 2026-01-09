/***************************************************************
 * Name:      hrng_chacha20.c
 * Purpose:   实现hrng_chacha20接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-09
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hrng_chacha20.h"
#include "hrng_linearcongruential.h"

hrng_chacha20_context_t hrng_chacha20_context_default=
{
    false,
    {
        0
    }
};

const  uint8_t hrng_chacha20_key[32]=
{
    0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
    0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
    0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
    0x55,0x55
};
const  uint8_t hrng_chacha20_nonce[12]=
{
    0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,
    0xAA,0xAA
};
static void hrng_chacha20_init(hrng_chacha20_context_t *ctx)
{
    if(ctx==NULL)
    {
        ctx=&hrng_chacha20_context_default;
    }
    if(!ctx->ctx_init)
    {
        hrng_chacha20_srand(ctx,hrng_chacha20_key,hrng_chacha20_nonce,hrng_linearcongruential_rand48_rand());
    }
}
void   hrng_chacha20_rand(hrng_chacha20_context_t *ctx,void *buffer,size_t buffer_length)
{
    if(ctx==NULL)
    {
        ctx=&hrng_chacha20_context_default;
    }
    hrng_chacha20_init(ctx);
    hchacha20_update(&ctx->ctx,buffer_length,(uint8_t *)buffer,(uint8_t *)buffer);
}

bool   hrng_chacha20_srand(hrng_chacha20_context_t *ctx,const uint8_t key[32],const uint8_t nonce[12],uint32_t conuter)
{
    if(ctx==NULL)
    {
        ctx=&hrng_chacha20_context_default;
    }
    if(key==NULL || nonce == NULL)
    {
        return false;
    }
    memset(ctx,0,sizeof(*ctx));
    ctx->ctx_init=true;
    {
        hchacha20_setkey(&ctx->ctx,key);
    }
    {
        hchacha20_starts(&ctx->ctx,nonce,conuter);
    }

    return true;
}
