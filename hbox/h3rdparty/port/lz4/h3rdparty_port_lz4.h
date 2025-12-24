/***************************************************************
 * Name:      h3rdparty_port_lz4.h
 * Purpose:   声明h3rdparty接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-09-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __H3RDPARTY_PORT_LZ4_H_INCLUDE__
#define __H3RDPARTY_PORT_LZ4_H_INCLUDE__

/*
 * 引入lz4库,用户需要手动引入相应头文件
 */
#ifdef  H3RDPARTY_USING_SYSTEM_LZ4
#ifndef H3RDPARTY_LZ4_HEADER
#define H3RDPARTY_LZ4_HEADER "lz4.h"
#endif
#ifndef H3RDPARTY_LZ4HC_HEADER
#define H3RDPARTY_LZ4HC_HEADER "lz4hc.h"
#endif
#ifndef H3RDPARTY_LZ4FRAME_HEADER
#define H3RDPARTY_LZ4FRAME_HEADER "lz4frame.h"
#endif
#ifndef H3RDPARTY_LZ4FILE_HEADER
#define H3RDPARTY_LZ4FILE_HEADER "lz4file.h"
#endif
#else
#ifndef H3RDPARTY_LZ4_HEADER
#define H3RDPARTY_LZ4_HEADER "h3rdparty/3rdparty/lz4/lz4.h"
#endif
#ifndef H3RDPARTY_LZ4HC_HEADER
#define H3RDPARTY_LZ4HC_HEADER "h3rdparty/3rdparty/lz4/lz4hc.h"
#endif
#ifndef H3RDPARTY_LZ4FRAME_HEADER
#define H3RDPARTY_LZ4FRAME_HEADER "h3rdparty/3rdparty/lz4/lz4frame.h"
#endif
#ifndef H3RDPARTY_LZ4FILE_HEADER
#define H3RDPARTY_LZ4FILE_HEADER "h3rdparty/3rdparty/lz4/lz4file.h"
#endif
#endif


#include "hlz4.h"

#endif // __H3RDPARTY_PORT_LZ4_H_INCLUDE__
