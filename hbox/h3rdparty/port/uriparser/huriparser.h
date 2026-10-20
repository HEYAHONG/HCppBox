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
 * \param uri_cstring const char* 字符串
 * \return huriparser_string_t 字符串
 *
 */
huriparser_string_t huriparser_parse_scheme(const char *uri_cstring);

/** \brief 获取用户信息
 *
 * \param uri_cstring const char* 字符串
 * \return huriparser_string_t 字符串
 *
 */
huriparser_string_t huriparser_parse_userinfo(const char *uri_cstring);

/** \brief 获取主机名
 *
 * \param uri_cstring const char* 字符串
 * \return huriparser_string_t 字符串
 *
 */
huriparser_string_t huriparser_parse_host(const char *uri_cstring);

/** \brief 获取端口
 *
 * \param uri_cstring const char* 字符串
 * \return huriparser_string_t 字符串
 *
 */
huriparser_string_t huriparser_parse_port(const char *uri_cstring);

/** \brief 获取查询
 *
 * \param uri_cstring const char* 字符串
 * \return huriparser_string_t 字符串
 *
 */
huriparser_string_t huriparser_parse_query(const char *uri_cstring);

/** \brief 获取分片
 *
 * \param uri_cstring const char* 字符串
 * \return huriparser_string_t 字符串
 *
 */
huriparser_string_t huriparser_parse_fragment(const char *uri_cstring);

/** \brief 获取HTTP目标（端口之后，分片之前）
 *
 * \param uri_cstring const char* 字符串
 * \return huriparser_string_t 字符串
 *
 */
huriparser_string_t huriparser_parse_http_target(const char *uri_cstring);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HURIPARSER_H_INCLUDE__
