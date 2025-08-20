/***************************************************************
 * Name:      hed25519_fprime.c
 * Purpose:   实现hed25519_fprime接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-08-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hed25519_fprime.h"


static void hed25519_raw_add(hed25519_fprime_t x, const hed25519_fprime_t p)
{
    uint16_t c = 0;
    int i;

    for (i = 0; i < HED25519_FPRIME_SIZE; i++)
    {
        c += ((uint16_t)x[i]) + ((uint16_t)p[i]);
        x[i] = c;
        c >>= 8;
    }
}

static void hed25519_raw_try_sub(hed25519_fprime_t x, const hed25519_fprime_t p)
{
    uint8_t minusp[HED25519_FPRIME_SIZE];
    uint16_t c = 0;
    int i;

    for (i = 0; i < HED25519_FPRIME_SIZE; i++)
    {
        c = ((uint16_t)x[i]) - ((uint16_t)p[i]) - c;
        minusp[i] = c;
        c = (c >> 8) & 1;
    }

    hed25519_fprime_select(x, minusp, x, c);
}

static int hed25519_prime_msb(const hed25519_fprime_t p)
{
    int i;
    uint8_t x;

    for (i = HED25519_FPRIME_SIZE - 1; i >= 0; i--)
        if (p[i])
            break;

    x = p[i];
    i <<= 3;

    while (x)
    {
        x >>= 1;
        i++;
    }

    return i - 1;
}

static void hed25519_shift_n_bits(hed25519_fprime_t x, int n)
{
    uint16_t c = 0;
    int i;

    for (i = 0; i < HED25519_FPRIME_SIZE; i++)
    {
        c |= ((uint16_t)x[i]) << n;
        x[i] = c;
        c >>= 8;
    }
}

static inline int hed25519_min_int(int a, int b)
{
    return a < b ? a : b;
}

void hed25519_fprime_from_bytes(hed25519_fprime_t n,const uint8_t *x, size_t len,const hed25519_fprime_t modulus)
{
    const int preload_total = hed25519_min_int(hed25519_prime_msb(modulus) - 1, len << 3);
    const int preload_bytes = preload_total >> 3;
    const int preload_bits = preload_total & 7;
    const int rbits = (len << 3) - preload_total;
    int i;

    memset(n, 0, HED25519_FPRIME_SIZE);

    for (i = 0; i < preload_bytes; i++)
        n[i] = x[len - preload_bytes + i];

    if (preload_bits)
    {
        hed25519_shift_n_bits(n, preload_bits);
        n[0] |= x[len - preload_bytes - 1] >> (8 - preload_bits);
    }

    for (i = rbits - 1; i >= 0; i--)
    {
        const uint8_t bit = (x[i >> 3] >> (i & 7)) & 1;

        hed25519_shift_n_bits(n, 1);
        n[0] |= bit;
        hed25519_raw_try_sub(n, modulus);
    }
}

void hed25519_fprime_select(hed25519_fprime_t dst,const hed25519_fprime_t zero, const hed25519_fprime_t one,uint8_t condition)
{
    const uint8_t mask = -condition;
    int i;

    for (i = 0; i < HED25519_FPRIME_SIZE; i++)
        dst[i] = zero[i] ^ (mask & (one[i] ^ zero[i]));
}

void hed25519_fprime_add(hed25519_fprime_t r, const hed25519_fprime_t a, const hed25519_fprime_t modulus)
{
    hed25519_raw_add(r, a);
    hed25519_raw_try_sub(r, modulus);
}

void hed25519_fprime_mul(hed25519_fprime_t r, const hed25519_fprime_t a, const hed25519_fprime_t b,const hed25519_fprime_t modulus)
{
    int i;

    memset(r, 0, HED25519_FPRIME_SIZE);

    for (i = hed25519_prime_msb(modulus); i >= 0; i--)
    {
        const uint8_t bit = (b[i >> 3] >> (i & 7)) & 1;
        uint8_t plusa[HED25519_FPRIME_SIZE];

        hed25519_shift_n_bits(r, 1);
        hed25519_raw_try_sub(r, modulus);

        hed25519_fprime_copy(plusa, r);
        hed25519_fprime_add(plusa, a, modulus);

        hed25519_fprime_select(r, r, plusa, bit);
    }
}






