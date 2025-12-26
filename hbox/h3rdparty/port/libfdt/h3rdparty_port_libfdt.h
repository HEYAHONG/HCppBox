/***************************************************************
 * Name:      h3rdparty_port_libfdt.h
 * Purpose:   声明h3rdparty接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-09-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __H3RDPARTY_PORT_LIBFDT_H_INCLUDE__
#define __H3RDPARTY_PORT_LIBFDT_H_INCLUDE__

/*
 * 引入libfdt库,用户需要手动引入相应头文件
 */
#ifdef  H3RDPARTY_USING_SYSTEM_LIBFDT
#ifndef H3RDPARTY_LIBFDT_HEADER
#define H3RDPARTY_LIBFDT_HEADER "libfdt.h"
#endif
#else
#ifndef H3RDPARTY_LIBFDT_HEADER
#define H3RDPARTY_LIBFDT_HEADER "h3rdparty/3rdparty/libfdt/libfdt.h"
#endif
#endif

#include "hlibfdt.h"


#endif // __H3RDPARTY_PORT_LIBFDT_H_INCLUDE__
