/***************************************************************
 * Name:      hclose.h
 * Purpose:   声明hclose接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCLOSE_H__
#define __HCLOSE_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief
 *
 * \param fd int 文件描述符
 * \return int 返回值,0表示成功，-1表示出错
 *
 */
int hclose(int fd);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HCLOSE_H__
