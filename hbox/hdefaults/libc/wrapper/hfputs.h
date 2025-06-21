/***************************************************************
 * Name:      hfputs.h
 * Purpose:   声明hfputs接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HFPUTS_H__
#define __HFPUTS_H__
#include "stdio.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 向文件写入字符串。
 *
 * \param str const char* 字符串
 * \param stream FILE* 文件指针
 * \return int 失败返回EOF
 *
 */
int hfputs( const char* str, FILE*          stream );

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HFPUTS_H__
