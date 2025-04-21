/***************************************************************
 * Name:      hsm4.c
 * Purpose:   实现hsm4接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hsm4.h"

#define HSM4_ROTL32(n,x) (((x)<<(n)) | ((x)>>((-(n)&31))))
#define HSM4_READ_UINT32(p)                         \
        (  (((uint32_t) (p)[0]) << 24)              \
        | (((uint32_t) (p)[1]) << 16)               \
        | (((uint32_t) (p)[2]) << 8)                \
        |  ((uint32_t) (p)[3]))

#define HSM4_WRITE_UINT32(p, i)                     \
        do {                                        \
        (p)[0] = ((i) >> 24) & 0xff;                \
        (p)[1] = ((i) >> 16) & 0xff;                \
        (p)[2] = ((i) >> 8) & 0xff;                 \
        (p)[3] = (i) & 0xff;                        \
        } while(0)

static const uint32_t hsm4_fk[4] =
{
    0xa3b1bac6, 0x56aa3350, 0x677d9197, 0xb27022dc
};

static const uint32_t hsm4_ck[32] =
{
    0x00070e15, 0x1c232a31, 0x383f464d, 0x545b6269,
    0x70777e85, 0x8c939aa1, 0xa8afb6bd, 0xc4cbd2d9,
    0xe0e7eef5, 0xfc030a11, 0x181f262d, 0x343b4249,
    0x50575e65, 0x6c737a81, 0x888f969d, 0xa4abb2b9,
    0xc0c7ced5, 0xdce3eaf1, 0xf8ff060d, 0x141b2229,
    0x30373e45, 0x4c535a61, 0x686f767d, 0x848b9299,
    0xa0a7aeb5, 0xbcc3cad1, 0xd8dfe6ed, 0xf4fb0209,
    0x10171e25, 0x2c333a41, 0x484f565d, 0x646b7279
};

static const uint8_t hsm4_sbox[256] =
{
    0xd6, 0x90, 0xe9, 0xfe, 0xcc, 0xe1, 0x3d, 0xb7,
    0x16, 0xb6, 0x14, 0xc2, 0x28, 0xfb, 0x2c, 0x05,
    0x2b, 0x67, 0x9a, 0x76, 0x2a, 0xbe, 0x04, 0xc3,
    0xaa, 0x44, 0x13, 0x26, 0x49, 0x86, 0x06, 0x99,
    0x9c, 0x42, 0x50, 0xf4, 0x91, 0xef, 0x98, 0x7a,
    0x33, 0x54, 0x0b, 0x43, 0xed, 0xcf, 0xac, 0x62,
    0xe4, 0xb3, 0x1c, 0xa9, 0xc9, 0x08, 0xe8, 0x95,
    0x80, 0xdf, 0x94, 0xfa, 0x75, 0x8f, 0x3f, 0xa6,
    0x47, 0x07, 0xa7, 0xfc, 0xf3, 0x73, 0x17, 0xba,
    0x83, 0x59, 0x3c, 0x19, 0xe6, 0x85, 0x4f, 0xa8,
    0x68, 0x6b, 0x81, 0xb2, 0x71, 0x64, 0xda, 0x8b,
    0xf8, 0xeb, 0x0f, 0x4b, 0x70, 0x56, 0x9d, 0x35,
    0x1e, 0x24, 0x0e, 0x5e, 0x63, 0x58, 0xd1, 0xa2,
    0x25, 0x22, 0x7c, 0x3b, 0x01, 0x21, 0x78, 0x87,
    0xd4, 0x00, 0x46, 0x57, 0x9f, 0xd3, 0x27, 0x52,
    0x4c, 0x36, 0x02, 0xe7, 0xa0, 0xc4, 0xc8, 0x9e,
    0xea, 0xbf, 0x8a, 0xd2, 0x40, 0xc7, 0x38, 0xb5,
    0xa3, 0xf7, 0xf2, 0xce, 0xf9, 0x61, 0x15, 0xa1,
    0xe0, 0xae, 0x5d, 0xa4, 0x9b, 0x34, 0x1a, 0x55,
    0xad, 0x93, 0x32, 0x30, 0xf5, 0x8c, 0xb1, 0xe3,
    0x1d, 0xf6, 0xe2, 0x2e, 0x82, 0x66, 0xca, 0x60,
    0xc0, 0x29, 0x23, 0xab, 0x0d, 0x53, 0x4e, 0x6f,
    0xd5, 0xdb, 0x37, 0x45, 0xde, 0xfd, 0x8e, 0x2f,
    0x03, 0xff, 0x6a, 0x72, 0x6d, 0x6c, 0x5b, 0x51,
    0x8d, 0x1b, 0xaf, 0x92, 0xbb, 0xdd, 0xbc, 0x7f,
    0x11, 0xd9, 0x5c, 0x41, 0x1f, 0x10, 0x5a, 0xd8,
    0x0a, 0xc1, 0x31, 0x88, 0xa5, 0xcd, 0x7b, 0xbd,
    0x2d, 0x74, 0xd0, 0x12, 0xb8, 0xe5, 0xb4, 0xb0,
    0x89, 0x69, 0x97, 0x4a, 0x0c, 0x96, 0x77, 0x7e,
    0x65, 0xb9, 0xf1, 0x09, 0xc5, 0x6e, 0xc6, 0x84,
    0x18, 0xf0, 0x7d, 0xec, 0x3a, 0xdc, 0x4d, 0x20,
    0x79, 0xee, 0x5f, 0x3e, 0xd7, 0xcb, 0x39, 0x48
};

static uint32_t hsm4_t_non_lin_sub(uint32_t x)
{
    uint32_t out=0;

    out  = (uint32_t)hsm4_sbox[x & 0xff];
    out |= (uint32_t)hsm4_sbox[(x >> 8) & 0xff] << 8;
    out |= (uint32_t)hsm4_sbox[(x >> 16) & 0xff] << 16;
    out |= (uint32_t)hsm4_sbox[(x >> 24) & 0xff] << 24;

    return out;
}

static uint32_t hsm4_key_lin_sub(uint32_t x)
{
    return x ^ HSM4_ROTL32(13, x) ^ HSM4_ROTL32(23, x);
}

static uint32_t hsm4_enc_lin_sub(uint32_t x)
{
    return x ^ HSM4_ROTL32(2, x) ^ HSM4_ROTL32(10, x) ^ HSM4_ROTL32(18, x) ^ HSM4_ROTL32(24, x);
}

static uint32_t hsm4_key_sub(uint32_t x)
{
    return hsm4_key_lin_sub(hsm4_t_non_lin_sub(x));
}

static uint32_t hsm4_enc_sub(uint32_t x)
{
    return hsm4_enc_lin_sub(hsm4_t_non_lin_sub(x));
}

static uint32_t hsm4_round(uint32_t x0, uint32_t x1, uint32_t x2, uint32_t x3, uint32_t rk)
{
    return x0 ^ hsm4_enc_sub(x1 ^ x2 ^ x3 ^ rk);
}

static void hsm4_set_key(hsm4_key_t *ctx, const uint8_t *key, int encrypt)
{
    uint32_t rk0, rk1, rk2, rk3;
    unsigned i;

    rk0 = HSM4_READ_UINT32(key +  0) ^ hsm4_fk[0];
    rk1 = HSM4_READ_UINT32(key +  4) ^ hsm4_fk[1];
    rk2 = HSM4_READ_UINT32(key +  8) ^ hsm4_fk[2];
    rk3 = HSM4_READ_UINT32(key + 12) ^ hsm4_fk[3];

    for (i = 0; i < 32; i += 4)
    {
        rk0 ^= hsm4_key_sub(rk1 ^ rk2 ^ rk3 ^ hsm4_ck[i + 0]);
        rk1 ^= hsm4_key_sub(rk2 ^ rk3 ^ rk0 ^ hsm4_ck[i + 1]);
        rk2 ^= hsm4_key_sub(rk3 ^ rk0 ^ rk1 ^ hsm4_ck[i + 2]);
        rk3 ^= hsm4_key_sub(rk0 ^ rk1 ^ rk2 ^ hsm4_ck[i + 3]);

        if (encrypt)
        {
            ctx->rkey[i + 0] = rk0;
            ctx->rkey[i + 1] = rk1;
            ctx->rkey[i + 2] = rk2;
            ctx->rkey[i + 3] = rk3;
        }
        else
        {
            ctx->rkey[31 - 0 - i] = rk0;
            ctx->rkey[31 - 1 - i] = rk1;
            ctx->rkey[31 - 2 - i] = rk2;
            ctx->rkey[31 - 3 - i] = rk3;
        }
    }
}


static void hsm4_crypt(const hsm4_key_t *context,uint8_t *dst,const uint8_t *src)
{
    const uint32_t *rk = context->rkey;

    {
        uint32_t x0, x1, x2, x3;
        unsigned i;

        x0 = HSM4_READ_UINT32(src + 0 * 4);
        x1 = HSM4_READ_UINT32(src + 1 * 4);
        x2 = HSM4_READ_UINT32(src + 2 * 4);
        x3 = HSM4_READ_UINT32(src + 3 * 4);

        for (i = 0; i < 32; i += 4)
        {
            x0 = hsm4_round(x0, x1, x2, x3, rk[i + 0]);
            x1 = hsm4_round(x1, x2, x3, x0, rk[i + 1]);
            x2 = hsm4_round(x2, x3, x0, x1, rk[i + 2]);
            x3 = hsm4_round(x3, x0, x1, x2, rk[i + 3]);
        }

        HSM4_WRITE_UINT32(dst + 0 * 4, x3);
        HSM4_WRITE_UINT32(dst + 1 * 4, x2);
        HSM4_WRITE_UINT32(dst + 2 * 4, x1);
        HSM4_WRITE_UINT32(dst + 3 * 4, x0);

        src += HSM4_BLOCK_SIZE;
        dst += HSM4_BLOCK_SIZE;
    }
}


int hsm4_set_encrypt_key(const uint8_t *userKey,hsm4_key_t *key)
{
    if(userKey == NULL ||key == NULL)
    {
        return -1;
    }
    hsm4_set_key(key, userKey, 1);
    return 0;
}
int hsm4_set_decrypt_key(const uint8_t *userKey, hsm4_key_t *key)
{
    if(userKey == NULL ||key == NULL)
    {
        return -1;
    }
    hsm4_set_key(key, userKey, 0);
    return 0;
}

void hsm4_encrypt(const uint8_t *in, uint8_t *out,const hsm4_key_t *key)
{
    if(in==NULL || out==NULL || key == NULL)
    {
        return;
    }
    hsm4_crypt(key,out,in);
}
void hsm4_decrypt(const uint8_t *in, uint8_t *out,const hsm4_key_t *key)
{
    if(in==NULL || out==NULL || key==NULL)
    {
        return;
    }
    hsm4_crypt(key,out,in);
}
void hsm4_cbc_encrypt(const uint8_t *in, uint8_t *out,const size_t length, const hsm4_key_t *key, uint8_t *ivec, bool enc)
{
    if(in == NULL || out == NULL || key==NULL ||ivec == NULL )
    {
        return;
    }
    size_t n;
    size_t len = length;
    uint8_t tmp[HSM4_BLOCK_SIZE]= {0};
    if (enc)
    {
        while (len >= HSM4_BLOCK_SIZE)
        {
            for(n=0; n < HSM4_BLOCK_SIZE; ++n)
            {
                tmp[n] = in[n] ^ ivec[n];
            }
            hsm4_encrypt(tmp, out, key);
            memcpy(ivec, out, HSM4_BLOCK_SIZE);
            len -= HSM4_BLOCK_SIZE;
            in += HSM4_BLOCK_SIZE;
            out += HSM4_BLOCK_SIZE;
        }
        if (len)
        {
            for(n=0; n < len; ++n)
            {
                tmp[n] = in[n] ^ ivec[n];
            }
            for(n=len; n < HSM4_BLOCK_SIZE; ++n)
            {
                tmp[n] = ivec[n];
            }
            hsm4_encrypt(tmp, tmp, key);
            memcpy(out, tmp, HSM4_BLOCK_SIZE);
            memcpy(ivec, tmp, HSM4_BLOCK_SIZE);
        }
    }
    else
    {
        while (len >= HSM4_BLOCK_SIZE)
        {
            memcpy(tmp, in, HSM4_BLOCK_SIZE);
            hsm4_decrypt(in, out, key);
            for(n=0; n < HSM4_BLOCK_SIZE; ++n)
            {
                out[n] ^= ivec[n];
            }
            memcpy(ivec, tmp, HSM4_BLOCK_SIZE);
            len -= HSM4_BLOCK_SIZE;
            in += HSM4_BLOCK_SIZE;
            out += HSM4_BLOCK_SIZE;
        }
        if (len)
        {
            memcpy(tmp, in, HSM4_BLOCK_SIZE);
            hsm4_decrypt(tmp, tmp, key);
            for(n=0; n < len; ++n)
            {
                out[n] = tmp[n] ^ ivec[n];
            }
            memcpy(ivec, tmp, HSM4_BLOCK_SIZE);
        }
    }
}

