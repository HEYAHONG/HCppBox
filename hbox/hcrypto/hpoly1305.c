/***************************************************************
 * Name:      hpoly1305.c
 * Purpose:   实现hpoly1305接口
 * Author:    HYH (hyhsyctxem.cn)
 * Created:   2025-05-09
 * Copyright: HYH (hyhsyctxem.cn)
 * License:   MIT
 **************************************************************/
#include "hpoly1305.h"

/*
 * poly1305 implementation using 32 bit * 32 bit = 64 bit multiplication
 * and 64 bit addition.
 */

/* interpret four 8 bit unsigned integers as a 32 bit unsigned integer in little endian */
static uint32_t hpoly1305_u8to32(const uint8_t *p)
{
    return (((uint32_t)(p[0] & 0xff)) |
            ((uint32_t)(p[1] & 0xff) <<  8) |
            ((uint32_t)(p[2] & 0xff) << 16) |
            ((uint32_t)(p[3] & 0xff) << 24));
}

/* ctxore a 32 bit unsigned integer as four 8 bit unsigned integers in little endian */
static void hpoly1305_u32to8(uint8_t *p, uint32_t v)
{
    p[0] = (v) & 0xff;
    p[1] = (v >>  8) & 0xff;
    p[2] = (v >> 16) & 0xff;
    p[3] = (v >> 24) & 0xff;
}


static void  poly1305_blocks(hpoly1305_context_t *ctx, const uint8_t *m, size_t bytes)
{
    const uint32_t hibit = (ctx->is_final) ? 0 : (1 << 24); /* 1 << 128 */
    uint32_t r0, r1, r2, r3, r4;
    uint32_t s1, s2, s3, s4;
    uint32_t h0, h1, h2, h3, h4;
    uint64_t d0, d1, d2, d3, d4;
    uint32_t c;

    r0 = ctx->r[0];
    r1 = ctx->r[1];
    r2 = ctx->r[2];
    r3 = ctx->r[3];
    r4 = ctx->r[4];

    s1 = r1 * 5;
    s2 = r2 * 5;
    s3 = r3 * 5;
    s4 = r4 * 5;

    h0 = ctx->h[0];
    h1 = ctx->h[1];
    h2 = ctx->h[2];
    h3 = ctx->h[3];
    h4 = ctx->h[4];

    while (bytes >= HPOLY1305_BLOCK_SIZE)
    {
        /* h += m[i] */
        h0 += (hpoly1305_u8to32(m + 0)) & 0x3ffffff;
        h1 += (hpoly1305_u8to32(m + 3) >> 2) & 0x3ffffff;
        h2 += (hpoly1305_u8to32(m + 6) >> 4) & 0x3ffffff;
        h3 += (hpoly1305_u8to32(m + 9) >> 6) & 0x3ffffff;
        h4 += (hpoly1305_u8to32(m + 12) >> 8) | hibit;

        /* h *= r */
        d0 = ((uint64_t)h0 * r0) +
             ((uint64_t)h1 * s4) +
             ((uint64_t)h2 * s3) +
             ((uint64_t)h3 * s2) +
             ((uint64_t)h4 * s1);
        d1 = ((uint64_t)h0 * r1) +
             ((uint64_t)h1 * r0) +
             ((uint64_t)h2 * s4) +
             ((uint64_t)h3 * s3) +
             ((uint64_t)h4 * s2);
        d2 = ((uint64_t)h0 * r2) +
             ((uint64_t)h1 * r1) +
             ((uint64_t)h2 * r0) +
             ((uint64_t)h3 * s4) +
             ((uint64_t)h4 * s3);
        d3 = ((uint64_t)h0 * r3) +
             ((uint64_t)h1 * r2) +
             ((uint64_t)h2 * r1) +
             ((uint64_t)h3 * r0) +
             ((uint64_t)h4 * s4);
        d4 = ((uint64_t)h0 * r4) +
             ((uint64_t)h1 * r3) +
             ((uint64_t)h2 * r2) +
             ((uint64_t)h3 * r1) +
             ((uint64_t)h4 * r0);

        /* (partial) h %= p */
        c = (uint32_t)(d0 >> 26);
        h0 = (uint32_t)d0 & 0x3ffffff;
        d1 += c;
        c = (uint32_t)(d1 >> 26);
        h1 = (uint32_t)d1 & 0x3ffffff;
        d2 += c;
        c = (uint32_t)(d2 >> 26);
        h2 = (uint32_t)d2 & 0x3ffffff;
        d3 += c;
        c = (uint32_t)(d3 >> 26);
        h3 = (uint32_t)d3 & 0x3ffffff;
        d4 += c;
        c = (uint32_t)(d4 >> 26);
        h4 = (uint32_t)d4 & 0x3ffffff;
        h0 += c * 5;
        c = (h0 >> 26);
        h0 = h0 & 0x3ffffff;
        h1 += c;

        m += HPOLY1305_BLOCK_SIZE;
        bytes -= HPOLY1305_BLOCK_SIZE;
    }

    ctx->h[0] = h0;
    ctx->h[1] = h1;
    ctx->h[2] = h2;
    ctx->h[3] = h3;
    ctx->h[4] = h4;
}

int hpoly1305_starts(hpoly1305_context_t *ctx,hpoly1305_key_t key)
{
    int ret=-1;
    if(ctx==NULL || key == NULL)
    {
        return ret;
    }

    memset(ctx,0,sizeof(hpoly1305_context_t));

    /* r &= 0xffffffc0ffffffc0ffffffc0fffffff */
    ctx->r[0] = (hpoly1305_u8to32(&key[0])) & 0x3ffffff;
    ctx->r[1] = (hpoly1305_u8to32(&key[3]) >> 2) & 0x3ffff03;
    ctx->r[2] = (hpoly1305_u8to32(&key[6]) >> 4) & 0x3ffc0ff;
    ctx->r[3] = (hpoly1305_u8to32(&key[9]) >> 6) & 0x3f03fff;
    ctx->r[4] = (hpoly1305_u8to32(&key[12]) >> 8) & 0x00fffff;

    /* h = 0 */
    ctx->h[0] = 0;
    ctx->h[1] = 0;
    ctx->h[2] = 0;
    ctx->h[3] = 0;
    ctx->h[4] = 0;

    /* save pad for later */
    ctx->pad[0] = hpoly1305_u8to32(&key[16]);
    ctx->pad[1] = hpoly1305_u8to32(&key[20]);
    ctx->pad[2] = hpoly1305_u8to32(&key[24]);
    ctx->pad[3] = hpoly1305_u8to32(&key[28]);

    ctx->leftover = 0;
    ctx->is_final = 0;
    ret=0;
    return ret;
}

int hpoly1305_update(hpoly1305_context_t *ctx,const uint8_t *message,size_t message_len)
{
    int ret=-1;
    if(ctx==NULL)
    {
        return ret;
    }
    ret=0;
    if(message==NULL || message_len == 0)
    {
        return ret;
    }

    size_t i;

    /* handle leftover */
    if (ctx->leftover)
    {
        size_t want = (HPOLY1305_BLOCK_SIZE - ctx->leftover);
        if (want > message_len)
            want = message_len;
        for (i = 0; i < want; i++)
            ctx->buffer[ctx->leftover + i] = message[i];
        message_len -= want;
        message += want;
        ctx->leftover += want;
        if (ctx->leftover < HPOLY1305_BLOCK_SIZE)
            return ret;
        poly1305_blocks(ctx, ctx->buffer, HPOLY1305_BLOCK_SIZE);
        ctx->leftover = 0;
    }

    /* process full blocks */
    if (message_len >= HPOLY1305_BLOCK_SIZE)
    {
        size_t want = (message_len & ~(HPOLY1305_BLOCK_SIZE - 1));
        poly1305_blocks(ctx, message, want);
        message += want;
        message_len -= want;
    }

    /* ctxore leftover */
    if (message_len)
    {
        for (i = 0; i < message_len; i++)
            ctx->buffer[ctx->leftover + i] = message[i];
        ctx->leftover += message_len;
    }


    return ret;
}

int hpoly1305_finish(hpoly1305_context_t  *ctx,hpoly1305_mac_t mac)
{
    int ret=-1;
    if(ctx==NULL ||mac == NULL)
    {
        return ret;
    }
    ret=0;

    uint32_t h0, h1, h2, h3, h4, c;
    uint32_t g0, g1, g2, g3, g4;
    uint64_t f;
    uint32_t mask;

    /* process the remaining block */
    if (ctx->leftover)
    {
        size_t i = ctx->leftover;
        ctx->buffer[i++] = 1;
        for (; i < HPOLY1305_BLOCK_SIZE; i++)
            ctx->buffer[i] = 0;
        ctx->is_final = 1;
        poly1305_blocks(ctx, ctx->buffer, HPOLY1305_BLOCK_SIZE);
    }

    /* fully carry h */
    h0 = ctx->h[0];
    h1 = ctx->h[1];
    h2 = ctx->h[2];
    h3 = ctx->h[3];
    h4 = ctx->h[4];

    c = h1 >> 26;
    h1 = h1 & 0x3ffffff;
    h2 += c;
    c = h2 >> 26;
    h2 = h2 & 0x3ffffff;
    h3 += c;
    c = h3 >> 26;
    h3 = h3 & 0x3ffffff;
    h4 += c;
    c = h4 >> 26;
    h4 = h4 & 0x3ffffff;
    h0 += c * 5;
    c = h0 >> 26;
    h0 = h0 & 0x3ffffff;
    h1 += c;

    /* compute h + -p */
    g0 = h0 + 5;
    c = g0 >> 26;
    g0 &= 0x3ffffff;
    g1 = h1 + c;
    c = g1 >> 26;
    g1 &= 0x3ffffff;
    g2 = h2 + c;
    c = g2 >> 26;
    g2 &= 0x3ffffff;
    g3 = h3 + c;
    c = g3 >> 26;
    g3 &= 0x3ffffff;
    g4 = h4 + c - (1 << 26);

    /* select h if h < p, or h + -p if h >= p */
    mask = (g4 >> ((sizeof(uint32_t) * 8) - 1)) - 1;
    g0 &= mask;
    g1 &= mask;
    g2 &= mask;
    g3 &= mask;
    g4 &= mask;
    mask = ~mask;
    h0 = (h0 & mask) | g0;
    h1 = (h1 & mask) | g1;
    h2 = (h2 & mask) | g2;
    h3 = (h3 & mask) | g3;
    h4 = (h4 & mask) | g4;

    /* h = h % (2^128) */
    h0 = ((h0) | (h1 << 26)) & 0xffffffff;
    h1 = ((h1 >>  6) | (h2 << 20)) & 0xffffffff;
    h2 = ((h2 >> 12) | (h3 << 14)) & 0xffffffff;
    h3 = ((h3 >> 18) | (h4 <<  8)) & 0xffffffff;

    /* mac = (h + pad) % (2^128) */
    f = (uint64_t)h0 + ctx->pad[0];
    h0 = (uint32_t)f;
    f = (uint64_t)h1 + ctx->pad[1] + (f >> 32);
    h1 = (uint32_t)f;
    f = (uint64_t)h2 + ctx->pad[2] + (f >> 32);
    h2 = (uint32_t)f;
    f = (uint64_t)h3 + ctx->pad[3] + (f >> 32);
    h3 = (uint32_t)f;

    hpoly1305_u32to8(mac +  0, h0);
    hpoly1305_u32to8(mac +  4, h1);
    hpoly1305_u32to8(mac +  8, h2);
    hpoly1305_u32to8(mac + 12, h3);

    /* zero out the state */
    ctx->h[0] = 0;
    ctx->h[1] = 0;
    ctx->h[2] = 0;
    ctx->h[3] = 0;
    ctx->h[4] = 0;
    ctx->r[0] = 0;
    ctx->r[1] = 0;
    ctx->r[2] = 0;
    ctx->r[3] = 0;
    ctx->r[4] = 0;
    ctx->pad[0] = 0;
    ctx->pad[1] = 0;
    ctx->pad[2] = 0;
    ctx->pad[3] = 0;

    return ret;
}
