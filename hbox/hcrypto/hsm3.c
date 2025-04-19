/***************************************************************
 * Name:      hsm3.c
 * Purpose:   实现hsm3接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hsm3.h"

int hsm3_starts(hsm3_context_t *ctx)
{
    if(ctx==NULL)
    {
        return -1;
    }
    memset(ctx,0,sizeof(hsm3_context_t));
    ctx->A = 0x7380166fUL;
    ctx->B = 0x4914b2b9UL;
    ctx->C = 0x172442d7UL;
    ctx->D = 0xda8a0600UL;
    ctx->E = 0xa96f30bcUL;
    ctx->F = 0x163138aaUL;
    ctx->G = 0xe38dee4dUL;
    ctx->H = 0xb0fb0e4eUL;
    return 0;
}

static const uint32_t hsm3_K[64] =
{
    0x79cc4519UL, 0xf3988a32UL, 0xe7311465UL, 0xce6228cbUL,
    0x9cc45197UL, 0x3988a32fUL, 0x7311465eUL, 0xe6228cbcUL,
    0xcc451979UL, 0x988a32f3UL, 0x311465e7UL, 0x6228cbceUL,
    0xc451979cUL, 0x88a32f39UL, 0x11465e73UL, 0x228cbce6UL,
    0x9d8a7a87UL, 0x3b14f50fUL, 0x7629ea1eUL, 0xec53d43cUL,
    0xd8a7a879UL, 0xb14f50f3UL, 0x629ea1e7UL, 0xc53d43ceUL,
    0x8a7a879dUL, 0x14f50f3bUL, 0x29ea1e76UL, 0x53d43cecUL,
    0xa7a879d8UL, 0x4f50f3b1UL, 0x9ea1e762UL, 0x3d43cec5UL,
    0x7a879d8aUL, 0xf50f3b14UL, 0xea1e7629UL, 0xd43cec53UL,
    0xa879d8a7UL, 0x50f3b14fUL, 0xa1e7629eUL, 0x43cec53dUL,
    0x879d8a7aUL, 0x0f3b14f5UL, 0x1e7629eaUL, 0x3cec53d4UL,
    0x79d8a7a8UL, 0xf3b14f50UL, 0xe7629ea1UL, 0xcec53d43UL,
    0x9d8a7a87UL, 0x3b14f50fUL, 0x7629ea1eUL, 0xec53d43cUL,
    0xd8a7a879UL, 0xb14f50f3UL, 0x629ea1e7UL, 0xc53d43ceUL,
    0x8a7a879dUL, 0x14f50f3bUL, 0x29ea1e76UL, 0x53d43cecUL,
    0xa7a879d8UL, 0x4f50f3b1UL, 0x9ea1e762UL, 0x3d43cec5UL
};



typedef struct hsm3_internal_state
{
    uint32_t W[16];
    uint32_t A;
    uint32_t B;
    uint32_t C;
    uint32_t D;
    uint32_t E;
    uint32_t F;
    uint32_t G;
    uint32_t H;
} hsm3_internal_state_t;
#ifdef ROTL32
#undef ROTL32
#endif // ROTL32
#ifdef R
#undef R
#endif // R
#ifdef R1
#undef R1
#endif // R1
#ifdef R2
#undef R2
#endif // R2
#ifdef FF1
#undef FF1
#endif // FF1
#ifdef FF2
#undef FF2
#endif // FF2
#ifdef GG1
#undef GG1
#endif // GG1
#ifdef GG2
#undef GG2
#endif // GG2
#ifdef P0
#undef P0
#endif // P0
#ifdef P1
#undef P1
#endif // P1
#ifdef W1
#undef W1
#endif // W1
#ifdef W2
#undef W2
#endif // W2
static int hsm3_internal_process(hsm3_context_t *ctx,const hsm3_message_block_t data)
{
    if(ctx==NULL || data ==NULL)
    {
        return -1;
    }
    hsm3_internal_state_t local= {0};
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

    local.A = ctx->A;
    local.B = ctx->B;
    local.C = ctx->C;
    local.D = ctx->D;
    local.E = ctx->E;
    local.F = ctx->F;
    local.G = ctx->G;
    local.H = ctx->H;

#define ROTL32(n,x) (((x)<<(n)) | ((x)>>((-(n)&31))))

#define R(i, a, b, c, d, e, f, g, h, t, w1, w2)                     \
    do                                                              \
    {                                                               \
        uint32_t ss1 = ROTL32(7, (ROTL32(12, (a)) + (e) + (t)));    \
        uint32_t ss2 = ss1 ^ ROTL32(12, (a));                       \
        d += FF ## i(a, b, c) + ss2 + ((w1) ^ (w2));                \
        h += GG ## i(e, f, g) + ss1 + (w1);                         \
        b = ROTL32(9, (b));                                         \
        f = ROTL32(19, (f));                                        \
        h = P0((h));                                                \
    } while (0)



#define R1(a,b,c,d,e,f,g,h,t,w1,w2) R(1,a,b,c,d,e,f,g,h,t,w1,w2)
#define R2(a,b,c,d,e,f,g,h,t,w1,w2) R(2,a,b,c,d,e,f,g,h,t,w1,w2)

#define FF1(x, y, z)  (x ^ y ^ z)
#define FF2(x, y, z)  ((x & y) | (x & z) | (y & z))

#define GG1(x, y, z)  (x ^ y ^ z)
#define GG2(x, y, z)  ((x & y) | ( ~x & z))

#define P0(x) ((x) ^ ROTL32(9, (x)) ^ ROTL32(17, (x)))
#define P1(x) ((x) ^ ROTL32(15, (x)) ^ ROTL32(23, (x)))
#define W1(i) (local.W[i & 0x0f])
#define W2(i) (local.W[i & 0x0f] =                                                          \
        P1(local.W[i & 0x0f] ^ local.W[(i-9) & 0x0f] ^ ROTL32(15, local.W[(i-3) & 0x0f]))   \
        ^ ROTL32(7, local.W[(i-13) & 0x0f])                                                 \
        ^ local.W[(i-6) & 0x0f])


    R1(local.A, local.B, local.C, local.D, local.E, local.F, local.G, local.H, hsm3_K[0], local.W[0], local.W[4]);
    R1(local.D, local.A, local.B, local.C, local.H, local.E, local.F, local.G, hsm3_K[1], local.W[1], local.W[5]);
    R1(local.C, local.D, local.A, local.B, local.G, local.H, local.E, local.F, hsm3_K[2], local.W[2], local.W[6]);
    R1(local.B, local.C, local.D, local.A, local.F, local.G, local.H, local.E, hsm3_K[3], local.W[3], local.W[7]);
    R1(local.A, local.B, local.C, local.D, local.E, local.F, local.G, local.H, hsm3_K[4], W1(4), local.W[8]);
    R1(local.D, local.A, local.B, local.C, local.H, local.E, local.F, local.G, hsm3_K[5], W1(5), local.W[9]);
    R1(local.C, local.D, local.A, local.B, local.G, local.H, local.E, local.F, hsm3_K[6], W1(6), local.W[10]);
    R1(local.B, local.C, local.D, local.A, local.F, local.G, local.H, local.E, hsm3_K[7], W1(7), local.W[11]);
    R1(local.A, local.B, local.C, local.D, local.E, local.F, local.G, local.H, hsm3_K[8], W1(8), local.W[12]);
    R1(local.D, local.A, local.B, local.C, local.H, local.E, local.F, local.G, hsm3_K[9], W1(9), local.W[13]);
    R1(local.C, local.D, local.A, local.B, local.G, local.H, local.E, local.F, hsm3_K[10], W1(10), local.W[14]);
    R1(local.B, local.C, local.D, local.A, local.F, local.G, local.H, local.E, hsm3_K[11], W1(11), local.W[15]);
    R1(local.A, local.B, local.C, local.D, local.E, local.F, local.G, local.H, hsm3_K[12], W1(12), W2(16));
    R1(local.D, local.A, local.B, local.C, local.H, local.E, local.F, local.G, hsm3_K[13], W1(13), W2(17));
    R1(local.C, local.D, local.A, local.B, local.G, local.H, local.E, local.F, hsm3_K[14], W1(14), W2(18));
    R1(local.B, local.C, local.D, local.A, local.F, local.G, local.H, local.E, hsm3_K[15], W1(15), W2(19));

    R2(local.A, local.B, local.C, local.D, local.E, local.F, local.G, local.H, hsm3_K[16], W1(16), W2(20));
    R2(local.D, local.A, local.B, local.C, local.H, local.E, local.F, local.G, hsm3_K[17], W1(17), W2(21));
    R2(local.C, local.D, local.A, local.B, local.G, local.H, local.E, local.F, hsm3_K[18], W1(18), W2(22));
    R2(local.B, local.C, local.D, local.A, local.F, local.G, local.H, local.E, hsm3_K[19], W1(19), W2(23));
    R2(local.A, local.B, local.C, local.D, local.E, local.F, local.G, local.H, hsm3_K[20], W1(20), W2(24));
    R2(local.D, local.A, local.B, local.C, local.H, local.E, local.F, local.G, hsm3_K[21], W1(21), W2(25));
    R2(local.C, local.D, local.A, local.B, local.G, local.H, local.E, local.F, hsm3_K[22], W1(22), W2(26));
    R2(local.B, local.C, local.D, local.A, local.F, local.G, local.H, local.E, hsm3_K[23], W1(23), W2(27));
    R2(local.A, local.B, local.C, local.D, local.E, local.F, local.G, local.H, hsm3_K[24], W1(24), W2(28));
    R2(local.D, local.A, local.B, local.C, local.H, local.E, local.F, local.G, hsm3_K[25], W1(25), W2(29));
    R2(local.C, local.D, local.A, local.B, local.G, local.H, local.E, local.F, hsm3_K[26], W1(26), W2(30));
    R2(local.B, local.C, local.D, local.A, local.F, local.G, local.H, local.E, hsm3_K[27], W1(27), W2(31));
    R2(local.A, local.B, local.C, local.D, local.E, local.F, local.G, local.H, hsm3_K[28], W1(28), W2(32));
    R2(local.D, local.A, local.B, local.C, local.H, local.E, local.F, local.G, hsm3_K[29], W1(29), W2(33));
    R2(local.C, local.D, local.A, local.B, local.G, local.H, local.E, local.F, hsm3_K[30], W1(30), W2(34));
    R2(local.B, local.C, local.D, local.A, local.F, local.G, local.H, local.E, hsm3_K[31], W1(31), W2(35));

    R2(local.A, local.B, local.C, local.D, local.E, local.F, local.G, local.H, hsm3_K[32], W1(32), W2(36));
    R2(local.D, local.A, local.B, local.C, local.H, local.E, local.F, local.G, hsm3_K[33], W1(33), W2(37));
    R2(local.C, local.D, local.A, local.B, local.G, local.H, local.E, local.F, hsm3_K[34], W1(34), W2(38));
    R2(local.B, local.C, local.D, local.A, local.F, local.G, local.H, local.E, hsm3_K[35], W1(35), W2(39));
    R2(local.A, local.B, local.C, local.D, local.E, local.F, local.G, local.H, hsm3_K[36], W1(36), W2(40));
    R2(local.D, local.A, local.B, local.C, local.H, local.E, local.F, local.G, hsm3_K[37], W1(37), W2(41));
    R2(local.C, local.D, local.A, local.B, local.G, local.H, local.E, local.F, hsm3_K[38], W1(38), W2(42));
    R2(local.B, local.C, local.D, local.A, local.F, local.G, local.H, local.E, hsm3_K[39], W1(39), W2(43));
    R2(local.A, local.B, local.C, local.D, local.E, local.F, local.G, local.H, hsm3_K[40], W1(40), W2(44));
    R2(local.D, local.A, local.B, local.C, local.H, local.E, local.F, local.G, hsm3_K[41], W1(41), W2(45));
    R2(local.C, local.D, local.A, local.B, local.G, local.H, local.E, local.F, hsm3_K[42], W1(42), W2(46));
    R2(local.B, local.C, local.D, local.A, local.F, local.G, local.H, local.E, hsm3_K[43], W1(43), W2(47));
    R2(local.A, local.B, local.C, local.D, local.E, local.F, local.G, local.H, hsm3_K[44], W1(44), W2(48));
    R2(local.D, local.A, local.B, local.C, local.H, local.E, local.F, local.G, hsm3_K[45], W1(45), W2(49));
    R2(local.C, local.D, local.A, local.B, local.G, local.H, local.E, local.F, hsm3_K[46], W1(46), W2(50));
    R2(local.B, local.C, local.D, local.A, local.F, local.G, local.H, local.E, hsm3_K[47], W1(47), W2(51));

    R2(local.A, local.B, local.C, local.D, local.E, local.F, local.G, local.H, hsm3_K[48], W1(48), W2(52));
    R2(local.D, local.A, local.B, local.C, local.H, local.E, local.F, local.G, hsm3_K[49], W1(49), W2(53));
    R2(local.C, local.D, local.A, local.B, local.G, local.H, local.E, local.F, hsm3_K[50], W1(50), W2(54));
    R2(local.B, local.C, local.D, local.A, local.F, local.G, local.H, local.E, hsm3_K[51], W1(51), W2(55));
    R2(local.A, local.B, local.C, local.D, local.E, local.F, local.G, local.H, hsm3_K[52], W1(52), W2(56));
    R2(local.D, local.A, local.B, local.C, local.H, local.E, local.F, local.G, hsm3_K[53], W1(53), W2(57));
    R2(local.C, local.D, local.A, local.B, local.G, local.H, local.E, local.F, hsm3_K[54], W1(54), W2(58));
    R2(local.B, local.C, local.D, local.A, local.F, local.G, local.H, local.E, hsm3_K[55], W1(55), W2(59));
    R2(local.A, local.B, local.C, local.D, local.E, local.F, local.G, local.H, hsm3_K[56], W1(56), W2(60));
    R2(local.D, local.A, local.B, local.C, local.H, local.E, local.F, local.G, hsm3_K[57], W1(57), W2(61));
    R2(local.C, local.D, local.A, local.B, local.G, local.H, local.E, local.F, hsm3_K[58], W1(58), W2(62));
    R2(local.B, local.C, local.D, local.A, local.F, local.G, local.H, local.E, hsm3_K[59], W1(59), W2(63));
    R2(local.A, local.B, local.C, local.D, local.E, local.F, local.G, local.H, hsm3_K[60], W1(60), W2(64));
    R2(local.D, local.A, local.B, local.C, local.H, local.E, local.F, local.G, hsm3_K[61], W1(61), W2(65));
    R2(local.C, local.D, local.A, local.B, local.G, local.H, local.E, local.F, hsm3_K[62], W1(62), W2(66));
    R2(local.B, local.C, local.D, local.A, local.F, local.G, local.H, local.E, hsm3_K[63], W1(63), W2(67));

#undef ROTL32
#undef R
#undef R1
#undef R2
#undef FF1
#undef FF2
#undef GG1
#undef GG2
#undef P0
#undef P1
#undef W1
#undef W2


    ctx->A ^= local.A;
    ctx->B ^= local.B;
    ctx->C ^= local.C;
    ctx->D ^= local.D;
    ctx->E ^= local.E;
    ctx->F ^= local.F;
    ctx->G ^= local.G;
    ctx->H ^= local.H;

    return 0;
}

int hsm3_update(hsm3_context_t *ctx,const uint8_t *input,size_t ilen)
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
        if ((ret = hsm3_internal_process(ctx, ctx->buffer)) != 0)
        {
            return ret;
        }

        input += fill;
        ilen  -= fill;
        left = 0;
    }

    while (ilen >= ((sizeof(ctx->buffer))))
    {
        if ((ret = hsm3_internal_process(ctx, input)) != 0)
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

static const uint8_t sm3_padding[64] =
{
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

int hsm3_finish(hsm3_context_t *ctx,hsm3_sm3_t output)
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
        msglen[0+3]=((high)&0xFF);
        msglen[0+2]=((high>>8)&0xFF);
        msglen[0+1]=((high>>16)&0xFF);
        msglen[0+0]=((high>>24)&0xFF);
    }
    {
        msglen[4+3]=((low)&0xFF);
        msglen[4+2]=((low>>8)&0xFF);
        msglen[4+1]=((low>>16)&0xFF);
        msglen[4+0]=((low>>24)&0xFF);
    }
    uint32_t last = ctx->total[0] & (sizeof(ctx->buffer)-1);
    uint32_t padn = (last < (sizeof(ctx->buffer)-sizeof(ctx->total))) ? ((sizeof(ctx->buffer)-sizeof(ctx->total)) - last) : ((sizeof(ctx->buffer)*2-sizeof(ctx->total)) - last);

    ret = hsm3_update(ctx, sm3_padding, padn);
    if (ret != 0)
    {
        return ret;
    }

    ret = hsm3_update(ctx, msglen, 8);
    if (ret != 0)
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
    {
        output[20+3]=((ctx->F)&0xFF);
        output[20+2]=((ctx->F>>8)&0xFF);
        output[20+1]=((ctx->F>>16)&0xFF);
        output[20+0]=((ctx->F>>24)&0xFF);
    }
    {
        output[24+3]=((ctx->G)&0xFF);
        output[24+2]=((ctx->G>>8)&0xFF);
        output[24+1]=((ctx->G>>16)&0xFF);
        output[24+0]=((ctx->G>>24)&0xFF);
    }
    {
        output[28+3]=((ctx->H)&0xFF);
        output[28+2]=((ctx->H>>8)&0xFF);
        output[28+1]=((ctx->H>>16)&0xFF);
        output[28+0]=((ctx->H>>24)&0xFF);
    }


    ret = 0;
    return ret;
}

int hsm3_sm3(const uint8_t *input,size_t ilen,hsm3_sm3_t output)
{
    int ret = -1;
    if(input==NULL || output == NULL)
    {
        return ret;
    }
    hsm3_context_t ctx;

    if ((ret = hsm3_starts(&ctx)) != 0)
    {
        goto exit;
    }

    if ((ret = hsm3_update(&ctx, input, ilen)) != 0)
    {
        goto exit;
    }

    if ((ret = hsm3_finish(&ctx, output)) != 0)
    {
        goto exit;
    }

exit:
    return ret;
}




