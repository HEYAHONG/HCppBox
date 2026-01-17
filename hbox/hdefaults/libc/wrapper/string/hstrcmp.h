/***************************************************************
 * Name:      hstrcmp.h
 * Purpose:   声明hstrcmp接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSTRCMP_H__
#define __HSTRCMP_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "string.h"


/** \brief 字符串比较
 *
 * \param lhs const char* 待比较的字符串
 * \param rhs const char* 待比较的字符串
 * \return int 0=字符串相等
 *
 */
int hstrcmp(const char* lhs,const char* rhs);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HSTRCMP_H__
