/***************************************************************
 * Name:      hed25519_ed25519.c
 * Purpose:   实现hed25519_ed25519接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-08-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hed25519_ed25519.h"

/* Base point is (numbers wrapped):
 *
 *     x = 151122213495354007725011514095885315114
 *         54012693041857206046113283949847762202
 *     y = 463168356949264781694283940034751631413
 *         07993866256225615783033603165251855960
 *
 * y is derived by transforming the original Montgomery base (u=9). x
 * is the corresponding positive coordinate for the new curve equation.
 * t is x*y.
 */
const hed25519_ed25519_pt_t hed25519_ed25519_base =
{
    {
        0x1a, 0xd5, 0x25, 0x8f, 0x60, 0x2d, 0x56, 0xc9,
        0xb2, 0xa7, 0x25, 0x95, 0x60, 0xc7, 0x2c, 0x69,
        0x5c, 0xdc, 0xd6, 0xfd, 0x31, 0xe2, 0xa4, 0xc0,
        0xfe, 0x53, 0x6e, 0xcd, 0xd3, 0x36, 0x69, 0x21
    },
    {
        0x58, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66,
        0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66,
        0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66,
        0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66
    },
    {
        0xa3, 0xdd, 0xb7, 0xa5, 0xb3, 0x8a, 0xde, 0x6d,
        0xf5, 0x52, 0x51, 0x77, 0x80, 0x9f, 0xf0, 0x20,
        0x7d, 0xe3, 0xab, 0x64, 0x8e, 0x4e, 0xea, 0x66,
        0x65, 0x76, 0x8b, 0xd7, 0x0f, 0x5f, 0x87, 0x67
    },
    {1, 0}
};

static const hed25519_ed25519_pt_t hed25519_ed25519_neutral =
{
    {0},
    {1, 0},
    {0},
    {1, 0}
};

/* Conversion to and from projective coordinates */
void hed25519_ed25519_project(hed25519_ed25519_pt_t *p, const hed25519_f25519_t x, const hed25519_f25519_t y)
{
    hed25519_f25519_copy(p->x, x);
    hed25519_f25519_copy(p->y, y);
    hed25519_f25519_load(p->z, 1);
    hed25519_f25519_mul__distinct(p->t, x, y);
}

void hed25519_ed25519_unproject(hed25519_f25519_t x, hed25519_f25519_t y,const hed25519_ed25519_pt_t *p)
{
    uint8_t z1[HED25519_F25519_SIZE];

    hed25519_f25519_inv__distinct(z1, p->z);
    hed25519_f25519_mul__distinct(x, p->x, z1);
    hed25519_f25519_mul__distinct(y, p->y, z1);

    hed25519_f25519_normalize(x);
    hed25519_f25519_normalize(y);
}

/* Compress/uncompress points. We compress points by storing the x
 * coordinate and the parity of the y coordinate.
 *
 * Rearranging the curve equation, we obtain explicit formulae for the
 * coordinates:
 *
 *     x = sqrt((y^2-1) / (1+dy^2))
 *     y = sqrt((x^2+1) / (1-dx^2))
 *
 * Where d = (-121665/121666), or:
 *
 *     d = 370957059346694393431380835087545651895
 *         42113879843219016388785533085940283555
 */

static const hed25519_f25519_t hed25519_ed25519_d =
{
    0xa3, 0x78, 0x59, 0x13, 0xca, 0x4d, 0xeb, 0x75,
    0xab, 0xd8, 0x41, 0x41, 0x4d, 0x0a, 0x70, 0x00,
    0x98, 0xe8, 0x79, 0x77, 0x79, 0x40, 0xc7, 0x8c,
    0x73, 0xfe, 0x6f, 0x2b, 0xee, 0x6c, 0x03, 0x52
};

void hed25519_ed25519_pack(hed25519_f25519_t c, const hed25519_f25519_t x, const hed25519_f25519_t y)
{
    uint8_t tmp[HED25519_F25519_SIZE];
    uint8_t parity;

    hed25519_f25519_copy(tmp, x);
    hed25519_f25519_normalize(tmp);
    parity = (tmp[0] & 1) << 7;

    hed25519_f25519_copy(c, y);
    hed25519_f25519_normalize(c);
    c[31] |= parity;
}

uint8_t hed25519_ed25519_try_unpack(hed25519_f25519_t x, hed25519_f25519_t y, const hed25519_f25519_t comp)
{
    const int parity = comp[31] >> 7;
    uint8_t a[HED25519_F25519_SIZE];
    uint8_t b[HED25519_F25519_SIZE];
    uint8_t c[HED25519_F25519_SIZE];

    /* Unpack y */
    hed25519_f25519_copy(y, comp);
    y[31] &= 127;

    /* Compute c = y^2 */
    hed25519_f25519_mul__distinct(c, y, y);

    /* Compute b = (1+dy^2)^-1 */
    hed25519_f25519_mul__distinct(b, c, hed25519_ed25519_d);
    hed25519_f25519_add(a, b, hed25519_f25519_one);
    hed25519_f25519_inv__distinct(b, a);

    /* Compute a = y^2-1 */
    hed25519_f25519_sub(a, c, hed25519_f25519_one);

    /* Compute c = a*b = (y^2-1)/(1-dy^2) */
    hed25519_f25519_mul__distinct(c, a, b);

    /* Compute a, b = +/-sqrt(c), if c is square */
    hed25519_f25519_sqrt(a, c);
    hed25519_f25519_neg(b, a);

    /* Select one of them, based on the compressed parity bit */
    hed25519_f25519_select(x, a, b, (a[0] ^ parity) & 1);

    /* Verify that x^2 = c */
    hed25519_f25519_mul__distinct(a, x, x);
    hed25519_f25519_normalize(a);
    hed25519_f25519_normalize(c);

    return hed25519_f25519_eq(a, c);
}

/* k = 2d */
static const hed25519_f25519_t hed25519_ed25519_k =
{
    0x59, 0xf1, 0xb2, 0x26, 0x94, 0x9b, 0xd6, 0xeb,
    0x56, 0xb1, 0x83, 0x82, 0x9a, 0x14, 0xe0, 0x00,
    0x30, 0xd1, 0xf3, 0xee, 0xf2, 0x80, 0x8e, 0x19,
    0xe7, 0xfc, 0xdf, 0x56, 0xdc, 0xd9, 0x06, 0x24
};

void hed25519_ed25519_add(hed25519_ed25519_pt_t *r, const hed25519_ed25519_pt_t *p1, const hed25519_ed25519_pt_t *p2)
{
    /* Explicit formulas database: add-2008-hwcd-3
     *
     * source 2008 Hisil--Wong--Carter--Dawson,
     *     http://eprint.iacr.org/2008/522, Section 3.1
     * appliesto extended-1
     * parameter k
     * assume k = 2 d
     * compute A = (Y1-X1)(Y2-X2)
     * compute B = (Y1+X1)(Y2+X2)
     * compute C = T1 k T2
     * compute D = Z1 2 Z2
     * compute E = B - A
     * compute F = D - C
     * compute G = D + C
     * compute H = B + A
     * compute X3 = E F
     * compute Y3 = G H
     * compute T3 = E H
     * compute Z3 = F G
     */
    uint8_t a[HED25519_F25519_SIZE];
    uint8_t b[HED25519_F25519_SIZE];
    uint8_t c[HED25519_F25519_SIZE];
    uint8_t d[HED25519_F25519_SIZE];
    uint8_t e[HED25519_F25519_SIZE];
    uint8_t f[HED25519_F25519_SIZE];
    uint8_t g[HED25519_F25519_SIZE];
    uint8_t h[HED25519_F25519_SIZE];

    /* A = (Y1-X1)(Y2-X2) */
    hed25519_f25519_sub(c, p1->y, p1->x);
    hed25519_f25519_sub(d, p2->y, p2->x);
    hed25519_f25519_mul__distinct(a, c, d);

    /* B = (Y1+X1)(Y2+X2) */
    hed25519_f25519_add(c, p1->y, p1->x);
    hed25519_f25519_add(d, p2->y, p2->x);
    hed25519_f25519_mul__distinct(b, c, d);

    /* C = T1 k T2 */
    hed25519_f25519_mul__distinct(d, p1->t, p2->t);
    hed25519_f25519_mul__distinct(c, d, hed25519_ed25519_k);

    /* D = Z1 2 Z2 */
    hed25519_f25519_mul__distinct(d, p1->z, p2->z);
    hed25519_f25519_add(d, d, d);

    /* E = B - A */
    hed25519_f25519_sub(e, b, a);

    /* F = D - C */
    hed25519_f25519_sub(f, d, c);

    /* G = D + C */
    hed25519_f25519_add(g, d, c);

    /* H = B + A */
    hed25519_f25519_add(h, b, a);

    /* X3 = E F */
    hed25519_f25519_mul__distinct(r->x, e, f);

    /* Y3 = G H */
    hed25519_f25519_mul__distinct(r->y, g, h);

    /* T3 = E H */
    hed25519_f25519_mul__distinct(r->t, e, h);

    /* Z3 = F G */
    hed25519_f25519_mul__distinct(r->z, f, g);
}

static void hed25519_ed25519_double(hed25519_ed25519_pt_t *r, const hed25519_ed25519_pt_t *p)
{
    /* Explicit formulas database: dbl-2008-hwcd
     *
     * source 2008 Hisil--Wong--Carter--Dawson,
     *     http://eprint.iacr.org/2008/522, Section 3.3
     * compute A = X1^2
     * compute B = Y1^2
     * compute C = 2 Z1^2
     * compute D = a A
     * compute E = (X1+Y1)^2-A-B
     * compute G = D + B
     * compute F = G - C
     * compute H = D - B
     * compute X3 = E F
     * compute Y3 = G H
     * compute T3 = E H
     * compute Z3 = F G
     */
    uint8_t a[HED25519_F25519_SIZE];
    uint8_t b[HED25519_F25519_SIZE];
    uint8_t c[HED25519_F25519_SIZE];
    uint8_t e[HED25519_F25519_SIZE];
    uint8_t f[HED25519_F25519_SIZE];
    uint8_t g[HED25519_F25519_SIZE];
    uint8_t h[HED25519_F25519_SIZE];

    /* A = X1^2 */
    hed25519_f25519_mul__distinct(a, p->x, p->x);

    /* B = Y1^2 */
    hed25519_f25519_mul__distinct(b, p->y, p->y);

    /* C = 2 Z1^2 */
    hed25519_f25519_mul__distinct(c, p->z, p->z);
    hed25519_f25519_add(c, c, c);

    /* D = a A (alter sign) */
    /* E = (X1+Y1)^2-A-B */
    hed25519_f25519_add(f, p->x, p->y);
    hed25519_f25519_mul__distinct(e, f, f);
    hed25519_f25519_sub(e, e, a);
    hed25519_f25519_sub(e, e, b);

    /* G = D + B */
    hed25519_f25519_sub(g, b, a);

    /* F = G - C */
    hed25519_f25519_sub(f, g, c);

    /* H = D - B */
    hed25519_f25519_neg(h, b);
    hed25519_f25519_sub(h, h, a);

    /* X3 = E F */
    hed25519_f25519_mul__distinct(r->x, e, f);

    /* Y3 = G H */
    hed25519_f25519_mul__distinct(r->y, g, h);

    /* T3 = E H */
    hed25519_f25519_mul__distinct(r->t, e, h);

    /* Z3 = F G */
    hed25519_f25519_mul__distinct(r->z, f, g);
}

void hed25519_ed25519_smult(hed25519_ed25519_pt_t *r_out, const hed25519_ed25519_pt_t *p,const hed25519_f25519_t e)
{
    hed25519_ed25519_pt_t r;
    int i;

    hed25519_ed25519_copy(&r, &hed25519_ed25519_neutral);

    for (i = 255; i >= 0; i--)
    {
        const uint8_t bit = (e[i >> 3] >> (i & 7)) & 1;
        hed25519_ed25519_pt_t s;

        hed25519_ed25519_double(&r, &r);
        hed25519_ed25519_add(&s, &r, p);

        hed25519_f25519_select(r.x, r.x, s.x, bit);
        hed25519_f25519_select(r.y, r.y, s.y, bit);
        hed25519_f25519_select(r.z, r.z, s.z, bit);
        hed25519_f25519_select(r.t, r.t, s.t, bit);
    }

    hed25519_ed25519_copy(r_out, &r);
}



