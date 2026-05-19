/***************************************************************
 * Name:      h3rdparty_port_Z80.h
 * Purpose:   声明h3rdparty接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-05-18
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __H3RDPARTY_PORT_Z80_H_INCLUDE__
#define __H3RDPARTY_PORT_Z80_H_INCLUDE__

/*
 * 引入Z80库
 */
#ifdef  H3RDPARTY_USING_SYSTEM_Z80
#ifndef H3RDPARTY_Z80_HEADER
#define H3RDPARTY_Z80_HEADER   "Z80.h"
#endif
#else
#ifndef Z80_EXTERNAL_HEADER
#define Z80_EXTERNAL_HEADER    "h3rdparty/3rdparty/Z80/Z80_config.h"
#endif
#ifndef Z80_WITH_LOCAL_HEADER
#define Z80_WITH_LOCAL_HEADER  1
#endif
#ifndef Z80_STATIC
#define Z80_STATIC             1
#endif
#ifndef H3RDPARTY_Z80_HEADER
#define H3RDPARTY_Z80_HEADER   "h3rdparty/3rdparty/Z80/Z80.h"
#endif
#endif

#include "hz80cpu.h"


#endif // __H3RDPARTY_PORT_Z80_H_INCLUDE__
