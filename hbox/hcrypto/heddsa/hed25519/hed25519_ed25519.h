/***************************************************************
 * Name:      hed25519_ed25519.h
 * Purpose:   声明hed25519_ed25519接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-08-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HED25519_ED25519_H__
#define __HED25519_ED25519_H__

#include "hed25519_f25519.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct hed25519_ed25519_pt hed25519_ed25519_pt_t;
struct hed25519_ed25519_pt
{
    hed25519_f25519_t		x;
    hed25519_f25519_t		y;
    hed25519_f25519_t		t;
    hed25519_f25519_t		z;
};

extern const hed25519_ed25519_pt_t hed25519_ed25519_base;

void hed25519_ed25519_project(hed25519_ed25519_pt_t *p,const hed25519_f25519_t x, const hed25519_f25519_t y);
void hed25519_ed25519_unproject(hed25519_f25519_t x, hed25519_f25519_t y,const hed25519_ed25519_pt_t *p);

#define HED25519_ED25519_PACK_SIZE	HED25519_F25519_SIZE

void hed25519_ed25519_pack(hed25519_f25519_t c, const hed25519_f25519_t x, const hed25519_f25519_t y);
uint8_t hed25519_ed25519_try_unpack(hed25519_f25519_t x, hed25519_f25519_t y, const hed25519_f25519_t c);

#define HED25519_ED25519_EXPONENT_SIZE	HED25519_F25519_SIZE

static inline void hed25519_ed25519_prepare(hed25519_f25519_t e)
{
    e[0] &= 0xf8;
    e[31] &= 0x7f;
    e[31] |= 0x40;
}

static inline void hed25519_ed25519_copy(hed25519_ed25519_pt_t *dst,	const hed25519_ed25519_pt_t *src)
{
    memcpy(dst, src, sizeof(*dst));
}

void hed25519_ed25519_add(hed25519_ed25519_pt_t *r, const hed25519_ed25519_pt_t *a, const hed25519_ed25519_pt_t *b);
void hed25519_ed25519_smult(hed25519_ed25519_pt_t *r, const hed25519_ed25519_pt_t *a,const hed25519_f25519_t e);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HED25519_ED25519_H__
