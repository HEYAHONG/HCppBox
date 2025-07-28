/***************************************************************
 * Name:      hfprintf.h
 * Purpose:   声明hfprintf接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-07-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HFPRINTF_H__
#define __HFPRINTF_H__
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
 * \param ... 格式字符串中的参数
 * \return int 失败返回EOF
 *
 */
int hfprintf(FILE*  stream,const char*  format, ... );

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HFPRINTF_H__
