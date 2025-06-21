/***************************************************************
 * Name:      hvfprintf.h
 * Purpose:   声明hvfprintf接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HVFPRINTF_H__
#define __HVFPRINTF_H__
#include "stdio.h"
#include "stdarg.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 向文件写入字符串。
 *
 * \param stream FILE* 文件指针
 * \param format const char* 格式字符串
 * \param vlist va_list 格式字符串中的参数
 * \return int 失败返回EOF
 *
 */
int hvfprintf(FILE* stream,const char* format,va_list vlist);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HVFPRINTF_H__
