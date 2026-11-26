/***************************************************************
 * Name:      moxiebox_libos.h
 * Purpose:   声明moxiebox libos内部共享转换函数
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-26
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __MOXIEBOX_LIBOS_H__
#define __MOXIEBOX_LIBOS_H__

#include <time.h>

int moxiebox_open_flags(int flags);
int moxiebox_open_flags_from_guest(int flags);
int moxiebox_mmap_prot(int prot);
clockid_t moxiebox_clockid(clockid_t id);

#endif /* __MOXIEBOX_LIBOS_H__ */
