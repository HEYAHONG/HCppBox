/***************************************************************
 * Name:      hsyscall_random.h
 * Purpose:   声明hsyscall_random接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-09
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HHSYSCALL_RANDOM_H__
#define __HHSYSCALL_RANDOM_H__
#include "../../wrapper/hgetrandom.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

hgetrandom_ssize_t hsyscall_getrandom (void *buffer, size_t length,unsigned int flags);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HHSYSCALL_RANDOM_H__
