/***************************************************************
 * Name:      hsntp.h
 * Purpose:   声明hsntp接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-09-29
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSNTP_H__
#define __HSNTP_H__
#include "stdbool.h"
#include "stdint.h"
#include "hdefaults.h"

/*
 * 参考标准:RFC5905
 */

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 获取当前纪元
 *
 * \return int32_t 纪元
 *
 */
int32_t hsntp_era_get(void);

/** \brief 设定当前纪元
 *
 * \param era int32_t 纪元，默认为0,当超过2036年时应当设置为具体值，如2040年应当设定为1
 *
 */
void hsntp_era_set(int32_t era);

/** \brief sntp时间戳(秒)转换为时间（秒）
 *
 * \param timestamp_seconds uint32_t  sntp时间戳（秒）
 * \return htime_t 时间（秒）
 *
 */
htime_t hsntp_timestamp_to_time_t(uint32_t timestamp_seconds);

/** \brief 时间转换为sntp时间戳(秒)
 *
 * \param time_seconds htime_t 时间（秒）
 * \return uint32_t sntp时间戳(秒)
 *
 */
uint32_t hsntp_time_t_to_timestamp(htime_t time_seconds);


/*
 * SNTP消息长度(不含扩展)
 */
#define HSNTP_MSG_LEN                                   48

union hsntp_packet;
typedef union hsntp_packet hsntp_packet_t;
union hsntp_packet
{
    uint8_t msg[HSNTP_MSG_LEN];
    struct
    {
        struct
        {
            uint8_t li:2;
            uint8_t vn:3;
            uint8_t mode:3;
        };
        int8_t stratum;
        int8_t poll;
        int8_t precision;
        uint32_t rootdealy;
        uint32_t rootdisp;
        uint32_t refid;
        uint64_t reference_timestamp;
        uint64_t origin_timestamp;
        uint64_t receive_timestamp;
        uint64_t transmit_timestamp;
    };
};

/** \brief SNTP消息编码,编码后的msg成员可用
 *
 * \param packet hsntp_packet_t* 包指针
 *
 */
void hsntp_packet_encode(hsntp_packet_t *packet);

/** \brief SNTP消息解码,解码后的除开msg成员的其余成员可用
 *
 * \param packet hsntp_packet_t* 包指针
 *
 */
void hsntp_packet_decode(hsntp_packet_t *packet);

/** \brief 默认包
 *
 * \return hsntp_packet_t 默认包，需要编码后才能发送
 *
 */
hsntp_packet_t hsntp_packet_default(void);

enum
{
    HSNTP_LI_NO_WARNING=0,
    HSNTP_LI_61_SECONDS=1,
    HSNTP_LI_59_SECONDS=2,
    HSNTP_LI_UNKNOWN=3,
};

enum
{
    HSNTP_VN_NTPV4=4,
};

enum
{
    HSNTP_MODE_RESERVED=0,
    HSNTP_MODE_SYMMETRIC_ACTIVE=1,
    HSNTP_MODE_SYMMETRIC_PASSIVE=2,
    HSNTP_MODE_CLIENT=3,
    HSNTP_MODE_SERVER=4,
    HSNTP_MODE_BROADCAST=5,
    HSNTP_MODE_CONTROL=6,
    HSNTP_MODE_PRIVATE=7,
};

enum
{
    HSNTP_STRATUM_UNSPECIFIED=0,
    HSNTP_STRATUM_PRIMARY=1,
    HSNTP_STRATUM_SECONDARY_START=2,
    HSNTP_STRATUM_SECONDARY_END=15,
    HSNTP_STRATUM_UNSYNCHRONIZED=16,
    HSNTP_STRATUM_RESERVED_START=17,
    HSNTP_STRATUM_RESERVED_END=255,
};

struct hsntp_short_format;
typedef struct hsntp_short_format hsntp_short_format_t;
struct hsntp_short_format
{
    uint16_t seconds;
    uint16_t fraction;      /**< 除以2的16次方得到小数位 */
};

/** \brief 短格式解码
 *
 * \param data uint32_t 数据
 * \return hsntp_short_format_t 短格式
 *
 */
hsntp_short_format_t hsntp_short_format_decode(uint32_t data);

/** \brief 短格式编码
 *
 * \param data hsntp_short_format_t 短格式
 * \return uint32_t 数据
 *
 */
uint32_t hsntp_short_format_encode(hsntp_short_format_t data);

struct hsntp_timestamp_format;
typedef struct hsntp_timestamp_format hsntp_timestamp_format_t;
struct hsntp_timestamp_format
{
    uint32_t seconds;
    uint32_t fraction;      /**< 除以2的32次方得到小数位 */
};

/** \brief 时间戳格式解码
 *
 * \param data uint64_t 数据
 * \return hsntp_timestamp_format_t 短格式
 *
 */
hsntp_timestamp_format_t hsntp_timestamp_format_decode(uint64_t data);

/** \brief 时间戳格式编码
 *
 * \param data hsntp_timestamp_format_t 短格式
 * \return uint64_t 数据
 *
 */
uint64_t hsntp_timestamp_format_encode(hsntp_timestamp_format_t data);

struct hsntp_ntp_client;
typedef struct hsntp_ntp_client hsntp_ntp_client_t;
typedef void (*hsntp_ntp_client_sendpacket_t)(const hsntp_ntp_client_t *ntp,const uint8_t *packet,size_t packet_size);
typedef size_t (*hsntp_ntp_client_receivepacket_t)(const hsntp_ntp_client_t *ntp,uint8_t *packet,size_t packet_size);
typedef int (*hsntp_ntp_client_settimeofday_t)(const hsntp_ntp_client_t *ntp,const htimeval_t *tv);
struct hsntp_ntp_client
{
    hsntp_ntp_client_sendpacket_t sendpacket;
    hsntp_ntp_client_receivepacket_t receivepacket;
    hsntp_ntp_client_settimeofday_t  settimeofday;
    uintptr_t usr;
};

/** \brief 初始化ntp
 *
 * \param ntp hsntp_ntp_client_t* NTP客户端
 * \param sendpacket hsntp_ntp_client_sendpacket_t 发送数据包
 * \param receivepacket hsntp_ntp_client_receivepacket_t 接收数据包
 * \param settimeofday hsntp_ntp_client_settimeofday_t 设置时间
 * \param usr void* 用户参数
 *
 */
void hsntp_ntp_client_init(hsntp_ntp_client_t *ntp,hsntp_ntp_client_sendpacket_t sendpacket,hsntp_ntp_client_receivepacket_t receivepacket,hsntp_ntp_client_settimeofday_t  settimeofday,void *usr);

/** \brief NTP发送请求数据包
 *
 * \param ntp hsntp_ntp_client_t* NTP客户端
 * \param packet hsntp_packet_t* 数据包(未编码)指针，为NULL发送默认数据包,注意：内部会编解码数据包,可能修改数据包内容
 * \param packet_size size_t 数据包长度
 * \return bool 是否成功
 *
 */
bool hsntp_ntp_client_sendpacket(hsntp_ntp_client_t *ntp,hsntp_packet_t *packet,size_t packet_size);

/** \brief NTP接收数据包
 *
 * \param ntp hsntp_ntp_client_t*
 * \param packet const uint8_t* 数据指针,为NULL时调用内部函数接收
 * \param packet_size size_t 数据包长度
 * \return bool 是否成功
 *
 */
bool hsntp_ntp_client_receivepacket(hsntp_ntp_client_t *ntp,const uint8_t *packet,size_t packet_size);



#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HSNTP_H__
