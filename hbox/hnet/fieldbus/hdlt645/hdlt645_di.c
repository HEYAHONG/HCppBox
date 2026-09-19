/***************************************************************
 * Name:      hdlt645_di.c
 * Purpose:   实现hdlt645_di接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-09-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include "hdlt645_di.h"
#include "hdlt645_utils.h"

static bool hdlt645_di_data_desc_xxxxxx_xx_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    const hdlt645_di_data_desc_type_xxxxxx_xx_t *dataptr=(const hdlt645_di_data_desc_type_xxxxxx_xx_t *)data;

    double val=dataptr->data;

    if(val < 0)
    {
        val = -val;
    }

    /*
     * 两位小数
     */
    val *= 100;

    hdlt645_bcd_le_set(buffer,buffer_size,((uint64_t)val)%100000000);

    return true;
};
static bool hdlt645_di_data_desc_xxxxxx_xx_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;


    hdlt645_di_data_desc_type_xxxxxx_xx_t *dataptr=(hdlt645_di_data_desc_type_xxxxxx_xx_t *)data;

    uint64_t val=hdlt645_bcd_le_get(buffer,buffer_size);

    dataptr->data=val;

    /*
     * 两位小数
     */
    dataptr->data /= 100.0;


    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_xxxxxx_xx=
{
    "XXXXXX.XX",
    4,
    HDLT645_DI_DATA_DESC_TYPE_XXXXXX_XX,
    hdlt645_di_data_desc_xxxxxx_xx_set_data,
    hdlt645_di_data_desc_xxxxxx_xx_get_data
};


static bool hdlt645_di_data_desc_xxxxxx_xx_signed_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    bool data_signed=false;

    const hdlt645_di_data_desc_type_xxxxxx_xx_signed_t *dataptr=(const hdlt645_di_data_desc_type_xxxxxx_xx_signed_t *)data;
    double val=dataptr->data;

    if(val < 0)
    {
        val = -val;
        data_signed=true;
    }

    /*
     * 两位小数
     */
    val *= 100;

    hdlt645_bcd_le_set(buffer,buffer_size,((uint64_t)val)%80000000);

    if(data_signed)
    {
        buffer[3] |= 0x80;
    }

    return true;
};
static bool hdlt645_di_data_desc_xxxxxx_xx_signed_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    bool data_signed=false;
    if(buffer[3] & 0x80)
    {
        data_signed=true;
    }

    hdlt645_di_data_desc_type_xxxxxx_xx_signed_t *dataptr=(hdlt645_di_data_desc_type_xxxxxx_xx_signed_t *)data;
    uint8_t val_buffer[4]= {0};
    memcpy(val_buffer,buffer,sizeof(val_buffer));

    val_buffer[3] &= (~(0x80));

    uint64_t val=hdlt645_bcd_le_get(val_buffer,sizeof(val_buffer));

    dataptr->data=val;

    /*
     * 两位小数
     */
    dataptr->data /= 100.0;

    if(data_signed)
    {
        dataptr->data = -dataptr->data;
    }

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_xxxxxx_xx_signed=
{
    "XXXXXX.XX",
    4,
    HDLT645_DI_DATA_DESC_TYPE_XXXXXX_XX_SIGNED,
    hdlt645_di_data_desc_xxxxxx_xx_signed_set_data,
    hdlt645_di_data_desc_xxxxxx_xx_signed_get_data
};

static bool hdlt645_di_data_desc_xx_xxxx_yymmddhhmm_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    const hdlt645_di_data_desc_type_xx_xxxx_yymmddhhmm_t *dataptr=(const hdlt645_di_data_desc_type_xx_xxxx_yymmddhhmm_t *)data;

    double val=dataptr->val;

    if(val < 0)
    {
        val = -val;
    }

    /*
     * 四位小数
     */
    val *= 10000;

    hdlt645_bcd_le_set(buffer,3,((uint64_t)val)%1000000);

    buffer[3]=hdlt645_uint64_to_bcd(dataptr->YY);
    buffer[4]=hdlt645_uint64_to_bcd(dataptr->MM);
    buffer[5]=hdlt645_uint64_to_bcd(dataptr->DD);
    buffer[6]=hdlt645_uint64_to_bcd(dataptr->HH);
    buffer[7]=hdlt645_uint64_to_bcd(dataptr->mm);

    return true;
};
static bool hdlt645_di_data_desc_xx_xxxx_yymmddhhmm_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;


    hdlt645_di_data_desc_type_xx_xxxx_yymmddhhmm_t *dataptr=(hdlt645_di_data_desc_type_xx_xxxx_yymmddhhmm_t *)data;

    uint64_t val=hdlt645_bcd_le_get(buffer,3);

    dataptr->val=val;

    /*
     * 四位小数
     */
    dataptr->val /= 10000.0;

    dataptr->YY=hdlt645_bcd_to_uint64(buffer[3]);
    dataptr->MM=hdlt645_bcd_to_uint64(buffer[4]);
    dataptr->DD=hdlt645_bcd_to_uint64(buffer[5]);
    dataptr->HH=hdlt645_bcd_to_uint64(buffer[6]);
    dataptr->mm=hdlt645_bcd_to_uint64(buffer[7]);

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_xx_xxxx_yymmddhhmm=
{
    "XX.XXXX" "YYMMDDHHmm",
    8,
    HDLT645_DI_DATA_DESC_TYPE_XX_XXXX_YYMMDDHHMM,
    hdlt645_di_data_desc_xx_xxxx_yymmddhhmm_set_data,
    hdlt645_di_data_desc_xx_xxxx_yymmddhhmm_get_data
};


static bool hdlt645_di_data_desc_xx_xxxx_yymmddhhmm_signed_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    bool data_signed=false;

    const hdlt645_di_data_desc_type_xx_xxxx_yymmddhhmm_signed_t *dataptr=(const hdlt645_di_data_desc_type_xx_xxxx_yymmddhhmm_signed_t *)data;
    double val=dataptr->val;

    if(val < 0)
    {
        val = -val;
        data_signed=true;
    }

    /*
     * 四位小数
     */
    val *= 10000;

    hdlt645_bcd_le_set(buffer,3,((uint64_t)val)%800000);

    if(data_signed)
    {
        buffer[2] |= 0x80;
    }

    buffer[3]=hdlt645_uint64_to_bcd(dataptr->YY);
    buffer[4]=hdlt645_uint64_to_bcd(dataptr->MM);
    buffer[5]=hdlt645_uint64_to_bcd(dataptr->DD);
    buffer[6]=hdlt645_uint64_to_bcd(dataptr->HH);
    buffer[7]=hdlt645_uint64_to_bcd(dataptr->mm);

    return true;
};
static bool hdlt645_di_data_desc_xx_xxxx_yymmddhhmm_signed_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    bool data_signed=false;
    if(buffer[2] & 0x80)
    {
        data_signed=true;
    }

    hdlt645_di_data_desc_type_xx_xxxx_yymmddhhmm_signed_t *dataptr=(hdlt645_di_data_desc_type_xx_xxxx_yymmddhhmm_signed_t *)data;
    uint8_t val_buffer[3]= {0};
    memcpy(val_buffer,buffer,sizeof(val_buffer));

    val_buffer[2] &= (~(0x80));

    uint64_t val=hdlt645_bcd_le_get(val_buffer,sizeof(val_buffer));

    dataptr->val=val;

    /*
     * 四位小数
     */
    dataptr->val /= 10000.0;

    if(data_signed)
    {
        dataptr->val = -dataptr->val;
    }

    dataptr->YY=hdlt645_bcd_to_uint64(buffer[3]);
    dataptr->MM=hdlt645_bcd_to_uint64(buffer[4]);
    dataptr->DD=hdlt645_bcd_to_uint64(buffer[5]);
    dataptr->HH=hdlt645_bcd_to_uint64(buffer[6]);
    dataptr->mm=hdlt645_bcd_to_uint64(buffer[7]);

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_xx_xxxx_yymmddhhmm_signed=
{
    "XX.XXXX" "YYMMDDHHmm",
    8,
    HDLT645_DI_DATA_DESC_TYPE_XX_XXXX_YYMMDDHHMM_SIGNED,
    hdlt645_di_data_desc_xx_xxxx_yymmddhhmm_signed_set_data,
    hdlt645_di_data_desc_xx_xxxx_yymmddhhmm_signed_get_data
};


const hdlt645_di_data_desc_t * hdlt645_di_data_desc_get(hdlt645_data_di_t *di)
{
    const hdlt645_di_data_desc_t *ret=NULL;
    if(di==NULL)
    {
        return ret;
    }

    switch(di->di[3])
    {
    case 0x00:
    {
        /*
         * DL/T 645 A.1 电能量数据标识编码表
         */
        switch(di->di[2])
        {
        case 0x00:  /**< 组合有功 */
        case 0x03:  /**< 组合无功1 */
        case 0x04:  /**< 组合无功2 */
        case 0x17:  /**< A相组合无功1 */
        case 0x18:  /**< A相组合无功2 */
        case 0x2B:  /**< B相组合无功1 */
        case 0x2C:  /**< B相组合无功2 */
        case 0x3F:  /**< C相组合无功1 */
        case 0x40:  /**< C相组合无功2 */
        {
            ret=&hdlt645_di_data_desc_xxxxxx_xx_signed;
        }
        break;
        default:
        {
            ret=&hdlt645_di_data_desc_xxxxxx_xx;
        }
        break;
        }
    }
    break;
    case 0x01:
    {
        /*
         * DL/T 645 A.2 最大需量及发生时间数据标识编码表
         */
        switch(di->di[2])
        {
        case 0x03:  /**< 组合无功1 */
        case 0x04:  /**< 组合无功2 */
        case 0x17:  /**< A相组合无功1 */
        case 0x18:  /**< A相组合无功2 */
        case 0x2B:  /**< B相组合无功1 */
        case 0x2C:  /**< B相组合无功2 */
        case 0x3F:  /**< C相组合无功1 */
        case 0x40:  /**< C相组合无功2 */
        {
            ret=&hdlt645_di_data_desc_xx_xxxx_yymmddhhmm_signed;
        }
        break;
        default:
        {
            ret=&hdlt645_di_data_desc_xx_xxxx_yymmddhhmm;
        }
        break;
        }
    }
    break;
    default:
    {

    }
    break;
    }

    return ret;
}


const hdlt645_di_data_desc_t * hdlt645_di_data_desc_get2(uint32_t di_num)
{
    hdlt645_data_di_t di;
    hdlt645_data_di_set(&di,di_num);
    return hdlt645_di_data_desc_get(&di);
}

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t * const hdlt645_di_data_desc[]=
{
    &hdlt645_di_data_desc_xxxxxx_xx,
    &hdlt645_di_data_desc_xxxxxx_xx_signed,
    &hdlt645_di_data_desc_xx_xxxx_yymmddhhmm,
    &hdlt645_di_data_desc_xx_xxxx_yymmddhhmm_signed,
    NULL
};

const hdlt645_di_data_desc_t * const * hdlt645_di_data_desc_get_all(void)
{
    return hdlt645_di_data_desc;
}

