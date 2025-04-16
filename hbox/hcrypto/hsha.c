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
    for(size_t i=0; i<sizeof(hsha1_message_block_t)/sizeof(local.W[0]); i++)
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
        if ((ret = hsha2_sha224_internal_process(ctx, ctx->buffer)) != 0)
        {
            return ret;
        }

        input += fill;
        ilen  -= fill;
        left = 0;
    }

    while (ilen >= 64)
    {
        if ((ret = hsha2_sha224_internal_process(ctx, input)) != 0)
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

        if ((ret = hsha2_sha224_internal_process(ctx, ctx->buffer)) != 0)
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
