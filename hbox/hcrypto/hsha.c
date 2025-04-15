/***************************************************************
 * Name:      hsha.c
 * Purpose:   实现hsha接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hsha.h"

int hsha1_starts(hsha1_context_t *ctx)
{
    if(ctx==NULL)
    {
        return -1;
    }
    memset(ctx,0,sizeof(hsha1_context_t));
    ctx->A=0x67452301;
    ctx->B=0xEFCDAB89;
    ctx->C=0x98BADCFE;
    ctx->D=0x10325476;
    ctx->E=0xC3D2E1F0;
    return 0;
}


typedef struct hsha1_internal_state
{
    uint32_t W[16];
    uint32_t A;
    uint32_t B;
    uint32_t C;
    uint32_t D;
    uint32_t E;
    uint32_t temp;
} hsha1_internal_state_t;
#ifdef P
#undef P
#endif // P
#ifdef S
#undef S
#endif // S
#ifdef F
#undef F
#endif // F
#ifdef R
#undef R
#endif // R
#ifdef K
#undef K
#endif // K
static int hsha1_internal_process(hsha1_context_t *ctx,const hsha1_message_block_t data)
{
    if(ctx==NULL || data ==NULL)
    {
        return -1;
    }
    hsha1_internal_state_t local= {0};
    for(size_t i=0; i<sizeof(local.W)/sizeof(local.W[0]); i++)
    {
        local.W[i]=data[4*i+0];
        local.W[i] <<= 8;
        local.W[i]+=data[4*i+1];
        local.W[i] <<= 8;
        local.W[i]+=data[4*i+2];
        local.W[i] <<= 8;
        local.W[i]+=data[4*i+3];
    }

#define S(x, n) (((x) << (n)) | (((x) & 0xFFFFFFFF) >> (32 - (n))))

#define R(t)                                                            \
    (                                                                   \
        local.temp = local.W[((t) -  3) & 0x0F] ^                       \
                     local.W[((t) -  8) & 0x0F] ^                       \
                     local.W[((t) - 14) & 0x0F] ^                       \
                     local.W[(t)        & 0x0F],                        \
        (local.W[(t) & 0x0F] = S(local.temp, 1))                        \
    )

#define P(a, b, c, d, e, x)                                             \
    do                                                                  \
    {                                                                   \
        (e) += S((a), 5) + F((b), (c), (d)) + K + (x);                  \
        (b) = S((b), 30);                                               \
    } while (0)

    local.A = ctx->A;
    local.B = ctx->B;
    local.C = ctx->C;
    local.D = ctx->D;
    local.E = ctx->E;

#define F(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#define K 0x5A827999

    P(local.A, local.B, local.C, local.D, local.E, local.W[0]);
    P(local.E, local.A, local.B, local.C, local.D, local.W[1]);
    P(local.D, local.E, local.A, local.B, local.C, local.W[2]);
    P(local.C, local.D, local.E, local.A, local.B, local.W[3]);
    P(local.B, local.C, local.D, local.E, local.A, local.W[4]);
    P(local.A, local.B, local.C, local.D, local.E, local.W[5]);
    P(local.E, local.A, local.B, local.C, local.D, local.W[6]);
    P(local.D, local.E, local.A, local.B, local.C, local.W[7]);
    P(local.C, local.D, local.E, local.A, local.B, local.W[8]);
    P(local.B, local.C, local.D, local.E, local.A, local.W[9]);
    P(local.A, local.B, local.C, local.D, local.E, local.W[10]);
    P(local.E, local.A, local.B, local.C, local.D, local.W[11]);
    P(local.D, local.E, local.A, local.B, local.C, local.W[12]);
    P(local.C, local.D, local.E, local.A, local.B, local.W[13]);
    P(local.B, local.C, local.D, local.E, local.A, local.W[14]);
    P(local.A, local.B, local.C, local.D, local.E, local.W[15]);
    P(local.E, local.A, local.B, local.C, local.D, R(16));
    P(local.D, local.E, local.A, local.B, local.C, R(17));
    P(local.C, local.D, local.E, local.A, local.B, R(18));
    P(local.B, local.C, local.D, local.E, local.A, R(19));

#undef K
#undef F

#define F(x, y, z) ((x) ^ (y) ^ (z))
#define K 0x6ED9EBA1

    P(local.A, local.B, local.C, local.D, local.E, R(20));
    P(local.E, local.A, local.B, local.C, local.D, R(21));
    P(local.D, local.E, local.A, local.B, local.C, R(22));
    P(local.C, local.D, local.E, local.A, local.B, R(23));
    P(local.B, local.C, local.D, local.E, local.A, R(24));
    P(local.A, local.B, local.C, local.D, local.E, R(25));
    P(local.E, local.A, local.B, local.C, local.D, R(26));
    P(local.D, local.E, local.A, local.B, local.C, R(27));
    P(local.C, local.D, local.E, local.A, local.B, R(28));
    P(local.B, local.C, local.D, local.E, local.A, R(29));
    P(local.A, local.B, local.C, local.D, local.E, R(30));
    P(local.E, local.A, local.B, local.C, local.D, R(31));
    P(local.D, local.E, local.A, local.B, local.C, R(32));
    P(local.C, local.D, local.E, local.A, local.B, R(33));
    P(local.B, local.C, local.D, local.E, local.A, R(34));
    P(local.A, local.B, local.C, local.D, local.E, R(35));
    P(local.E, local.A, local.B, local.C, local.D, R(36));
    P(local.D, local.E, local.A, local.B, local.C, R(37));
    P(local.C, local.D, local.E, local.A, local.B, R(38));
    P(local.B, local.C, local.D, local.E, local.A, R(39));

#undef K
#undef F

#define F(x, y, z) (((x) & (y)) | ((z) & ((x) | (y))))
#define K 0x8F1BBCDC

    P(local.A, local.B, local.C, local.D, local.E, R(40));
    P(local.E, local.A, local.B, local.C, local.D, R(41));
    P(local.D, local.E, local.A, local.B, local.C, R(42));
    P(local.C, local.D, local.E, local.A, local.B, R(43));
    P(local.B, local.C, local.D, local.E, local.A, R(44));
    P(local.A, local.B, local.C, local.D, local.E, R(45));
    P(local.E, local.A, local.B, local.C, local.D, R(46));
    P(local.D, local.E, local.A, local.B, local.C, R(47));
    P(local.C, local.D, local.E, local.A, local.B, R(48));
    P(local.B, local.C, local.D, local.E, local.A, R(49));
    P(local.A, local.B, local.C, local.D, local.E, R(50));
    P(local.E, local.A, local.B, local.C, local.D, R(51));
    P(local.D, local.E, local.A, local.B, local.C, R(52));
    P(local.C, local.D, local.E, local.A, local.B, R(53));
    P(local.B, local.C, local.D, local.E, local.A, R(54));
    P(local.A, local.B, local.C, local.D, local.E, R(55));
    P(local.E, local.A, local.B, local.C, local.D, R(56));
    P(local.D, local.E, local.A, local.B, local.C, R(57));
    P(local.C, local.D, local.E, local.A, local.B, R(58));
    P(local.B, local.C, local.D, local.E, local.A, R(59));

#undef K
#undef F

#define F(x, y, z) ((x) ^ (y) ^ (z))
#define K 0xCA62C1D6

    P(local.A, local.B, local.C, local.D, local.E, R(60));
    P(local.E, local.A, local.B, local.C, local.D, R(61));
    P(local.D, local.E, local.A, local.B, local.C, R(62));
    P(local.C, local.D, local.E, local.A, local.B, R(63));
    P(local.B, local.C, local.D, local.E, local.A, R(64));
    P(local.A, local.B, local.C, local.D, local.E, R(65));
    P(local.E, local.A, local.B, local.C, local.D, R(66));
    P(local.D, local.E, local.A, local.B, local.C, R(67));
    P(local.C, local.D, local.E, local.A, local.B, R(68));
    P(local.B, local.C, local.D, local.E, local.A, R(69));
    P(local.A, local.B, local.C, local.D, local.E, R(70));
    P(local.E, local.A, local.B, local.C, local.D, R(71));
    P(local.D, local.E, local.A, local.B, local.C, R(72));
    P(local.C, local.D, local.E, local.A, local.B, R(73));
    P(local.B, local.C, local.D, local.E, local.A, R(74));
    P(local.A, local.B, local.C, local.D, local.E, R(75));
    P(local.E, local.A, local.B, local.C, local.D, R(76));
    P(local.D, local.E, local.A, local.B, local.C, R(77));
    P(local.C, local.D, local.E, local.A, local.B, R(78));
    P(local.B, local.C, local.D, local.E, local.A, R(79));

#undef K
#undef F
#undef R
#undef S
#undef P

    ctx->A += local.A;
    ctx->B += local.B;
    ctx->C += local.C;
    ctx->D += local.D;
    ctx->E += local.E;

    return 0;
}

int hsha1_update(hsha1_context_t *ctx,const uint8_t *input,size_t ilen)
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

    left = ctx->total[0] & 0x3F;
    fill = 64 - left;

    ctx->total[0] += (uint32_t) ilen;
    ctx->total[0] &= 0xFFFFFFFF;

    if (ctx->total[0] < (uint32_t) ilen)
    {
        ctx->total[1]++;
    }

    if (left && ilen >= fill)
    {
        memcpy((void *) (ctx->buffer + left), input, fill);
        if ((ret = hsha1_internal_process(ctx, ctx->buffer)) != 0)
        {
            return ret;
        }

        input += fill;
        ilen  -= fill;
        left = 0;
    }

    while (ilen >= 64)
    {
        if ((ret = hsha1_internal_process(ctx, input)) != 0)
        {
            return ret;
        }

        input += 64;
        ilen  -= 64;
    }

    if (ilen > 0)
    {
        memcpy((void *) (ctx->buffer + left), input, ilen);
    }

    return 0;
}

int hsha1_finish(hsha1_context_t *ctx,hsha1_sha_t output)
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
    used = ctx->total[0] & 0x3F;

    ctx->buffer[used++] = 0x80;

    if (used <= 56)
    {
        /* 末尾足够放长度 */
        memset(ctx->buffer + used, 0, 56 - used);
    }
    else
    {
        /* 需要一个额外的块 */
        memset(ctx->buffer + used, 0, 64 - used);

        if ((ret = hsha1_internal_process(ctx, ctx->buffer)) != 0)
        {
            return ret;
        }

        memset(ctx->buffer, 0, 56);
    }

    /*
     * 添加消息长度
     */
    high = (ctx->total[0] >> 29)| (ctx->total[1] <<  3);
    low  = (ctx->total[0] <<  3);

    {
        ctx->buffer[56+3]=((high)&0xFF);
        ctx->buffer[56+2]=((high>>8)&0xFF);
        ctx->buffer[56+1]=((high>>16)&0xFF);
        ctx->buffer[56+0]=((high>>24)&0xFF);
    }
    {
        ctx->buffer[60+3]=((low)&0xFF);
        ctx->buffer[60+2]=((low>>8)&0xFF);
        ctx->buffer[60+1]=((low>>16)&0xFF);
        ctx->buffer[60+0]=((low>>24)&0xFF);
    }

    if ((ret = hsha1_internal_process(ctx, ctx->buffer)) != 0)
    {
        return ret;
    }

    /*
     * 输出
     */
    {
        output[0+3]=((ctx->A)&0xFF);
        output[0+2]=((ctx->A>>8)&0xFF);
        output[0+1]=((ctx->A>>16)&0xFF);
        output[0+0]=((ctx->A>>24)&0xFF);
    }
    {
        output[4+3]=((ctx->B)&0xFF);
        output[4+2]=((ctx->B>>8)&0xFF);
        output[4+1]=((ctx->B>>16)&0xFF);
        output[4+0]=((ctx->B>>24)&0xFF);
    }
    {
        output[8+3]=((ctx->C)&0xFF);
        output[8+2]=((ctx->C>>8)&0xFF);
        output[8+1]=((ctx->C>>16)&0xFF);
        output[8+0]=((ctx->C>>24)&0xFF);
    }
    {
        output[12+3]=((ctx->D)&0xFF);
        output[12+2]=((ctx->D>>8)&0xFF);
        output[12+1]=((ctx->D>>16)&0xFF);
        output[12+0]=((ctx->D>>24)&0xFF);
    }
    {
        output[16+3]=((ctx->E)&0xFF);
        output[16+2]=((ctx->E>>8)&0xFF);
        output[16+1]=((ctx->E>>16)&0xFF);
        output[16+0]=((ctx->E>>24)&0xFF);
    }

    ret = 0;
    return ret;
}

int hsha1_sha(const uint8_t *input,size_t ilen,hsha1_sha_t output)
{
    int ret = -1;
    if(input==NULL || output == NULL)
    {
        return ret;
    }
    hsha1_context_t ctx;

    if ((ret = hsha1_starts(&ctx)) != 0)
    {
        goto exit;
    }

    if ((ret = hsha1_update(&ctx, input, ilen)) != 0)
    {
        goto exit;
    }

    if ((ret = hsha1_finish(&ctx, output)) != 0)
    {
        goto exit;
    }

exit:
    return ret;
}




