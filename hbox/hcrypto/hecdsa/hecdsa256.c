/***************************************************************
 * Name:      hecdsa256.c
 * Purpose:   实现hecdsa256接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-08-13
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hecdsa256.h"

#define HECDSA256_NUM_DIGITS	    (HECDSA256_BYTES / sizeof(uint64_t))
#define HECDSA256_MAX_RETRY			(16)

const hecdsa256_curve_t hecdsa256_curve_secp256r1=
{
    /*
     * p，低位在前
     */
    {
        0xFFFFFFFFFFFFFFFFull, 0x00000000FFFFFFFFull, 0x0000000000000000ull, 0xFFFFFFFF00000001ull
    },
    /*
     * a的相反数
     */
    {
        0x0000000000000003ull, 0x00000000000000000ull, 0x0000000000000000ull, 0x000000000000000ull
    },
    /*
     * b,低位在前
     */
    {
        0x3BCE3C3E27D2604Bull, 0x651D06B0CC53B0F6ull, 0xB3EBBD55769886BCull, 0x5AC635D8AA3A93E7ull
    },
    /*
     * n,低位在前
     */
    {
        0xF3B9CAC2FC632551ull, 0xBCE6FAADA7179E84ull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFF00000000ull
    },
    /*
     * x,低位在前
     */
    {
        0xF4A13945D898C296ull, 0x77037D812DEB33A0ull, 0xF8BCE6E563A440F2ull, 0x6B17D1F2E12C4247ull
    },
    /*
     * y,低位在前
     */
    {
        0xCBB6406837BF51F5ull, 0x2BCE33576B315ECEull, 0x8EE7EB4A7C0F9E16ull, 0x4FE342E2FE1A7F9Bull
    }
};

struct hecdsa256_uint128_t
{
    uint64_t m_low;
    uint64_t m_high;
};

struct hecdsa256_point_t
{
    uint64_t x[HECDSA256_NUM_DIGITS];
    uint64_t y[HECDSA256_NUM_DIGITS];
};

#ifndef HECDSA256_RAND
#include "hdefaults.h"
#endif // HECDSA256_RAND

static int hecdsa256_get_random_number(uint64_t * vli)
{
    uint64_t v;
    int i;
#ifndef HECDSA256_RAND
    hgetrandom(vli,sizeof(uint64_t),0);
#else
    for(i = 0; i < HECDSA256_NUM_DIGITS; i++)
    {
        v = (uint64_t)(HECDSA256_RAND() & 0xffff) << 0;
        v |= (uint64_t)(HECDSA256_RAND() & 0xffff) << 16;
        v |= (uint64_t)(HECDSA256_RAND() & 0xffff) << 32;
        v |= (uint64_t)(HECDSA256_RAND() & 0xffff) << 48;
        vli[i] = v;
    }
#endif
    return 1;
}

static void hecdsa256_vli_clear(uint64_t * vli)
{
    int i;

    for(i = 0; i < HECDSA256_NUM_DIGITS; i++)
        vli[i] = 0;
}

static int hecdsa256_vli_iszero(const uint64_t * vli)
{
    int i;

    for(i = 0; i < HECDSA256_NUM_DIGITS; i++)
    {
        if(vli[i])
            return 0;
    }
    return 1;
}

static uint64_t hecdsa256_vli_testbit(uint64_t * vli, unsigned int bit)
{
    return (vli[bit / 64] & ((uint64_t)1 << (bit % 64)));
}

static unsigned int hecdsa256_vli_numdigits(const uint64_t * vli)
{
    int i;

    for(i = HECDSA256_NUM_DIGITS - 1; i >= 0 && vli[i] == 0; --i);
    return (i + 1);
}

static unsigned int hecdsa256_vli_numbits(const uint64_t * vli)
{
    uint64_t digit;
    unsigned int i;
    unsigned int n = hecdsa256_vli_numdigits(vli);

    if(n == 0)
        return 0;
    digit = vli[n - 1];
    for(i = 0; digit; i++)
        digit >>= 1;
    return ((n - 1) * 64 + i);
}

static void hecdsa256_vli_set(uint64_t * dst, const uint64_t * src)
{
    int i;

    for(i = 0; i < HECDSA256_NUM_DIGITS; i++)
        dst[i] = src[i];
}

static int hecdsa256_vli_cmp(const uint64_t * left,const uint64_t * right)
{
    int i;

    for(i = HECDSA256_NUM_DIGITS - 1; i >= 0; --i)
    {
        if(left[i] > right[i])
            return 1;
        else if(left[i] < right[i])
            return -1;
    }
    return 0;
}

static uint64_t hecdsa256_vli_lshift(uint64_t * result,const uint64_t * in, unsigned int shift)
{
    uint64_t carry = 0;
    int i;

    for(i = 0; i < HECDSA256_NUM_DIGITS; i++)
    {
        uint64_t temp = in[i];
        result[i] = (temp << shift) | carry;
        carry = temp >> (64 - shift);
    }
    return carry;
}

static void hecdsa256_vli_rshift1(uint64_t * vli)
{
    uint64_t * end = vli;
    uint64_t carry = 0;

    vli += HECDSA256_NUM_DIGITS;
    while(vli-- > end)
    {
        uint64_t temp = *vli;
        *vli = (temp >> 1) | carry;
        carry = temp << 63;
    }
}

static uint64_t hecdsa256_vli_add(uint64_t * result, const uint64_t * left,const uint64_t * right)
{
    uint64_t carry = 0;
    int i;

    for(i = 0; i < HECDSA256_NUM_DIGITS; i++)
    {
        uint64_t sum = left[i] + right[i] + carry;
        if(sum != left[i])
            carry = (sum < left[i]);
        result[i] = sum;
    }
    return carry;
}

static uint64_t hecdsa256_vli_sub(uint64_t * result, const uint64_t * left, const uint64_t * right)
{
    uint64_t borrow = 0;
    int i;

    for(i = 0; i < HECDSA256_NUM_DIGITS; i++)
    {
        uint64_t diff = left[i] - right[i] - borrow;
        if(diff != left[i])
            borrow = (diff > left[i]);
        result[i] = diff;
    }
    return borrow;
}

static struct hecdsa256_uint128_t hecdsa256_mul_64_64(uint64_t left, uint64_t right)
{
    struct hecdsa256_uint128_t result;

    uint64_t a0 = left & 0xffffffffull;
    uint64_t a1 = left >> 32;
    uint64_t b0 = right & 0xffffffffull;
    uint64_t b1 = right >> 32;
    uint64_t m0 = a0 * b0;
    uint64_t m1 = a0 * b1;
    uint64_t m2 = a1 * b0;
    uint64_t m3 = a1 * b1;

    m2 += (m0 >> 32);
    m2 += m1;
    if(m2 < m1)
        m3 += 0x100000000ull;
    result.m_low = (m0 & 0xffffffffull) | (m2 << 32);
    result.m_high = m3 + (m2 >> 32);

    return result;
}

static struct hecdsa256_uint128_t hecdsa256_add_128_128(struct hecdsa256_uint128_t a, struct hecdsa256_uint128_t b)
{
    struct hecdsa256_uint128_t result;
    result.m_low = a.m_low + b.m_low;
    result.m_high = a.m_high + b.m_high + (result.m_low < a.m_low);
    return result;
}

static void hecdsa256_vli_mult(uint64_t * result,const uint64_t * left, const uint64_t * right)
{
    struct hecdsa256_uint128_t r01 = { 0, 0 };
    uint64_t r2 = 0;
    unsigned int i, k;

    for(k = 0; k < HECDSA256_NUM_DIGITS * 2 - 1; ++k)
    {
        unsigned int min = (k < HECDSA256_NUM_DIGITS ? 0 : (k + 1) - HECDSA256_NUM_DIGITS);
        for(i = min; i <= k && i < HECDSA256_NUM_DIGITS; i++)
        {
            struct hecdsa256_uint128_t product = hecdsa256_mul_64_64(left[i], right[k - i]);
            r01 = hecdsa256_add_128_128(r01, product);
            r2 += (r01.m_high < product.m_high);
        }
        result[k] = r01.m_low;
        r01.m_low = r01.m_high;
        r01.m_high = r2;
        r2 = 0;
    }
    result[HECDSA256_NUM_DIGITS * 2 - 1] = r01.m_low;
}

static void hecdsa256_vli_square(uint64_t *result, uint64_t *left)
{
    struct hecdsa256_uint128_t r01 = { 0, 0 };
    uint64_t r2 = 0;
    unsigned int i, k;

    for(k = 0; k < HECDSA256_NUM_DIGITS * 2 - 1; ++k)
    {
        unsigned int min = (k < HECDSA256_NUM_DIGITS ? 0 : (k + 1) - HECDSA256_NUM_DIGITS);
        for(i = min; i <= k && i <= k - i; i++)
        {
            struct hecdsa256_uint128_t product = hecdsa256_mul_64_64(left[i], left[k - i]);
            if(i < k - i)
            {
                r2 += product.m_high >> 63;
                product.m_high = (product.m_high << 1) | (product.m_low >> 63);
                product.m_low <<= 1;
            }
            r01 = hecdsa256_add_128_128(r01, product);
            r2 += (r01.m_high < product.m_high);
        }
        result[k] = r01.m_low;
        r01.m_low = r01.m_high;
        r01.m_high = r2;
        r2 = 0;
    }
    result[HECDSA256_NUM_DIGITS * 2 - 1] = r01.m_low;
}

static void hecdsa256_vli_modadd(uint64_t * result, const uint64_t * left,const uint64_t * right,const uint64_t * mod)
{
    uint64_t carry = hecdsa256_vli_add(result, left, right);
    if(carry || hecdsa256_vli_cmp(result, mod) >= 0)
        hecdsa256_vli_sub(result, result, mod);
}

static void hecdsa256_vli_modsub(uint64_t * result,const uint64_t * left,const uint64_t * right,const uint64_t * mod)
{
    uint64_t borrow = hecdsa256_vli_sub(result, left, right);
    if(borrow)
        hecdsa256_vli_add(result, result, mod);
}

static void hecdsa256_vli_mmod_fast(const hecdsa256_curve_t *curve,uint64_t * result, uint64_t * product)
{
    uint64_t tmp[HECDSA256_NUM_DIGITS];
    int carry;

    if(curve==NULL)
    {
        curve=&hecdsa256_curve_secp256r1;
    }

    hecdsa256_vli_set(result, product);

    tmp[0] = 0;
    tmp[1] = product[5] & 0xffffffff00000000ull;
    tmp[2] = product[6];
    tmp[3] = product[7];
    carry = hecdsa256_vli_lshift(tmp, tmp, 1);
    carry += hecdsa256_vli_add(result, result, tmp);

    tmp[1] = product[6] << 32;
    tmp[2] = (product[6] >> 32) | (product[7] << 32);
    tmp[3] = product[7] >> 32;
    carry += hecdsa256_vli_lshift(tmp, tmp, 1);
    carry += hecdsa256_vli_add(result, result, tmp);

    tmp[0] = product[4];
    tmp[1] = product[5] & 0xffffffff;
    tmp[2] = 0;
    tmp[3] = product[7];
    carry += hecdsa256_vli_add(result, result, tmp);

    tmp[0] = (product[4] >> 32) | (product[5] << 32);
    tmp[1] = (product[5] >> 32) | (product[6] & 0xffffffff00000000ull);
    tmp[2] = product[7];
    tmp[3] = (product[6] >> 32) | (product[4] << 32);
    carry += hecdsa256_vli_add(result, result, tmp);

    tmp[0] = (product[5] >> 32) | (product[6] << 32);
    tmp[1] = (product[6] >> 32);
    tmp[2] = 0;
    tmp[3] = (product[4] & 0xffffffff) | (product[5] << 32);
    carry -= hecdsa256_vli_sub(result, result, tmp);

    tmp[0] = product[6];
    tmp[1] = product[7];
    tmp[2] = 0;
    tmp[3] = (product[4] >> 32) | (product[5] & 0xffffffff00000000ull);
    carry -= hecdsa256_vli_sub(result, result, tmp);

    tmp[0] = (product[6] >> 32) | (product[7] << 32);
    tmp[1] = (product[7] >> 32) | (product[4] << 32);
    tmp[2] = (product[4] >> 32) | (product[5] << 32);
    tmp[3] = (product[6] << 32);
    carry -= hecdsa256_vli_sub(result, result, tmp);

    tmp[0] = product[7];
    tmp[1] = product[4] & 0xffffffff00000000ull;
    tmp[2] = product[5];
    tmp[3] = product[6] & 0xffffffff00000000ull;
    carry -= hecdsa256_vli_sub(result, result, tmp);

    if(carry < 0)
    {
        do
        {
            carry += hecdsa256_vli_add(result, result, curve->p);
        }
        while(carry < 0);
    }
    else
    {
        while(carry || hecdsa256_vli_cmp(curve->p, result) != 1)
        {
            carry -= hecdsa256_vli_sub(result, result, curve->p);
        }
    }
}

static void hecdsa256_vli_modMult_fast(const hecdsa256_curve_t *curve,uint64_t * result, uint64_t * left, uint64_t * right)
{
    uint64_t product[2 * HECDSA256_NUM_DIGITS];
    hecdsa256_vli_mult(product, left, right);
    hecdsa256_vli_mmod_fast(curve,result, product);
}

static void hecdsa256_vli_modSquare_fast(const hecdsa256_curve_t *curve,uint64_t * result, uint64_t * left)
{
    uint64_t product[2 * HECDSA256_NUM_DIGITS];
    hecdsa256_vli_square(product, left);
    hecdsa256_vli_mmod_fast(curve,result, product);
}

static void hecdsa256_vli_modinv(uint64_t * result, const uint64_t * input,const uint64_t * mod)
{
    uint64_t a[HECDSA256_NUM_DIGITS], b[HECDSA256_NUM_DIGITS], u[HECDSA256_NUM_DIGITS], v[HECDSA256_NUM_DIGITS];
    uint64_t carry;
    int cmpresult;

    if(hecdsa256_vli_iszero(input))
    {
        hecdsa256_vli_clear(result);
        return;
    }
    hecdsa256_vli_set(a, input);
    hecdsa256_vli_set(b, mod);
    hecdsa256_vli_clear(u);
    u[0] = 1;
    hecdsa256_vli_clear(v);

    while((cmpresult = hecdsa256_vli_cmp(a, b)) != 0)
    {
        carry = 0;
        if(!(a[0] & 1))
        {
            hecdsa256_vli_rshift1(a);
            if(u[0] & 1)
                carry = hecdsa256_vli_add(u, u, mod);
            hecdsa256_vli_rshift1(u);
            if(carry)
                u[HECDSA256_NUM_DIGITS - 1] |= 0x8000000000000000ull;
        }
        else if(!(b[0] & 1))
        {
            hecdsa256_vli_rshift1(b);
            if(v[0] & 1)
                carry = hecdsa256_vli_add(v, v, mod);
            hecdsa256_vli_rshift1(v);
            if(carry)
                v[HECDSA256_NUM_DIGITS - 1] |= 0x8000000000000000ull;
        }
        else if(cmpresult > 0)
        {
            hecdsa256_vli_sub(a, a, b);
            hecdsa256_vli_rshift1(a);
            if(hecdsa256_vli_cmp(u, v) < 0)
                hecdsa256_vli_add(u, u, mod);
            hecdsa256_vli_sub(u, u, v);
            if(u[0] & 1)
                carry = hecdsa256_vli_add(u, u, mod);
            hecdsa256_vli_rshift1(u);
            if(carry)
                u[HECDSA256_NUM_DIGITS - 1] |= 0x8000000000000000ull;
        }
        else
        {
            hecdsa256_vli_sub(b, b, a);
            hecdsa256_vli_rshift1(b);
            if(hecdsa256_vli_cmp(v, u) < 0)
                hecdsa256_vli_add(v, v, mod);
            hecdsa256_vli_sub(v, v, u);
            if(v[0] & 1)
                carry = hecdsa256_vli_add(v, v, mod);
            hecdsa256_vli_rshift1(v);
            if(carry)
                v[HECDSA256_NUM_DIGITS - 1] |= 0x8000000000000000ull;
        }
    }
    hecdsa256_vli_set(result, u);
}

static int hecdsa256_eccpoint_iszero(struct hecdsa256_point_t * point)
{
    return (hecdsa256_vli_iszero(point->x) && hecdsa256_vli_iszero(point->y));
}

static void hecdsa256_eccpoint_double_jacobian(const hecdsa256_curve_t *curve,uint64_t * x1, uint64_t * y1, uint64_t * z1)
{
    uint64_t t4[HECDSA256_NUM_DIGITS];
    uint64_t t5[HECDSA256_NUM_DIGITS];

    if(curve==NULL)
    {
        curve=&hecdsa256_curve_secp256r1;
    }

    if(hecdsa256_vli_iszero(z1))
        return;
    hecdsa256_vli_modSquare_fast(curve,t4, y1);
    hecdsa256_vli_modMult_fast(curve,t5, x1, t4);
    hecdsa256_vli_modSquare_fast(curve,t4, t4);
    hecdsa256_vli_modMult_fast(curve,y1, y1, z1);
    hecdsa256_vli_modSquare_fast(curve,z1, z1);
    hecdsa256_vli_modadd(x1, x1, z1, curve->p);
    hecdsa256_vli_modadd(z1, z1, z1, curve->p);
    hecdsa256_vli_modsub(z1, x1, z1, curve->p);
    hecdsa256_vli_modMult_fast(curve,x1, x1, z1);
    hecdsa256_vli_modadd(z1, x1, x1, curve->p);
    hecdsa256_vli_modadd(x1, x1, z1, curve->p);
    if(hecdsa256_vli_testbit(x1, 0))
    {
        uint64_t carry = hecdsa256_vli_add(x1, x1, curve->p);
        hecdsa256_vli_rshift1(x1);
        x1[HECDSA256_NUM_DIGITS - 1] |= carry << 63;
    }
    else
    {
        hecdsa256_vli_rshift1(x1);
    }
    hecdsa256_vli_modSquare_fast(curve,z1, x1);
    hecdsa256_vli_modsub(z1, z1, t5, curve->p);
    hecdsa256_vli_modsub(z1, z1, t5, curve->p);
    hecdsa256_vli_modsub(t5, t5, z1, curve->p);
    hecdsa256_vli_modMult_fast(curve,x1, x1, t5);
    hecdsa256_vli_modsub(t4, x1, t4, curve->p);
    hecdsa256_vli_set(x1, z1);
    hecdsa256_vli_set(z1, y1);
    hecdsa256_vli_set(y1, t4);
}

static void hecdsa256_apply_z(const hecdsa256_curve_t *curve,uint64_t * x1, uint64_t * y1, uint64_t * z)
{
    uint64_t t1[HECDSA256_NUM_DIGITS];

    hecdsa256_vli_modSquare_fast(curve,t1, z);
    hecdsa256_vli_modMult_fast(curve,x1, x1, t1);
    hecdsa256_vli_modMult_fast(curve,t1, t1, z);
    hecdsa256_vli_modMult_fast(curve,y1, y1, t1);
}

static void hecdsa256_xycz_initial_double(const hecdsa256_curve_t *curve,uint64_t * x1, uint64_t * y1, uint64_t * x2, uint64_t * y2, uint64_t * initialz)
{
    uint64_t z[HECDSA256_NUM_DIGITS];

    if(curve==NULL)
    {
        curve=&hecdsa256_curve_secp256r1;
    }

    hecdsa256_vli_set(x2, x1);
    hecdsa256_vli_set(y2, y1);
    hecdsa256_vli_clear(z);
    z[0] = 1;
    if(initialz)
        hecdsa256_vli_set(z, initialz);
    hecdsa256_apply_z(curve,x1, y1, z);
    hecdsa256_eccpoint_double_jacobian(curve,x1, y1, z);
    hecdsa256_apply_z(curve,x2, y2, z);
}

static void hecdsa256_xycz_add(const hecdsa256_curve_t *curve,uint64_t * x1, uint64_t * y1, uint64_t * x2, uint64_t * y2)
{
    uint64_t t5[HECDSA256_NUM_DIGITS];

    if(curve==NULL)
    {
        curve=&hecdsa256_curve_secp256r1;
    }

    hecdsa256_vli_modsub(t5, x2, x1, curve->p);
    hecdsa256_vli_modSquare_fast(curve,t5, t5);
    hecdsa256_vli_modMult_fast(curve,x1, x1, t5);
    hecdsa256_vli_modMult_fast(curve,x2, x2, t5);
    hecdsa256_vli_modsub(y2, y2, y1, curve->p);
    hecdsa256_vli_modSquare_fast(curve,t5, y2);
    hecdsa256_vli_modsub(t5, t5, x1, curve->p);
    hecdsa256_vli_modsub(t5, t5, x2, curve->p);
    hecdsa256_vli_modsub(x2, x2, x1, curve->p);
    hecdsa256_vli_modMult_fast(curve,y1, y1, x2);
    hecdsa256_vli_modsub(x2, x1, t5, curve->p);
    hecdsa256_vli_modMult_fast(curve,y2, y2, x2);
    hecdsa256_vli_modsub(y2, y2, y1, curve->p);
    hecdsa256_vli_set(x2, t5);
}

static void hecdsa256_xycz_addc(const hecdsa256_curve_t *curve,uint64_t * x1, uint64_t * y1, uint64_t * x2, uint64_t * y2)
{
    uint64_t t5[HECDSA256_NUM_DIGITS];
    uint64_t t6[HECDSA256_NUM_DIGITS];
    uint64_t t7[HECDSA256_NUM_DIGITS];

    if(curve==NULL)
    {
        curve=&hecdsa256_curve_secp256r1;
    }

    hecdsa256_vli_modsub(t5, x2, x1, curve->p);
    hecdsa256_vli_modSquare_fast(curve,t5, t5);
    hecdsa256_vli_modMult_fast(curve,x1, x1, t5);
    hecdsa256_vli_modMult_fast(curve,x2, x2, t5);
    hecdsa256_vli_modadd(t5, y2, y1, curve->p);
    hecdsa256_vli_modsub(y2, y2, y1, curve->p);
    hecdsa256_vli_modsub(t6, x2, x1, curve->p);
    hecdsa256_vli_modMult_fast(curve,y1, y1, t6);
    hecdsa256_vli_modadd(t6, x1, x2, curve->p);
    hecdsa256_vli_modSquare_fast(curve,x2, y2);
    hecdsa256_vli_modsub(x2, x2, t6, curve->p);
    hecdsa256_vli_modsub(t7, x1, x2, curve->p);
    hecdsa256_vli_modMult_fast(curve,y2, y2, t7);
    hecdsa256_vli_modsub(y2, y2, y1, curve->p);
    hecdsa256_vli_modSquare_fast(curve,t7, t5);
    hecdsa256_vli_modsub(t7, t7, t6, curve->p);
    hecdsa256_vli_modsub(t6, t7, x1, curve->p);
    hecdsa256_vli_modMult_fast(curve,t6, t6, t5);
    hecdsa256_vli_modsub(y1, t6, y1, curve->p);
    hecdsa256_vli_set(x1, t7);
}

static void hecdsa256_eccpoint_mult(const hecdsa256_curve_t *curve,struct hecdsa256_point_t * result, struct hecdsa256_point_t * point, uint64_t * scalar, uint64_t * initialz)
{
    uint64_t Rx[2][HECDSA256_NUM_DIGITS];
    uint64_t Ry[2][HECDSA256_NUM_DIGITS];
    uint64_t z[HECDSA256_NUM_DIGITS];
    int i, nb;

    if(curve==NULL)
    {
        curve=&hecdsa256_curve_secp256r1;
    }

    hecdsa256_vli_set(Rx[1], point->x);
    hecdsa256_vli_set(Ry[1], point->y);
    hecdsa256_xycz_initial_double(curve,Rx[1], Ry[1], Rx[0], Ry[0], initialz);
    for(i = hecdsa256_vli_numbits(scalar) - 2; i > 0; --i)
    {
        nb = !hecdsa256_vli_testbit(scalar, i);
        hecdsa256_xycz_addc(curve,Rx[1 - nb], Ry[1 - nb], Rx[nb], Ry[nb]);
        hecdsa256_xycz_add(curve,Rx[nb], Ry[nb], Rx[1 - nb], Ry[1 - nb]);
    }
    nb = !hecdsa256_vli_testbit(scalar, 0);
    hecdsa256_xycz_addc(curve,Rx[1 - nb], Ry[1 - nb], Rx[nb], Ry[nb]);
    hecdsa256_vli_modsub(z, Rx[1], Rx[0], curve->p);
    hecdsa256_vli_modMult_fast(curve,z, z, Ry[1 - nb]);
    hecdsa256_vli_modMult_fast(curve,z, z, point->x);
    hecdsa256_vli_modinv(z, z, curve->p);
    hecdsa256_vli_modMult_fast(curve,z, z, point->y);
    hecdsa256_vli_modMult_fast(curve,z, z, Rx[1 - nb]);
    hecdsa256_xycz_add(curve,Rx[nb], Ry[nb], Rx[1 - nb], Ry[1 - nb]);
    hecdsa256_apply_z(curve,Rx[0], Ry[0], z);
    hecdsa256_vli_set(result->x, Rx[0]);
    hecdsa256_vli_set(result->y, Ry[0]);
}

static void hecdsa256_ecc_bytes2native(uint64_t * native, const uint8_t * bytes)
{
    int i;

    for(i = 0; i < HECDSA256_NUM_DIGITS; i++)
    {
        const uint8_t * digit = bytes + 8 * (HECDSA256_NUM_DIGITS - 1 - i);
        native[i] = ((uint64_t)digit[0] << 56) | ((uint64_t)digit[1] << 48) | ((uint64_t)digit[2] << 40) | ((uint64_t)digit[3] << 32)
                    | ((uint64_t)digit[4] << 24) | ((uint64_t)digit[5] << 16) | ((uint64_t)digit[6] << 8) | (uint64_t)digit[7];
    }
}

static void hecdsa256_ecc_native2bytes(uint8_t * bytes, const uint64_t * native)
{
    int i;

    for(i = 0; i < HECDSA256_NUM_DIGITS; i++)
    {
        uint8_t *digit = bytes + 8 * (HECDSA256_NUM_DIGITS - 1 - i);
        digit[0] = native[i] >> 56;
        digit[1] = native[i] >> 48;
        digit[2] = native[i] >> 40;
        digit[3] = native[i] >> 32;
        digit[4] = native[i] >> 24;
        digit[5] = native[i] >> 16;
        digit[6] = native[i] >> 8;
        digit[7] = native[i];
    }
}

static void hecdsa256_mod_sqrt(const hecdsa256_curve_t *curve,uint64_t * a)
{
    uint64_t p1[HECDSA256_NUM_DIGITS] = { 1 };
    uint64_t result[HECDSA256_NUM_DIGITS] = { 1 };
    int i;

    if(curve==NULL)
    {
        curve=&hecdsa256_curve_secp256r1;
    }

    hecdsa256_vli_add(p1, curve->p, p1);
    for(i = hecdsa256_vli_numbits(p1) - 1; i > 1; --i)
    {
        hecdsa256_vli_modSquare_fast(curve,result, result);
        if(hecdsa256_vli_testbit(p1, i))
            hecdsa256_vli_modMult_fast(curve,result, result, a);
    }
    hecdsa256_vli_set(a, result);
}

static void hecdsa256_ecc_point_decompress(const hecdsa256_curve_t *curve,struct hecdsa256_point_t * point, const uint8_t * compressed)
{

    if(curve==NULL)
    {
        curve=&hecdsa256_curve_secp256r1;
    }

    hecdsa256_ecc_bytes2native(point->x, compressed + 1);
    hecdsa256_vli_modSquare_fast(curve,point->y, point->x);
    hecdsa256_vli_modsub(point->y, point->y, curve->a_inv, curve->p);
    hecdsa256_vli_modMult_fast(curve,point->y, point->y, point->x);
    hecdsa256_vli_modadd(point->y, point->y, curve->b, curve->p);
    hecdsa256_mod_sqrt(curve,point->y);
    if((point->y[0] & 0x01) != (compressed[0] & 0x01))
        hecdsa256_vli_sub(point->y, curve->p, point->y);
}

static void hecdsa256_vli_modmult(uint64_t * result, const uint64_t * left, const uint64_t * right,const uint64_t * mod)
{
    uint64_t product[2 * HECDSA256_NUM_DIGITS];
    uint64_t modmultiple[2 * HECDSA256_NUM_DIGITS];
    unsigned int digitshift, bitshift;
    unsigned int productbits;
    unsigned int modbits = hecdsa256_vli_numbits(mod);

    hecdsa256_vli_mult(product, left, right);
    productbits = hecdsa256_vli_numbits(product + HECDSA256_NUM_DIGITS);
    if(productbits)
        productbits += HECDSA256_NUM_DIGITS * 64;
    else
        productbits = hecdsa256_vli_numbits(product);
    if(productbits < modbits)
    {
        hecdsa256_vli_set(result, product);
        return;
    }
    hecdsa256_vli_clear(modmultiple);
    hecdsa256_vli_clear(modmultiple + HECDSA256_NUM_DIGITS);
    digitshift = (productbits - modbits) / 64;
    bitshift = (productbits - modbits) % 64;
    if(bitshift)
        modmultiple[digitshift + HECDSA256_NUM_DIGITS] = hecdsa256_vli_lshift(modmultiple + digitshift, mod, bitshift);
    else
        hecdsa256_vli_set(modmultiple + digitshift, mod);
    hecdsa256_vli_clear(result);
    result[0] = 1;
    while(productbits > HECDSA256_NUM_DIGITS * 64 || hecdsa256_vli_cmp(modmultiple, mod) >= 0)
    {
        int l_cmp = hecdsa256_vli_cmp(modmultiple + HECDSA256_NUM_DIGITS, product + HECDSA256_NUM_DIGITS);
        if(l_cmp < 0 || (l_cmp == 0 && hecdsa256_vli_cmp(modmultiple, product) <= 0))
        {
            if(hecdsa256_vli_sub(product, product, modmultiple))
                hecdsa256_vli_sub(product + HECDSA256_NUM_DIGITS, product + HECDSA256_NUM_DIGITS, result);
            hecdsa256_vli_sub(product + HECDSA256_NUM_DIGITS, product + HECDSA256_NUM_DIGITS, modmultiple + HECDSA256_NUM_DIGITS);
        }
        uint64_t carry = (modmultiple[HECDSA256_NUM_DIGITS] & 0x01) << 63;
        hecdsa256_vli_rshift1(modmultiple + HECDSA256_NUM_DIGITS);
        hecdsa256_vli_rshift1(modmultiple);
        modmultiple[HECDSA256_NUM_DIGITS - 1] |= carry;
        --productbits;
    }
    hecdsa256_vli_set(result, product);
}

static unsigned int hecdsa256_umax(unsigned int a, unsigned int b)
{
    return (a > b ? a : b);
}

int hecdsa256_ecdh256_keygen(const hecdsa256_curve_t *curve,const hecdsa256_public_key_t public_key, const hecdsa256_private_key_t private_key, hecdsa256_shared_key_t shared_key)
{
    struct hecdsa256_point_t lpublic;
    struct hecdsa256_point_t product;
    uint64_t lprivate[HECDSA256_NUM_DIGITS];
    uint64_t lrandom[HECDSA256_NUM_DIGITS];

    if(!hecdsa256_get_random_number(lrandom))
        return 0;
    hecdsa256_ecc_point_decompress(curve,&lpublic, public_key);
    hecdsa256_ecc_bytes2native(lprivate, private_key);
    hecdsa256_eccpoint_mult(curve,&product, &lpublic, lprivate, lrandom);
    hecdsa256_ecc_native2bytes(shared_key, product.x);
    return !hecdsa256_eccpoint_iszero(&product);
}

int hecdsa256_keygen(const hecdsa256_curve_t *curve,hecdsa256_public_key_t public_key, hecdsa256_private_key_t private_key)
{
    uint64_t lprivate[HECDSA256_NUM_DIGITS];
    struct hecdsa256_point_t lpublic;
    int retry = 0;

    if(curve==NULL)
    {
        curve=&hecdsa256_curve_secp256r1;
    }

    do
    {
        if(!hecdsa256_get_random_number(lprivate) || (retry++ >= HECDSA256_MAX_RETRY))
            return 0;
        if(hecdsa256_vli_iszero(lprivate))
            continue;
        if(hecdsa256_vli_cmp(curve->n, lprivate) != 1)
            hecdsa256_vli_sub(lprivate, lprivate, curve->n);
        struct hecdsa256_point_t curve_g;
        memcpy(&curve_g.x,curve->x,sizeof(curve->x));
        memcpy(&curve_g.y,curve->y,sizeof(curve->y));
        hecdsa256_eccpoint_mult(curve,&lpublic, &curve_g, lprivate, NULL);
    }
    while(hecdsa256_eccpoint_iszero(&lpublic));
    hecdsa256_ecc_native2bytes(private_key, lprivate);
    hecdsa256_ecc_native2bytes(public_key + 1, lpublic.x);
    public_key[0] = 2 + (lpublic.y[0] & 0x01);

    return 1;
}

int hecdsa256_sign(const hecdsa256_curve_t *curve,const hecdsa256_private_key_t private_key, const hecdsa256_hash_t hash256, hecdsa256_signature_t signature)
{
    uint64_t k[HECDSA256_NUM_DIGITS];
    uint64_t tmp[HECDSA256_NUM_DIGITS];
    uint64_t s[HECDSA256_NUM_DIGITS];
    struct hecdsa256_point_t p;
    int retry = 0;
    if(curve==NULL)
    {
        curve=&hecdsa256_curve_secp256r1;
    }
    struct hecdsa256_point_t curve_g;
    memcpy(&curve_g.x,curve->x,sizeof(curve->x));
    memcpy(&curve_g.y,curve->y,sizeof(curve->y));



    do
    {
        if(!hecdsa256_get_random_number(k) || (retry++ >= HECDSA256_MAX_RETRY))
            return 0;
        if(hecdsa256_vli_iszero(k))
            continue;
        if(hecdsa256_vli_cmp(curve->n, k) != 1)
            hecdsa256_vli_sub(k, k, curve->n);
        hecdsa256_eccpoint_mult(curve,&p, &curve_g, k, NULL);
        if(hecdsa256_vli_cmp(curve->n, p.x) != 1)
            hecdsa256_vli_sub(p.x, p.x, curve->n);
    }
    while(hecdsa256_vli_iszero(p.x));
    hecdsa256_ecc_native2bytes(signature, p.x);
    hecdsa256_ecc_bytes2native(tmp, private_key);
    hecdsa256_vli_modmult(s, p.x, tmp, curve->n);
    hecdsa256_ecc_bytes2native(tmp, hash256);
    hecdsa256_vli_modadd(s, tmp, s, curve->n);
    hecdsa256_vli_modinv(k, k, curve->n);
    hecdsa256_vli_modmult(s, s, k, curve->n);
    hecdsa256_ecc_native2bytes(signature + HECDSA256_BYTES, s);

    return 1;
}

int hecdsa256_verify(const hecdsa256_curve_t *curve,const hecdsa256_public_key_t public_key, const hecdsa256_hash_t hash256, const hecdsa256_signature_t signature)
{
    uint64_t u1[HECDSA256_NUM_DIGITS], u2[HECDSA256_NUM_DIGITS];
    uint64_t z[HECDSA256_NUM_DIGITS];
    struct hecdsa256_point_t lpublic, sum;
    uint64_t rx[HECDSA256_NUM_DIGITS];
    uint64_t ry[HECDSA256_NUM_DIGITS];
    uint64_t tx[HECDSA256_NUM_DIGITS];
    uint64_t ty[HECDSA256_NUM_DIGITS];
    uint64_t tz[HECDSA256_NUM_DIGITS];
    uint64_t r[HECDSA256_NUM_DIGITS], s[HECDSA256_NUM_DIGITS];
    int i;
    if(curve==NULL)
    {
        curve=&hecdsa256_curve_secp256r1;
    }
    struct hecdsa256_point_t curve_g;
    memcpy(&curve_g.x,curve->x,sizeof(curve->x));
    memcpy(&curve_g.y,curve->y,sizeof(curve->y));



    hecdsa256_ecc_point_decompress(curve,&lpublic, public_key);
    hecdsa256_ecc_bytes2native(r, signature);
    hecdsa256_ecc_bytes2native(s, signature + HECDSA256_BYTES);
    if(hecdsa256_vli_iszero(r) || hecdsa256_vli_iszero(s))
        return 0;
    if(hecdsa256_vli_cmp(curve->n, r) != 1 || hecdsa256_vli_cmp(curve->n, s) != 1)
        return 0;
    hecdsa256_vli_modinv(z, s, curve->n);
    hecdsa256_ecc_bytes2native(u1, hash256);
    hecdsa256_vli_modmult(u1, u1, z, curve->n);
    hecdsa256_vli_modmult(u2, r, z, curve->n);
    hecdsa256_vli_set(sum.x, lpublic.x);
    hecdsa256_vli_set(sum.y, lpublic.y);
    hecdsa256_vli_set(tx, curve_g.x);
    hecdsa256_vli_set(ty, curve_g.y);
    hecdsa256_vli_modsub(z, sum.x, tx, curve->p);
    hecdsa256_xycz_add(curve,tx, ty, sum.x, sum.y);
    hecdsa256_vli_modinv(z, z, curve->p);
    hecdsa256_apply_z(curve,sum.x, sum.y, z);
    struct hecdsa256_point_t * points[4] = { NULL, &curve_g, &lpublic, &sum };
    unsigned int numbits = hecdsa256_umax(hecdsa256_vli_numbits(u1), hecdsa256_vli_numbits(u2));
    struct hecdsa256_point_t * point = points[(!!hecdsa256_vli_testbit(u1, numbits - 1)) | ((!!hecdsa256_vli_testbit(u2, numbits - 1)) << 1)];
    hecdsa256_vli_set(rx, point->x);
    hecdsa256_vli_set(ry, point->y);
    hecdsa256_vli_clear(z);
    z[0] = 1;
    for(i = numbits - 2; i >= 0; --i)
    {
        hecdsa256_eccpoint_double_jacobian(curve,rx, ry, z);
        int index = (!!hecdsa256_vli_testbit(u1, i)) | ((!!hecdsa256_vli_testbit(u2, i)) << 1);
        struct hecdsa256_point_t * point = points[index];
        if(point)
        {
            hecdsa256_vli_set(tx, point->x);
            hecdsa256_vli_set(ty, point->y);
            hecdsa256_apply_z(curve,tx, ty, z);
            hecdsa256_vli_modsub(tz, rx, tx, curve->p);
            hecdsa256_xycz_add(curve,tx, ty, rx, ry);
            hecdsa256_vli_modMult_fast(curve,z, z, tz);
        }
    }
    hecdsa256_vli_modinv(z, z, curve->p);
    hecdsa256_apply_z(curve,rx, ry, z);
    if(hecdsa256_vli_cmp(curve->n, rx) != 1)
        hecdsa256_vli_sub(rx, rx, curve->n);
    return (hecdsa256_vli_cmp(rx, r) == 0);
}
