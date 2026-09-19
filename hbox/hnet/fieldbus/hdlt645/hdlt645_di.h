/***************************************************************
 * Name:      hdlt645_di.h
 * Purpose:   声明hdlt645_di接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-09-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDLT645_DI_H_INCLUDED__
#define __HDLT645_DI_H_INCLUDED__

#include "hdlt645_common.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


typedef enum
{
    HDLT645_DI_DATA_DESC_TYPE_XXXXXX_XX=0,
    HDLT645_DI_DATA_DESC_TYPE_XXXXXX_XX_SIGNED,
} hdlt645_di_data_desc_type_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_XXXXXX_XX
 */
typedef struct
{
    double data;
} hdlt645_di_data_desc_type_xxxxxx_xx_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_XXXXXX_XX_SIGNED
 */
typedef struct
{
    double data;
} hdlt645_di_data_desc_type_xxxxxx_xx_signed_t;

struct hdlt645_di_data_desc;
typedef struct hdlt645_di_data_desc hdlt645_di_data_desc_t;
struct hdlt645_di_data_desc
{
    const char *format;                                                                                                 /**< 格式自字符串 */
    size_t data_length;                                                                                                 /**< 数据存储长度 */
    hdlt645_di_data_desc_type_t type;                                                                                   /**< 数据对应的格式 */
    bool (*set_data)(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data);           /**< 设置数据 */
    bool (*get_data)(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data);           /**< 读取数据 */
};

/** \brief 读取数据标识数据描述
 *
 * \param di hdlt645_data_di_t* 数据标识
 * \return const hdlt645_di_data_desc_t* 数据标识描述,失败返回NULL
 *
 */
const hdlt645_di_data_desc_t * hdlt645_di_data_desc_get(hdlt645_data_di_t *di);


/** \brief 读取数据标识数据描述
 *
 * \param di_num uint32_t 数据标识
 * \return const hdlt645_di_data_desc_t* 数据标识描述,失败返回NULL
 *
 */
const hdlt645_di_data_desc_t * hdlt645_di_data_desc_get2(uint32_t di_num);


/** \brief 读取数据标识数据描述
 *
 * \return const hdlt645_di_data_desc_t** 数据标识描述数组指针，以NULL结尾
 *
 */
const hdlt645_di_data_desc_t *const * hdlt645_di_data_desc_get_all(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HDLT645_DI_H_INCLUDED
