/***************************************************************
 * Name:      hfclose.h
 * Purpose:   声明hfclose接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-02-13
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HFCLOSE_H__
#define __HFCLOSE_H__
#include "stdio.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 关闭文件
 *
 * \param stream FILE* 待关闭的文件
 * \return int 成功返回0,失败返回EOF(-1)
 *
 */
int hfclose(FILE * stream);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HFCLOSE_H__
