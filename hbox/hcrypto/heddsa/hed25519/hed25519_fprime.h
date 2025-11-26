/***************************************************************
 * Name:      hed25519_fprime.h
 * Purpose:   声明hed25519_fprime接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-08-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HED25519_FPRIME_H__
#define __HED25519_FPRIME_H__
#include "stdint.h"
#include "string.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define HED25519_FPRIME_SIZE        (32)

typedef uint8_t hed25519_fprime_t[HED25519_FPRIME_SIZE];

void hed25519_fprime_from_bytes(hed25519_fprime_t x,const uint8_t *in, size_t len,const hed25519_fprime_t modulus);

static inline void hed25519_fprime_copy(hed25519_fprime_t x, const hed25519_fprime_t a)
{
    memcpy(x, a, HED25519_FPRIME_SIZE);
}

uint8_t hed25519_fprime_eq(const hed25519_fprime_t x, const hed25519_fprime_t y);

void hed25519_fprime_select(hed25519_fprime_t dst,const hed25519_fprime_t zero, const hed25519_fprime_t one,uint8_t condition);

void hed25519_fprime_add(hed25519_fprime_t r, const hed25519_fprime_t  a, const hed25519_fprime_t modulus);

void hed25519_fprime_mul(hed25519_fprime_t r, const hed25519_fprime_t a, const hed25519_fprime_t b,const hed25519_fprime_t modulus);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HED25519_FPRIME_H__
