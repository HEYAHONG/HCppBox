/***************************************************************
 * Name:      huriparser.h
 * Purpose:   声明huriparser接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-10-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HURIPARSER_H_INCLUDE__
#define __HURIPARSER_H_INCLUDE__
#include "h3rdparty_port_uriparser.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

struct huriparser_string;
typedef struct huriparser_string huriparser_string_t;
struct huriparser_string
{
    const char *str;
    size_t str_len;
};

/** \brief uri字符串是否为空
 *
 * \param uriparser_string huriparser_string_t uri字符串
 * \return bool 是否问为空
 *
 */
bool huriparser_string_is_empty(huriparser_string_t uriparser_string);


/** \brief 获取方案
 *
 * \param uri_cstring const char* 字符串,特殊字符与中文字符需要先使用%组编码
 * \return huriparser_string_t 字符串
 *
 */
huriparser_string_t huriparser_parse_scheme(const char *uri_cstring);

/** \brief 获取用户信息
 *
 * \param uri_cstring const char* 字符串,特殊字符与中文字符需要先使用%组编码
 * \return huriparser_string_t 字符串
 *
 */
huriparser_string_t huriparser_parse_userinfo(const char *uri_cstring);

/** \brief 获取主机名
 *
 * \param uri_cstring const char* 字符串,特殊字符与中文字符需要先使用%组编码
 * \return huriparser_string_t 字符串
 *
 */
huriparser_string_t huriparser_parse_host(const char *uri_cstring);

/** \brief 获取端口
 *
 * \param uri_cstring const char* 字符串,特殊字符与中文字符需要先使用%组编码
 * \return huriparser_string_t 字符串
 *
 */
huriparser_string_t huriparser_parse_port(const char *uri_cstring);

/** \brief 获取查询
 *
 * \param uri_cstring const char* 字符串,特殊字符与中文字符需要先使用%组编码
 * \return huriparser_string_t 字符串
 *
 */
huriparser_string_t huriparser_parse_query(const char *uri_cstring);

/** \brief 获取分片
 *
 * \param uri_cstring const char* 字符串,特殊字符与中文字符需要先使用%组编码
 * \return huriparser_string_t 字符串
 *
 */
huriparser_string_t huriparser_parse_fragment(const char *uri_cstring);

/** \brief 获取HTTP目标（端口之后，分片之前）
 *
 * \param uri_cstring const char* 字符串,特殊字符与中文字符需要先使用%组编码
 * \return huriparser_string_t 字符串
 *
 */
huriparser_string_t huriparser_parse_http_target(const char *uri_cstring);

/** \brief 获取HTTP主机
 *
 * \param uri_cstring const char* 字符串,特殊字符与中文字符需要先使用%组编码
 * \return huriparser_string_t 字符串
 *
 */
huriparser_string_t huriparser_parse_http_host(const char *uri_cstring);


/** \brief 去除%组，还原字符串
 *
 * \param uri_string char* 字符串（一般用于处理路径与查询字符串的值或者键名）,以'\0'结尾
 * \return const char* '\0'存在的位置
 *
 */
const char *huriparser_unescape_in_place(char *uri_string);

/** \brief 去除特殊字符，添加%组
 *
 * \param uri_in_string const char* 字符串（一般用于处理路径与查询字符串中的值或者键名）,以'\0'结尾
 * \param uri_out_string char* 输出缓冲，需要三倍于输入字符串长度（额外预留一个'\0'空间）。
 * \return const char* '\0'存在的位置
 *
 */
const char *huriparser_escape(const char *uri_in_string,char *uri_out_string);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HURIPARSER_H_INCLUDE__
