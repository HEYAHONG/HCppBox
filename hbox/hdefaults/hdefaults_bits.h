/***************************************************************
 * Name:      hdefaults_bits.h
 * Purpose:   定义一些默认宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDEFAULTS_BITS_H__
#define __HDEFAULTS_BITS_H__

#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stdbool.h"
#include "stdarg.h"

//定义位宽(仅用于区分32位或者64位环境，其它位数不予保证)
#if (SIZE_MAX) <= (UINT16_MAX)
#define HDEFAULTS_BITS_16_OR_8      1
#elif (SIZE_MAX) <= (UINT32_MAX)
#define HDEFAULTS_BITS_32           1
#elif (SIZE_MAX) <= (UINT64_MAX)
#define HDEFAULTS_BITS_64           1
#else
#define HDEFAULTS_BITS_ABOVE_64     1
#endif

#endif
