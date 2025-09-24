/***************************************************************
 * Name:      h3rdparty_port_qrencode.h
 * Purpose:   声明h3rdparty接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-09-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __H3RDPARTY_PORT_QRENCODE_H_INCLUDE__
#define __H3RDPARTY_PORT_QRENCODE_H_INCLUDE__

/*
 * 引入qrencode库,用户需要手动引入相应头文件
 */
#ifdef  H3RDPARTY_USING_SYSTEM_LIBQRENCODE
#ifndef H3RDPARTY_LIBQRENCODE_HEADER
#define H3RDPARTY_LIBQRENCODE_HEADER "qrencode.h"
#endif
#else
#ifndef H3RDPARTY_LIBQRENCODE_HEADER
#define H3RDPARTY_LIBQRENCODE_HEADER "h3rdparty/3rdparty/libqrencode/qrencode.h"
#endif
#endif


#endif // __H3RDPARTY_PORT_QRENCODE_H_INCLUDE__
