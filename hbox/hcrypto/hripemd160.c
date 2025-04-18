/***************************************************************
 * Name:      hripemd160.c
 * Purpose:   实现hripemd160接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hripemd160.h"

int hripemd160_starts(hripemd160_context_t *ctx)
{
    if(ctx==NULL)
    {
        return -1;
    }
    memset(ctx,0,sizeof(hripemd160_context_t));
    ctx->A=0x67452301;
    ctx->B=0xEFCDAB89;
    ctx->C=0x98BADCFE;
    ctx->D=0x10325476;
    ctx->E=0xC3D2E1F0;
    return 0;
}


typedef struct hripemd160_internal_state
{
    uint32_t X[16];
    uint32_t A;
    uint32_t B;
    uint32_t C;
    uint32_t D;
    uint32_t E;
    uint32_t Ap;
    uint32_t Bp;
    uint32_t Cp;
    uint32_t Dp;
    uint32_t Ep;
} hripemd160_internal_state_t;
#ifdef P
#undef P
#endif // P
#ifdef P2
#undef P2
#endif // P2
#ifdef S
#undef S
#endif // S
#ifdef F
#undef F
#endif // F
#ifdef F1
#undef F1
#endif // F1
#ifdef F2
#undef F2
#endif // F2
#ifdef F3
#undef F3
#endif // F3
#ifdef F4
#undef F4
#endif // F4
#ifdef F5
#undef F5
#endif // F5
#ifdef Fp
#undef Fp
#endif // Fp
#ifdef K
#undef K
#endif // K
#ifdef Kp
#undef Kp
#endif // Kp
static int hripemd160_internal_process(hripemd160_context_t *ctx,const hripemd160_message_block_t data)
{
    if(ctx==NULL || data ==NULL)
    {
        return -1;
    }
    hripemd160_internal_state_t local= {0};
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
    local.E = ctx->E;
    local.Ap = ctx->A;
    local.Bp = ctx->B;
    local.Cp = ctx->C;
    local.Dp = ctx->D;
    local.Ep = ctx->E;

#define F1(x, y, z)   ((x) ^ (y) ^ (z))
#define F2(x, y, z)   (((x) & (y)) | (~(x) & (z)))
#define F3(x, y, z)   (((x) | ~(y)) ^ (z))
#define F4(x, y, z)   (((x) & (z)) | ((y) & ~(z)))
#define F5(x, y, z)   ((x) ^ ((y) | ~(z)))

#define S(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

#define P(a, b, c, d, e, r, s, f, k)                                    \
    do                                                                  \
    {                                                                   \
        (a) += f((b), (c), (d)) + local.X[r] + (k);                     \
        (a) = S((a), (s)) + (e);                                        \
        (c) = S((c), 10);                                               \
    } while (0)

#define P2(a, b, c, d, e, r, s, rp, sp)                                 \
    do                                                                  \
    {                                                                   \
        P((a), (b), (c), (d), (e), (r), (s), F, K);                     \
        P(a ## p, b ## p, c ## p, d ## p, e ## p,                       \
          (rp), (sp), Fp, Kp);                                          \
    } while (0)

#define F   F1
#define K   0x00000000
#define Fp  F5
#define Kp  0x50A28BE6
    P2(local.A, local.B, local.C, local.D, local.E,  0, 11,  5,  8);
    P2(local.E, local.A, local.B, local.C, local.D,  1, 14, 14,  9);
    P2(local.D, local.E, local.A, local.B, local.C,  2, 15,  7,  9);
    P2(local.C, local.D, local.E, local.A, local.B,  3, 12,  0, 11);
    P2(local.B, local.C, local.D, local.E, local.A,  4,  5,  9, 13);
    P2(local.A, local.B, local.C, local.D, local.E,  5,  8,  2, 15);
    P2(local.E, local.A, local.B, local.C, local.D,  6,  7, 11, 15);
    P2(local.D, local.E, local.A, local.B, local.C,  7,  9,  4,  5);
    P2(local.C, local.D, local.E, local.A, local.B,  8, 11, 13,  7);
    P2(local.B, local.C, local.D, local.E, local.A,  9, 13,  6,  7);
    P2(local.A, local.B, local.C, local.D, local.E, 10, 14, 15,  8);
    P2(local.E, local.A, local.B, local.C, local.D, 11, 15,  8, 11);
    P2(local.D, local.E, local.A, local.B, local.C, 12,  6,  1, 14);
    P2(local.C, local.D, local.E, local.A, local.B, 13,  7, 10, 14);
    P2(local.B, local.C, local.D, local.E, local.A, 14,  9,  3, 12);
    P2(local.A, local.B, local.C, local.D, local.E, 15,  8, 12,  6);
#undef F
#undef K
#undef Fp
#undef Kp

#define F   F2
#define K   0x5A827999
#define Fp  F4
#define Kp  0x5C4DD124
    P2(local.E, local.A, local.B, local.C, local.D,  7,  7,  6,  9);
    P2(local.D, local.E, local.A, local.B, local.C,  4,  6, 11, 13);
    P2(local.C, local.D, local.E, local.A, local.B, 13,  8,  3, 15);
    P2(local.B, local.C, local.D, local.E, local.A,  1, 13,  7,  7);
    P2(local.A, local.B, local.C, local.D, local.E, 10, 11,  0, 12);
    P2(local.E, local.A, local.B, local.C, local.D,  6,  9, 13,  8);
    P2(local.D, local.E, local.A, local.B, local.C, 15,  7,  5,  9);
    P2(local.C, local.D, local.E, local.A, local.B,  3, 15, 10, 11);
    P2(local.B, local.C, local.D, local.E, local.A, 12,  7, 14,  7);
    P2(local.A, local.B, local.C, local.D, local.E,  0, 12, 15,  7);
    P2(local.E, local.A, local.B, local.C, local.D,  9, 15,  8, 12);
    P2(local.D, local.E, local.A, local.B, local.C,  5,  9, 12,  7);
    P2(local.C, local.D, local.E, local.A, local.B,  2, 11,  4,  6);
    P2(local.B, local.C, local.D, local.E, local.A, 14,  7,  9, 15);
    P2(local.A, local.B, local.C, local.D, local.E, 11, 13,  1, 13);
    P2(local.E, local.A, local.B, local.C, local.D,  8, 12,  2, 11);
#undef F
#undef K
#undef Fp
#undef Kp

#define F   F3
#define K   0x6ED9EBA1
#define Fp  F3
#define Kp  0x6D703EF3
    P2(local.D, local.E, local.A, local.B, local.C,  3, 11, 15,  9);
    P2(local.C, local.D, local.E, local.A, local.B, 10, 13,  5,  7);
    P2(local.B, local.C, local.D, local.E, local.A, 14,  6,  1, 15);
    P2(local.A, local.B, local.C, local.D, local.E,  4,  7,  3, 11);
    P2(local.E, local.A, local.B, local.C, local.D,  9, 14,  7,  8);
    P2(local.D, local.E, local.A, local.B, local.C, 15,  9, 14,  6);
    P2(local.C, local.D, local.E, local.A, local.B,  8, 13,  6,  6);
    P2(local.B, local.C, local.D, local.E, local.A,  1, 15,  9, 14);
    P2(local.A, local.B, local.C, local.D, local.E,  2, 14, 11, 12);
    P2(local.E, local.A, local.B, local.C, local.D,  7,  8,  8, 13);
    P2(local.D, local.E, local.A, local.B, local.C,  0, 13, 12,  5);
    P2(local.C, local.D, local.E, local.A, local.B,  6,  6,  2, 14);
    P2(local.B, local.C, local.D, local.E, local.A, 13,  5, 10, 13);
    P2(local.A, local.B, local.C, local.D, local.E, 11, 12,  0, 13);
    P2(local.E, local.A, local.B, local.C, local.D,  5,  7,  4,  7);
    P2(local.D, local.E, local.A, local.B, local.C, 12,  5, 13,  5);
#undef F
#undef K
#undef Fp
#undef Kp

#define F   F4
#define K   0x8F1BBCDC
#define Fp  F2
#define Kp  0x7A6D76E9
    P2(local.C, local.D, local.E, local.A, local.B,  1, 11,  8, 15);
    P2(local.B, local.C, local.D, local.E, local.A,  9, 12,  6,  5);
    P2(local.A, local.B, local.C, local.D, local.E, 11, 14,  4,  8);
    P2(local.E, local.A, local.B, local.C, local.D, 10, 15,  1, 11);
    P2(local.D, local.E, local.A, local.B, local.C,  0, 14,  3, 14);
    P2(local.C, local.D, local.E, local.A, local.B,  8, 15, 11, 14);
    P2(local.B, local.C, local.D, local.E, local.A, 12,  9, 15,  6);
    P2(local.A, local.B, local.C, local.D, local.E,  4,  8,  0, 14);
    P2(local.E, local.A, local.B, local.C, local.D, 13,  9,  5,  6);
    P2(local.D, local.E, local.A, local.B, local.C,  3, 14, 12,  9);
    P2(local.C, local.D, local.E, local.A, local.B,  7,  5,  2, 12);
    P2(local.B, local.C, local.D, local.E, local.A, 15,  6, 13,  9);
    P2(local.A, local.B, local.C, local.D, local.E, 14,  8,  9, 12);
    P2(local.E, local.A, local.B, local.C, local.D,  5,  6,  7,  5);
    P2(local.D, local.E, local.A, local.B, local.C,  6,  5, 10, 15);
    P2(local.C, local.D, local.E, local.A, local.B,  2, 12, 14,  8);
#undef F
#undef K
#undef Fp
#undef Kp

#define F   F5
#define K   0xA953FD4E
#define Fp  F1
#define Kp  0x00000000
    P2(local.B, local.C, local.D, local.E, local.A,  4,  9, 12,  8);
    P2(local.A, local.B, local.C, local.D, local.E,  0, 15, 15,  5);
    P2(local.E, local.A, local.B, local.C, local.D,  5,  5, 10, 12);
    P2(local.D, local.E, local.A, local.B, local.C,  9, 11,  4,  9);
    P2(local.C, local.D, local.E, local.A, local.B,  7,  6,  1, 12);
    P2(local.B, local.C, local.D, local.E, local.A, 12,  8,  5,  5);
    P2(local.A, local.B, local.C, local.D, local.E,  2, 13,  8, 14);
    P2(local.E, local.A, local.B, local.C, local.D, 10, 12,  7,  6);
    P2(local.D, local.E, local.A, local.B, local.C, 14,  5,  6,  8);
    P2(local.C, local.D, local.E, local.A, local.B,  1, 12,  2, 13);
    P2(local.B, local.C, local.D, local.E, local.A,  3, 13, 13,  6);
    P2(local.A, local.B, local.C, local.D, local.E,  8, 14, 14,  5);
    P2(local.E, local.A, local.B, local.C, local.D, 11, 11,  0, 15);
    P2(local.D, local.E, local.A, local.B, local.C,  6,  8,  3, 13);
    P2(local.C, local.D, local.E, local.A, local.B, 15,  5,  9, 11);
    P2(local.B, local.C, local.D, local.E, local.A, 13,  6, 11, 11);
#undef F
#undef K
#undef Fp
#undef Kp
#undef S
#undef P
#undef P2
#undef F1
#undef F2
#undef F3
#undef F4
#undef F5

    local.C       = ctx->B + local.C + local.Dp;
    ctx->B = ctx->C + local.D + local.Ep;
    ctx->C = ctx->D + local.E + local.Ap;
    ctx->D = ctx->E + local.A + local.Bp;
    ctx->E = ctx->A + local.B + local.Cp;
    ctx->A = local.C;

    return 0;
}

int hripemd160_update(hripemd160_context_t *ctx,const uint8_t *input,size_t ilen)
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
        if ((ret = hripemd160_internal_process(ctx, ctx->buffer)) != 0)
        {
            return ret;
        }

        input += fill;
        ilen  -= fill;
        left = 0;
    }

    while (ilen >= ((sizeof(ctx->buffer))))
    {
        if ((ret = hripemd160_internal_process(ctx, input)) != 0)
        {
            return ret;
        }

        input += ((sizeof(ctx->buffer)));
        ilen  -= ((sizeof(ctx->buffer)));
    }

    if (ilen > 0)
    {
        memcpy((void *) (ctx->buffer + left), input, ilen);
    }

    return 0;
}

static const uint8_t ripemd160_padding[64] =
{
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

int hripemd160_finish(hripemd160_context_t *ctx,hripemd160_ripemd160_t output)
{
    int ret = -1;
    if(ctx==NULL || output == NULL)
    {
        return ret;
    }
    uint8_t msglen[8]= {0};

    uint32_t high = (ctx->total[0] >> 29) | (ctx->total[1] <<  3);
    uint32_t low  = (ctx->total[0] <<  3);

    {
        msglen[0+0]=((low)&0xFF);
        msglen[0+1]=((low>>8)&0xFF);
        msglen[0+2]=((low>>16)&0xFF);
        msglen[0+3]=((low>>24)&0xFF);
    }
    {
        msglen[4+0]=((high)&0xFF);
        msglen[4+1]=((high>>8)&0xFF);
        msglen[4+2]=((high>>16)&0xFF);
        msglen[4+3]=((high>>24)&0xFF);
    }
    uint32_t last = ctx->total[0] & (sizeof(ctx->buffer)-1);
    uint32_t padn = (last < (sizeof(ctx->buffer)-sizeof(ctx->total))) ? ((sizeof(ctx->buffer)-sizeof(ctx->total)) - last) : ((sizeof(ctx->buffer)*2-sizeof(ctx->total)) - last);

    ret = hripemd160_update(ctx, ripemd160_padding, padn);
    if (ret != 0)
    {
        return ret;
    }

    ret = hripemd160_update(ctx, msglen, 8);
    if (ret != 0)
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
    {
        output[16+0]=((ctx->E)&0xFF);
        output[16+1]=((ctx->E>>8)&0xFF);
        output[16+2]=((ctx->E>>16)&0xFF);
        output[16+3]=((ctx->E>>24)&0xFF);
    }

    ret = 0;
    return ret;
}

int hripemd160_ripemd160(const uint8_t *input,size_t ilen,hripemd160_ripemd160_t output)
{
    int ret = -1;
    if(input==NULL || output == NULL)
    {
        return ret;
    }
    hripemd160_context_t ctx;

    if ((ret = hripemd160_starts(&ctx)) != 0)
    {
        goto exit;
    }

    if ((ret = hripemd160_update(&ctx, input, ilen)) != 0)
    {
        goto exit;
    }

    if ((ret = hripemd160_finish(&ctx, output)) != 0)
    {
        goto exit;
    }

exit:
    return ret;
}




