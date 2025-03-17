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

/*
 *  此模块引入第三方库
 */

/*
 * 引入cJSON库
 */
#ifndef H3RDPARTY_USING_SYSTEM_CJSON
#include "3rdparty/cJSON/cJSON.h"
#include "3rdparty/cJSON/cJSON_Utils.h"
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
#ifndef H3RDPARTY_USING_SYSTEM_NANOPB
#include "3rdparty/nanopb/pb.h"
#include "3rdparty/nanopb/pb_encode.h"
#include "3rdparty/nanopb/pb_decode.h"
#else
#include "pb.h"
#include "pb_encode.h"
#include "pb_decode.h"
#endif

/*
 * 引入paho.mqtt.embedded-c库,用户需要手动引入相应头文件
 */
#ifndef H3RDPARTY_PAHO_MQTT_EMBEDDED_C_PACKET_HEADER
#define H3RDPARTY_PAHO_MQTT_EMBEDDED_C_PACKET_HEADER "3rdparty/paho.mqtt.embedded-c/MQTTPacket/src/MQTTPacket.h"
#endif

/*
 * 引入softfp库,用户需要手动引入相应头文件
 */
#ifndef H3RDPARTY_SOFTFP_HEADER
#define H3RDPARTY_SOFTFP_HEADER "3rdparty/softfp/softfp.h"
#endif
#ifndef H3RDPARTY_SOFTFLOAT_HEADER
#define H3RDPARTY_SOFTFLOAT_HEADER "3rdparty/softfp/softfloat.h"
#endif

#endif // __H3RDPARTY_H__


