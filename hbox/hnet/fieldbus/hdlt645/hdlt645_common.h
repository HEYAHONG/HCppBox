/***************************************************************
 * Name:      hdlt645_common.h
 * Purpose:   声明hdlt645_common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-08-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDLT645_COMMON_H_INCLUDED__
#define __HDLT645_COMMON_H_INCLUDED__
#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"


#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 * 帧 : 帧起始符(1字节)+地址域（6字节）+ 帧起始符(1字节) + 控制码(1字节) + 数据域长度(1字节) + 数据域(数据域长度) + 校验码(1字节) + 帧结束符(1字节)
 */


/*
 * 帧前导，用于唤醒从机或者同步波特率(某些MCU的串口自动波特率可通过发送7个1实现)
 * 注意：本组件中前导不应当出现在接收的数据帧中或者发送的数据帧中，需要用户自行在数据接收中去除或者在数据发送时添加。
 */
#define HDLT645_FRAME_PREAMBLE              (0xFE)

/*
 * 帧起始符
 */
#define HDLT645_FRAME_SOF                   (0x68)

/*
 * 地址（低字节在前）
 */
typedef struct
{
    uint8_t A[6];
} hdlt645_bcd_addr_t;

/** \brief 地址是否匹配
 *
 * \param addr_src hdlt645_bcd_addr_t* 源地址
 * \param addr_dst hdlt645_bcd_addr_t* 目标地址
 * \return bool 是否成功匹配
 *
 */
bool hdlt645_bcd_addr_match(hdlt645_bcd_addr_t *addr_src,hdlt645_bcd_addr_t *addr_dst);


/** \brief 设置地址
 *
 * \param addr_src dlt645_bcd_addr_t* 地址
 * \param bcd_addr uint64_t BCD码表示的地址
 *
 */
void hdlt645_bcd_addr_set(hdlt645_bcd_addr_t *addr,uint64_t bcd_addr);

/** \brief 获取地址
 *
 * \param addr hdlt645_bcd_addr_t* 地址
 * \return uint64_t BCD码表示的地址
 *
 */
uint64_t hdlt645_bcd_addr_get(hdlt645_bcd_addr_t *addr);


/*
 * 地址通配符
 */
#define HDLT645_FRAME_ADDR_WILDCARD_BYTE    (0xAA)

/*
 * 广播地址（从机不回复）
 */
#define HDLT645_FRAME_BOARDCAST_BCD_ADDR    (0x999999999999)

/*
 * 任意播地址(从机按照正常的地址匹配,从机使用自身地址正常回复)
 */
#define HDLT645_FRAME_ANYCAST_BCD_ADDR      (0xAAAAAAAAAAAA)

/*
 * 控制码（共1字节）
 */
typedef struct
{
    uint8_t fct:5;              /**< 功能码 */
    uint8_t ext:1;              /**< 后续数据 */
    uint8_t ack:1;              /**< 应答编码 */
    uint8_t dir:1;              /**< 方向 */
} hdlt645_control_t;

/** \brief 控制码解码
 *
 * \param control_code uint8_t 待解码的控制码
 * \return hdlt645_control_t 控制码
 *
 */
hdlt645_control_t hdlt645_control_decode(uint8_t control_code);

/** \brief 控制码编码
 *
 * \param control hdlt645_control_t 控制码
 * \return uint8_t 编码后的控制码
 *
 */
uint8_t hdlt645_control_encode(hdlt645_control_t control);

#define HDLT645_FRAME_CONTROL_DIR_MASK      (0x80)
#define HDLT645_FRAME_CONTROL_DIR_MASTER    (0x00)
#define HDLT645_FRAME_CONTROL_DIR_SLAVE     (0x80)
#define HDLT645_FRAME_CONTROL_ACK_MASK      (0x40)
#define HDLT645_FRAME_CONTROL_ACK_NORMAL    (0x00)
#define HDLT645_FRAME_CONTROL_ACK_ERROR     (0x40)
#define HDLT645_FRAME_CONTROL_EXT_MASK      (0x20)
#define HDLT645_FRAME_CONTROL_EXT_OFF       (0x00)
#define HDLT645_FRAME_CONTROL_EXT_ON        (0x20)
#define HDLT645_FRAME_CONTROL_FCT_MASK      (0x1F)
#define HDLT645_FRAME_CONTROL_FCT_RESERVED  (0b00000)
#define HDLT645_FRAME_CONTROL_FCT_TIME      (0b01000)
#define HDLT645_FRAME_CONTROL_FCT_READ      (0b10001)
#define HDLT645_FRAME_CONTROL_FCT_READEXT   (0b10010)
#define HDLT645_FRAME_CONTROL_FCT_READADDR  (0b10011)
#define HDLT645_FRAME_CONTROL_FCT_WRITE     (0b10100)
#define HDLT645_FRAME_CONTROL_FCT_WRITEADDR (0b10101)
#define HDLT645_FRAME_CONTROL_FCT_FREEZE    (0b10110)
#define HDLT645_FRAME_CONTROL_FCT_CHCOM     (0b10111)
#define HDLT645_FRAME_CONTROL_FCT_CHPASS    (0b11000)
#define HDLT645_FRAME_CONTROL_FCT_CLRMR     (0b11001)
#define HDLT645_FRAME_CONTROL_FCT_CLRENERGY (0b11010)
#define HDLT645_FRAME_CONTROL_FCT_CLREVENT  (0b11011)


/*
 * 数据长度（1个字节），通过此宏定义可得:
 *     最大帧(不含前导)长度=数据长度+12
 *     最大帧(含4字节前导)长度=数据长度+16
 */
#define HDLT645_FRAME_DATALENGTH_MAX_READ       (200)
#define HDLT645_FRAME_DATALENGTH_MAX_WRITE      (50)

typedef struct
{
    uint8_t di[4];
} hdlt645_data_di_t;


/** \brief 设置数据标识
 *
 * \param di hdlt645_data_di_t* 数据标识
 * \param di_num uint32_t 数据标识
 *
 */
void hdlt645_data_di_set(hdlt645_data_di_t *di,uint32_t di_num);


/** \brief 读取数据标识
 *
 * \param di const hdlt645_data_di_t* 数据标识
 * \return uint32_t 数据标识
 *
 */
uint32_t hdlt645_data_di_get(const hdlt645_data_di_t *di);

typedef struct
{
    uint8_t pa;     /**< 操作权限 */
    uint8_t p0;     /**< 密码 */
    uint8_t p1;     /**< 密码 */
    uint8_t p2;     /**< 密码 */
} hdlt645_data_p_t;

/** \brief 设置密码
 *
 * \param p hdlt645_data_p_t* 密码
 * \param p_num uint32_t 密码
 *
 */
void hdlt645_data_p_set(hdlt645_data_p_t *p,uint32_t p_num);


/** \brief 读取密码
 *
 * \param p const hdlt645_data_p_t* 密码
 * \return uint32_t 密码
 *
 */
uint32_t hdlt645_data_p_get(const hdlt645_data_p_t *p);

typedef struct
{
    uint8_t c[4];
} hdlt645_data_c_t;

/** \brief 设置操作者
 *
 * \param c hdlt645_data_c_t* 操作者
 * \param c_num uint32_t 操作者
 *
 */
void hdlt645_data_c_set(hdlt645_data_c_t *c,uint32_t c_num);


/** \brief 读取操作者
 *
 * \param c const hdlt645_data_c_t* 操作者
 * \return uint32_t 操作者
 *
 */
uint32_t hdlt645_data_c_get(const hdlt645_data_c_t *c);


/** \brief 数据域数据打包(避免大量0对通信的影响)
 *
 * \param data uint8_t* 待打包的数据
 * \param datalen size_t 待打包的数据长度
 *
 */
void hdlt645_data_pack(uint8_t *data,size_t datalen);

/** \brief 数据域数据解包(避免大量0对通信的影响)
 *
 * \param data uint8_t* 待解包的数据
 * \param datalen size_t 待解包的数据长度
 *
 */
void hdlt645_data_unpack(uint8_t *data,size_t datalen);


/** \brief 校验和计算
 *
 * \param frame uint8_t* 帧指针,不含前导（计算校验时，数据域必须处于打包状态）
 * \param frame_check_len size_t 需要检查的长度（一般从帧起始到校验码前）
 * \return uint8_t 校验和
 *
 */
uint8_t hdlt645_checksum_calculate(const uint8_t *frame,size_t frame_check_len);


/*
 * 帧结束符
 */
#define HDLT645_FRAME_EOF               (0x16)



#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HDLT645_COMMON_H_INCLUDED
