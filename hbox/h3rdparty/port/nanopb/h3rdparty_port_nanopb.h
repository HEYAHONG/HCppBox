/***************************************************************
 * Name:      h3rdparty_port_nanopb.h
 * Purpose:   声明h3rdparty接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-09-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __H3RDPARTY_PORT_NANOPB_H_INCLUDE__
#define __H3RDPARTY_PORT_NANOPB_H_INCLUDE__

/*
 *  引入nanopb库
 */
#ifndef H3RDPARTY_NANOPB_PB_HEADER
#ifdef  H3RDPARTY_USING_SYSTEM_NANOPB
#define H3RDPARTY_NANOPB_PB_HEADER "pb.h"
#else
#define H3RDPARTY_NANOPB_PB_HEADER "h3rdparty/3rdparty/nanopb/pb.h"
#endif
#endif
#ifndef H3RDPARTY_NANOPB_PB_ENCODE_HEADER
#ifdef  H3RDPARTY_USING_SYSTEM_NANOPB
#define H3RDPARTY_NANOPB_PB_ENCODE_HEADER "pb_encode.h"
#else
#define H3RDPARTY_NANOPB_PB_ENCODE_HEADER "h3rdparty/3rdparty/nanopb/pb_encode.h"
#endif
#endif
#ifndef H3RDPARTY_NANOPB_PB_DECODE_HEADER
#ifdef  H3RDPARTY_USING_SYSTEM_NANOPB
#define H3RDPARTY_NANOPB_PB_DECODE_HEADER "pb_decode.h"
#else
#define H3RDPARTY_NANOPB_PB_DECODE_HEADER "h3rdparty/3rdparty/nanopb/pb_decode.h"
#endif
#endif


/*
 * 已经移植的消息
 */
#include "hpb_msg_main_entry.h"


#endif // __H3RDPARTY_PORT_NANOPB_H_INCLUDE__
