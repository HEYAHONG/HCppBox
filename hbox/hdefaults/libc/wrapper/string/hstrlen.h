/***************************************************************
 * Name:      hstrlen.h
 * Purpose:   声明hstrlen接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-18
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSTRLEN_H__
#define __HSTRLEN_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "string.h"


/** \brief 获取字符串长度
 *
 * \param str const char* 字符串
 * \return size_t 字符串长度
 *
 */
size_t hstrlen(const char* str);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HSTRLEN_H__
