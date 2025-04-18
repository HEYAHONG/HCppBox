/***************************************************************
 * Name:      hmd5.c
 * Purpose:   实现hmd5接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hmd5.h"

int hmd5_starts(hmd5_context_t *ctx)
{
    if(ctx==NULL)
    {
        return -1;
    }
    memset(ctx,0,sizeof(hmd5_context_t));
    ctx->A=0x67452301;
    ctx->B=0xEFCDAB89;
    ctx->C=0x98BADCFE;
    ctx->D=0x10325476;
    return 0;
}


typedef struct hmd5_internal_state
{
    uint32_t X[16];
    uint32_t A;
    uint32_t B;
    uint32_t C;
    uint32_t D;
} hmd5_internal_state_t;
#ifdef P
#undef P
#endif // P
#ifdef S
#undef S
#endif // S
#ifdef F
#undef F
#endif // F
static int hmd5_internal_process(hmd5_context_t *ctx,const hmd5_message_block_t data)
{
    if(ctx==NULL || data ==NULL)
    {
        return -1;
    }
    hmd5_internal_state_t local= {0};
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

#define S(x, n)                                                                 \
    (((x) << (n)) | (((x) & 0xFFFFFFFF) >> (32 - (n))))

#define P(a, b, c, d, k, s, t)                                                  \
    do                                                                          \
    {                                                                           \
        (a) += F((b), (c), (d)) + local.X[(k)] + (t);                           \
        (a) = S((a), (s)) + (b);                                                \
    } while (0)

    local.A = ctx->A;
    local.B = ctx->B;
    local.C = ctx->C;
    local.D = ctx->D;

#define F(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))

    P(local.A, local.B, local.C, local.D,  0,  7, 0xD76AA478);
    P(local.D, local.A, local.B, local.C,  1, 12, 0xE8C7B756);
    P(local.C, local.D, local.A, local.B,  2, 17, 0x242070DB);
    P(local.B, local.C, local.D, local.A,  3, 22, 0xC1BDCEEE);
    P(local.A, local.B, local.C, local.D,  4,  7, 0xF57C0FAF);
    P(local.D, local.A, local.B, local.C,  5, 12, 0x4787C62A);
    P(local.C, local.D, local.A, local.B,  6, 17, 0xA8304613);
    P(local.B, local.C, local.D, local.A,  7, 22, 0xFD469501);
    P(local.A, local.B, local.C, local.D,  8,  7, 0x698098D8);
    P(local.D, local.A, local.B, local.C,  9, 12, 0x8B44F7AF);
    P(local.C, local.D, local.A, local.B, 10, 17, 0xFFFF5BB1);
    P(local.B, local.C, local.D, local.A, 11, 22, 0x895CD7BE);
    P(local.A, local.B, local.C, local.D, 12,  7, 0x6B901122);
    P(local.D, local.A, local.B, local.C, 13, 12, 0xFD987193);
    P(local.C, local.D, local.A, local.B, 14, 17, 0xA679438E);
    P(local.B, local.C, local.D, local.A, 15, 22, 0x49B40821);

#undef F

#define F(x, y, z) ((y) ^ ((z) & ((x) ^ (y))))

    P(local.A, local.B, local.C, local.D,  1,  5, 0xF61E2562);
    P(local.D, local.A, local.B, local.C,  6,  9, 0xC040B340);
    P(local.C, local.D, local.A, local.B, 11, 14, 0x265E5A51);
    P(local.B, local.C, local.D, local.A,  0, 20, 0xE9B6C7AA);
    P(local.A, local.B, local.C, local.D,  5,  5, 0xD62F105D);
    P(local.D, local.A, local.B, local.C, 10,  9, 0x02441453);
    P(local.C, local.D, local.A, local.B, 15, 14, 0xD8A1E681);
    P(local.B, local.C, local.D, local.A,  4, 20, 0xE7D3FBC8);
    P(local.A, local.B, local.C, local.D,  9,  5, 0x21E1CDE6);
    P(local.D, local.A, local.B, local.C, 14,  9, 0xC33707D6);
    P(local.C, local.D, local.A, local.B,  3, 14, 0xF4D50D87);
    P(local.B, local.C, local.D, local.A,  8, 20, 0x455A14ED);
    P(local.A, local.B, local.C, local.D, 13,  5, 0xA9E3E905);
    P(local.D, local.A, local.B, local.C,  2,  9, 0xFCEFA3F8);
    P(local.C, local.D, local.A, local.B,  7, 14, 0x676F02D9);
    P(local.B, local.C, local.D, local.A, 12, 20, 0x8D2A4C8A);

#undef F

#define F(x, y, z) ((x) ^ (y) ^ (z))

    P(local.A, local.B, local.C, local.D,  5,  4, 0xFFFA3942);
    P(local.D, local.A, local.B, local.C,  8, 11, 0x8771F681);
    P(local.C, local.D, local.A, local.B, 11, 16, 0x6D9D6122);
    P(local.B, local.C, local.D, local.A, 14, 23, 0xFDE5380C);
    P(local.A, local.B, local.C, local.D,  1,  4, 0xA4BEEA44);
    P(local.D, local.A, local.B, local.C,  4, 11, 0x4BDECFA9);
    P(local.C, local.D, local.A, local.B,  7, 16, 0xF6BB4B60);
    P(local.B, local.C, local.D, local.A, 10, 23, 0xBEBFBC70);
    P(local.A, local.B, local.C, local.D, 13,  4, 0x289B7EC6);
    P(local.D, local.A, local.B, local.C,  0, 11, 0xEAA127FA);
    P(local.C, local.D, local.A, local.B,  3, 16, 0xD4EF3085);
    P(local.B, local.C, local.D, local.A,  6, 23, 0x04881D05);
    P(local.A, local.B, local.C, local.D,  9,  4, 0xD9D4D039);
    P(local.D, local.A, local.B, local.C, 12, 11, 0xE6DB99E5);
    P(local.C, local.D, local.A, local.B, 15, 16, 0x1FA27CF8);
    P(local.B, local.C, local.D, local.A,  2, 23, 0xC4AC5665);

#undef F

#define F(x, y, z) ((y) ^ ((x) | ~(z)))

    P(local.A, local.B, local.C, local.D,  0,  6, 0xF4292244);
    P(local.D, local.A, local.B, local.C,  7, 10, 0x432AFF97);
    P(local.C, local.D, local.A, local.B, 14, 15, 0xAB9423A7);
    P(local.B, local.C, local.D, local.A,  5, 21, 0xFC93A039);
    P(local.A, local.B, local.C, local.D, 12,  6, 0x655B59C3);
    P(local.D, local.A, local.B, local.C,  3, 10, 0x8F0CCC92);
    P(local.C, local.D, local.A, local.B, 10, 15, 0xFFEFF47D);
    P(local.B, local.C, local.D, local.A,  1, 21, 0x85845DD1);
    P(local.A, local.B, local.C, local.D,  8,  6, 0x6FA87E4F);
    P(local.D, local.A, local.B, local.C, 15, 10, 0xFE2CE6E0);
    P(local.C, local.D, local.A, local.B,  6, 15, 0xA3014314);
    P(local.B, local.C, local.D, local.A, 13, 21, 0x4E0811A1);
    P(local.A, local.B, local.C, local.D,  4,  6, 0xF7537E82);
    P(local.D, local.A, local.B, local.C, 11, 10, 0xBD3AF235);
    P(local.C, local.D, local.A, local.B,  2, 15, 0x2AD7D2BB);
    P(local.B, local.C, local.D, local.A,  9, 21, 0xEB86D391);

#undef F
#undef P
#undef S

    ctx->A += local.A;
    ctx->B += local.B;
    ctx->C += local.C;
    ctx->D += local.D;

    return 0;
}

int hmd5_update(hmd5_context_t *ctx,const uint8_t *input,size_t ilen)
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
        if ((ret = hmd5_internal_process(ctx, ctx->buffer)) != 0)
        {
            return ret;
        }

        input += fill;
        ilen  -= fill;
        left = 0;
    }

    while (ilen >= (sizeof(ctx->buffer)))
    {
        if ((ret = hmd5_internal_process(ctx, input)) != 0)
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

int hmd5_finish(hmd5_context_t *ctx,hmd5_md5_t output)
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

        if ((ret = hmd5_internal_process(ctx, ctx->buffer)) != 0)
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

    if ((ret = hmd5_internal_process(ctx, ctx->buffer)) != 0)
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

int hmd5_md5(const uint8_t *input,size_t ilen,hmd5_md5_t output)
{
    int ret = -1;
    if(input==NULL || output == NULL)
    {
        return ret;
    }
    hmd5_context_t ctx;

    if ((ret = hmd5_starts(&ctx)) != 0)
    {
        goto exit;
    }

    if ((ret = hmd5_update(&ctx, input, ilen)) != 0)
    {
        goto exit;
    }

    if ((ret = hmd5_finish(&ctx, output)) != 0)
    {
        goto exit;
    }

exit:
    return ret;
}

