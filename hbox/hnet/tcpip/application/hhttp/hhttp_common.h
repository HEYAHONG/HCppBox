/***************************************************************
 * Name:      hhttp_common.h
 * Purpose:   声明hhttp_common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-10-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HHTTP_COMMON_H__
#define __HHTTP_COMMON_H__

#include "stdlib.h"
#include "stdbool.h"
#include "stdint.h"

/*
 * 此文件适用于HTTP/1.0、HTTP/1.1
 */

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 *  HTTP 字符串
 */
struct hhttp_string;
typedef struct hhttp_string hhttp_string_t;
struct hhttp_string
{
    const char *str;
    size_t      str_len;
};

/** \brief C字符串转字符串
 *
 * \param c_str const char* C字符串，其生命周期一定要比返回的字符串长
 * \return hhttp_string_t 字符串
 *
 */
hhttp_string_t hhttp_string_from_cstring(const char *c_str);

/** \brief 从缓冲区创建字符串
 *
 * \param buffer const uint8_t* 缓冲区指针,其生命周期一定要比返回的字符串长
 * \param buffer_len size_t 缓冲区长度
 * \return hhttp_string_t 字符串
 *
 */
hhttp_string_t hhttp_string_from_buffer(const uint8_t *buffer,size_t buffer_len);


/** \brief 字符串是否为空
 *
 * \param src hhttp_string_t 字符串
 * \return bool 是否为空
 *
 */
bool hhttp_string_is_empty(hhttp_string_t src);

/** \brief 字符串是否相等
 *
 * \param src hhttp_string_t 源字符串
 * \param dst hhttp_string_t 目标字符串
 * \return bool 是否相等
 *
 */
bool hhttp_string_equal(hhttp_string_t src,hhttp_string_t dst);

/** \brief 查找小字符串在大字符串的位置
 *
 * \param big_str hhttp_string_t 大字符串
 * \param small_str hhttp_string_t 小字符串
 * \return hhttp_string_t 字符串
 *
 */
hhttp_string_t hhttp_string_strstr(hhttp_string_t big_str,hhttp_string_t small_str);

/** \brief 十进制字符串转数字
 *
 * \param src hhttp_string_t 十进制数字符串，遇到第一个非数字字符时停止
 * \return uint64_t 数字
 *
 */
uint64_t hhttp_string_dec_to_uint64(hhttp_string_t src);


/** \brief 数字转字符串
 *
 * \param buffer uint8_t* 缓冲区，必须足够容纳数字，否则将返回空字符串
 * \param buffer_len size_t 缓冲区长度
 * \param number uint64_t 数字
 * \return hhttp_string_t 字符串
 *
 */
hhttp_string_t hhttp_string_dec_from_uint64(uint8_t *buffer,size_t buffer_len,uint64_t number);

/** \brief 十六进制字符串转数字
 *
 * \param src hhttp_string_t 十六进制数字符串，遇到第一个非数字字符时停止
 * \return uint64_t 数字
 *
 */
uint64_t hhttp_string_hex_to_uint64(hhttp_string_t src);


/** \brief 数字转字符串
 *
 * \param buffer uint8_t* 缓冲区，必须足够容纳数字，否则将返回空字符串
 * \param buffer_len size_t 缓冲区长度
 * \param number uint64_t 数字
 * \return hhttp_string_t 字符串
 *
 */
hhttp_string_t hhttp_string_hex_from_uint64(uint8_t *buffer,size_t buffer_len,uint64_t number);

/*
 * HTTP行结束(\r\n)
 */
#define HHTTP_LINE_END_STRING "\r\n"

/** \brief 检查字符串是否为行
 *
 * \param line hhttp_string_t 行字符串串
 * \return bool 是否为行字符串
 *
 */
bool hhttp_string_check_line(hhttp_string_t line);

/** \brief 从行获取内容（包含行结束（\r\n））
 *
 * \param line hhttp_string_t 行字符串（可包含额外内容）
 * \return hhttp_string_t 行字符串（以\r\n结尾的行字符串）
 *
 */
hhttp_string_t hhttp_string_line_get_content(hhttp_string_t line);

/** \brief 内容是否为空白行,空白行通常作为头与内容的分割
 *
 * \param line_content hhttp_string_t 行内容
 * \return bool 是否空白行
 *
 */
bool hhttp_string_line_content_is_empty(hhttp_string_t line_content);


typedef enum
{
    HHTTP_METHOD_UNKNOWN=0,
    HHTTP_METHOD_GET,
    HHTTP_METHOD_HEAD,
    HHTTP_METHOD_POST,
    HHTTP_METHOD_PUT,
    HHTTP_METHOD_DELETE,
    HHTTP_METHOD_CONNECT,
    HHTTP_METHOD_OPTIONS,
    HHTTP_METHOD_TRACE
} hhttp_method_t;

#define HHTTP_METHOD_GET_STRING         "GET"
#define HHTTP_METHOD_HEAD_STRING        "HEAD"
#define HHTTP_METHOD_POST_STRING        "POST"
#define HHTTP_METHOD_PUT_STRING         "PUT"
#define HHTTP_METHOD_DELETE_STRING      "DELETE"
#define HHTTP_METHOD_CONNECT_STRING     "CONNECT"
#define HHTTP_METHOD_OPTIONS_STRING     "OPTIONS"
#define HHTTP_METHOD_TRACE_STRING       "TRACE"

/** \brief 获取方法字符串
 *
 * \param method hhttp_method_t 方法
 * \return hhttp_string_t 字符串
 *
 */
hhttp_string_t hhttp_method_std_to_string(hhttp_method_t method);

/** \brief 从字符串获取方法
 *
 * \param str hhttp_string_t 字符串
 * \return hhttp_method_t 方法
 *
 */
hhttp_method_t hhttp_method_from_string(hhttp_string_t str);

typedef enum
{
    HHTTP_VERSION_UNKNOWN=0,
    HHTTP_VERSION_1_0,
    HHTTP_VERSION_1_1,
} hhttp_version_t;

#define HHTTP_VERSION_1_0_STRING "HTTP/1.0"
#define HHTTP_VERSION_1_1_STRING "HTTP/1.1"

/** \brief 获取版本字符串
 *
 * \param version hhttp_version_t 版本
 * \return hhttp_string_t 字符串
 *
 */
hhttp_string_t hhttp_version_std_to_string(hhttp_version_t version);

/** \brief 从字符串获取版本
 *
 * \param str hhttp_string_t 字符串
 * \return hhttp_version_t 版本
 *
 */
hhttp_version_t hhttp_version_from_string(hhttp_string_t str);

struct hhttp_request_line;
typedef struct hhttp_request_line hhttp_request_line_t;
struct hhttp_request_line
{
    hhttp_method_t          method;
    hhttp_string_t          target;
    hhttp_version_t         version;
};

/** \brief 请求行解析
 *
 * \param request_line hhttp_request_line_t* 请求行
 * \param line hhttp_string_t 行
 * \return bool 是否成功
 *
 */
bool hhttp_request_line_parse(hhttp_request_line_t *request_line,hhttp_string_t line);


/** \brief 请求行打包回调
 *
 * \param usr void* 用户参数
 * \param offset size_t 偏移
 * \param part_string hhttp_string_t 部分字符串
 *
 */
typedef void (*hhttp_request_line_pack_write_callback_t)(void *usr,size_t offset,hhttp_string_t part_string);

/** \brief 请求行打包
 *
 * \param request_line hhttp_request_line_t* 请求行打包
 * \param cb hhttp_request_line_pack_write_callback_t 请求行打包回调
 * \param usr void* 用户参数
 * \return bool 是否成功
 *
 */
bool hhttp_request_line_pack(hhttp_request_line_t *request_line,hhttp_request_line_pack_write_callback_t cb,void *usr);

typedef enum
{
    /*
     * 信息 1XX
     */
    HHTTP_STATUS_CODE_100=100,  /**< 继续 */
    HHTTP_STATUS_CODE_101=101,  /**< 切换协议 */

    /*
     * 成功
     */
    HHTTP_STATUS_CODE_200=200,  /**< 成功 */
    HHTTP_STATUS_CODE_201=201,  /**< 已创建*/
    HHTTP_STATUS_CODE_202=202,  /**< 已接受 */
    HHTTP_STATUS_CODE_203=203,  /**< 没有认证信息 */
    HHTTP_STATUS_CODE_204=204,  /**< 没有内容 */
    HHTTP_STATUS_CODE_205=205,  /**< 重置链接 */
    HHTTP_STATUS_CODE_206=206,  /**< 部分内容 */

    /*
     * 重定向
     */
    HHTTP_STATUS_CODE_300=300,  /**< 多个选择 */
    HHTTP_STATUS_CODE_301=301,  /**< 永久移动 */
    HHTTP_STATUS_CODE_302=302,  /**< 已找到 */
    HHTTP_STATUS_CODE_303=303,  /**< 见其它 */
    HHTTP_STATUS_CODE_304=304,  /**< 未修改 */
    HHTTP_STATUS_CODE_305=305,  /**< 使用代理 */
    HHTTP_STATUS_CODE_306=306,  /**< (未使用) */
    HHTTP_STATUS_CODE_307=307,  /**< 临时重定向 */
    HHTTP_STATUS_CODE_308=308,  /**< 永久重定向 */

    /*
     * 客户端错误
     */
    HHTTP_STATUS_CODE_400=400,  /**< 请求错误 */
    HHTTP_STATUS_CODE_401=401,  /**< 未认证 */
    HHTTP_STATUS_CODE_402=402,  /**< 需要支付 */
    HHTTP_STATUS_CODE_403=403,  /**< 禁止访问 */
    HHTTP_STATUS_CODE_404=404,  /**< 未找到 */
    HHTTP_STATUS_CODE_405=405,  /**< 不允许的方法 */
    HHTTP_STATUS_CODE_406=406,  /**< 不可接受 */
    HHTTP_STATUS_CODE_407=407,  /**< 需要代理认证 */
    HHTTP_STATUS_CODE_408=408,  /**< 请求超时 */
    HHTTP_STATUS_CODE_409=409,  /**< 冲突 */
    HHTTP_STATUS_CODE_410=410,  /**< 已消失 */
    HHTTP_STATUS_CODE_411=411,  /**< 需要长度 */
    HHTTP_STATUS_CODE_412=412,  /**< 条件失败 */
    HHTTP_STATUS_CODE_413=413,  /**< 内容太长 */
    HHTTP_STATUS_CODE_414=414,  /**< URI太长 */
    HHTTP_STATUS_CODE_415=415,  /**< 不支持的媒体类型 */
    HHTTP_STATUS_CODE_416=416,  /**< 范围不合适 */
    HHTTP_STATUS_CODE_417=417,  /**< 预期结果失败 */
    HHTTP_STATUS_CODE_418=418,  /**< (未使用) */
    HHTTP_STATUS_CODE_421=421,  /**< 错误方向的请求 */
    HHTTP_STATUS_CODE_422=422,  /**< 无法处理的内容 */
    HHTTP_STATUS_CODE_426=426,  /**< 需要升级协议 */

    /*
     * 服务器错误
     */
    HHTTP_STATUS_CODE_500=500,  /**< 内部服务器错误 */
    HHTTP_STATUS_CODE_501=501,  /**< 未实现 */
    HHTTP_STATUS_CODE_502=502,  /**< 网关错误 */
    HHTTP_STATUS_CODE_503=503,  /**< 服务不可用 */
    HHTTP_STATUS_CODE_504=504,  /**< 网关超时 */
    HHTTP_STATUS_CODE_505=505   /**< HTTP版本不支持 */

} hhttp_status_code_t;

/*
 * 信息 1XX
 */
#define HHTTP_STATUS_CODE_100_STRING "100"                              /**< 继续 */
#define HHTTP_STATUS_CODE_101_STRING "101"                              /**< 切换协议 */

/*
 * 成功
 */
#define HHTTP_STATUS_CODE_200_STRING "200"                              /**< 成功 */
#define HHTTP_STATUS_CODE_201_STRING "201"                              /**< 已创建*/
#define HHTTP_STATUS_CODE_202_STRING "202"                              /**< 已接受 */
#define HHTTP_STATUS_CODE_203_STRING "203"                              /**< 没有认证信息 */
#define HHTTP_STATUS_CODE_204_STRING "204"                              /**< 没有内容 */
#define HHTTP_STATUS_CODE_205_STRING "205"                              /**< 重置链接 */
#define HHTTP_STATUS_CODE_206_STRING "206"                              /**< 部分内容 */

/*
 * 重定向
 */
#define HHTTP_STATUS_CODE_300_STRING "300"                              /**< 多个选择 */
#define HHTTP_STATUS_CODE_301_STRING "301"                              /**< 永久移动 */
#define HHTTP_STATUS_CODE_302_STRING "302"                              /**< 已找到 */
#define HHTTP_STATUS_CODE_303_STRING "303"                              /**< 见其它 */
#define HHTTP_STATUS_CODE_304_STRING "304"                              /**< 未修改 */
#define HHTTP_STATUS_CODE_305_STRING "305"                              /**< 使用代理 */
#define HHTTP_STATUS_CODE_306_STRING "306"                              /**< (未使用) */
#define HHTTP_STATUS_CODE_307_STRING "307"                              /**< 临时重定向 */
#define HHTTP_STATUS_CODE_308_STRING "308"                              /**< 永久重定向 */

/*
 * 客户端错误
 */
#define HHTTP_STATUS_CODE_400_STRING "400"                              /**< 请求错误 */
#define HHTTP_STATUS_CODE_401_STRING "401"                              /**< 未认证 */
#define HHTTP_STATUS_CODE_402_STRING "402"                              /**< 需要支付 */
#define HHTTP_STATUS_CODE_403_STRING "403"                              /**< 禁止访问 */
#define HHTTP_STATUS_CODE_404_STRING "404"                              /**< 未找到 */
#define HHTTP_STATUS_CODE_405_STRING "405"                              /**< 不允许的方法 */
#define HHTTP_STATUS_CODE_406_STRING "406"                              /**< 不可接受 */
#define HHTTP_STATUS_CODE_407_STRING "407"                              /**< 需要代理认证 */
#define HHTTP_STATUS_CODE_408_STRING "408"                              /**< 请求超时 */
#define HHTTP_STATUS_CODE_409_STRING "409"                              /**< 冲突 */
#define HHTTP_STATUS_CODE_410_STRING "410"                              /**< 已消失 */
#define HHTTP_STATUS_CODE_411_STRING "411"                              /**< 需要长度 */
#define HHTTP_STATUS_CODE_412_STRING "412"                              /**< 条件失败 */
#define HHTTP_STATUS_CODE_413_STRING "413"                              /**< 内容太长 */
#define HHTTP_STATUS_CODE_414_STRING "414"                              /**< URI太长 */
#define HHTTP_STATUS_CODE_415_STRING "415"                              /**< 不支持的媒体类型 */
#define HHTTP_STATUS_CODE_416_STRING "416"                              /**< 范围不合适 */
#define HHTTP_STATUS_CODE_417_STRING "417"                              /**< 预期结果失败 */
#define HHTTP_STATUS_CODE_418_STRING "418"                              /**< (未使用) */
#define HHTTP_STATUS_CODE_421_STRING "421"                              /**< 错误方向的请求 */
#define HHTTP_STATUS_CODE_422_STRING "422"                              /**< 无法处理的内容 */
#define HHTTP_STATUS_CODE_426_STRING "426"                              /**< 需要升级协议 */

/*
 * 服务器错误
 */
#define HHTTP_STATUS_CODE_500_STRING "500"                              /**< 内部服务器错误 */
#define HHTTP_STATUS_CODE_501_STRING "501"                              /**< 未实现 */
#define HHTTP_STATUS_CODE_502_STRING "502"                              /**< 网关错误 */
#define HHTTP_STATUS_CODE_503_STRING "503"                              /**< 服务不可用 */
#define HHTTP_STATUS_CODE_504_STRING "504"                              /**< 网关超时 */
#define HHTTP_STATUS_CODE_505_STRING "505"                              /**< HTTP版本不支持 */

/** \brief 标准状态码的字符串
 *
 * \param status_code hhttp_status_code_t 标准状态码（非标状态码自行处理）
 * \return hhttp_string_t 字符串
 *
 */
hhttp_string_t hhttp_status_code_std_to_string(hhttp_status_code_t status_code);

/** \brief 从字符串获取状态码
 *
 * \param str hhttp_string_t  状态码字符串
 * \return int 状态码
 *
 */
int hhttp_status_code_from_string(hhttp_string_t str);

/*
 * 信息 1XX
 */
#define HHTTP_STATUS_CODE_100_REASON_STRING "Continue"                          /**< 继续 */
#define HHTTP_STATUS_CODE_101_REASON_STRING "Switching Protocols"               /**< 切换协议 */

/*
 * 成功
 */
#define HHTTP_STATUS_CODE_200_REASON_STRING "OK"                                /**< 成功 */
#define HHTTP_STATUS_CODE_201_REASON_STRING "Created"                           /**< 已创建*/
#define HHTTP_STATUS_CODE_202_REASON_STRING "Accepted"                          /**< 已接受 */
#define HHTTP_STATUS_CODE_203_REASON_STRING "Non-Authoritative Information"     /**< 没有认证信息 */
#define HHTTP_STATUS_CODE_204_REASON_STRING "No Content"                        /**< 没有内容 */
#define HHTTP_STATUS_CODE_205_REASON_STRING "Reset Content"                     /**< 重置链接 */
#define HHTTP_STATUS_CODE_206_REASON_STRING "Partial Content"                   /**< 部分内容 */

/*
 * 重定向
 */
#define HHTTP_STATUS_CODE_300_REASON_STRING "Multiple Choices"                  /**< 多个选择 */
#define HHTTP_STATUS_CODE_301_REASON_STRING "Moved Permanently"                 /**< 永久移动 */
#define HHTTP_STATUS_CODE_302_REASON_STRING "Found"                             /**< 已找到 */
#define HHTTP_STATUS_CODE_303_REASON_STRING "See Other"                         /**< 见其它 */
#define HHTTP_STATUS_CODE_304_REASON_STRING "Not Modified"                      /**< 未修改 */
#define HHTTP_STATUS_CODE_305_REASON_STRING "Use Proxy"                         /**< 使用代理 */
#define HHTTP_STATUS_CODE_306_REASON_STRING "(UNUSED)"                          /**< (未使用) */
#define HHTTP_STATUS_CODE_307_REASON_STRING "Temporary Redirect"                /**< 临时重定向 */
#define HHTTP_STATUS_CODE_308_REASON_STRING "Permanent Redirect"                /**< 永久重定向 */

/*
 * 客户端错误
 */
#define HHTTP_STATUS_CODE_400_REASON_STRING "Bad Request"                       /**< 请求错误 */
#define HHTTP_STATUS_CODE_401_REASON_STRING "Unauthorized"                      /**< 未认证 */
#define HHTTP_STATUS_CODE_402_REASON_STRING "Payment Required"                  /**< 需要支付 */
#define HHTTP_STATUS_CODE_403_REASON_STRING "Forbidden"                         /**< 禁止访问 */
#define HHTTP_STATUS_CODE_404_REASON_STRING "Not Found"                         /**< 未找到 */
#define HHTTP_STATUS_CODE_405_REASON_STRING "Method Not Allowed"                /**< 不允许的方法 */
#define HHTTP_STATUS_CODE_406_REASON_STRING "Not Acceptable"                    /**< 不可接受 */
#define HHTTP_STATUS_CODE_407_REASON_STRING "Proxy Authentication Required"     /**< 需要代理认证 */
#define HHTTP_STATUS_CODE_408_REASON_STRING "Request Timeout"                   /**< 请求超时 */
#define HHTTP_STATUS_CODE_409_REASON_STRING "Conflict"                          /**< 冲突 */
#define HHTTP_STATUS_CODE_410_REASON_STRING "Gone"                              /**< 已消失 */
#define HHTTP_STATUS_CODE_411_REASON_STRING "Length Required"                   /**< 需要长度 */
#define HHTTP_STATUS_CODE_412_REASON_STRING "Precondition Failed"               /**< 条件失败 */
#define HHTTP_STATUS_CODE_413_REASON_STRING "Content Too Large"                 /**< 内容太长 */
#define HHTTP_STATUS_CODE_414_REASON_STRING "URI Too Long"                      /**< URI太长 */
#define HHTTP_STATUS_CODE_415_REASON_STRING "Unsupported Media Type"            /**< 不支持的媒体类型 */
#define HHTTP_STATUS_CODE_416_REASON_STRING "Range Not Satisfiable"             /**< 范围不合适 */
#define HHTTP_STATUS_CODE_417_REASON_STRING "Expectation Failed"                /**< 预期结果失败 */
#define HHTTP_STATUS_CODE_418_REASON_STRING "(UNUSED)"                          /**< (未使用) */
#define HHTTP_STATUS_CODE_421_REASON_STRING "Misdirected Request"               /**< 错误方向的请求 */
#define HHTTP_STATUS_CODE_422_REASON_STRING "Unprocessable Content"             /**< 无法处理的内容 */
#define HHTTP_STATUS_CODE_426_REASON_STRING "Upgrade Required"                  /**< 需要升级协议 */

/*
 * 服务器错误
 */
#define HHTTP_STATUS_CODE_500_REASON_STRING "Internal Server Error"             /**< 内部服务器错误 */
#define HHTTP_STATUS_CODE_501_REASON_STRING "Not Implemented"                   /**< 未实现 */
#define HHTTP_STATUS_CODE_502_REASON_STRING "Bad Gateway"                       /**< 网关错误 */
#define HHTTP_STATUS_CODE_503_REASON_STRING "Service Unavailable"               /**< 服务不可用 */
#define HHTTP_STATUS_CODE_504_REASON_STRING "Gateway Timeout"                   /**< 网关超时 */
#define HHTTP_STATUS_CODE_505_REASON_STRING "HTTP Version Not Supported"        /**< HTTP版本不支持 */

/** \brief 标准状态码的原因字符串
 *
 * \param status_code hhttp_status_code_t 标准状态码（非标状态码自行处理）
 * \return hhttp_string_t 字符串
 *
 */
hhttp_string_t hhttp_status_code_std_to_reason_string(hhttp_status_code_t status_code);

struct hhttp_status_line;
typedef struct hhttp_status_line hhttp_status_line_t;
struct hhttp_status_line
{
    hhttp_version_t     version;
    hhttp_status_code_t status;
    hhttp_string_t      reason;
};

/** \brief 状态行解析
 *
 * \param status_line hhttp_status_line_t* 状态行
 * \param line hhttp_string_t 行
 * \return bool 是否成功
 *
 */
bool hhttp_status_line_parse(hhttp_status_line_t *status_line,hhttp_string_t line);

/** \brief 状态行打包回调
 *
 * \param usr void* 用户参数
 * \param offset size_t 偏移
 * \param part_string hhttp_string_t 部分字符串
 *
 */
typedef void (*hhttp_status_line_pack_write_callback_t)(void *usr,size_t offset,hhttp_string_t part_string);

/** \brief 状态行打包
 *
 * \param status_line hhttp_status_line_t* 状态行打包
 * \param cb hhttp_status_line_pack_write_callback_t 状态行打包回调
 * \param usr void* 用户参数
 * \return bool 是否成功
 *
 */
bool hhttp_status_line_pack(hhttp_status_line_t *status_line,hhttp_status_line_pack_write_callback_t cb,void *usr);

/*
 * 头部字段:以:分割的键名键值对
 */
struct hhttp_field;
typedef struct hhttp_field hhttp_field_t;
struct hhttp_field
{
    hhttp_string_t field_name;
    hhttp_string_t field_values;
};

/** \brief 头部字段行解析
 *
 * \param field_line hhttp_field_t* 头部字段行
 * \param line hhttp_string_t 行
 * \return bool 是否成功
 *
 */
bool hhttp_field_parse(hhttp_field_t *field_line,hhttp_string_t line);

/** \brief 头部字段行打包回调
 *
 * \param usr void* 用户参数
 * \param offset size_t 偏移
 * \param part_string hhttp_string_t 部分字符串
 *
 */
typedef void (*hhttp_field_line_pack_write_callback_t)(void *usr,size_t offset,hhttp_string_t part_string);

/** \brief 头部字段行打包
 *
 * \param field_line hhttp_field_t* 头部字段行打包
 * \param cb hhttp_field_line_pack_write_callback_t 头部字段行打包回调
 * \param usr void* 用户参数
 * \return bool 是否成功
 *
 */
bool hhttp_field_line_pack(hhttp_field_t *field_line,hhttp_field_line_pack_write_callback_t cb,void *usr);

/*
 * 头部字段(Authorization,Basic认证)
 */
struct hhttp_field_basic_authorization;
typedef struct hhttp_field_basic_authorization hhttp_field_basic_authorization_t;
struct hhttp_field_basic_authorization
{
    hhttp_string_t userinfo;                        /**< 用户信息，通常是以:分割的用户名与密码,但有时直接将token写在用于用户信息的位置  */
};

/** \brief 头部字段（Authorization,Basic认证）行打包
 *
 * \param fbasic_authorization hhttp_field_basic_authorization_t* 头部字段（Authorization,Basic认证）行打包
 * \param cb hhttp_field_line_pack_write_callback_t 头部字段行打包回调
 * \param usr void* 用户参数
 * \return bool 是否成功
 *
 */
bool hhttp_field_basic_authorization_pack(hhttp_field_basic_authorization_t *basic_authorization,hhttp_field_line_pack_write_callback_t cb,void *usr);




/*
 * 主体（body）与头部字段(field)使用空行分割，本组件主要支持以下情况:
 *      -当Connection等于close(HTTP/1.0默认)时，内容截至当前连接结束,无需其它字段支持。
 *      -当Connection等于keep-alive(HTTP/1.1默认)时，需要使用Transfer-Encoding或者Content-Length解析长度。
 *
 */


/** \brief 主体是否为行开始
 *
 * \param line hhttp_string_t 行
 * \return bool 是否为主体开始
 *
 */
bool hhttp_body_is_begin(hhttp_string_t line);

struct hhttp_body_normal_parse;
typedef struct hhttp_body_normal_parse hhttp_body_normal_parse_t;
typedef void (*hhttp_body_normal_parse_data_t)(hhttp_body_normal_parse_t *body_normal,uint64_t offset,hhttp_string_t part_data);
struct hhttp_body_normal_parse
{
    hhttp_body_normal_parse_data_t data;        /**< 数据处理函数 */
    uint64_t offset;                            /**< 数据偏移（等于已写入的数据大小） */
    uint64_t max_length;                        /**< 最大数据大小,当最大数据大小为((uint64_t)-1)时，表示未指定数据大小，此时数据大小由连接关闭确认 */
    void *usr;                                  /**< 用户参数 */
    struct
    {
        uint32_t data_end_call:1;               /**< 已调用数据结束回调 */
    } flags;

};

#define HHTTP_BODY_NORMAL_MAX_LENGTH_UNDEFINED      ((uint64_t)-1)

/** \brief 正常主体初始化
 *
 * \param body_normal hhttp_body_normal_parse_t* 主体
 * \param data hhttp_body_normal_parse_data_t 数据处理函数
 * \param max_length size_t 最大大小
 * \param usr void* 用户参数
 * \return bool 是否成功
 *
 */
bool hhttp_body_normal_parse_init(hhttp_body_normal_parse_t *body_normal,hhttp_body_normal_parse_data_t data,size_t max_length,void *usr);

/** \brief 是否已完成本次主体的接收
 *
 * \param body_normal hhttp_body_normal_parse_t* 主体
 * \return bool 是否结束
 *
 */
bool hhttp_body_normal_parse_is_end(hhttp_body_normal_parse_t *body_normal);

/** \brief 主体数据输入
 *
 * \param body_normal hhttp_body_normal_parse_t* 主体
 * \param data const uint8_t* 数据，不包含用于分割头部的空行
 * \param data_length size_t* 数据长度，传入数据长度，传出已使用的长度，当输入与输出不相等时表明主体已结束且需要处理后续字节
 * \return bool 是否正确处理
 *
 */
bool hhttp_body_normal_parse_input(hhttp_body_normal_parse_t *body_normal,const uint8_t *data,size_t *data_length);

/** \brief 主体输入结束，一般在连接结束时调用
 *
 * \param body_normal hhttp_body_normal_parse_t* 主体
 * \return bool 是否正确处理
 *
 */
bool hhttp_body_normal_parse_input_end(hhttp_body_normal_parse_t *body_normal);

struct hhttp_body_chunked_parse;
typedef struct hhttp_body_chunked_parse hhttp_body_chunked_parse_t;
typedef void (*hhttp_body_chunked_parse_data_t)(hhttp_body_chunked_parse_t *body_chunked,uint64_t offset,hhttp_string_t part_data);
struct hhttp_body_chunked_parse
{
    hhttp_body_chunked_parse_data_t data;           /**< 数据处理函数 */
    uint64_t offset;                                /**< 数据偏移（等于已写入的数据大小） */
    uint64_t single_offset;                         /**< 单次偏移 */
    uint64_t single_length;                         /**< 单次长度  */
    void *usr;
    struct
    {
        uint32_t l_d:1;                             /**< 长度(0)或者数据(1) */
        uint32_t data_end:1;                        /**< 数据结束 */
        uint32_t data_end_call:1;                   /**< 已调用数据结束回调 */
    } flags;
    uint8_t  length_string_buffer[20];              /**< 用于存储长度字符换 */
};

/** \brief chunked主体初始化
 *
 * \param body_chunked hhttp_body_chunked_parse_t* 主体
 * \param data hhttp_body_chunked_parse_data_t 数据处理函数
 * \param usr void* 用户参数
 * \return bool 是否正确处理
 *
 */
bool hhttp_body_chunked_parse_init(hhttp_body_chunked_parse_t *body_chunked,hhttp_body_chunked_parse_data_t data,void *usr);

/** \brief 是否已完成本次主体的接收
 *
 * \param body_chunked hhttp_body_chunked_parse_t* 主体
 * \return bool 是否结束
 *
 */
bool hhttp_body_chunked_parse_is_end(hhttp_body_chunked_parse_t *body_chunked);

/** \brief chunked主体数据输入
 *
 * \param body_chunked hhttp_body_chunked_parse_t* 主体
 * \param data const uint8_t* 数据，不包含用于分割头部的空行
 * \param data_length size_t* 数据长度，传入数据长度，传出已使用的长度，当输入与输出不相等时需要处理后续字节
 * \return bool 是否正确处理
 *
 */
bool hhttp_body_chunked_parse_input(hhttp_body_chunked_parse_t *body_chunked,const uint8_t *data,size_t *data_length);

/** \brief chunked主体写入回调
 *
 * \param usr void* 用户参数
 * \param offset size_t 偏移
 * \param part_string hhttp_string_t 部分字符串
 *
 */
typedef void (*hhttp_body_write_callback_t)(void *usr,size_t offset,hhttp_string_t part_string);

/** \brief 主体起始
 *
 * \param cb hhttp_body_write_callback_t 写入回调
 * \param usr void* 用户参数
 * \return bool 是否成功
 *
 */
bool hhttp_body_begin(hhttp_body_write_callback_t cb,void *usr);

/** \brief 主体打包chunked数据
 *
 * \param cb hhttp_body_write_callback_t 写入回调
 * \param usr void* 用户参数
 * \param data hhttp_string_t 数据,当数据为空时表示写入结束，不可继续写入数据
 * \return bool 是否成功
 *
 */
bool hhttp_body_chunked_pack(hhttp_body_write_callback_t cb,void *usr,hhttp_string_t data);


/** \brief 主体chunked数据结束
 *
 * \param cb hhttp_body_write_callback_t 写入回调
 * \param usr void* 用户数据
 * \return bool 是否成功
 *
 */
bool hhttp_body_chunked_end(hhttp_body_write_callback_t cb,void *usr);



#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HHTTP_COMMON_H__
