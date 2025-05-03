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
        if ((ret = hsha1_internal_process(ctx, ctx->buffer)) != 0)
        {
            return ret;
        }

        input += fill;
        ilen  -= fill;
        left = 0;
    }

    while (ilen >= (sizeof(ctx->buffer)))
    {
        if ((ret = hsha1_internal_process(ctx, input)) != 0)
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

        if ((ret = hsha1_internal_process(ctx, ctx->buffer)) != 0)
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

int hsha2_sha224_starts(hsha2_sha224_context_t *ctx)
{
    if(ctx==NULL)
    {
        return -1;
    }
    memset(ctx,0,sizeof(hsha2_sha224_context_t));
    ctx->A = 0xC1059ED8;
    ctx->B = 0x367CD507;
    ctx->C = 0x3070DD17;
    ctx->D = 0xF70E5939;
    ctx->E = 0xFFC00B31;
    ctx->F = 0x68581511;
    ctx->G = 0x64F98FA7;
    ctx->H = 0xBEFA4FA4;
    return 0;
}

static const uint32_t hsha2_sha224_K[] =
{
    0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5,
    0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
    0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3,
    0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
    0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC,
    0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
    0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7,
    0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
    0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13,
    0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
    0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3,
    0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
    0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5,
    0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
    0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208,
    0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2,
};

typedef struct hsha2_sha224_internal_state
{
    uint32_t W[64];
    uint32_t A[8];
    uint32_t temp1;
    uint32_t temp2;
} hsha2_sha224_internal_state_t;
#ifdef P
#undef P
#endif
#ifdef R
#undef R
#endif
#ifdef F1
#undef F1
#endif
#ifdef F0
#undef F0
#endif
#ifdef S3
#undef S3
#endif
#ifdef S2
#undef S2
#endif
#ifdef S1
#undef S1
#endif
#ifdef S0
#undef S0
#endif
#ifdef ROTR
#undef ROTR
#endif
#ifdef SHR
#undef SHR
#endif
static int hsha2_sha224_internal_process(hsha2_sha224_context_t *ctx,const hsha2_message_block_t data)
{
    if(ctx==NULL || data ==NULL)
    {
        return -1;
    }
    hsha2_sha224_internal_state_t local= {0};
    for(size_t i=0; i<sizeof(hsha2_message_block_t)/sizeof(local.W[0]); i++)
    {
        local.W[i]=data[4*i+0];
        local.W[i] <<= 8;
        local.W[i]+=data[4*i+1];
        local.W[i] <<= 8;
        local.W[i]+=data[4*i+2];
        local.W[i] <<= 8;
        local.W[i]+=data[4*i+3];
    }

    local.A[0]=ctx->A;
    local.A[1]=ctx->B;
    local.A[2]=ctx->C;
    local.A[3]=ctx->D;
    local.A[4]=ctx->E;
    local.A[5]=ctx->F;
    local.A[6]=ctx->G;
    local.A[7]=ctx->H;


#define  SHR(x, n) (((x) & 0xFFFFFFFF) >> (n))
#define ROTR(x, n) (SHR(x, n) | ((x) << (32 - (n))))

#define S0(x) (ROTR(x, 7) ^ ROTR(x, 18) ^  SHR(x, 3))
#define S1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^  SHR(x, 10))

#define S2(x) (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define S3(x) (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))

#define F0(x, y, z) (((x) & (y)) | ((z) & ((x) | (y))))
#define F1(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))

#define R(t)                                                                \
    (                                                                       \
        local.W[t] = S1(local.W[(t) -  2]) + local.W[(t) -  7] +            \
                     S0(local.W[(t) - 15]) + local.W[(t) - 16]              \
    )

#define P(a, b, c, d, e, f, g, h, x, K)                                     \
    do                                                                      \
    {                                                                       \
        local.temp1 = (h) + S3(e) + F1((e), (f), (g)) + (K) + (x);          \
        local.temp2 = S2(a) + F0((a), (b), (c));                            \
        (d) += local.temp1; (h) = local.temp1 + local.temp2;                \
    } while (0)

    for (size_t i = 0; i < 16; i += 8)
    {
        P(local.A[0], local.A[1], local.A[2], local.A[3], local.A[4], local.A[5], local.A[6], local.A[7], local.W[i+0], hsha2_sha224_K[i+0]);
        P(local.A[7], local.A[0], local.A[1], local.A[2], local.A[3], local.A[4], local.A[5], local.A[6], local.W[i+1], hsha2_sha224_K[i+1]);
        P(local.A[6], local.A[7], local.A[0], local.A[1], local.A[2], local.A[3], local.A[4], local.A[5], local.W[i+2], hsha2_sha224_K[i+2]);
        P(local.A[5], local.A[6], local.A[7], local.A[0], local.A[1], local.A[2], local.A[3], local.A[4], local.W[i+3], hsha2_sha224_K[i+3]);
        P(local.A[4], local.A[5], local.A[6], local.A[7], local.A[0], local.A[1], local.A[2], local.A[3], local.W[i+4], hsha2_sha224_K[i+4]);
        P(local.A[3], local.A[4], local.A[5], local.A[6], local.A[7], local.A[0], local.A[1], local.A[2], local.W[i+5], hsha2_sha224_K[i+5]);
        P(local.A[2], local.A[3], local.A[4], local.A[5], local.A[6], local.A[7], local.A[0], local.A[1], local.W[i+6], hsha2_sha224_K[i+6]);
        P(local.A[1], local.A[2], local.A[3], local.A[4], local.A[5], local.A[6], local.A[7], local.A[0], local.W[i+7], hsha2_sha224_K[i+7]);
    }

    for (size_t i = 16; i < 64; i += 8)
    {
        P(local.A[0], local.A[1], local.A[2], local.A[3], local.A[4], local.A[5], local.A[6], local.A[7], R(i+0), hsha2_sha224_K[i+0]);
        P(local.A[7], local.A[0], local.A[1], local.A[2], local.A[3], local.A[4], local.A[5], local.A[6], R(i+1), hsha2_sha224_K[i+1]);
        P(local.A[6], local.A[7], local.A[0], local.A[1], local.A[2], local.A[3], local.A[4], local.A[5], R(i+2), hsha2_sha224_K[i+2]);
        P(local.A[5], local.A[6], local.A[7], local.A[0], local.A[1], local.A[2], local.A[3], local.A[4], R(i+3), hsha2_sha224_K[i+3]);
        P(local.A[4], local.A[5], local.A[6], local.A[7], local.A[0], local.A[1], local.A[2], local.A[3], R(i+4), hsha2_sha224_K[i+4]);
        P(local.A[3], local.A[4], local.A[5], local.A[6], local.A[7], local.A[0], local.A[1], local.A[2], R(i+5), hsha2_sha224_K[i+5]);
        P(local.A[2], local.A[3], local.A[4], local.A[5], local.A[6], local.A[7], local.A[0], local.A[1], R(i+6), hsha2_sha224_K[i+6]);
        P(local.A[1], local.A[2], local.A[3], local.A[4], local.A[5], local.A[6], local.A[7], local.A[0], R(i+7), hsha2_sha224_K[i+7]);
    }

#undef P
#undef R
#undef F1
#undef F0
#undef S3
#undef S2
#undef S1
#undef S0
#undef ROTR
#undef SHR

    ctx->A+=local.A[0];
    ctx->B+=local.A[1];
    ctx->C+=local.A[2];
    ctx->D+=local.A[3];
    ctx->E+=local.A[4];
    ctx->F+=local.A[5];
    ctx->G+=local.A[6];
    ctx->H+=local.A[7];


    return 0;
}

int hsha2_sha224_update(hsha2_sha224_context_t *ctx,const uint8_t *input,size_t ilen)
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
        if ((ret = hsha2_sha224_internal_process(ctx, ctx->buffer)) != 0)
        {
            return ret;
        }

        input += fill;
        ilen  -= fill;
        left = 0;
    }

    while (ilen >= (sizeof(ctx->buffer)))
    {
        if ((ret = hsha2_sha224_internal_process(ctx, input)) != 0)
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

int hsha2_sha224_finish(hsha2_sha224_context_t *ctx,hsha2_sha224_t output)
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

        if ((ret = hsha2_sha224_internal_process(ctx, ctx->buffer)) != 0)
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

    if ((ret = hsha2_sha224_internal_process(ctx, ctx->buffer)) != 0)
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

    ret = 0;
    return ret;

}

int hsha2_sha224(const uint8_t *input,size_t ilen,hsha2_sha224_t output)
{
    int ret = -1;
    if(input==NULL || output == NULL)
    {
        return ret;
    }
    hsha2_sha224_context_t ctx;

    if ((ret = hsha2_sha224_starts(&ctx)) != 0)
    {
        goto exit;
    }

    if ((ret = hsha2_sha224_update(&ctx, input, ilen)) != 0)
    {
        goto exit;
    }

    if ((ret = hsha2_sha224_finish(&ctx, output)) != 0)
    {
        goto exit;
    }

exit:
    return ret;

}

int hsha2_sha256_starts(hsha2_sha256_context_t *ctx)
{
    if(ctx==NULL)
    {
        return -1;
    }
    memset(ctx,0,sizeof(hsha2_sha256_context_t));
    ctx->A = 0x6A09E667;
    ctx->B = 0xBB67AE85;
    ctx->C = 0x3C6EF372;
    ctx->D = 0xA54FF53A;
    ctx->E = 0x510E527F;
    ctx->F = 0x9B05688C;
    ctx->G = 0x1F83D9AB;
    ctx->H = 0x5BE0CD19;
    return 0;
}

int hsha2_sha256_update(hsha2_sha256_context_t *ctx,const uint8_t *input,size_t ilen)
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
        if ((ret = hsha2_sha224_internal_process(ctx, ctx->buffer)) != 0)
        {
            return ret;
        }

        input += fill;
        ilen  -= fill;
        left = 0;
    }

    while (ilen >= (sizeof(ctx->buffer)))
    {
        if ((ret = hsha2_sha224_internal_process(ctx, input)) != 0)
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

int hsha2_sha256_finish(hsha2_sha256_context_t *ctx,hsha2_sha256_t output)
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

        if ((ret = hsha2_sha224_internal_process(ctx, ctx->buffer)) != 0)
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

    if ((ret = hsha2_sha224_internal_process(ctx, ctx->buffer)) != 0)
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

int hsha2_sha256(const uint8_t *input,size_t ilen,hsha2_sha256_t output)
{
    int ret = -1;
    if(input==NULL || output == NULL)
    {
        return ret;
    }
    hsha2_sha256_context_t ctx;

    if ((ret = hsha2_sha256_starts(&ctx)) != 0)
    {
        goto exit;
    }

    if ((ret = hsha2_sha256_update(&ctx, input, ilen)) != 0)
    {
        goto exit;
    }

    if ((ret = hsha2_sha256_finish(&ctx, output)) != 0)
    {
        goto exit;
    }

exit:
    return ret;

}

int hsha2_sha384_starts(hsha2_sha384_context_t *ctx)
{
    if(ctx==NULL)
    {
        return -1;
    }
    memset(ctx,0,sizeof(hsha2_sha384_context_t));
    ctx->A = (uint64_t)(0xCBBB9D5DC1059ED8);
    ctx->B = (uint64_t)(0x629A292A367CD507);
    ctx->C = (uint64_t)(0x9159015A3070DD17);
    ctx->D = (uint64_t)(0x152FECD8F70E5939);
    ctx->E = (uint64_t)(0x67332667FFC00B31);
    ctx->F = (uint64_t)(0x8EB44A8768581511);
    ctx->G = (uint64_t)(0xDB0C2E0D64F98FA7);
    ctx->H = (uint64_t)(0x47B5481DBEFA4FA4);
    return 0;
}



static const uint64_t hsha2_sha384_K[] =
{
    (uint64_t)(0x428A2F98D728AE22),  (uint64_t)(0x7137449123EF65CD),
    (uint64_t)(0xB5C0FBCFEC4D3B2F),  (uint64_t)(0xE9B5DBA58189DBBC),
    (uint64_t)(0x3956C25BF348B538),  (uint64_t)(0x59F111F1B605D019),
    (uint64_t)(0x923F82A4AF194F9B),  (uint64_t)(0xAB1C5ED5DA6D8118),
    (uint64_t)(0xD807AA98A3030242),  (uint64_t)(0x12835B0145706FBE),
    (uint64_t)(0x243185BE4EE4B28C),  (uint64_t)(0x550C7DC3D5FFB4E2),
    (uint64_t)(0x72BE5D74F27B896F),  (uint64_t)(0x80DEB1FE3B1696B1),
    (uint64_t)(0x9BDC06A725C71235),  (uint64_t)(0xC19BF174CF692694),
    (uint64_t)(0xE49B69C19EF14AD2),  (uint64_t)(0xEFBE4786384F25E3),
    (uint64_t)(0x0FC19DC68B8CD5B5),  (uint64_t)(0x240CA1CC77AC9C65),
    (uint64_t)(0x2DE92C6F592B0275),  (uint64_t)(0x4A7484AA6EA6E483),
    (uint64_t)(0x5CB0A9DCBD41FBD4),  (uint64_t)(0x76F988DA831153B5),
    (uint64_t)(0x983E5152EE66DFAB),  (uint64_t)(0xA831C66D2DB43210),
    (uint64_t)(0xB00327C898FB213F),  (uint64_t)(0xBF597FC7BEEF0EE4),
    (uint64_t)(0xC6E00BF33DA88FC2),  (uint64_t)(0xD5A79147930AA725),
    (uint64_t)(0x06CA6351E003826F),  (uint64_t)(0x142929670A0E6E70),
    (uint64_t)(0x27B70A8546D22FFC),  (uint64_t)(0x2E1B21385C26C926),
    (uint64_t)(0x4D2C6DFC5AC42AED),  (uint64_t)(0x53380D139D95B3DF),
    (uint64_t)(0x650A73548BAF63DE),  (uint64_t)(0x766A0ABB3C77B2A8),
    (uint64_t)(0x81C2C92E47EDAEE6),  (uint64_t)(0x92722C851482353B),
    (uint64_t)(0xA2BFE8A14CF10364),  (uint64_t)(0xA81A664BBC423001),
    (uint64_t)(0xC24B8B70D0F89791),  (uint64_t)(0xC76C51A30654BE30),
    (uint64_t)(0xD192E819D6EF5218),  (uint64_t)(0xD69906245565A910),
    (uint64_t)(0xF40E35855771202A),  (uint64_t)(0x106AA07032BBD1B8),
    (uint64_t)(0x19A4C116B8D2D0C8),  (uint64_t)(0x1E376C085141AB53),
    (uint64_t)(0x2748774CDF8EEB99),  (uint64_t)(0x34B0BCB5E19B48A8),
    (uint64_t)(0x391C0CB3C5C95A63),  (uint64_t)(0x4ED8AA4AE3418ACB),
    (uint64_t)(0x5B9CCA4F7763E373),  (uint64_t)(0x682E6FF3D6B2B8A3),
    (uint64_t)(0x748F82EE5DEFB2FC),  (uint64_t)(0x78A5636F43172F60),
    (uint64_t)(0x84C87814A1F0AB72),  (uint64_t)(0x8CC702081A6439EC),
    (uint64_t)(0x90BEFFFA23631E28),  (uint64_t)(0xA4506CEBDE82BDE9),
    (uint64_t)(0xBEF9A3F7B2C67915),  (uint64_t)(0xC67178F2E372532B),
    (uint64_t)(0xCA273ECEEA26619C),  (uint64_t)(0xD186B8C721C0C207),
    (uint64_t)(0xEADA7DD6CDE0EB1E),  (uint64_t)(0xF57D4F7FEE6ED178),
    (uint64_t)(0x06F067AA72176FBA),  (uint64_t)(0x0A637DC5A2C898A6),
    (uint64_t)(0x113F9804BEF90DAE),  (uint64_t)(0x1B710B35131C471B),
    (uint64_t)(0x28DB77F523047D84),  (uint64_t)(0x32CAAB7B40C72493),
    (uint64_t)(0x3C9EBE0A15C9BEBC),  (uint64_t)(0x431D67C49C100D4C),
    (uint64_t)(0x4CC5D4BECB3E42B6),  (uint64_t)(0x597F299CFC657E2A),
    (uint64_t)(0x5FCB6FAB3AD6FAEC),  (uint64_t)(0x6C44198C4A475817)
};

typedef struct hsha2_sha384_internal_state
{
    uint64_t W[80];
    uint64_t A[8];
    uint64_t temp1;
    uint64_t temp2;
} hsha2_sha384_internal_state_t;
#ifdef P
#undef P
#endif
#ifdef R
#undef R
#endif
#ifdef F1
#undef F1
#endif
#ifdef F0
#undef F0
#endif
#ifdef S3
#undef S3
#endif
#ifdef S2
#undef S2
#endif
#ifdef S1
#undef S1
#endif
#ifdef S0
#undef S0
#endif
#ifdef ROTR
#undef ROTR
#endif
#ifdef SHR
#undef SHR
#endif
static int hsha2_sha384_internal_process(hsha2_sha384_context_t *ctx,const hsha2_message_block_t data)
{
    if(ctx==NULL || data ==NULL)
    {
        return -1;
    }
    hsha2_sha384_internal_state_t local= {0};
    for(size_t i=0; i<sizeof(hsha2_message2_block_t)/sizeof(local.W[0]); i++)
    {
        local.W[i]=data[8*i+0];
        local.W[i] <<= 8;
        local.W[i]+=data[8*i+1];
        local.W[i] <<= 8;
        local.W[i]+=data[8*i+2];
        local.W[i] <<= 8;
        local.W[i]+=data[8*i+3];
        local.W[i] <<= 8;
        local.W[i]+=data[8*i+4];
        local.W[i] <<= 8;
        local.W[i]+=data[8*i+5];
        local.W[i] <<= 8;
        local.W[i]+=data[8*i+6];
        local.W[i] <<= 8;
        local.W[i]+=data[8*i+7];
    }

    local.A[0]=ctx->A;
    local.A[1]=ctx->B;
    local.A[2]=ctx->C;
    local.A[3]=ctx->D;
    local.A[4]=ctx->E;
    local.A[5]=ctx->F;
    local.A[6]=ctx->G;
    local.A[7]=ctx->H;


#define  SHR(x, n) ((x) >> (n))
#define ROTR(x, n) (SHR((x), (n)) | ((x) << (64 - (n))))

#define S0(x) (ROTR(x, 1) ^ ROTR(x, 8) ^  SHR(x, 7))
#define S1(x) (ROTR(x, 19) ^ ROTR(x, 61) ^  SHR(x, 6))

#define S2(x) (ROTR(x, 28) ^ ROTR(x, 34) ^ ROTR(x, 39))
#define S3(x) (ROTR(x, 14) ^ ROTR(x, 18) ^ ROTR(x, 41))

#define F0(x, y, z) (((x) & (y)) | ((z) & ((x) | (y))))
#define F1(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))

#define P(a, b, c, d, e, f, g, h, x, K)                                     \
    do                                                                      \
    {                                                                       \
        local.temp1 = (h) + S3(e) + F1((e), (f), (g)) + (K) + (x);          \
        local.temp2 = S2(a) + F0((a), (b), (c));                            \
        (d) += local.temp1; (h) = local.temp1 + local.temp2;                \
    } while (0)

    for (size_t i=sizeof(hsha2_message2_block_t)/sizeof(local.W[0]); i < 80; i++)
    {
        local.W[i] = S1(local.W[i -  2]) + local.W[i -  7] + S0(local.W[i - 15]) + local.W[i - 16];
    }

    for(size_t i=0; i<80;)
    {
        P(local.A[0], local.A[1], local.A[2], local.A[3], local.A[4], local.A[5], local.A[6], local.A[7], local.W[i], hsha2_sha384_K[i]);
        i++;
        P(local.A[7], local.A[0], local.A[1], local.A[2], local.A[3], local.A[4], local.A[5], local.A[6], local.W[i], hsha2_sha384_K[i]);
        i++;
        P(local.A[6], local.A[7], local.A[0], local.A[1], local.A[2], local.A[3], local.A[4], local.A[5], local.W[i], hsha2_sha384_K[i]);
        i++;
        P(local.A[5], local.A[6], local.A[7], local.A[0], local.A[1], local.A[2], local.A[3], local.A[4], local.W[i], hsha2_sha384_K[i]);
        i++;
        P(local.A[4], local.A[5], local.A[6], local.A[7], local.A[0], local.A[1], local.A[2], local.A[3], local.W[i], hsha2_sha384_K[i]);
        i++;
        P(local.A[3], local.A[4], local.A[5], local.A[6], local.A[7], local.A[0], local.A[1], local.A[2], local.W[i], hsha2_sha384_K[i]);
        i++;
        P(local.A[2], local.A[3], local.A[4], local.A[5], local.A[6], local.A[7], local.A[0], local.A[1], local.W[i], hsha2_sha384_K[i]);
        i++;
        P(local.A[1], local.A[2], local.A[3], local.A[4], local.A[5], local.A[6], local.A[7], local.A[0], local.W[i], hsha2_sha384_K[i]);
        i++;
    }

#undef P
#undef R
#undef F1
#undef F0
#undef S3
#undef S2
#undef S1
#undef S0
#undef ROTR
#undef SHR

    ctx->A+=local.A[0];
    ctx->B+=local.A[1];
    ctx->C+=local.A[2];
    ctx->D+=local.A[3];
    ctx->E+=local.A[4];
    ctx->F+=local.A[5];
    ctx->G+=local.A[6];
    ctx->H+=local.A[7];


    return 0;
}

int hsha2_sha384_update(hsha2_sha384_context_t *ctx,const uint8_t *input,size_t ilen)
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

    if (ctx->total[0] < (uint32_t) ilen)
    {
        ctx->total[1]++;
    }

    if (left && ilen >= fill)
    {
        memcpy((void *) (ctx->buffer + left), input, fill);
        if ((ret = hsha2_sha384_internal_process(ctx, ctx->buffer)) != 0)
        {
            return ret;
        }

        input += fill;
        ilen  -= fill;
        left = 0;
    }

    while (ilen >= (sizeof(ctx->buffer)))
    {
        if ((ret = hsha2_sha384_internal_process(ctx, input)) != 0)
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

int hsha2_sha384_finish(hsha2_sha384_context_t *ctx,hsha2_sha384_t output)
{
    int ret = -1;
    if(ctx==NULL || output == NULL)
    {
        return ret;
    }
    uint32_t used=0;
    uint64_t high=0, low=0;

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

        if ((ret = hsha2_sha384_internal_process(ctx, ctx->buffer)) != 0)
        {
            return ret;
        }

        memset(ctx->buffer, 0, (sizeof(ctx->buffer)-sizeof(ctx->total)));
    }

    /*
     * 添加消息长度
     */
    high = (ctx->total[0] >> 61)| (ctx->total[1] <<  3);
    low  = (ctx->total[0] <<  3);

    {
        ctx->buffer[112+7]=((high)&0xFF);
        ctx->buffer[112+6]=((high>>8)&0xFF);
        ctx->buffer[112+5]=((high>>16)&0xFF);
        ctx->buffer[112+4]=((high>>24)&0xFF);
        ctx->buffer[112+3]=((high>>32)&0xFF);
        ctx->buffer[112+2]=((high>>40)&0xFF);
        ctx->buffer[112+1]=((high>>48)&0xFF);
        ctx->buffer[112+0]=((high>>56)&0xFF);
    }
    {
        ctx->buffer[120+7]=((low)&0xFF);
        ctx->buffer[120+6]=((low>>8)&0xFF);
        ctx->buffer[120+5]=((low>>16)&0xFF);
        ctx->buffer[120+4]=((low>>24)&0xFF);
        ctx->buffer[120+3]=((low>>32)&0xFF);
        ctx->buffer[120+2]=((low>>40)&0xFF);
        ctx->buffer[120+1]=((low>>48)&0xFF);
        ctx->buffer[120+0]=((low>>56)&0xFF);
    }

    if ((ret = hsha2_sha384_internal_process(ctx, ctx->buffer)) != 0)
    {
        return ret;
    }

    /*
     * 输出
     */
    {
        output[0+7]=((ctx->A)&0xFF);
        output[0+6]=((ctx->A>>8)&0xFF);
        output[0+5]=((ctx->A>>16)&0xFF);
        output[0+4]=((ctx->A>>24)&0xFF);
        output[0+3]=((ctx->A>>32)&0xFF);
        output[0+2]=((ctx->A>>40)&0xFF);
        output[0+1]=((ctx->A>>48)&0xFF);
        output[0+0]=((ctx->A>>56)&0xFF);
    }
    {
        output[8+7]=((ctx->B)&0xFF);
        output[8+6]=((ctx->B>>8)&0xFF);
        output[8+5]=((ctx->B>>16)&0xFF);
        output[8+4]=((ctx->B>>24)&0xFF);
        output[8+3]=((ctx->B>>32)&0xFF);
        output[8+2]=((ctx->B>>40)&0xFF);
        output[8+1]=((ctx->B>>48)&0xFF);
        output[8+0]=((ctx->B>>56)&0xFF);
    }
    {
        output[16+7]=((ctx->C)&0xFF);
        output[16+6]=((ctx->C>>8)&0xFF);
        output[16+5]=((ctx->C>>16)&0xFF);
        output[16+4]=((ctx->C>>24)&0xFF);
        output[16+3]=((ctx->C>>32)&0xFF);
        output[16+2]=((ctx->C>>40)&0xFF);
        output[16+1]=((ctx->C>>48)&0xFF);
        output[16+0]=((ctx->C>>56)&0xFF);
    }
    {
        output[24+7]=((ctx->D)&0xFF);
        output[24+6]=((ctx->D>>8)&0xFF);
        output[24+5]=((ctx->D>>16)&0xFF);
        output[24+4]=((ctx->D>>24)&0xFF);
        output[24+3]=((ctx->D>>32)&0xFF);
        output[24+2]=((ctx->D>>40)&0xFF);
        output[24+1]=((ctx->D>>48)&0xFF);
        output[24+0]=((ctx->D>>56)&0xFF);
    }
    {
        output[32+7]=((ctx->E)&0xFF);
        output[32+6]=((ctx->E>>8)&0xFF);
        output[32+5]=((ctx->E>>16)&0xFF);
        output[32+4]=((ctx->E>>24)&0xFF);
        output[32+3]=((ctx->E>>32)&0xFF);
        output[32+2]=((ctx->E>>40)&0xFF);
        output[32+1]=((ctx->E>>48)&0xFF);
        output[32+0]=((ctx->E>>56)&0xFF);
    }
    {
        output[40+7]=((ctx->F)&0xFF);
        output[40+6]=((ctx->F>>8)&0xFF);
        output[40+5]=((ctx->F>>16)&0xFF);
        output[40+4]=((ctx->F>>24)&0xFF);
        output[40+3]=((ctx->F>>32)&0xFF);
        output[40+2]=((ctx->F>>40)&0xFF);
        output[40+1]=((ctx->F>>48)&0xFF);
        output[40+0]=((ctx->F>>56)&0xFF);
    }

    ret = 0;
    return ret;

}

int hsha2_sha384(const uint8_t *input,size_t ilen,hsha2_sha384_t output)
{
    int ret = -1;
    if(input==NULL || output == NULL)
    {
        return ret;
    }
    hsha2_sha384_context_t ctx;

    if ((ret = hsha2_sha384_starts(&ctx)) != 0)
    {
        goto exit;
    }

    if ((ret = hsha2_sha384_update(&ctx, input, ilen)) != 0)
    {
        goto exit;
    }

    if ((ret = hsha2_sha384_finish(&ctx, output)) != 0)
    {
        goto exit;
    }

exit:
    return ret;

}


int hsha2_sha512_starts(hsha2_sha512_context_t *ctx)
{
    if(ctx==NULL)
    {
        return -1;
    }
    memset(ctx,0,sizeof(hsha2_sha512_context_t));
    ctx->A = (uint64_t)(0x6A09E667F3BCC908);
    ctx->B = (uint64_t)(0xBB67AE8584CAA73B);
    ctx->C = (uint64_t)(0x3C6EF372FE94F82B);
    ctx->D = (uint64_t)(0xA54FF53A5F1D36F1);
    ctx->E = (uint64_t)(0x510E527FADE682D1);
    ctx->F = (uint64_t)(0x9B05688C2B3E6C1F);
    ctx->G = (uint64_t)(0x1F83D9ABFB41BD6B);
    ctx->H = (uint64_t)(0x5BE0CD19137E2179);
    return 0;
}

int hsha2_sha512_update(hsha2_sha512_context_t *ctx,const uint8_t *input,size_t ilen)
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

    if (ctx->total[0] < (uint32_t) ilen)
    {
        ctx->total[1]++;
    }

    if (left && ilen >= fill)
    {
        memcpy((void *) (ctx->buffer + left), input, fill);
        if ((ret = hsha2_sha384_internal_process(ctx, ctx->buffer)) != 0)
        {
            return ret;
        }

        input += fill;
        ilen  -= fill;
        left = 0;
    }

    while (ilen >= (sizeof(ctx->buffer)))
    {
        if ((ret = hsha2_sha384_internal_process(ctx, input)) != 0)
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

int hsha2_sha512_finish(hsha2_sha512_context_t *ctx,hsha2_sha512_t output)
{
    int ret = -1;
    if(ctx==NULL || output == NULL)
    {
        return ret;
    }
    uint32_t used=0;
    uint64_t high=0, low=0;

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

        if ((ret = hsha2_sha384_internal_process(ctx, ctx->buffer)) != 0)
        {
            return ret;
        }

        memset(ctx->buffer, 0, (sizeof(ctx->buffer)-sizeof(ctx->total)));
    }

    /*
     * 添加消息长度
     */
    high = (ctx->total[0] >> 61)| (ctx->total[1] <<  3);
    low  = (ctx->total[0] <<  3);

    {
        ctx->buffer[112+7]=((high)&0xFF);
        ctx->buffer[112+6]=((high>>8)&0xFF);
        ctx->buffer[112+5]=((high>>16)&0xFF);
        ctx->buffer[112+4]=((high>>24)&0xFF);
        ctx->buffer[112+3]=((high>>32)&0xFF);
        ctx->buffer[112+2]=((high>>40)&0xFF);
        ctx->buffer[112+1]=((high>>48)&0xFF);
        ctx->buffer[112+0]=((high>>56)&0xFF);
    }
    {
        ctx->buffer[120+7]=((low)&0xFF);
        ctx->buffer[120+6]=((low>>8)&0xFF);
        ctx->buffer[120+5]=((low>>16)&0xFF);
        ctx->buffer[120+4]=((low>>24)&0xFF);
        ctx->buffer[120+3]=((low>>32)&0xFF);
        ctx->buffer[120+2]=((low>>40)&0xFF);
        ctx->buffer[120+1]=((low>>48)&0xFF);
        ctx->buffer[120+0]=((low>>56)&0xFF);
    }

    if ((ret = hsha2_sha384_internal_process(ctx, ctx->buffer)) != 0)
    {
        return ret;
    }

    /*
     * 输出
     */
    {
        output[0+7]=((ctx->A)&0xFF);
        output[0+6]=((ctx->A>>8)&0xFF);
        output[0+5]=((ctx->A>>16)&0xFF);
        output[0+4]=((ctx->A>>24)&0xFF);
        output[0+3]=((ctx->A>>32)&0xFF);
        output[0+2]=((ctx->A>>40)&0xFF);
        output[0+1]=((ctx->A>>48)&0xFF);
        output[0+0]=((ctx->A>>56)&0xFF);
    }
    {
        output[8+7]=((ctx->B)&0xFF);
        output[8+6]=((ctx->B>>8)&0xFF);
        output[8+5]=((ctx->B>>16)&0xFF);
        output[8+4]=((ctx->B>>24)&0xFF);
        output[8+3]=((ctx->B>>32)&0xFF);
        output[8+2]=((ctx->B>>40)&0xFF);
        output[8+1]=((ctx->B>>48)&0xFF);
        output[8+0]=((ctx->B>>56)&0xFF);
    }
    {
        output[16+7]=((ctx->C)&0xFF);
        output[16+6]=((ctx->C>>8)&0xFF);
        output[16+5]=((ctx->C>>16)&0xFF);
        output[16+4]=((ctx->C>>24)&0xFF);
        output[16+3]=((ctx->C>>32)&0xFF);
        output[16+2]=((ctx->C>>40)&0xFF);
        output[16+1]=((ctx->C>>48)&0xFF);
        output[16+0]=((ctx->C>>56)&0xFF);
    }
    {
        output[24+7]=((ctx->D)&0xFF);
        output[24+6]=((ctx->D>>8)&0xFF);
        output[24+5]=((ctx->D>>16)&0xFF);
        output[24+4]=((ctx->D>>24)&0xFF);
        output[24+3]=((ctx->D>>32)&0xFF);
        output[24+2]=((ctx->D>>40)&0xFF);
        output[24+1]=((ctx->D>>48)&0xFF);
        output[24+0]=((ctx->D>>56)&0xFF);
    }
    {
        output[32+7]=((ctx->E)&0xFF);
        output[32+6]=((ctx->E>>8)&0xFF);
        output[32+5]=((ctx->E>>16)&0xFF);
        output[32+4]=((ctx->E>>24)&0xFF);
        output[32+3]=((ctx->E>>32)&0xFF);
        output[32+2]=((ctx->E>>40)&0xFF);
        output[32+1]=((ctx->E>>48)&0xFF);
        output[32+0]=((ctx->E>>56)&0xFF);
    }
    {
        output[40+7]=((ctx->F)&0xFF);
        output[40+6]=((ctx->F>>8)&0xFF);
        output[40+5]=((ctx->F>>16)&0xFF);
        output[40+4]=((ctx->F>>24)&0xFF);
        output[40+3]=((ctx->F>>32)&0xFF);
        output[40+2]=((ctx->F>>40)&0xFF);
        output[40+1]=((ctx->F>>48)&0xFF);
        output[40+0]=((ctx->F>>56)&0xFF);
    }
    {
        output[48+7]=((ctx->G)&0xFF);
        output[48+6]=((ctx->G>>8)&0xFF);
        output[48+5]=((ctx->G>>16)&0xFF);
        output[48+4]=((ctx->G>>24)&0xFF);
        output[48+3]=((ctx->G>>32)&0xFF);
        output[48+2]=((ctx->G>>40)&0xFF);
        output[48+1]=((ctx->G>>48)&0xFF);
        output[48+0]=((ctx->G>>56)&0xFF);
    }
    {
        output[56+7]=((ctx->H)&0xFF);
        output[56+6]=((ctx->H>>8)&0xFF);
        output[56+5]=((ctx->H>>16)&0xFF);
        output[56+4]=((ctx->H>>24)&0xFF);
        output[56+3]=((ctx->H>>32)&0xFF);
        output[56+2]=((ctx->H>>40)&0xFF);
        output[56+1]=((ctx->H>>48)&0xFF);
        output[56+0]=((ctx->H>>56)&0xFF);
    }

    ret = 0;
    return ret;

}

int hsha2_sha512(const uint8_t *input,size_t ilen,hsha2_sha512_t output)
{
    int ret = -1;
    if(input==NULL || output == NULL)
    {
        return ret;
    }
    hsha2_sha512_context_t ctx;

    if ((ret = hsha2_sha512_starts(&ctx)) != 0)
    {
        goto exit;
    }

    if ((ret = hsha2_sha512_update(&ctx, input, ilen)) != 0)
    {
        goto exit;
    }

    if ((ret = hsha2_sha512_finish(&ctx, output)) != 0)
    {
        goto exit;
    }

exit:
    return ret;

}



#ifdef H
#undef H
#endif // H
#define H(b63, b31, b15) (b63 << 6 | b31 << 5 | b15 << 4)
static const uint8_t hsha3_iota_r_packed[24] =
{
    H(0, 0, 0) | 0x01, H(0, 0, 1) | 0x82, H(1, 0, 1) | 0x8a, H(1, 1, 1) | 0x00,
    H(0, 0, 1) | 0x8b, H(0, 1, 0) | 0x01, H(1, 1, 1) | 0x81, H(1, 0, 1) | 0x09,
    H(0, 0, 0) | 0x8a, H(0, 0, 0) | 0x88, H(0, 1, 1) | 0x09, H(0, 1, 0) | 0x0a,
    H(0, 1, 1) | 0x8b, H(1, 0, 0) | 0x8b, H(1, 0, 1) | 0x89, H(1, 0, 1) | 0x03,
    H(1, 0, 1) | 0x02, H(1, 0, 0) | 0x80, H(0, 0, 1) | 0x0a, H(1, 1, 0) | 0x0a,
    H(1, 1, 1) | 0x81, H(1, 0, 1) | 0x80, H(0, 1, 0) | 0x01, H(1, 1, 1) | 0x08,
};
#undef H

static const uint32_t hsha3_rho[6] =
{
    0x3f022425, 0x1c143a09, 0x2c3d3615, 0x27191713, 0x312b382e, 0x3e030832
};

static const uint32_t hsha3_pi[6] =
{
    0x110b070a, 0x10050312, 0x04181508, 0x0d13170f, 0x0e14020c, 0x01060916
};

#ifdef ROTR64
#undef ROTR64
#endif // ROTR64
#define ROTR64(x, y) (((x) << (64U - (y))) | ((x) >> (y))) // 64-bit rotate right
#ifdef ABSORB
#undef ABSORB
#endif // ABSORB
#define ABSORB(ctx, idx, v) do { ctx->state[(idx) >> 3] ^= ((uint64_t) (v)) << (((idx) & 0x7) << 3); \
} while (0)
#ifdef  SQUEEZE
#undef  SQUEEZE
#endif // SQUEEZE
#define SQUEEZE(ctx, idx) ((uint8_t) (ctx->state[(idx) >> 3] >> (((idx) & 0x7) << 3)))
#ifdef SWAP
#undef SWAP
#endif // SWAP
#define SWAP(x, y) do { uint64_t tmp = (x); (x) = (y); (y) = tmp; } while (0)

/* The permutation function.  */
static void hsha3_keccak_f1600(hsha3_context_t *ctx)
{
    uint64_t lane[5];
    uint64_t *s = ctx->state;
    int i;
    for (int round = 0; round < 24; round++)
    {
        uint64_t t;
        for (i = 0; i < 5; i++)
        {
            lane[i] = s[i] ^ s[i + 5] ^ s[i + 10] ^ s[i + 15] ^ s[i + 20];
        }
        for (i = 0; i < 5; i++)
        {
            t = lane[(i + 4) % 5] ^ ROTR64(lane[(i + 1) % 5], 63);
            s[i] ^= t;
            s[i + 5] ^= t;
            s[i + 10] ^= t;
            s[i + 15] ^= t;
            s[i + 20] ^= t;
        }

        /* Rho */
        for (i = 1; i < 25; i += 4)
        {
            uint32_t r = hsha3_rho[(i - 1) >> 2];
            for (int j = i; j < i + 4; j++) {
                uint8_t r8 = (uint8_t) (r >> 24);
                r <<= 8;
                s[j] = ROTR64(s[j], r8);
            }
        }

        /* Pi */
        t = s[1];
        for (i = 0; i < 24; i += 4)
        {
            uint32_t p = hsha3_pi[i >> 2];
            for (unsigned j = 0; j < 4; j++) {
                SWAP(s[p & 0xff], t);
                p >>= 8;
            }
        }

        /* Chi */
        for (i = 0; i <= 20; i += 5)
        {
            lane[0] = s[i];
            lane[1] = s[i + 1];
            lane[2] = s[i + 2];
            lane[3] = s[i + 3];
            lane[4] = s[i + 4];
            s[i + 0] ^= (~lane[1]) & lane[2];
            s[i + 1] ^= (~lane[2]) & lane[3];
            s[i + 2] ^= (~lane[3]) & lane[4];
            s[i + 3] ^= (~lane[4]) & lane[0];
            s[i + 4] ^= (~lane[0]) & lane[1];
        }

        /* Iota */
        /* Decompress the round masks (see definition of rc) */
        s[0] ^= ((hsha3_iota_r_packed[round] & 0x40ull) << 57 |
                 (hsha3_iota_r_packed[round] & 0x20ull) << 26 |
                 (hsha3_iota_r_packed[round] & 0x10ull) << 11 |
                 (hsha3_iota_r_packed[round] & 0x8f));
    }
}

#define HSHA3_GET_UINT64_LE( data, offset )               \
    (                                                       \
          ( (uint64_t) ( data )[( offset ) + 7] << 56 )     \
        | ( (uint64_t) ( data )[( offset ) + 6] << 48 )     \
        | ( (uint64_t) ( data )[( offset ) + 5] << 40 )     \
        | ( (uint64_t) ( data )[( offset ) + 4] << 32 )     \
        | ( (uint64_t) ( data )[( offset ) + 3] << 24 )     \
        | ( (uint64_t) ( data )[( offset ) + 2] << 16 )     \
        | ( (uint64_t) ( data )[( offset ) + 1] <<  8 )     \
        | ( (uint64_t) ( data )[( offset )    ]       )     \
    )

static int hsha3_update(hsha3_context_t *ctx,const uint8_t *input,size_t ilen)
{
    if (ilen >= 8)
    {
        // 8-byte align index
        int align_bytes = 8 - (ctx->index % 8);
        if (align_bytes)
        {
            for (; align_bytes > 0; align_bytes--)
            {
                ABSORB(ctx, ctx->index, *input++);
                ilen--;
                ctx->index++;
            }
            if ((ctx->index = ctx->index % ctx->max_block_size) == 0)
            {
                hsha3_keccak_f1600(ctx);
            }
        }

        // process input in 8-byte chunks
        while (ilen >= 8)
        {
            ABSORB(ctx, ctx->index, HSHA3_GET_UINT64_LE(input, 0));
            input += 8;
            ilen -= 8;
            if ((ctx->index = (ctx->index + 8) % ctx->max_block_size) == 0)
            {
                hsha3_keccak_f1600(ctx);
            }
        }
    }

    // handle remaining bytes
    while (ilen-- > 0)
    {
        ABSORB(ctx, ctx->index, *input++);
        if ((ctx->index = (ctx->index + 1) % ctx->max_block_size) == 0)
        {
            hsha3_keccak_f1600(ctx);
        }
    }

    return 0;
}

#define HSHA3_XOR_BYTE 0x6

static int hsha3_finish(hsha3_context_t *ctx,uint8_t *output, size_t olen)
{
    int ret = -1;

    /* Catch SHA-3 families, with fixed output length */
    if (ctx->olen > 0)
    {
        if (ctx->olen > olen)
        {
            ret = -1;
            return ret;
        }
        olen = ctx->olen;
    }

    ABSORB(ctx, ctx->index, HSHA3_XOR_BYTE);
    ABSORB(ctx, ctx->max_block_size - 1, 0x80);
    hsha3_keccak_f1600(ctx);
    ctx->index = 0;

    while (olen-- > 0)
    {
        *output++ = SQUEEZE(ctx, ctx->index);

        if ((ctx->index = (ctx->index + 1) % ctx->max_block_size) == 0)
        {
            hsha3_keccak_f1600(ctx);
        }
    }

    ret = 0;
    return ret;
}

#define HSHA3_SHAKE_XOR_BYTE 0x1f

static int hsha3_shake_finish(hsha3_context_t *ctx,uint8_t *output, size_t olen)
{
    int ret = -1;

    if (ctx->olen > 0)
    {
        if (ctx->olen > olen)
        {
            ret = -1;
            return ret;
        }
    }

    ABSORB(ctx, ctx->index, HSHA3_SHAKE_XOR_BYTE);
    ABSORB(ctx, ctx->max_block_size - 1, 0x80);
    hsha3_keccak_f1600(ctx);
    ctx->index = 0;

    while (olen-- > 0)
    {
        *output++ = SQUEEZE(ctx, ctx->index);

        if ((ctx->index = (ctx->index + 1) % ctx->max_block_size) == 0)
        {
            hsha3_keccak_f1600(ctx);
        }
    }

    ret = 0;
    return ret;
}

#undef ROTR64
#undef ABSORB
#undef SQUEEZE
#undef SWAP

int hsha3_sha224_starts(hsha3_sha224_context_t *ctx)
{
    if(ctx==NULL)
    {
        return -1;
    }
    memset(ctx,0,sizeof(hsha3_sha224_context_t));
    ctx->olen=sizeof(hsha3_sha224_t);
    ctx->max_block_size=sizeof(hsha3_sha224_message_block_t);
    return 0;
}

int hsha3_sha224_update(hsha3_sha224_context_t *ctx,const uint8_t *input,size_t ilen)
{
    if(ctx==NULL)
    {
        return -1;
    }
    if(input==NULL || ilen == 0)
    {
        return 0;
    }
    return hsha3_update(ctx,input,ilen);
}

int hsha3_sha224_finish(hsha3_sha224_context_t *ctx,hsha3_sha224_t output)
{
    if(ctx==NULL || output == NULL)
    {
        return -1;
    }
    return hsha3_finish(ctx,output,sizeof(hsha3_sha224_t));
}


int hsha3_sha224(const uint8_t *input,size_t ilen,hsha3_sha224_t output)
{
    int ret = -1;
    if(input==NULL || output == NULL)
    {
        return ret;
    }
    hsha3_sha224_context_t ctx;

    if ((ret = hsha3_sha224_starts(&ctx)) != 0)
    {
        goto exit;
    }

    if ((ret = hsha3_sha224_update(&ctx, input, ilen)) != 0)
    {
        goto exit;
    }

    if ((ret = hsha3_sha224_finish(&ctx, output)) != 0)
    {
        goto exit;
    }

exit:
    return ret;

}


int hsha3_sha256_starts(hsha3_sha256_context_t *ctx)
{
    if(ctx==NULL)
    {
        return -1;
    }
    memset(ctx,0,sizeof(hsha3_sha256_context_t));
    ctx->olen=sizeof(hsha3_sha256_t);
    ctx->max_block_size=sizeof(hsha3_sha256_message_block_t);
    return 0;
}

int hsha3_sha256_update(hsha3_sha256_context_t *ctx,const uint8_t *input,size_t ilen)
{
    if(ctx==NULL)
    {
        return -1;
    }
    if(input==NULL || ilen == 0)
    {
        return 0;
    }
    return hsha3_update(ctx,input,ilen);
}

int hsha3_sha256_finish(hsha3_sha256_context_t *ctx,hsha3_sha256_t output)
{
    if(ctx==NULL || output == NULL)
    {
        return -1;
    }
    return hsha3_finish(ctx,output,sizeof(hsha3_sha256_t));
}


int hsha3_sha256(const uint8_t *input,size_t ilen,hsha3_sha256_t output)
{
    int ret = -1;
    if(input==NULL || output == NULL)
    {
        return ret;
    }
    hsha3_sha256_context_t ctx;

    if ((ret = hsha3_sha256_starts(&ctx)) != 0)
    {
        goto exit;
    }

    if ((ret = hsha3_sha256_update(&ctx, input, ilen)) != 0)
    {
        goto exit;
    }

    if ((ret = hsha3_sha256_finish(&ctx, output)) != 0)
    {
        goto exit;
    }

exit:
    return ret;

}


int hsha3_sha384_starts(hsha3_sha384_context_t *ctx)
{
    if(ctx==NULL)
    {
        return -1;
    }
    memset(ctx,0,sizeof(hsha3_sha384_context_t));
    ctx->olen=sizeof(hsha3_sha384_t);
    ctx->max_block_size=sizeof(hsha3_sha384_message_block_t);
    return 0;
}

int hsha3_sha384_update(hsha3_sha384_context_t *ctx,const uint8_t *input,size_t ilen)
{
    if(ctx==NULL)
    {
        return -1;
    }
    if(input==NULL || ilen == 0)
    {
        return 0;
    }
    return hsha3_update(ctx,input,ilen);
}

int hsha3_sha384_finish(hsha3_sha384_context_t *ctx,hsha3_sha384_t output)
{
    if(ctx==NULL || output == NULL)
    {
        return -1;
    }
    return hsha3_finish(ctx,output,sizeof(hsha3_sha384_t));
}


int hsha3_sha384(const uint8_t *input,size_t ilen,hsha3_sha384_t output)
{
    int ret = -1;
    if(input==NULL || output == NULL)
    {
        return ret;
    }
    hsha3_sha384_context_t ctx;

    if ((ret = hsha3_sha384_starts(&ctx)) != 0)
    {
        goto exit;
    }

    if ((ret = hsha3_sha384_update(&ctx, input, ilen)) != 0)
    {
        goto exit;
    }

    if ((ret = hsha3_sha384_finish(&ctx, output)) != 0)
    {
        goto exit;
    }

exit:
    return ret;

}

int hsha3_sha512_starts(hsha3_sha512_context_t *ctx)
{
    if(ctx==NULL)
    {
        return -1;
    }
    memset(ctx,0,sizeof(hsha3_sha512_context_t));
    ctx->olen=sizeof(hsha3_sha512_t);
    ctx->max_block_size=sizeof(hsha3_sha512_message_block_t);
    return 0;
}

int hsha3_sha512_update(hsha3_sha512_context_t *ctx,const uint8_t *input,size_t ilen)
{
    if(ctx==NULL)
    {
        return -1;
    }
    if(input==NULL || ilen == 0)
    {
        return 0;
    }
    return hsha3_update(ctx,input,ilen);
}

int hsha3_sha512_finish(hsha3_sha512_context_t *ctx,hsha3_sha512_t output)
{
    if(ctx==NULL || output == NULL)
    {
        return -1;
    }
    return hsha3_finish(ctx,output,sizeof(hsha3_sha512_t));
}


int hsha3_sha512(const uint8_t *input,size_t ilen,hsha3_sha512_t output)
{
    int ret = -1;
    if(input==NULL || output == NULL)
    {
        return ret;
    }
    hsha3_sha512_context_t ctx;

    if ((ret = hsha3_sha512_starts(&ctx)) != 0)
    {
        goto exit;
    }

    if ((ret = hsha3_sha512_update(&ctx, input, ilen)) != 0)
    {
        goto exit;
    }

    if ((ret = hsha3_sha512_finish(&ctx, output)) != 0)
    {
        goto exit;
    }

exit:
    return ret;

}

int hsha3_shake128_starts(hsha3_shake128_context_t *ctx)
{
    if(ctx==NULL)
    {
        return -1;
    }
    memset(ctx,0,sizeof(hsha3_shake128_context_t));
    ctx->olen=sizeof(hsha3_shake128_t);
    ctx->max_block_size=sizeof(hsha3_shake128_message_block_t);
    return 0;
}

int hsha3_shake128_update(hsha3_shake128_context_t *ctx,const uint8_t *input,size_t ilen)
{
    if(ctx==NULL)
    {
        return -1;
    }
    if(input==NULL || ilen == 0)
    {
        return 0;
    }
    return hsha3_update(ctx,input,ilen);
}

int hsha3_shake128_finish(hsha3_shake128_context_t *ctx,hsha3_shake128_t output,size_t output_len)
{
    if(ctx==NULL || output == NULL || output_len < sizeof(hsha3_shake128_t) )
    {
        return -1;
    }
    return hsha3_shake_finish(ctx,output,output_len);
}


int hsha3_shake128(const uint8_t *input,size_t ilen,hsha3_shake128_t output,size_t output_len)
{
    int ret = -1;
    if(input==NULL || output == NULL)
    {
        return ret;
    }
    hsha3_shake128_context_t ctx;

    if ((ret = hsha3_shake128_starts(&ctx)) != 0)
    {
        goto exit;
    }

    if ((ret = hsha3_shake128_update(&ctx, input, ilen)) != 0)
    {
        goto exit;
    }

    if ((ret = hsha3_shake128_finish(&ctx, output,output_len)) != 0)
    {
        goto exit;
    }

exit:
    return ret;

}

int hsha3_shake256_starts(hsha3_shake256_context_t *ctx)
{
    if(ctx==NULL)
    {
        return -1;
    }
    memset(ctx,0,sizeof(hsha3_shake256_context_t));
    ctx->olen=sizeof(hsha3_shake256_t);
    ctx->max_block_size=sizeof(hsha3_shake256_message_block_t);
    return 0;
}

int hsha3_shake256_update(hsha3_shake256_context_t *ctx,const uint8_t *input,size_t ilen)
{
    if(ctx==NULL)
    {
        return -1;
    }
    if(input==NULL || ilen == 0)
    {
        return 0;
    }
    return hsha3_update(ctx,input,ilen);
}

int hsha3_shake256_finish(hsha3_shake256_context_t *ctx,hsha3_shake256_t output,size_t output_len)
{
    if(ctx==NULL || output == NULL || output_len < sizeof(hsha3_shake256_t) )
    {
        return -1;
    }
    return hsha3_shake_finish(ctx,output,output_len);
}


int hsha3_shake256(const uint8_t *input,size_t ilen,hsha3_shake256_t output,size_t output_len)
{
    int ret = -1;
    if(input==NULL || output == NULL)
    {
        return ret;
    }
    hsha3_shake256_context_t ctx;

    if ((ret = hsha3_shake256_starts(&ctx)) != 0)
    {
        goto exit;
    }

    if ((ret = hsha3_shake256_update(&ctx, input, ilen)) != 0)
    {
        goto exit;
    }

    if ((ret = hsha3_shake256_finish(&ctx, output,output_len)) != 0)
    {
        goto exit;
    }

exit:
    return ret;

}





