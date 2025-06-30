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

/*
 *  此模块引入第三方库
 */

/*
 * 引入cJSON库
 */
#ifndef H3RDPARTY_USING_SYSTEM_CJSON
#include "h3rdparty/3rdparty/cJSON/cJSON.h"
#include "h3rdparty/3rdparty/cJSON/cJSON_Utils.h"
#else
#include "cJSON.h"
#include "cJSON_Utils.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 初始化第三方库
 *
 *
 */
void h3rdparty_init(void);

/*
 * 内嵌printf,为保证打印代码输出的一致性，可使用此系列函数进行打印操作
 */

/** \brief 设置hprintf输出的回调函数
 *
 * \param cb void (*)(char c) 回调函数
 *
 */
void hprintf_set_callback(void (*cb)(char c));

/** \brief 类似printf
 *
 *
 */
int hprintf(const char* format, ...);
/** \brief 类似sprintf
 *
 *
 */
int hsprintf(char* buffer, const char* format, ...);
/** \brief 类似snprintf
 *
 *
 */
int hsnprintf(char* buffer, size_t count, const char* format, ...);
/** \brief 类似vsnprintf
 *
 *
 */
int hvsnprintf(char* buffer, size_t count, const char* format, va_list va);
/** \brief 类似vprintf
 *
 *
 */
int hvprintf(const char* format, va_list va);
/** \brief 用于使用自定义输出函数(out)输出打印
 *
 *
 */
int hfctprintf(void (*out)(char character, void* arg), void* arg, const char* format, ...);

/** \brief 用于使用自定义输出函数(out)输出打印
 *
 *
 */
int hvfctprintf(void (*out)(char character, void* arg), void* arg, const char* format, va_list va);

#ifdef __cplusplus
}
#endif // __cplusplus

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
#include H3RDPARTY_NANOPB_PB_HEADER
#include H3RDPARTY_NANOPB_PB_ENCODE_HEADER
#include H3RDPARTY_NANOPB_PB_DECODE_HEADER




/*
 * 引入paho.mqtt.embedded-c库,用户需要手动引入相应头文件
 */
#ifndef H3RDPARTY_PAHO_MQTT_EMBEDDED_C_PACKET_HEADER
#define H3RDPARTY_PAHO_MQTT_EMBEDDED_C_PACKET_HEADER "h3rdparty/3rdparty/paho.mqtt.embedded-c/MQTTPacket/src/MQTTPacket.h"
#endif

/*
 * 引入softfp库,用户需要手动引入相应头文件
 */
#ifndef H3RDPARTY_SOFTFP_HEADER
#define H3RDPARTY_SOFTFP_HEADER "h3rdparty/3rdparty/softfp/softfp.h"
#endif
#ifndef H3RDPARTY_SOFTFLOAT_HEADER
#define H3RDPARTY_SOFTFLOAT_HEADER "h3rdparty/3rdparty/softfp/softfloat.h"
#endif

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

/*
 * 引入argtable3库,用户需要手动引入相应头文件
 */
#ifdef  H3RDPARTY_USING_SYSTEM_ARGTABLE3
#ifndef H3RDPARTY_ARGTABLE3_HEADER
#define H3RDPARTY_ARGTABLE3_HEADER "argtable3.h"
#endif
#else
#ifndef H3RDPARTY_ARGTABLE3_HEADER
#define H3RDPARTY_ARGTABLE3_HEADER "h3rdparty/3rdparty/argtable3/argtable3.h"
#endif
#endif

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

#endif // __H3RDPARTY_H__


