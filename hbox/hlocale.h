/***************************************************************
 * Name:      hlocale.h
 * Purpose:   声明hlocale接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-26
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HLOCALE_H_INCLUDE__
#define __HLOCALE_H_INCLUDE__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "hlocale/hlocale.h"
/*
 * 本组件中，采用unicode作为核心组件，其它字符集必须实现向unicode的转换
 */
#include "hlocale/hunicode.h"
#include "hlocale/hgb2312.h"

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HLOCALE_H_INCLUDE__
