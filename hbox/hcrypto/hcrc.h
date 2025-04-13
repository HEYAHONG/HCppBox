/***************************************************************
 * Name:      hcrc.h
 * Purpose:   声明hcrc接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-13
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCRC_H__
#define __HCRC_H__
#include "stdint.h"
#include "stdbool.h"
/*
 * CRC是一种常用的校验方式
 */
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct hcrc_crc8 hcrc_crc8_t;
struct hcrc_crc8
{
    uint8_t init;       /**< 初始值 */
    uint8_t poly;       /**< 表达式，不包含表达式最高位 */
    uint8_t xorout;     /**< 输出结果值异或 */
    bool    refin;      /**< 输入数据是否按位反转 */
    bool    refout;     /**< 输出结果是否按位反转（在异或之前） */
};

extern const hcrc_crc8_t  hcrc_crc8_default;    /**< 默认CRC8校验 */
extern const hcrc_crc8_t  hcrc_crc8_itu;        /**< CRC8/ITU */
extern const hcrc_crc8_t  hcrc_crc8_rohc;       /**< CRC8/ROHC */
extern const hcrc_crc8_t  hcrc_crc8_maxim;      /**< CRC8/MAXIM */

/** \brief crc8 计算
 *
 * \param config const hcrc_crc8_t * CRC8配置
 * \param data const uint8_t* 数据指针
 * \param datalen size_t 数据长度
 * \return uint8_t 校验结果
 *
 */
uint8_t hcrc_crc8_calculate(const hcrc_crc8_t *config,const uint8_t *data,size_t datalen);

/** \brief crc8 校验
 *
 * \param config const hcrc_crc8_t * CRC8配置
 * \param data const uint8_t* 数据指针
 * \param datalen size_t 数据长度
 * \param check uint8_t 校验值
 * \return bool 是否校验成功
 *
 */
bool hcrc_crc8_check(const hcrc_crc8_t *config,const uint8_t *data,size_t datalen,uint8_t check);

typedef struct hcrc_crc16 hcrc_crc16_t;
struct hcrc_crc16
{
    uint16_t init;      /**< 初始值 */
    uint16_t poly;      /**< 表达式，不包含表达式最高位 */
    uint16_t xorout;    /**< 输出结果值异或 */
    bool    refin;      /**< 输入数据是否按位反转 */
    bool    refout;     /**< 输出结果是否按位反转（在异或之前） */
};

extern const hcrc_crc16_t  hcrc_crc16_ibm;          /**< CRC16/IBM */
extern const hcrc_crc16_t  hcrc_crc16_maxim;        /**< CRC16/MAXIM */
extern const hcrc_crc16_t  hcrc_crc16_usb;          /**< CRC16/USB */
extern const hcrc_crc16_t  hcrc_crc16_modbus;       /**< CRC16/MODBUS */
extern const hcrc_crc16_t  hcrc_crc16_ccitt;        /**< CRC16/CCITT */
extern const hcrc_crc16_t  hcrc_crc16_ccitt_false;  /**< CRC16/CCITT_FALSE */
extern const hcrc_crc16_t  hcrc_crc16_x25;          /**< CRC16/X25 */
extern const hcrc_crc16_t  hcrc_crc16_xmodem;       /**< CRC16/XMODEM */
extern const hcrc_crc16_t  hcrc_crc16_dnp;          /**< CRC16/DNP */

/** \brief crc16 计算
 *
 * \param config const hcrc_crc16_t * CRC8配置
 * \param data const uint8_t* 数据指针
 * \param datalen size_t 数据长度
 * \return uint16_t 校验结果
 *
 */
uint16_t hcrc_crc16_calculate(const hcrc_crc16_t *config,const uint8_t *data,size_t datalen);

/** \brief crc16 校验
 *
 * \param config const hcrc_crc16_t * CRC8配置
 * \param data const uint8_t* 数据指针
 * \param datalen size_t 数据长度
 * \param check uint16_t 校验值
 * \return bool 是否校验成功
 *
 */
bool hcrc_crc16_check(const hcrc_crc16_t *config,const uint8_t *data,size_t datalen,uint16_t check);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HCRC_H__
