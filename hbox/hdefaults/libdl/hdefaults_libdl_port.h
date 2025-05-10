/***************************************************************
 * Name:      hdefaults_libdl_port.h
 * Purpose:   声明hdefaults_libdl_port接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-10
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDEFAULTS_LIBDL_PORT_H__
#define __HDEFAULTS_LIBDL_PORT_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#ifdef HAVE_DLFCN_H
#include "dlfcn.h"
#define HRTLD_NOW           RTLD_NOW

#define HRTLD_LAZY          RTLD_LAZY

#define HRTLD_GLOBAL        RTLD_GLOBAL

#define HRTLD_LOCAL         RTLD_LOCAL

#define HRTLD_DEFAULT       RTLD_DEFAULT

#define HRTLD_NEXT          RTLD_NEXT
#else
#define HRTLD_NOW           0

#define HRTLD_LAZY          HRTLD_NOW

#define HRTLD_GLOBAL        (1 << 1)

#define HRTLD_LOCAL         (1 << 2)

#define HRTLD_DEFAULT       ((void *)0)

#define HRTLD_NEXT          ((void *)-1)

#endif // HAVE_DLFCN_H

/** \brief 打开动态库
 *
 * \param file const char*  文件名称(为NULL打开自身)
 * \param mode int 模式
 * \return void* 成功返回动态库句柄
 *
 */
void *hdlopen(const char *file, int mode);


/** \brief 关闭动态库
 *
 * \param handle void* 动态库句柄
 * \return int 0=成功
 *
 */
int hdlclose(void *handle);


/** \brief 获取动态库符号
 *
 * \param handle void* 动态库句柄（本组件中为NULL表示自身(无需打开可直接获取符号)）
 * \param name const char* 名称
 * \return void* 符号地址
 *
 */
void *hdlsym(void *handle, const char *name);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HDEFAULTS_LIBDL_PORT_H__
