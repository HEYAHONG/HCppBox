/***************************************************************
 * Name:      hfeof.h
 * Purpose:   声明hfeof接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-08-26
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HFEOF_H__
#define __HFEOF_H__
#include "stdio.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 检查文件EOF错误
 *
 * \param stream FILE* 文件指针
 * \return int 0=未出错，非0=出错
 *
 */
int hfeof( FILE *stream );

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HFEOF_H__
