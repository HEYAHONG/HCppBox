/***************************************************************
 * Name:      h3rdparty_port_tlsf.h
 * Purpose:   声明h3rdparty接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-09-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __H3RDPARTY_PORT_TLSF_H_INCLUDE__
#define __H3RDPARTY_PORT_TLSF_H_INCLUDE__

/*
 * 引入tlsf库,用户需要手动引入相应头文件
 */
#ifdef  H3RDPARTY_USING_SYSTEM_TLSF
#ifndef H3RDPARTY_TLSF_HEADER
#define H3RDPARTY_TLSF_HEADER "tlsf.h"
#endif
#else
#ifndef H3RDPARTY_TLSF_HEADER
#define H3RDPARTY_TLSF_HEADER "h3rdparty/3rdparty/tlsf/tlsf.h"
#endif
#endif

#include "htlsf.h"


#endif // __H3RDPARTY_PORT_TLSF_H_INCLUDE__
