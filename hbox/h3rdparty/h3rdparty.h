/***************************************************************
 * Name:      h3rdparty.h
 * Purpose:   引入第三方头文件
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-09-27
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __H3RDPARTY_H__
#define __H3RDPARTY_H__

#include "stdarg.h"
#include "hdefaults.h"

/** \brief 初始化第三方库
 *  注意：默认由hruntime组件调用
 *
 */
void h3rdparty_init(void);

/** \brief 循环第三方库
 *  注意：默认由hruntime组件调用
 *
 */
void h3rdparty_loop(void);


/*
 *  此模块引入第三方库
 */

#include "h3rdparty/port/cJSON/h3rdparty_port_cJSON.h"
#include "h3rdparty/port/printf/h3rdparty_port_printf.h"
#include "h3rdparty/port/nanopb/h3rdparty_port_nanopb.h"
#include "h3rdparty/port/paho.mqtt.embedded-c/h3rdparty_port_paho.mqtt.embedded-c.h"
#include "h3rdparty/port/softfp/h3rdparty_port_softfp.h"
#include "h3rdparty/port/zlib/h3rdparty_port_zlib.h"
#include "h3rdparty/port/lz4/h3rdparty_port_lz4.h"
#include "h3rdparty/port/qrencode/h3rdparty_port_qrencode.h"
#include "h3rdparty/port/argtable3/h3rdparty_port_argtable3.h"
#include "h3rdparty/port/tlsf/h3rdparty_port_tlsf.h"
#include "h3rdparty/port/libfdt/h3rdparty_port_libfdt.h"
#include "h3rdparty/port/FreeRTOS/h3rdparty_port_FreeRTOS.h"
#include "h3rdparty/port/mini-rv32ima/h3rdparty_port_mini_rv32ima.h"
#include "h3rdparty/port/uriparser/h3rdparty_port_uriparser.h"


#endif // __H3RDPARTY_H__


