/***************************************************************
 * Name:      hstrncmp.h
 * Purpose:   声明hstrncmp接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSTRNCMP_H__
#define __HSTRNCMP_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "string.h"


/** \brief 字符串比较
 *
 * \param lhs const char* 待比较的字符串
 * \param rhs const char* 待比较的字符串
 * \param max_count size_t 最大长度
 * \return int 0=字符串相等
 *
 */
int hstrncmp(const char* lhs,const char* rhs,size_t max_count);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HSTRNCMP_H__
