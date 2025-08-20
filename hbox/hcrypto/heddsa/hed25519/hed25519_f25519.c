/***************************************************************
 * Name:      hed25519_f25519.c
 * Purpose:   实现hed25519_f25519接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-08-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hed25519_f25519.h"

const hed25519_f25519_t hed25519_f25519_one = {1};

void hed25519_f25519_load(hed25519_f25519_t x, uint32_t c)
{
    int i;

    for (i = 0; i < sizeof(c); i++)
    {
        x[i] = c;
        c >>= 8;
    }

    for (; i < HED25519_F25519_SIZE; i++)
        x[i] = 0;
}

void hed25519_f25519_normalize(hed25519_f25519_t x)
{
    uint8_t minusp[HED25519_F25519_SIZE];
    uint16_t c;
    int i;

    /* Reduce using 2^255 = 19 mod p */
    c = (x[31] >> 7) * 19;
    x[31] &= 127;

    for (i = 0; i < HED25519_F25519_SIZE; i++)
    {
        c += x[i];
        x[i] = c;
        c >>= 8;
    }

    /* The number is now less than 2^255 + 18, and therefore less than
     * 2p. Try subtracting p, and conditionally load the subtracted
     * value if underflow did not occur.
     */
    c = 19;

    for (i = 0; i + 1 < HED25519_F25519_SIZE; i++)
    {
        c += x[i];
        minusp[i] = c;
        c >>= 8;
    }

    c += ((uint16_t)x[i]) - 128;
    minusp[31] = c;

    /* Load x-p if no underflow */
    hed25519_f25519_select(x, minusp, x, (c >> 15) & 1);
}

uint8_t hed25519_f25519_eq(const hed25519_f25519_t x, const hed25519_f25519_t y)
{
    uint8_t sum = 0;
    int i;

    for (i = 0; i < HED25519_F25519_SIZE; i++)
        sum |= x[i] ^ y[i];

    sum |= (sum >> 4);
    sum |= (sum >> 2);
    sum |= (sum >> 1);

    return (sum ^ 1) & 1;
}

void hed25519_f25519_select(hed25519_f25519_t dst,const hed25519_f25519_t zero, const hed25519_f25519_t one,uint8_t condition)
{
    const uint8_t mask = -condition;
    int i;

    for (i = 0; i < HED25519_F25519_SIZE; i++)
        dst[i] = zero[i] ^ (mask & (one[i] ^ zero[i]));
}

void hed25519_f25519_add(hed25519_f25519_t r, const hed25519_f25519_t a, const hed25519_f25519_t b)
{
    uint16_t c = 0;
    int i;

    /* Add */
    for (i = 0; i < HED25519_F25519_SIZE; i++)
    {
        c >>= 8;
        c += ((uint16_t)a[i]) + ((uint16_t)b[i]);
        r[i] = c;
    }

    /* Reduce with 2^255 = 19 mod p */
    r[31] &= 127;
    c = (c >> 7) * 19;

    for (i = 0; i < HED25519_F25519_SIZE; i++)
    {
        c += r[i];
        r[i] = c;
        c >>= 8;
    }
}

void hed25519_f25519_sub(hed25519_f25519_t r, const hed25519_f25519_t a, const hed25519_f25519_t b)
{
    uint32_t c = 0;
    int i;

    /* Calculate a + 2p - b, to avoid underflow */
    c = 218;
    for (i = 0; i + 1 < HED25519_F25519_SIZE; i++)
    {
        c += 65280 + ((uint32_t)a[i]) - ((uint32_t)b[i]);
        r[i] = c;
        c >>= 8;
    }

    c += ((uint32_t)a[31]) - ((uint32_t)b[31]);
    r[31] = c & 127;
    c = (c >> 7) * 19;

    for (i = 0; i < HED25519_F25519_SIZE; i++)
    {
        c += r[i];
        r[i] = c;
        c >>= 8;
    }
}

void hed25519_f25519_neg(hed25519_f25519_t r, const hed25519_f25519_t a)
{
    uint32_t c = 0;
    int i;

    /* Calculate 2p - a, to avoid underflow */
    c = 218;
    for (i = 0; i + 1 < HED25519_F25519_SIZE; i++)
    {
        c += 65280 - ((uint32_t)a[i]);
        r[i] = c;
        c >>= 8;
    }

    c -= ((uint32_t)a[31]);
    r[31] = c & 127;
    c = (c >> 7) * 19;

    for (i = 0; i < HED25519_F25519_SIZE; i++)
    {
        c += r[i];
        r[i] = c;
        c >>= 8;
    }
}

void hed25519_f25519_mul__distinct(hed25519_f25519_t r, const hed25519_f25519_t a, const hed25519_f25519_t b)
{
    uint32_t c = 0;
    int i;

    for (i = 0; i < HED25519_F25519_SIZE; i++)
    {
        int j;

        c >>= 8;
        for (j = 0; j <= i; j++)
            c += ((uint32_t)a[j]) * ((uint32_t)b[i - j]);

        for (; j < HED25519_F25519_SIZE; j++)
            c += ((uint32_t)a[j]) *
                 ((uint32_t)b[i + HED25519_F25519_SIZE - j]) * 38;

        r[i] = c;
    }

    r[31] &= 127;
    c = (c >> 7) * 19;

    for (i = 0; i < HED25519_F25519_SIZE; i++)
    {
        c += r[i];
        r[i] = c;
        c >>= 8;
    }
}

static void hed25519_f25519_mul_c(hed25519_f25519_t r, const hed25519_f25519_t a, uint32_t b)
{
    uint32_t c = 0;
    int i;

    for (i = 0; i < HED25519_F25519_SIZE; i++)
    {
        c >>= 8;
        c += b * ((uint32_t)a[i]);
        r[i] = c;
    }

    r[31] &= 127;
    c >>= 7;
    c *= 19;

    for (i = 0; i < HED25519_F25519_SIZE; i++)
    {
        c += r[i];
        r[i] = c;
        c >>= 8;
    }
}

void hed25519_f25519_inv__distinct(hed25519_f25519_t r, const hed25519_f25519_t x)
{
    uint8_t s[HED25519_F25519_SIZE];
    int i;

    /* This is a prime field, so by Fermat's little theorem:
     *
     *     x^(p-1) = 1 mod p
     *
     * Therefore, raise to (p-2) = 2^255-21 to get a multiplicative
     * inverse.
     *
     * This is a 255-bit binary number with the digits:
     *
     *     11111111... 01011
     *
     * We compute the result by the usual binary chain, but
     * alternate between keeping the accumulator in r and s, so as
     * to avoid copying temporaries.
     */

    /* 1 1 */
    hed25519_f25519_mul__distinct(s, x, x);
    hed25519_f25519_mul__distinct(r, s, x);

    /* 1 x 248 */
    for (i = 0; i < 248; i++)
    {
        hed25519_f25519_mul__distinct(s, r, r);
        hed25519_f25519_mul__distinct(r, s, x);
    }

    /* 0 */
    hed25519_f25519_mul__distinct(s, r, r);

    /* 1 */
    hed25519_f25519_mul__distinct(r, s, s);
    hed25519_f25519_mul__distinct(s, r, x);

    /* 0 */
    hed25519_f25519_mul__distinct(r, s, s);

    /* 1 */
    hed25519_f25519_mul__distinct(s, r, r);
    hed25519_f25519_mul__distinct(r, s, x);

    /* 1 */
    hed25519_f25519_mul__distinct(s, r, r);
    hed25519_f25519_mul__distinct(r, s, x);
}

/* Raise x to the power of (p-5)/8 = 2^252-3, using s for temporary
 * storage.
 */
static void hed25519_exp2523(hed25519_f25519_t r, const hed25519_f25519_t x, hed25519_f25519_t s)
{
    int i;

    /* This number is a 252-bit number with the binary expansion:
     *
     *     111111... 01
     */

    /* 1 1 */
    hed25519_f25519_mul__distinct(r, x, x);
    hed25519_f25519_mul__distinct(s, r, x);

    /* 1 x 248 */
    for (i = 0; i < 248; i++)
    {
        hed25519_f25519_mul__distinct(r, s, s);
        hed25519_f25519_mul__distinct(s, r, x);
    }

    /* 0 */
    hed25519_f25519_mul__distinct(r, s, s);

    /* 1 */
    hed25519_f25519_mul__distinct(s, r, r);
    hed25519_f25519_mul__distinct(r, s, x);
}

void hed25519_f25519_sqrt(hed25519_f25519_t r, const hed25519_f25519_t a)
{
    uint8_t v[HED25519_F25519_SIZE];
    uint8_t i[HED25519_F25519_SIZE];
    uint8_t x[HED25519_F25519_SIZE];
    uint8_t y[HED25519_F25519_SIZE];

    /* v = (2a)^((p-5)/8) [x = 2a] */
    hed25519_f25519_mul_c(x, a, 2);
    hed25519_exp2523(v, x, y);

    /* i = 2av^2 - 1 */
    hed25519_f25519_mul__distinct(y, v, v);
    hed25519_f25519_mul__distinct(i, x, y);
    hed25519_f25519_load(y, 1);
    hed25519_f25519_sub(i, i, y);

    /* r = avi */
    hed25519_f25519_mul__distinct(x, v, a);
    hed25519_f25519_mul__distinct(r, x, i);
}




