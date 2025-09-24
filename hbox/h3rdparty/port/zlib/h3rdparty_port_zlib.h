/***************************************************************
 * Name:      h3rdparty_port_zlib.h
 * Purpose:   声明h3rdparty接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-09-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __H3RDPARTY_PORT_ZLIB_H_INCLUDE__
#define __H3RDPARTY_PORT_ZLIB_H_INCLUDE__

/*
 * 引入zlib库,用户需要手动引入相应头文件
 */
#ifdef  H3RDPARTY_USING_SYSTEM_ZLIB
#ifndef H3RDPARTY_ZLIB_HEADER
#define H3RDPARTY_ZLIB_HEADER "zlib.h"
#endif
#else
#if defined(HDEFAULTS_LIBC_NEWLIB) || defined(HDEFAULTS_LIBC_PICOLIBC) || defined(HDEFAULTS_OS_UNIX) ||  defined(HDEFAULTS_OS_RTTHREAD) || defined(HDEFAULTS_OS_NUTTX)
#include "unistd.h"
#else

#endif
/*
 * 启用zlib前缀配置，防止与其它库冲突
 */
#define Z_PREFIX 1
#ifndef H3RDPARTY_ZLIB_HEADER
#define H3RDPARTY_ZLIB_HEADER "h3rdparty/3rdparty/zlib/zlib.h"
#endif
#endif // H3RDPARTY_USING_SYSTEM_ZLIB




#endif // __H3RDPARTY_PORT_ZLIB_H_INCLUDE__
