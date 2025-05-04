/***************************************************************
 * Name:      hbignum.h
 * Purpose:   声明hbignum接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-23
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HBIGNUM_H__
#define __HBIGNUM_H__

/*
 * 大数计算,计算时应选择 最大参数(不包括中间值)位数*2+32 的位数的大数,这样可以保证加、减、乘、除及幂取模运算不会溢出。
 */


#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "hbignum/huint.h"

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HBIGNUM_H__
