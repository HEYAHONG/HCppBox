/***************************************************************
 * Name:      hdefaults_endian.h
 * Purpose:   定义一些默认宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-06-05
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDEFAULTS_ENDIAN_H__
#define __HDEFAULTS_ENDIAN_H__

#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stdbool.h"
#include "stdarg.h"

#define HDEFAULTS_ENDIAN_ORDER_LITTLE  1234
#define HDEFAULTS_ENDIAN_ORDER_BIG     4321

/*
 * GNU C扩展
 */
#if defined(__BYTE_ORDER__)

#if        (__BYTE_ORDER__)==(__ORDER_LITTLE_ENDIAN__)

#if !defined(HDEFAULTS_ENDIAN_ORDER)
#define HDEFAULTS_ENDIAN_ORDER HDEFAULTS_ENDIAN_ORDER_LITTLE
#endif

#elif      (__BYTE_ORDER__)==(__ORDER_BIG_ENDIAN__)

#if !defined(HDEFAULTS_ENDIAN_ORDER)
#define HDEFAULTS_ENDIAN_ORDER HDEFAULTS_ENDIAN_ORDER_BIG
#endif

#endif

#endif

/*
 * 小端宏定义
 */
#if (defined(__LITTLE_ENDIAN__) || defined(_LITTLE_ENDIAN))

#if !defined(HDEFAULTS_ENDIAN_ORDER)
#define HDEFAULTS_ENDIAN_ORDER HDEFAULTS_ENDIAN_ORDER_LITTLE
#endif

#endif

/*
 * 大端宏定义
 */
#if (defined(__BIG_ENDIAN__) || defined(_BIG_ENDIAN))

#if !defined(HDEFAULTS_ENDIAN_ORDER)
#define HDEFAULTS_ENDIAN_ORDER HDEFAULTS_ENDIAN_ORDER_BIG
#endif

#endif

/*
 * x86与x86_64默认为小端
 */
#if (defined(HDEFAULTS_ARCH_X86) ||defined(HDEFAULTS_ARCH_X86_64))

#if !defined(HDEFAULTS_ENDIAN_ORDER)
#define HDEFAULTS_ENDIAN_ORDER HDEFAULTS_ENDIAN_ORDER_LITTLE
#endif

#endif

/*
 * armcc判断大小端
 */
#if defined(__BIG_ENDIAN)

#if !defined(HDEFAULTS_ENDIAN_ORDER)
#define HDEFAULTS_ENDIAN_ORDER HDEFAULTS_ENDIAN_ORDER_BIG
#endif

#endif
#if defined(__CC_ARM)

#if !defined(HDEFAULTS_ENDIAN_ORDER)
#define HDEFAULTS_ENDIAN_ORDER HDEFAULTS_ENDIAN_ORDER_LITTLE
#endif

#endif

#if !defined(HDEFAULTS_ENDIAN_ORDER)
/*
 * 默认为小端序
 */
#warning "endian is not detected!endian defaults to little-endian!"
#define HDEFAULTS_ENDIAN_ORDER HDEFAULTS_ENDIAN_ORDER_LITTLE
#endif

#endif
