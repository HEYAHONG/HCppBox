/***************************************************************
 * Name:      hasn1_ber.h
 * Purpose:   声明hasn1_ber接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-25
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HASN1_BER_H__
#define __HASN1_BER_H__
#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 *  BER传输TLV三元组<Type,Length, Value>，其中三元组内部采用大端模式。
 */

typedef enum
{
    HASN1_BER_TYPE_CLASS_UNIVERSAL              = 0<<6,
    HASN1_BER_TYPE_CLASS_APPLICATION            = 1<<6,
    HASN1_BER_TYPE_CLASS_CONTEXT_SPECIFIC       = 2<<6,
    HASN1_BER_TYPE_CLASS_PRIVATE                = 3<<6
}   hasn1_ber_type_class_t;  /**< 类，一般占用类型字节(第1个)的[6,7] */

typedef enum
{
    HASN1_BER_TYPE_PRIMITIVE                    = 0 << 5,
    HASN1_BER_TYPE_CONSTRUCTED                  = 1 << 5
}   hasn1_ber_type_p_c_t; /**< 结构类型，一般占用类型字节(第1个)的位5 */

typedef enum
{
    HASN1_BER_TYPE_TAG_BER_RESEVED_0            = 0,
    HASN1_BER_TYPE_TAG_BOOLEAN                  = 1,
    HASN1_BER_TYPE_TAG_INTEGER                  = 2,
    HASN1_BER_TYPE_TAG_BITSTRING                = 3,
    HASN1_BER_TYPE_TAG_OCTETSTRING              = 4,
    HASN1_BER_TYPE_TAG_NULL                     = 5,
    HASN1_BER_TYPE_TAG_IDENTIFIER               = 6,
    HASN1_BER_TYPE_TAG_DESCRIPTION              = 7,
    HASN1_BER_TYPE_TAG_EXTERNAL                 = 8,
    HASN1_BER_TYPE_TAG_REAL                     = 9,
    HASN1_BER_TYPE_TAG_ENUMERATED               = 10,
    HASN1_BER_TYPE_TAG_EMBEDDED_PDV             = 11,
    HASN1_BER_TYPE_TAG_UTF8STRING               = 12,
    HASN1_BER_TYPE_TAG_RELATIVE_OID             = 13,
    HASN1_BER_TYPE_TAG_RESEVED_14               = 14,
    HASN1_BER_TYPE_TAG_RESEVED_15               = 15,
    HASN1_BER_TYPE_TAG_SEQUENCE                 = 16,       /**<  一般配合HASN1_BER_TYPE_CONSTRUCTED使用*/
    HASN1_BER_TYPE_TAG_SET                      = 17,       /**<  一般配合HASN1_BER_TYPE_CONSTRUCTED使用*/
    HASN1_BER_TYPE_TAG_NUMERICSTRING            = 18,
    HASN1_BER_TYPE_TAG_PRINTABLESTRING          = 19,
    HASN1_BER_TYPE_TAG_TELETEXSTRING            = 20,
    HASN1_BER_TYPE_TAG_VIDEOTEXSTRING           = 21,
    HASN1_BER_TYPE_TAG_IA5STRING                = 22,
    HASN1_BER_TYPE_TAG_UTC                      = 23,
    HASN1_BER_TYPE_TAG_GENERALIZEDTIME          = 24,
    HASN1_BER_TYPE_TAG_GRAPHICSTRING            = 25,
    HASN1_BER_TYPE_TAG_VISIBLESSTRING           = 26,
    HASN1_BER_TYPE_TAG_GENERALSTRING            = 27,
    HASN1_BER_TYPE_TAG_UNIVERSALSTRING          = 28,
    HASN1_BER_TYPE_TAG_CHARACTER_STRING         = 29,
    HASN1_BER_TYPE_TAG_BMPSTRING                = 30,
    HASN1_BER_TYPE_TAG_RESEVED_31               = 31,
    HASN1_BER_TYPE_TAG_FOLLOW_BYTES             = 31        /**< 标签将在紧接的字节给出，直到字节高位为0的字节结束 */
}   hasn1_ber_type_base_tag_t; /**< 标签,低5位.此枚举通常用于通用类 */

#ifndef HASN1_BER_TYPE_TAG_MAX_LENGTH
#define HASN1_BER_TYPE_TAG_MAX_LENGTH 10
#endif // HASN1_BER_TYPE_TAG_MAX_LENGTH

typedef struct hasn1_ber_type hasn1_ber_type_t;
struct hasn1_ber_type
{
    uint8_t type;                                   /**< 类型 */
    uint8_t tag[HASN1_BER_TYPE_TAG_MAX_LENGTH];     /**< TAG,编码后的TAG。即结尾字节的最高位为0，其余字节的最高位为1 */
};

/** \brief 获取类
 *
 * \param type const hasn1_ber_type_t* 类型
 * \return hasn1_ber_type_class_t 类
 *
 */
hasn1_ber_type_class_t hasn1_ber_type_class_get(const hasn1_ber_type_t *type);

/** \brief 设置类
 *
 * \param type const hasn1_ber_type_t* 类型
 * \param type_class hasn1_ber_type_class_t 类
 *
 */
void hasn1_ber_type_class_set(hasn1_ber_type_t *type,const hasn1_ber_type_class_t type_class);


/** \brief 获取P_C
 *
 * \param type const hasn1_ber_type_t* 类型
 * \return hasn1_ber_type_p_c_t P_C
 *
 */
hasn1_ber_type_p_c_t hasn1_ber_type_p_c_get(const hasn1_ber_type_t *type);

/** \brief 设置P_C
 *
 * \param type const hasn1_ber_type_t* 类型
 * \param type_class hasn1_ber_type_p_c_t P_C
 *
 */
void hasn1_ber_type_p_c_set(hasn1_ber_type_t *type,const hasn1_ber_type_p_c_t type_p_c);

/** \brief 获取TAG
 *
 * \param type const hasn1_ber_type_t*  类型
 * \return uint64_t tag
 *
 */
uint64_t hasn1_ber_type_tag_get(const hasn1_ber_type_t *type);

/** \brief 设置TAG
 *
 * \param type const hasn1_ber_type_t* 类型
 * \param tag uint64_t tag
 *
 */
void hasn1_ber_type_tag_set(hasn1_ber_type_t *type,uint64_t tag);

/** \brief 从数据中提取类型
 *
 * \param type hasn1_ber_type_t*            类型
 * \param data const uint8_t*               数据（按照TLV三元组存放的数据）
 * \param data_length size_t                数据长度。
 * \return size_t 类型所占长度，0表示出错。
 *
 */
size_t hasn1_ber_type_get(hasn1_ber_type_t *type,const uint8_t *data,size_t data_length);

/** \brief 向数据中设置类型
 *
 * \param type const hasn1_ber_type_t*      类型
 * \param data uint8_t*                     数据（按照TLV三元组存放的数据）
 * \param data_length size_t                数据长度。
 * \return size_t 类型所占长度，0表示出错。
 *
 */
size_t hasn1_ber_type_set(const hasn1_ber_type_t *type,uint8_t *data,size_t data_length);

typedef struct  hasn1_ber_length hasn1_ber_length_t;
struct hasn1_ber_length
{
    uint64_t length;  /**< 本库默认支持64位长度,理论上BER支持更长的长度 */
};

/** \brief 从数据中提取长度
 *
 * \param length hasn1_ber_length_t*    长度
 * \param data const uint8_t*           数据（按照TLV三元组存放的数据）
 * \param data_length size_t            数据长度。
 * \return size_t 长度所占长度，0表示出错。
 *
 */
size_t hasn1_ber_length_get( hasn1_ber_length_t *length,const uint8_t *data,size_t data_length);


/** \brief 向数据中设置长度（需要先设置好类型）
 *
 * \param length const hasn1_ber_length_t*  长度(为NULL时表示动态长度)
 * \param data uint8_t*                     数据（按照TLV三元组存放的数据）
 * \param data_length size_t                数据长度。
 * \return size_t  长度所占长度，0表示出错。
 *
 */
size_t hasn1_ber_length_set(const hasn1_ber_length_t *length,uint8_t *data,size_t data_length);

typedef struct  hasn1_ber_value hasn1_ber_value_t;
struct  hasn1_ber_value
{
    const uint8_t *value;           /**< 值地址 */
    uint64_t length;                /**< 值长度,本库默认支持64位长度,理论上BER支持更长的长度 */
};

/** \brief 从数据中提取值
 *
 * \param length  hasn1_ber_value_t*    值
 * \param data const uint8_t*           数据（按照TLV三元组存放的数据）
 * \param data_length size_t            数据长度。
 * \return size_t 值所占长度（若为动态长度，包含末尾的两个00），0表示出错。
 *
 */
size_t hasn1_ber_value_get(hasn1_ber_value_t *value,const uint8_t *data,size_t data_length);


/** \brief 向数据中设置值（需要先设置好类型）
 *
 * \param length const hasn1_ber_value_t*   值(为NULL时表示动态长度)
 * \param data uint8_t*                     数据（按照TLV三元组存放的数据）
 * \param data_length size_t                数据长度。
 * \return size_t  长度所占长度（若为动态长度，包含末尾的两个00），0表示出错。
 *
 */
size_t hasn1_ber_value_set(const hasn1_ber_value_t *value,uint8_t *data,size_t data_length);


/** \brief 解析回调
 *
 * \param usr void*                             用户参数
 * \param depth size_t                          深度
 * \param index size_t                          当前层级的引索
 * \param type const hasn1_ber_type_t*          类型
 * \param value const hasn1_ber_value_t*        值
 *
 */
typedef void (*hasn1_ber_parse_callback_t)(void *usr,size_t depth,size_t index,const hasn1_ber_type_t *type,const hasn1_ber_value_t *value);


/** \brief HASN.1解析
 *
 * \param callback hasn1_ber_parse_callback_t   回调
 * \param usr void*                             用户参数
 * \param depth size_t                          当前解析的层级，用户一般传0
 * \param data const uint8_t*                   数据（按照TLV三元组存放的数据）
 * \param data_length size_t                    数据长度。
 * \return bool 是否解析成功
 *
 */
bool hasn1_ber_parse(hasn1_ber_parse_callback_t callback,void *usr,size_t depth,const uint8_t *data,size_t data_length);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HASN1_BER_H__
