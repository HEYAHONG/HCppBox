/***************************************************************
 * Name:      hmd4.c
 * Purpose:   实现hmd4接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hmd4.h"

int hmd4_starts(hmd4_context_t *ctx)
{
    if(ctx==NULL)
    {
        return -1;
    }
    memset(ctx,0,sizeof(hmd4_context_t));
    ctx->A=0x67452301;
    ctx->B=0xEFCDAB89;
    ctx->C=0x98BADCFE;
    ctx->D=0x10325476;
    return 0;
}


typedef struct hmd4_internal_state
{
    uint32_t X[16];
    uint32_t A;
    uint32_t B;
    uint32_t C;
    uint32_t D;
} hmd4_internal_state_t;
#ifdef F
#undef F
#endif // F
#ifdef G
#undef G
#endif // G
#ifdef H
#undef H
#endif // H
#ifdef ROUND
#undef ROUND
#endif // ROUND
static int hmd4_internal_process(hmd4_context_t *ctx,const hmd4_message_block_t data)
{
    if(ctx==NULL || data ==NULL)
    {
        return -1;
    }
    hmd4_internal_state_t local= {0};
    for(size_t i=0; i<sizeof(local.X)/sizeof(local.X[0]); i++)
    {
        local.X[i]=data[4*i+3];
        local.X[i] <<= 8;
        local.X[i]+=data[4*i+2];
        local.X[i] <<= 8;
        local.X[i]+=data[4*i+1];
        local.X[i] <<= 8;
        local.X[i]+=data[4*i+0];
    }


    local.A = ctx->A;
    local.B = ctx->B;
    local.C = ctx->C;
    local.D = ctx->D;


#define F(x, y, z) (((y) & (x)) | ((z) & ~(x)))
#define G(x, y, z) (((y) & (x)) | ((z) & (x)) | ((y) & (z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))

#define ROUND(f, w, x, y, z, data, s) ( w += f(x, y, z) + data,  w = w<<s | w>>(32-s) )

    ROUND(F, local.A, local.B, local.C, local.D, local.X[ 0], 3);
    ROUND(F, local.D, local.A, local.B, local.C, local.X[ 1], 7);
    ROUND(F, local.C, local.D, local.A, local.B, local.X[ 2], 11);
    ROUND(F, local.B, local.C, local.D, local.A, local.X[ 3], 19);
    ROUND(F, local.A, local.B, local.C, local.D, local.X[ 4], 3);
    ROUND(F, local.D, local.A, local.B, local.C, local.X[ 5], 7);
    ROUND(F, local.C, local.D, local.A, local.B, local.X[ 6], 11);
    ROUND(F, local.B, local.C, local.D, local.A, local.X[ 7], 19);
    ROUND(F, local.A, local.B, local.C, local.D, local.X[ 8], 3);
    ROUND(F, local.D, local.A, local.B, local.C, local.X[ 9], 7);
    ROUND(F, local.C, local.D, local.A, local.B, local.X[10], 11);
    ROUND(F, local.B, local.C, local.D, local.A, local.X[11], 19);
    ROUND(F, local.A, local.B, local.C, local.D, local.X[12], 3);
    ROUND(F, local.D, local.A, local.B, local.C, local.X[13], 7);
    ROUND(F, local.C, local.D, local.A, local.B, local.X[14], 11);
    ROUND(F, local.B, local.C, local.D, local.A, local.X[15], 19);

    ROUND(G, local.A, local.B, local.C, local.D, local.X[ 0] + 0x5a827999, 3);
    ROUND(G, local.D, local.A, local.B, local.C, local.X[ 4] + 0x5a827999, 5);
    ROUND(G, local.C, local.D, local.A, local.B, local.X[ 8] + 0x5a827999, 9);
    ROUND(G, local.B, local.C, local.D, local.A, local.X[12] + 0x5a827999, 13);
    ROUND(G, local.A, local.B, local.C, local.D, local.X[ 1] + 0x5a827999, 3);
    ROUND(G, local.D, local.A, local.B, local.C, local.X[ 5] + 0x5a827999, 5);
    ROUND(G, local.C, local.D, local.A, local.B, local.X[ 9] + 0x5a827999, 9);
    ROUND(G, local.B, local.C, local.D, local.A, local.X[13] + 0x5a827999, 13);
    ROUND(G, local.A, local.B, local.C, local.D, local.X[ 2] + 0x5a827999, 3);
    ROUND(G, local.D, local.A, local.B, local.C, local.X[ 6] + 0x5a827999, 5);
    ROUND(G, local.C, local.D, local.A, local.B, local.X[10] + 0x5a827999, 9);
    ROUND(G, local.B, local.C, local.D, local.A, local.X[14] + 0x5a827999, 13);
    ROUND(G, local.A, local.B, local.C, local.D, local.X[ 3] + 0x5a827999, 3);
    ROUND(G, local.D, local.A, local.B, local.C, local.X[ 7] + 0x5a827999, 5);
    ROUND(G, local.C, local.D, local.A, local.B, local.X[11] + 0x5a827999, 9);
    ROUND(G, local.B, local.C, local.D, local.A, local.X[15] + 0x5a827999, 13);

    ROUND(H, local.A, local.B, local.C, local.D, local.X[ 0] + 0x6ed9eba1, 3);
    ROUND(H, local.D, local.A, local.B, local.C, local.X[ 8] + 0x6ed9eba1, 9);
    ROUND(H, local.C, local.D, local.A, local.B, local.X[ 4] + 0x6ed9eba1, 11);
    ROUND(H, local.B, local.C, local.D, local.A, local.X[12] + 0x6ed9eba1, 15);
    ROUND(H, local.A, local.B, local.C, local.D, local.X[ 2] + 0x6ed9eba1, 3);
    ROUND(H, local.D, local.A, local.B, local.C, local.X[10] + 0x6ed9eba1, 9);
    ROUND(H, local.C, local.D, local.A, local.B, local.X[ 6] + 0x6ed9eba1, 11);
    ROUND(H, local.B, local.C, local.D, local.A, local.X[14] + 0x6ed9eba1, 15);
    ROUND(H, local.A, local.B, local.C, local.D, local.X[ 1] + 0x6ed9eba1, 3);
    ROUND(H, local.D, local.A, local.B, local.C, local.X[ 9] + 0x6ed9eba1, 9);
    ROUND(H, local.C, local.D, local.A, local.B, local.X[ 5] + 0x6ed9eba1, 11);
    ROUND(H, local.B, local.C, local.D, local.A, local.X[13] + 0x6ed9eba1, 15);
    ROUND(H, local.A, local.B, local.C, local.D, local.X[ 3] + 0x6ed9eba1, 3);
    ROUND(H, local.D, local.A, local.B, local.C, local.X[11] + 0x6ed9eba1, 9);
    ROUND(H, local.C, local.D, local.A, local.B, local.X[ 7] + 0x6ed9eba1, 11);
    ROUND(H, local.B, local.C, local.D, local.A, local.X[15] + 0x6ed9eba1, 15);

#undef F
#undef G
#undef H
#undef ROUND
    ctx->A += local.A;
    ctx->B += local.B;
    ctx->C += local.C;
    ctx->D += local.D;

    return 0;
}

int hmd4_update(hmd4_context_t *ctx,const uint8_t *input,size_t ilen)
{
    int ret = -1;
    if(ctx == NULL )
    {
        return ret;
    }
    size_t fill=0;
    uint32_t left=0;

    if (input ==NULL || ilen == 0)
    {
        return 0;
    }

    left = ctx->total[0] & (sizeof(ctx->buffer)-1);
    fill = (sizeof(ctx->buffer)) - left;

    ctx->total[0] += (uint32_t) ilen;
    ctx->total[0] &= 0xFFFFFFFF;

    if (ctx->total[0] < (uint32_t) ilen)
    {
        ctx->total[1]++;
    }

    if (left && ilen >= fill)
    {
        memcpy((void *) (ctx->buffer + left), input, fill);
        if ((ret = hmd4_internal_process(ctx, ctx->buffer)) != 0)
        {
            return ret;
        }

        input += fill;
        ilen  -= fill;
        left = 0;
    }

    while (ilen >= (sizeof(ctx->buffer)))
    {
        if ((ret = hmd4_internal_process(ctx, input)) != 0)
        {
            return ret;
        }

        input += (sizeof(ctx->buffer));
        ilen  -= (sizeof(ctx->buffer));
    }

    if (ilen > 0)
    {
        memcpy((void *) (ctx->buffer + left), input, ilen);
    }

    return 0;
}

int hmd4_finish(hmd4_context_t *ctx,hmd4_md4_t output)
{
    int ret = -1;
    if(ctx==NULL || output == NULL)
    {
        return ret;
    }
    uint32_t used=0;
    uint32_t high=0, low=0;

    /*
     * 添加填充
     */
    used = ctx->total[0] & (sizeof(ctx->buffer)-1);

    ctx->buffer[used++] = 0x80;

    if (used <= (sizeof(ctx->buffer)-sizeof(ctx->total)))
    {
        /* 末尾足够放长度 */
        memset(ctx->buffer + used, 0, (sizeof(ctx->buffer)-sizeof(ctx->total)) - used);
    }
    else
    {
        /* 需要一个额外的块 */
        memset(ctx->buffer + used, 0, (sizeof(ctx->buffer)) - used);

        if ((ret = hmd4_internal_process(ctx, ctx->buffer)) != 0)
        {
            return ret;
        }

        memset(ctx->buffer, 0, (sizeof(ctx->buffer)-sizeof(ctx->total)));
    }

    /*
     * 添加消息长度
     */
    high = (ctx->total[0] >> 29)| (ctx->total[1] <<  3);
    low  = (ctx->total[0] <<  3);

    {
        ctx->buffer[56+0]=((low)&0xFF);
        ctx->buffer[56+1]=((low>>8)&0xFF);
        ctx->buffer[56+2]=((low>>16)&0xFF);
        ctx->buffer[56+3]=((low>>24)&0xFF);
    }
    {
        ctx->buffer[60+0]=((high)&0xFF);
        ctx->buffer[60+1]=((high>>8)&0xFF);
        ctx->buffer[60+2]=((high>>16)&0xFF);
        ctx->buffer[60+3]=((high>>24)&0xFF);
    }

    if ((ret = hmd4_internal_process(ctx, ctx->buffer)) != 0)
    {
        return ret;
    }

    /*
     * 输出
     */
    {
        output[0+0]=((ctx->A)&0xFF);
        output[0+1]=((ctx->A>>8)&0xFF);
        output[0+2]=((ctx->A>>16)&0xFF);
        output[0+3]=((ctx->A>>24)&0xFF);
    }
    {
        output[4+0]=((ctx->B)&0xFF);
        output[4+1]=((ctx->B>>8)&0xFF);
        output[4+2]=((ctx->B>>16)&0xFF);
        output[4+3]=((ctx->B>>24)&0xFF);
    }
    {
        output[8+0]=((ctx->C)&0xFF);
        output[8+1]=((ctx->C>>8)&0xFF);
        output[8+2]=((ctx->C>>16)&0xFF);
        output[8+3]=((ctx->C>>24)&0xFF);
    }
    {
        output[12+0]=((ctx->D)&0xFF);
        output[12+1]=((ctx->D>>8)&0xFF);
        output[12+2]=((ctx->D>>16)&0xFF);
        output[12+3]=((ctx->D>>24)&0xFF);
    }

    ret = 0;
    return ret;
}

int hmd4_md4(const uint8_t *input,size_t ilen,hmd4_md4_t output)
{
    int ret = -1;
    if(input==NULL || output == NULL)
    {
        return ret;
    }
    hmd4_context_t ctx;

    if ((ret = hmd4_starts(&ctx)) != 0)
    {
        goto exit;
    }

    if ((ret = hmd4_update(&ctx, input, ilen)) != 0)
    {
        goto exit;
    }

    if ((ret = hmd4_finish(&ctx, output)) != 0)
    {
        goto exit;
    }

exit:
    return ret;
}

