/***************************************************************
 * Name:      hfseek.h
 * Purpose:   声明hfseek接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-02-13
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HFSEEK_H__
#define __HFSEEK_H__
#include "stdio.h"
#include "stdint.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 设定文件偏移
 *
 * \param stream FILE* 文件
 * \param offset int64_t 偏移
 * \param whence int 从何处开始
 * \return int64_t 据文件头的字节数，失败返回-1
 *
 */
int64_t hfseek(FILE * stream,int64_t offset,int whence);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HFSEEK_H__
