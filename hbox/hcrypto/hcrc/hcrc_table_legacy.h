/***************************************************************
 * Name:      hcrc_table_legacy.h
 * Purpose:   声明hcrc接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-12-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCRC_TABLE_LEGACY_H__
#define __HCRC_TABLE_LEGACY_H__
#include "hcrc_base.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


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


#endif // __HCRC_TABLE_LEGACY_H__
