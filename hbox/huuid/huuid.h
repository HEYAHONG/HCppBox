/***************************************************************
 * Name:      huuid.h
 * Purpose:   声明huuid接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-09
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HUUID_H_INCLUDED__
#define __HUUID_H_INCLUDED__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "limits.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 * 定义uuid类型
 */
typedef uint8_t huuid_t[16];    /**< 128位uuid */

/*
 * 定义UUID(局部)
 */
#define HUUID_DEFINE_LOCAL(name,u0,u1,u2,u3,u4,u5,u6,u7,u8,u9,u10,u11,u12,u13,u14,u15) \
	static const huuid_t name = {u0,u1,u2,u3,u4,u5,u6,u7,u8,u9,u10,u11,u12,u13,u14,u15}

/*
 * 定义UUID(全局)
 */
#define HUUID_DEFINE_GLOBAL(name,u0,u1,u2,u3,u4,u5,u6,u7,u8,u9,u10,u11,u12,u13,u14,u15) \
	const huuid_t name = {u0,u1,u2,u3,u4,u5,u6,u7,u8,u9,u10,u11,u12,u13,u14,u15}

/*
 * 声明UUID(全局)
 */
#define HUUID_DECLARE_GLOBAL(name) \
	extern const huuid_t name


HUUID_DECLARE_GLOBAL(huuid_null_uuid);      /**< 声明空uuid */

/** \brief uuid清零
 *
 * \param uu huuid_t uuid
 *
 */
void huuid_clear(huuid_t uu);

/** \brief uuid比较
 *
 * \param uu1 const huuid_t uuid1
 * \param uu2 const huuid_t uuid2
 * \return int 等于0时表示uuid相等
 *
 */
int huuid_compare(const huuid_t uu1, const huuid_t uu2);

/** \brief uuid复制
 *
 * \param dst huuid_t 目标uuid
 * \param src const huuid_t 源uuid
 *
 */
void huuid_copy(huuid_t dst, const huuid_t src);

/** \brief uuid是否为空
 *
 * \param uu const huuid_t uuid
 * \return int 等于0时表示相等
 *
 */
int huuid_is_null(const huuid_t uu);

struct huuid_base_version;
typedef struct huuid_base_version huuid_base_version_t;     /**< 基础版本的uuid,通常指V1版本的UUID */
struct huuid_base_version
{
    uint32_t	time_low;
    uint16_t	time_mid;
    uint16_t	time_hi_and_version;
    uint16_t	clock_seq;
    uint8_t	    node[6];
};

/** \brief uuid打包（将uuid基础版本转化为uuid）
 *
 * \param uu const huuid_base_version_t* uuid基础版本
 * \param ptr huuid_t uuid
 *
 */
void huuid_pack(const huuid_base_version_t *uu, huuid_t ptr);


/** \brief uuid解包（将uuid转化为uuid基础版本）
 *
 * \param in const huuid_t uuid
 * \param uu huuid_base_version_t* uuid基础版本
 *
 */
void huuid_unpack(const huuid_t in, huuid_base_version_t *uu);

/*
 * uuid中使用的node
 */
typedef uint8_t huuid_node_t[6];

/** \brief 随机node格式化，当使用随机数作为uuid的node时，需要使用此函数进行格式化。如果使用MAC地址则不需要使用此函数格式化
 *
 * \param node huuid_node_t node
 *
 */
void huuid_node_format(huuid_node_t node);


/** \brief 将UNIX时间戳转换为Gregorian Epoch时间戳（由uuid v1或者uuid v6使用）
 *
 * \param unix_ts_us uint64_t UNIX时间戳（us）（UNIX或类UNIX可通过gettimeofday获取）
 * \return uint64_t Gregorian Epoch时间戳（单位100ns）
 *
 */
uint64_t huuid_timestamp_convert(uint64_t unix_ts_us);

/** \brief 时间uuid(V1)格式化。
 *
 * \param time_uuid huuid_t 待格式化的
 * \param timestamp uint64_t 60位时间戳(Gregorian Epoch时间戳)
 * \param clock_seq uint16_t clock_seq
 * \param node huuid_node_t node
 *
 */
void huuid_time_uuid_format(huuid_t time_uuid,uint64_t timestamp,uint16_t clock_seq,huuid_node_t node);

/*
 * 声明一些命名空间(用于V3或者V5)
 */
HUUID_DECLARE_GLOBAL(huuid_dns_uuid);       /**< 声明DNS uuid */
HUUID_DECLARE_GLOBAL(huuid_url_uuid);       /**< 声明URL uuid */
HUUID_DECLARE_GLOBAL(huuid_oid_uuid);       /**< 声明OID uuid */
HUUID_DECLARE_GLOBAL(huuid_x500_uuid);      /**< 声明X500 uuid */

/** \brief MD5 uuid(V3)生成
 *
 * \param output huuid_t 待输出的uuid缓冲
 * \param name const uint8_t* 名称
 * \param name_len size_t 名称长度
 * \param namespace_uuid const huuid_t 命名空间
 * \return bool 是否产生成功
 *
 */
bool huuid_md5_uuid_generate(huuid_t output,const uint8_t *name,size_t name_len,const huuid_t namespace_uuid);

/** \brief 随机数uuid(V4)格式化。当使用随机数填充uuid后，再使用此函数格式化则可得到符合标准的uuid
 *
 * \param random_uuid uint8_t 待格式化的随机数据uuid
 *
 */
void huuid_random_uuid_format(huuid_t random_uuid);


/** \brief SHA1 uuid(V5)生成
 *
 * \param output huuid_t 待输出的uuid缓冲
 * \param name const uint8_t* 名称
 * \param name_len size_t 名称长度
 * \param namespace_uuid const huuid_t 命名空间
 * \return bool 是否产生成功
 *
 */
bool huuid_sha1_uuid_generate(huuid_t output,const uint8_t *name,size_t name_len,const huuid_t namespace_uuid);


/** \brief 时间uuid(V6)格式化。
 *
 * \param time_uuid huuid_t 待格式化的
 * \param timestamp uint64_t 60位时间戳(Gregorian Epoch时间戳)
 * \param clock_seq uint16_t clock_seq
 * \param node huuid_node_t node
 *
 */
void huuid_time_be_uuid_format(huuid_t time_uuid,uint64_t timestamp,uint16_t clock_seq,huuid_node_t node);


/** \brief 时间顺序+随机uuid(V7)格式化。当使用随机数填充uuid（后80位）后，再使用此函数格式化则可得到符合标准的uuid
 *
 * \param time_ordered_random_uuid huuid_t 待格式化的时间顺序+随机uuid
 * \param unix_ts_ms uint64_t 48位UNIX毫秒时间戳
 *
 */
void huuid_time_ordered_random_uuid_format(huuid_t time_ordered_random_uuid,uint64_t unix_ts_ms);

/** \brief 自定义uuid(V8)格式化。当用户自行设计uuid值时,使用此函数格式化后可得到符合标准的uuid
 *
 * \param custom_uuid huuid_t 待格式化的自定义uuid
 *
 */
void huuid_custom_uuid_format(huuid_t custom_uuid);

typedef char huuid_string_t[sizeof(huuid_t)*2+4+1]; /**< uuid字符串 */

/** \brief uuid转换为uuid字符串(默认大写)
 *
 * \param out huuid_string_t uuid字符串
 * \param uuid const huuid_t uuid
 *
 */
void huuid_unparse(huuid_string_t out,const huuid_t uuid);

/** \brief uuid转换为uuid字符串
 *
 * \param out huuid_string_t uuid字符串
 * \param uuid const huuid_t uuid
 *
 */
void huuid_unparse_upper(huuid_string_t out,const huuid_t uuid);

/** \brief uuid转换为uuid字符串
 *
 * \param out huuid_string_t uuid字符串
 * \param uuid const huuid_t uuid
 *
 */
void huuid_unparse_lower(huuid_string_t out,const huuid_t uuid);

/** \brief 将uuid字符串转化为uuid
 *
 * \param out huuid_t uuid
 * \param uuid_string const char* 输入（uuid字符串）
 * \return bool 是否转换成功
 *
 */
bool huuid_parse(huuid_t out,const char *uuid_string);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HUUID_H_INCLUDED__
