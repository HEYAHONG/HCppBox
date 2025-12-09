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


typedef uint8_t hcrc_crc8_context_t;        /**< crc8上下文 */

/** \brief CRC8开始
 *
 * \param config const hcrc_crc8_t* CRC8配置（注意:开始、更新、完成需要使用同一个配置）
 * \return hcrc_crc8_context_t CRC8上下文
 *
 */
hcrc_crc8_context_t hcrc_crc8_starts(const hcrc_crc8_t *config);


/** \brief CRC8更新
 *
 * \param config const hcrc_crc8_t* CRC8配置（注意:开始、更新、完成需要使用同一个配置）
 * \param ctx hcrc_crc8_context_t  CRC8上下文
 * \param data const uint8_t* 数据
 * \param datalen size_t 数据长度
 * \return hcrc_crc8_context_t 更新后的上下文
 *
 */
hcrc_crc8_context_t hcrc_crc8_update(const hcrc_crc8_t *config,hcrc_crc8_context_t ctx,const uint8_t *data,size_t datalen);


/** \brief CRC8完成
 *
 * \param config const hcrc_crc8_t* CRC8配置（注意:开始、更新、完成需要使用同一个配置）
 * \param ctx hcrc_crc8_context_t CRC8上下文
 * \return uint8_t CRC8
 *
 */
uint8_t hcrc_crc8_finish(const hcrc_crc8_t *config,hcrc_crc8_context_t ctx);

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


/** \brief CRC8开始(查表法)
 *
 * \return hcrc_crc8_context_t CRC8上下文
 *
 */
hcrc_crc8_context_t hcrc_crc8_fast_starts(void);


/** \brief CRC8更新(查表法)
 *
 * \param ctx hcrc_crc8_context_t  CRC8上下文
 * \param data const uint8_t* 数据
 * \param datalen size_t 数据长度
 * \return hcrc_crc8_context_t 更新后的上下文
 *
 */
hcrc_crc8_context_t hcrc_crc8_fast_update(hcrc_crc8_context_t ctx,const uint8_t *data,size_t datalen);


/** \brief CRC8完成(查表法)
 *
 * \param ctx hcrc_crc8_context_t CRC8上下文
 * \return uint8_t CRC8
 *
 */
uint8_t hcrc_crc8_fast_finish(hcrc_crc8_context_t ctx);

/** \brief crc8 计算(查表法)
 *
 * \param data const uint8_t* 数据指针
 * \param datalen size_t 数据长度
 * \return uint8_t 校验结果
 *
 */
uint8_t hcrc_crc8_fast_calculate(const uint8_t *data,size_t datalen);

/** \brief crc8 校验（查表法）
 *
 * \param data const uint8_t* 数据指针
 * \param datalen size_t 数据长度
 * \param check uint8_t 校验值
 * \return bool 是否校验成功
 *
 */
bool hcrc_crc8_fast_check(const uint8_t *data,size_t datalen,uint8_t check);

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

typedef uint16_t hcrc_crc16_context_t;        /**< crc16上下文 */

/** \brief CRC16开始
 *
 * \param config const hcrc_crc16_t* CRC16配置（注意:开始、更新、完成需要使用同一个配置）
 * \return hcrc_crc16_context_t CRC16上下文
 *
 */
hcrc_crc16_context_t hcrc_crc16_starts(const hcrc_crc16_t *config);


/** \brief CRC16更新
 *
 * \param config const hcrc_crc16_t* CRC16配置（注意:开始、更新、完成需要使用同一个配置）
 * \param ctx hcrc_crc16_context_t  CRC16上下文
 * \param data const uint16_t* 数据
 * \param datalen size_t 数据长度
 * \return hcrc_crc16_context_t 更新后的上下文
 *
 */
hcrc_crc16_context_t hcrc_crc16_update(const hcrc_crc16_t *config,hcrc_crc16_context_t ctx,const uint8_t *data,size_t datalen);


/** \brief CRC16完成
 *
 * \param config const hcrc_crc16_t* CRC16配置（注意:开始、更新、完成需要使用同一个配置）
 * \param ctx hcrc_crc16_context_t CRC16上下文
 * \return uint16_t CRC16
 *
 */
uint16_t hcrc_crc16_finish(const hcrc_crc16_t *config,hcrc_crc16_context_t ctx);

/** \brief crc16 计算
 *
 * \param config const hcrc_crc16_t * CRC16配置
 * \param data const uint8_t* 数据指针
 * \param datalen size_t 数据长度
 * \return uint16_t 校验结果
 *
 */
uint16_t hcrc_crc16_calculate(const hcrc_crc16_t *config,const uint8_t *data,size_t datalen);

/** \brief crc16 校验
 *
 * \param config const hcrc_crc16_t * CRC16配置
 * \param data const uint8_t* 数据指针
 * \param datalen size_t 数据长度
 * \param check uint16_t 校验值
 * \return bool 是否校验成功
 *
 */
bool hcrc_crc16_check(const hcrc_crc16_t *config,const uint8_t *data,size_t datalen,uint16_t check);


/** \brief CRC16/modbus开始(查表法)
 *
 * \return hcrc_crc16_context_t CRC16上下文
 *
 */
hcrc_crc16_context_t hcrc_crc16_modbus_starts(void);


/** \brief CRC16/modbus更新(查表法)
 *
 * \param ctx hcrc_crc16_context_t  CRC16上下文
 * \param data const uint16_t* 数据
 * \param datalen size_t 数据长度
 * \return hcrc_crc16_context_t 更新后的上下文
 *
 */
hcrc_crc16_context_t hcrc_crc16_modbus_update(hcrc_crc16_context_t ctx,const uint8_t *data,size_t datalen);


/** \brief CRC16/modbus完成(查表法)
 *
 * \param ctx hcrc_crc16_context_t CRC16上下文
 * \return uint16_t CRC16
 *
 */
uint16_t hcrc_crc16_modbus_finish(hcrc_crc16_context_t ctx);

/** \brief crc16/modbus 计算(查表法)
 *
 * \param data const uint8_t* 数据指针
 * \param datalen size_t 数据长度
 * \return uint16_t 校验结果
 *
 */
uint16_t hcrc_crc16_modbus_calculate(const uint8_t *data,size_t datalen);

/** \brief crc16/modbus  校验(查表法)
 *
 * \param data const uint8_t* 数据指针
 * \param datalen size_t 数据长度
 * \param check uint16_t 校验值
 * \return bool 是否校验成功
 *
 */
bool hcrc_crc16_modbus_check(const uint8_t *data,size_t datalen,uint16_t check);

typedef struct hcrc_crc32 hcrc_crc32_t;
struct hcrc_crc32
{
    uint32_t init;      /**< 初始值 */
    uint32_t poly;      /**< 表达式，不包含表达式最高位 */
    uint32_t xorout;    /**< 输出结果值异或 */
    bool    refin;      /**< 输入数据是否按位反转 */
    bool    refout;     /**< 输出结果是否按位反转（在异或之前） */
};

extern const hcrc_crc32_t hcrc_crc32_default; /**< 默认CRC32校验 */
extern const hcrc_crc32_t hcrc_crc32_mpeg_2;  /**< CRC32/MPEG-2 */
extern const hcrc_crc32_t hcrc_crc32_cksum;   /**< CRC32/CKSUM,Coreutils的CKSUM使用的CRC32 */

typedef uint32_t hcrc_crc32_context_t;        /**< crc32上下文 */

/** \brief CRC32开始
 *
 * \param config const hcrc_crc32_t* CRC32配置（注意:开始、更新、完成需要使用同一个配置）
 * \return hcrc_crc32_context_t CRC32上下文
 *
 */
hcrc_crc32_context_t hcrc_crc32_starts(const hcrc_crc32_t *config);


/** \brief CRC32更新
 *
 * \param config const hcrc_crc32_t* CRC32配置（注意:开始、更新、完成需要使用同一个配置）
 * \param ctx hcrc_crc32_context_t  CRC32上下文
 * \param data const uint32_t* 数据
 * \param datalen size_t 数据长度
 * \return hcrc_crc32_context_t 更新后的上下文
 *
 */
hcrc_crc32_context_t hcrc_crc32_update(const hcrc_crc32_t *config,hcrc_crc32_context_t ctx,const uint8_t *data,size_t datalen);


/** \brief CRC32完成
 *
 * \param config const hcrc_crc32_t* CRC32配置（注意:开始、更新、完成需要使用同一个配置）
 * \param ctx hcrc_crc32_context_t CRC32上下文
 * \return uint32_t CRC32
 *
 */
uint32_t hcrc_crc32_finish(const hcrc_crc32_t *config,hcrc_crc32_context_t ctx);

/** \brief crc32 计算
 *
 * \param config const hcrc_crc32_t * CRC32配置
 * \param data const uint8_t* 数据指针
 * \param datalen size_t 数据长度
 * \return uint32_t 校验结果
 *
 */
uint32_t hcrc_crc32_calculate(const hcrc_crc32_t *config,const uint8_t *data,size_t datalen);

/** \brief crc32 校验
 *
 * \param config const hcrc_crc32_t * CRC32配置
 * \param data const uint8_t* 数据指针
 * \param datalen size_t 数据长度
 * \param check uint32_t 校验值
 * \return bool 是否校验成功
 *
 */
bool hcrc_crc32_check(const hcrc_crc32_t *config,const uint8_t *data,size_t datalen,uint32_t check);

/** \brief CRC32开始(查表法)
 *
 * \return hcrc_crc32_context_t CRC32上下文
 *
 */
hcrc_crc32_context_t hcrc_crc32_fast_starts(void);


/** \brief CRC32更新(查表法)
 *
 * \param ctx hcrc_crc32_context_t  CRC32上下文
 * \param data const uint32_t* 数据
 * \param datalen size_t 数据长度
 * \return hcrc_crc32_context_t 更新后的上下文
 *
 */
hcrc_crc32_context_t hcrc_crc32_fast_update(hcrc_crc32_context_t ctx,const uint8_t *data,size_t datalen);


/** \brief CRC32完成(查表法)
 *
 * \param ctx hcrc_crc32_context_t CRC32上下文
 * \return uint32_t CRC32
 *
 */
uint32_t hcrc_crc32_fast_finish(hcrc_crc32_context_t ctx);


/** \brief crc32 计算(查表法)
 *
 * \param data const uint8_t* 数据指针
 * \param datalen size_t 数据长度
 * \return uint32_t 校验结果
 *
 */
uint32_t hcrc_crc32_fast_calculate(const uint8_t *data,size_t datalen);

/** \brief crc32 校验(查表法)
 *
 * \param data const uint8_t* 数据指针
 * \param datalen size_t 数据长度
 * \param check uint32_t 校验值
 * \return bool 是否校验成功
 *
 */
bool hcrc_crc32_fast_check(const uint8_t *data,size_t datalen,uint32_t check);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HCRC_H__
