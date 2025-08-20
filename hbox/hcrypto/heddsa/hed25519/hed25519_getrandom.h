/***************************************************************
 * Name:      hed25519_getrandom.h
 * Purpose:   声明hed25519_getrandom接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-08-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HED25519_GETRANDOM_H__
#define __HED25519_GETRANDOM_H__
#include "stdlib.h"
#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#ifndef HED25519_GETRANDOM
#include "hdefaults.h"
#define HED25519_GETRANDOM(buffer,buffer_length,flags) hgetrandom(buffer,buffer_length,flags)
#endif // HED25519_GETRANDOM


#ifdef  RAND_MAX
#define HED25519_RAND_MAX RAND_MAX
#else
#define HED25519_RAND_MAX (65536)
#endif // RAND_MAX

int hed25519_rand(void);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HED25519_GETRANDOM_H__
