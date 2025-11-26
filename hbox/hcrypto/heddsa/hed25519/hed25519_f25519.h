/***************************************************************
 * Name:      hed25519_f25519.h
 * Purpose:   声明hed25519_f25519接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-08-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HED25519_F25519_H__
#define __HED25519_F25519_H__
#include "stdint.h"
#include "stdlib.h"
#include "string.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define HED25519_F25519_SIZE        (32)

typedef uint8_t hed25519_f25519_t[HED25519_F25519_SIZE];

extern const hed25519_f25519_t hed25519_f25519_one;

void hed25519_f25519_load(hed25519_f25519_t x, uint32_t c);

static inline void hed25519_f25519_copy(hed25519_f25519_t x, const hed25519_f25519_t a)
{
    memcpy(x, a, HED25519_F25519_SIZE);
}

void hed25519_f25519_normalize(hed25519_f25519_t x);

uint8_t hed25519_f25519_eq(const hed25519_f25519_t x, const hed25519_f25519_t y);

void hed25519_f25519_select(hed25519_f25519_t dst,const hed25519_f25519_t zero, const hed25519_f25519_t one, uint8_t condition);

void hed25519_f25519_add(hed25519_f25519_t r, const hed25519_f25519_t a, const hed25519_f25519_t b);
void hed25519_f25519_sub(hed25519_f25519_t r, const hed25519_f25519_t a, const hed25519_f25519_t b);

void hed25519_f25519_neg(hed25519_f25519_t r, const hed25519_f25519_t a);

void hed25519_f25519_mul__distinct(hed25519_f25519_t r, const hed25519_f25519_t a, const hed25519_f25519_t b);

void hed25519_f25519_inv__distinct(hed25519_f25519_t r, const hed25519_f25519_t x);

void hed25519_f25519_sqrt(hed25519_f25519_t r, const hed25519_f25519_t x);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HED25519_F25519_H__
