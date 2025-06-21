/***************************************************************
 * Name:      hputs.h
 * Purpose:   声明hputs接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HPUTS_H__
#define __HPUTS_H__
#include "stdio.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 输出字符串
 *
 * \param str const char* 字符串
 * \return int 失败返回EOF
 *
 */
int hputs( const char* str);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HPUTS_H__
