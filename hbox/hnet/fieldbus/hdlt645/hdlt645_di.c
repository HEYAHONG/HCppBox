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

    hdlt645_bcd_le_set(buffer,buffer_size,((uint64_t)(val + 0.5))%100000000);

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

    hdlt645_bcd_le_set(buffer,buffer_size,((uint64_t)(val + 0.5))%80000000);

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

    hdlt645_bcd_le_set(buffer,3,((uint64_t)(val + 0.5))%1000000);

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

    hdlt645_bcd_le_set(buffer,3,((uint64_t)(val + 0.5))%800000);

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


static bool hdlt645_di_data_desc_xxx_x_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    const hdlt645_di_data_desc_type_xxx_x_t *dataptr=(const hdlt645_di_data_desc_type_xxx_x_t *)data;

    double val=dataptr->data;

    if(val < 0)
    {
        val = -val;
    }

    /*
     * 一位小数
     */
    val *= 10;

    hdlt645_bcd_le_set(buffer,buffer_size,((uint64_t)(val + 0.5))%10000);

    return true;
};
static bool hdlt645_di_data_desc_xxx_x_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;


    hdlt645_di_data_desc_type_xxx_x_t *dataptr=(hdlt645_di_data_desc_type_xxx_x_t *)data;

    uint64_t val=hdlt645_bcd_le_get(buffer,buffer_size);

    dataptr->data=val;

    /*
     * 一位小数
     */
    dataptr->data /= 10.0;


    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_xxx_x=
{
    "XXX.X",
    2,
    HDLT645_DI_DATA_DESC_TYPE_XXX_X,
    hdlt645_di_data_desc_xxx_x_set_data,
    hdlt645_di_data_desc_xxx_x_get_data
};


static bool hdlt645_di_data_desc_xxx_x_signed_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    bool data_signed=false;

    const hdlt645_di_data_desc_type_xxx_x_signed_t *dataptr=(const hdlt645_di_data_desc_type_xxx_x_signed_t *)data;
    double val=dataptr->data;

    if(val < 0)
    {
        val = -val;
        data_signed=true;
    }

    /*
     * 一位小数
     */
    val *= 10;

    hdlt645_bcd_le_set(buffer,buffer_size,((uint64_t)(val + 0.5))%8000);

    if(data_signed)
    {
        buffer[1] |= 0x80;
    }

    return true;
};
static bool hdlt645_di_data_desc_xxx_x_signed_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    bool data_signed=false;
    if(buffer[1] & 0x80)
    {
        data_signed=true;
    }

    hdlt645_di_data_desc_type_xxx_x_signed_t *dataptr=(hdlt645_di_data_desc_type_xxx_x_signed_t *)data;
    uint8_t val_buffer[2]= {0};
    memcpy(val_buffer,buffer,sizeof(val_buffer));

    val_buffer[1] &= (~(0x80));

    uint64_t val=hdlt645_bcd_le_get(val_buffer,sizeof(val_buffer));

    dataptr->data=val;

    /*
     * 一位小数
     */
    dataptr->data /= 10.0;

    if(data_signed)
    {
        dataptr->data = -dataptr->data;
    }

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_xxx_x_signed=
{
    "XXX.X",
    2,
    HDLT645_DI_DATA_DESC_TYPE_XXX_X_SIGNED,
    hdlt645_di_data_desc_xxx_x_signed_set_data,
    hdlt645_di_data_desc_xxx_x_signed_get_data
};

static bool hdlt645_di_data_desc_xx_xx_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    const hdlt645_di_data_desc_type_xx_xx_t *dataptr=(const hdlt645_di_data_desc_type_xx_xx_t *)data;

    double val=dataptr->data;

    if(val < 0)
    {
        val = -val;
    }

    /*
     * 两位小数
     */
    val *= 100;

    hdlt645_bcd_le_set(buffer,buffer_size,((uint64_t)(val + 0.5))%10000);

    return true;
};
static bool hdlt645_di_data_desc_xx_xx_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;


    hdlt645_di_data_desc_type_xx_xx_t *dataptr=(hdlt645_di_data_desc_type_xx_xx_t *)data;

    uint64_t val=hdlt645_bcd_le_get(buffer,buffer_size);

    dataptr->data=val;

    /*
     * 两位小数
     */
    dataptr->data /= 100.0;


    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_xx_xx=
{
    "XX.XX",
    2,
    HDLT645_DI_DATA_DESC_TYPE_XX_XX,
    hdlt645_di_data_desc_xx_xx_set_data,
    hdlt645_di_data_desc_xx_xx_get_data
};


static bool hdlt645_di_data_desc_xx_xx_signed_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    bool data_signed=false;

    const hdlt645_di_data_desc_type_xx_xx_signed_t *dataptr=(const hdlt645_di_data_desc_type_xx_xx_signed_t *)data;
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

    hdlt645_bcd_le_set(buffer,buffer_size,((uint64_t)(val + 0.5))%8000);

    if(data_signed)
    {
        buffer[1] |= 0x80;
    }

    return true;
};
static bool hdlt645_di_data_desc_xx_xx_signed_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    bool data_signed=false;
    if(buffer[1] & 0x80)
    {
        data_signed=true;
    }

    hdlt645_di_data_desc_type_xx_xx_signed_t *dataptr=(hdlt645_di_data_desc_type_xx_xx_signed_t *)data;
    uint8_t val_buffer[2]= {0};
    memcpy(val_buffer,buffer,sizeof(val_buffer));

    val_buffer[1] &= (~(0x80));

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
const hdlt645_di_data_desc_t hdlt645_di_data_desc_xx_xx_signed=
{
    "XX.XX",
    2,
    HDLT645_DI_DATA_DESC_TYPE_XX_XX_SIGNED,
    hdlt645_di_data_desc_xx_xx_signed_set_data,
    hdlt645_di_data_desc_xx_xx_signed_get_data
};

static bool hdlt645_di_data_desc_x_xxx_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    const hdlt645_di_data_desc_type_x_xxx_t *dataptr=(const hdlt645_di_data_desc_type_x_xxx_t *)data;

    double val=dataptr->data;

    if(val < 0)
    {
        val = -val;
    }

    /*
     * 三位小数
     */
    val *= 1000;

    hdlt645_bcd_le_set(buffer,buffer_size,((uint64_t)(val + 0.5))%10000);

    return true;
};
static bool hdlt645_di_data_desc_x_xxx_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;


    hdlt645_di_data_desc_type_x_xxx_t *dataptr=(hdlt645_di_data_desc_type_x_xxx_t *)data;

    uint64_t val=hdlt645_bcd_le_get(buffer,buffer_size);

    dataptr->data=val;

    /*
     * 三位小数
     */
    dataptr->data /= 1000.0;


    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_x_xxx=
{
    "X.XXX",
    2,
    HDLT645_DI_DATA_DESC_TYPE_X_XXX,
    hdlt645_di_data_desc_x_xxx_set_data,
    hdlt645_di_data_desc_x_xxx_get_data
};


static bool hdlt645_di_data_desc_x_xxx_signed_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    bool data_signed=false;

    const hdlt645_di_data_desc_type_x_xxx_signed_t *dataptr=(const hdlt645_di_data_desc_type_x_xxx_signed_t *)data;
    double val=dataptr->data;

    if(val < 0)
    {
        val = -val;
        data_signed=true;
    }

    /*
     * 三位小数
     */
    val *= 1000;

    hdlt645_bcd_le_set(buffer,buffer_size,((uint64_t)(val + 0.5))%8000);

    if(data_signed)
    {
        buffer[1] |= 0x80;
    }

    return true;
};
static bool hdlt645_di_data_desc_x_xxx_signed_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    bool data_signed=false;
    if(buffer[1] & 0x80)
    {
        data_signed=true;
    }

    hdlt645_di_data_desc_type_x_xxx_signed_t *dataptr=(hdlt645_di_data_desc_type_x_xxx_signed_t *)data;
    uint8_t val_buffer[2]= {0};
    memcpy(val_buffer,buffer,sizeof(val_buffer));

    val_buffer[1] &= (~(0x80));

    uint64_t val=hdlt645_bcd_le_get(val_buffer,sizeof(val_buffer));

    dataptr->data=val;

    /*
     * 三位小数
     */
    dataptr->data /= 1000.0;

    if(data_signed)
    {
        dataptr->data = -dataptr->data;
    }

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_x_xxx_signed=
{
    "X.XXX",
    2,
    HDLT645_DI_DATA_DESC_TYPE_X_XXX_SIGNED,
    hdlt645_di_data_desc_x_xxx_signed_set_data,
    hdlt645_di_data_desc_x_xxx_signed_get_data
};

static bool hdlt645_di_data_desc_xxx_xxx_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    const hdlt645_di_data_desc_type_xxx_xxx_t *dataptr=(const hdlt645_di_data_desc_type_xxx_xxx_t *)data;

    double val=dataptr->data;

    if(val < 0)
    {
        val = -val;
    }

    /*
     * 三位小数
     */
    val *= 1000;

    hdlt645_bcd_le_set(buffer,buffer_size,((uint64_t)(val + 0.5))%1000000);

    return true;
};
static bool hdlt645_di_data_desc_xxx_xxx_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;


    hdlt645_di_data_desc_type_xxx_xxx_t *dataptr=(hdlt645_di_data_desc_type_xxx_xxx_t *)data;

    uint64_t val=hdlt645_bcd_le_get(buffer,buffer_size);

    dataptr->data=val;

    /*
     * 三位小数
     */
    dataptr->data /= 1000.0;


    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_xxx_xxx=
{
    "XXX.XXX",
    3,
    HDLT645_DI_DATA_DESC_TYPE_XXX_XXX,
    hdlt645_di_data_desc_xxx_xxx_set_data,
    hdlt645_di_data_desc_xxx_xxx_get_data
};


static bool hdlt645_di_data_desc_xxx_xxx_signed_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    bool data_signed=false;

    const hdlt645_di_data_desc_type_xxx_xxx_signed_t *dataptr=(const hdlt645_di_data_desc_type_xxx_xxx_signed_t *)data;
    double val=dataptr->data;

    if(val < 0)
    {
        val = -val;
        data_signed=true;
    }

    /*
     * 三位小数
     */
    val *= 1000;

    hdlt645_bcd_le_set(buffer,buffer_size,((uint64_t)(val + 0.5))%800000);

    if(data_signed)
    {
        buffer[2] |= 0x80;
    }

    return true;
};
static bool hdlt645_di_data_desc_xxx_xxx_signed_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
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

    hdlt645_di_data_desc_type_xxx_xxx_signed_t *dataptr=(hdlt645_di_data_desc_type_xxx_xxx_signed_t *)data;
    uint8_t val_buffer[3]= {0};
    memcpy(val_buffer,buffer,sizeof(val_buffer));

    val_buffer[2] &= (~(0x80));

    uint64_t val=hdlt645_bcd_le_get(val_buffer,sizeof(val_buffer));

    dataptr->data=val;

    /*
     * 三位小数
     */
    dataptr->data /= 1000.0;

    if(data_signed)
    {
        dataptr->data = -dataptr->data;
    }

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_xxx_xxx_signed=
{
    "XXX.XXX",
    3,
    HDLT645_DI_DATA_DESC_TYPE_XXX_XXX_SIGNED,
    hdlt645_di_data_desc_xxx_xxx_signed_set_data,
    hdlt645_di_data_desc_xxx_xxx_signed_get_data
};

static bool hdlt645_di_data_desc_xx_xxxx_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    const hdlt645_di_data_desc_type_xx_xxxx_t *dataptr=(const hdlt645_di_data_desc_type_xx_xxxx_t *)data;

    double val=dataptr->data;

    if(val < 0)
    {
        val = -val;
    }

    /*
     * 四位小数
     */
    val *= 10000;

    hdlt645_bcd_le_set(buffer,buffer_size,((uint64_t)(val + 0.5))%1000000);

    return true;
};
static bool hdlt645_di_data_desc_xx_xxxx_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;


    hdlt645_di_data_desc_type_xx_xxxx_t *dataptr=(hdlt645_di_data_desc_type_xx_xxxx_t *)data;

    uint64_t val=hdlt645_bcd_le_get(buffer,buffer_size);

    dataptr->data=val;

    /*
     * 四位小数
     */
    dataptr->data /= 10000.0;


    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_xx_xxxx=
{
    "XX.XXXX",
    3,
    HDLT645_DI_DATA_DESC_TYPE_XX_XXXX,
    hdlt645_di_data_desc_xx_xxxx_set_data,
    hdlt645_di_data_desc_xx_xxxx_get_data
};


static bool hdlt645_di_data_desc_xx_xxxx_signed_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    bool data_signed=false;

    const hdlt645_di_data_desc_type_xx_xxxx_signed_t *dataptr=(const hdlt645_di_data_desc_type_xx_xxxx_signed_t *)data;
    double val=dataptr->data;

    if(val < 0)
    {
        val = -val;
        data_signed=true;
    }

    /*
     * 四位小数
     */
    val *= 10000;

    hdlt645_bcd_le_set(buffer,buffer_size,((uint64_t)(val + 0.5))%800000);

    if(data_signed)
    {
        buffer[2] |= 0x80;
    }

    return true;
};
static bool hdlt645_di_data_desc_xx_xxxx_signed_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
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

    hdlt645_di_data_desc_type_xx_xxxx_signed_t *dataptr=(hdlt645_di_data_desc_type_xx_xxxx_signed_t *)data;
    uint8_t val_buffer[3]= {0};
    memcpy(val_buffer,buffer,sizeof(val_buffer));

    val_buffer[2] &= (~(0x80));

    uint64_t val=hdlt645_bcd_le_get(val_buffer,sizeof(val_buffer));

    dataptr->data=val;

    /*
     * 四位小数
     */
    dataptr->data /= 10000.0;

    if(data_signed)
    {
        dataptr->data = -dataptr->data;
    }

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_xx_xxxx_signed=
{
    "XX.XXXX",
    3,
    HDLT645_DI_DATA_DESC_TYPE_XX_XXXX_SIGNED,
    hdlt645_di_data_desc_xx_xxxx_signed_set_data,
    hdlt645_di_data_desc_xx_xxxx_signed_get_data
};

static bool hdlt645_di_data_desc_xxxxxxxx_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    const hdlt645_di_data_desc_type_xxxxxxxx_t *dataptr=(const hdlt645_di_data_desc_type_xxxxxxxx_t *)data;

    double val=dataptr->data;

    if(val < 0)
    {
        val = -val;
    }


    hdlt645_bcd_le_set(buffer,buffer_size,((uint64_t)(val + 0.5))%100000000);

    return true;
};
static bool hdlt645_di_data_desc_xxxxxxxx_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;


    hdlt645_di_data_desc_type_xxxxxxxx_t *dataptr=(hdlt645_di_data_desc_type_xxxxxxxx_t *)data;

    uint64_t val=hdlt645_bcd_le_get(buffer,buffer_size);

    dataptr->data=val;



    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_xxxxxxxx=
{
    "XXXXXXXX",
    4,
    HDLT645_DI_DATA_DESC_TYPE_XXXXXXXX,
    hdlt645_di_data_desc_xxxxxxxx_set_data,
    hdlt645_di_data_desc_xxxxxxxx_get_data
};


static bool hdlt645_di_data_desc_xxxxxxxx_signed_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    bool data_signed=false;

    const hdlt645_di_data_desc_type_xxxxxxxx_signed_t *dataptr=(const hdlt645_di_data_desc_type_xxxxxxxx_signed_t *)data;
    double val=dataptr->data;

    if(val < 0)
    {
        val = -val;
        data_signed=true;
    }


    hdlt645_bcd_le_set(buffer,buffer_size,((uint64_t)(val + 0.5))%80000000);

    if(data_signed)
    {
        buffer[3] |= 0x80;
    }

    return true;
};
static bool hdlt645_di_data_desc_xxxxxxxx_signed_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
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

    hdlt645_di_data_desc_type_xxxxxxxx_signed_t *dataptr=(hdlt645_di_data_desc_type_xxxxxxxx_signed_t *)data;
    uint8_t val_buffer[4]= {0};
    memcpy(val_buffer,buffer,sizeof(val_buffer));

    val_buffer[3] &= (~(0x80));

    uint64_t val=hdlt645_bcd_le_get(val_buffer,sizeof(val_buffer));

    dataptr->data=val;


    if(data_signed)
    {
        dataptr->data = -dataptr->data;
    }

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_xxxxxxxx_signed=
{
    "XXXXXXXX",
    4,
    HDLT645_DI_DATA_DESC_TYPE_XXXXXXXX_SIGNED,
    hdlt645_di_data_desc_xxxxxxxx_signed_set_data,
    hdlt645_di_data_desc_xxxxxxxx_signed_get_data
};

static bool hdlt645_di_data_desc_nn_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    const hdlt645_di_data_desc_type_nn_t *dataptr=(const hdlt645_di_data_desc_type_nn_t *)data;

    double val=dataptr->data;

    if(val < 0)
    {
        val = -val;
    }

    hdlt645_bcd_le_set(buffer,buffer_size,((uint64_t)(val + 0.5))%100);

    return true;
};
static bool hdlt645_di_data_desc_nn_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    hdlt645_di_data_desc_type_nn_t *dataptr=(hdlt645_di_data_desc_type_nn_t *)data;

    dataptr->data=hdlt645_bcd_le_get(buffer,buffer_size);

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_nn=
{
    "NN",
    1,
    HDLT645_DI_DATA_DESC_TYPE_NN,
    hdlt645_di_data_desc_nn_set_data,
    hdlt645_di_data_desc_nn_get_data
};

static bool hdlt645_di_data_desc_nnnn_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    const hdlt645_di_data_desc_type_nnnn_t *dataptr=(const hdlt645_di_data_desc_type_nnnn_t *)data;

    double val=dataptr->data;

    if(val < 0)
    {
        val = -val;
    }

    hdlt645_bcd_le_set(buffer,buffer_size,((uint64_t)(val + 0.5))%10000);

    return true;
};
static bool hdlt645_di_data_desc_nnnn_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    hdlt645_di_data_desc_type_nnnn_t *dataptr=(hdlt645_di_data_desc_type_nnnn_t *)data;

    dataptr->data=hdlt645_bcd_le_get(buffer,buffer_size);

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_nnnn=
{
    "NNNN",
    2,
    HDLT645_DI_DATA_DESC_TYPE_NNNN,
    hdlt645_di_data_desc_nnnn_set_data,
    hdlt645_di_data_desc_nnnn_get_data
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_xxxx=
{
    "XXXX",
    2,
    HDLT645_DI_DATA_DESC_TYPE_NNNN,
    hdlt645_di_data_desc_nnnn_set_data,
    hdlt645_di_data_desc_nnnn_get_data
};

static bool hdlt645_di_data_desc_xxxxxx_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    const hdlt645_di_data_desc_type_xxxxxx_t *dataptr=(const hdlt645_di_data_desc_type_xxxxxx_t *)data;

    double val=dataptr->data;

    if(val < 0)
    {
        val = -val;
    }

    hdlt645_bcd_le_set(buffer,buffer_size,((uint64_t)(val + 0.5))%1000000);

    return true;
};
static bool hdlt645_di_data_desc_xxxxxx_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    hdlt645_di_data_desc_type_xxxxxx_t *dataptr=(hdlt645_di_data_desc_type_xxxxxx_t *)data;

    dataptr->data=hdlt645_bcd_le_get(buffer,buffer_size);

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_xxxxxx=
{
    "XXXXXX",
    3,
    HDLT645_DI_DATA_DESC_TYPE_XXXXXX,
    hdlt645_di_data_desc_xxxxxx_set_data,
    hdlt645_di_data_desc_xxxxxx_get_data
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_nnnnnnnn=
{
    "NNNNNNNN",
    4,
    HDLT645_DI_DATA_DESC_TYPE_XXXXXXXX,
    hdlt645_di_data_desc_xxxxxxxx_set_data,
    hdlt645_di_data_desc_xxxxxxxx_get_data
};

static bool hdlt645_di_data_desc_nnnnnnnnnnnn_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    const hdlt645_di_data_desc_type_nnnnnnnnnnnn_t *dataptr=(const hdlt645_di_data_desc_type_nnnnnnnnnnnn_t *)data;

    double val=dataptr->data;

    if(val < 0)
    {
        val = -val;
    }

    hdlt645_bcd_le_set(buffer,buffer_size,((uint64_t)(val + 0.5))%1000000000000ULL);

    return true;
};
static bool hdlt645_di_data_desc_nnnnnnnnnnnn_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    hdlt645_di_data_desc_type_nnnnnnnnnnnn_t *dataptr=(hdlt645_di_data_desc_type_nnnnnnnnnnnn_t *)data;

    dataptr->data=hdlt645_bcd_le_get(buffer,buffer_size);

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_nnnnnnnnnnnn=
{
    "NNNNNNNNNNNN",
    6,
    HDLT645_DI_DATA_DESC_TYPE_NNNNNNNNNNNN,
    hdlt645_di_data_desc_nnnnnnnnnnnn_set_data,
    hdlt645_di_data_desc_nnnnnnnnnnnn_get_data
};

static bool hdlt645_di_data_desc_yymmddww_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    const hdlt645_di_data_desc_type_yymmddww_t *dataptr=(const hdlt645_di_data_desc_type_yymmddww_t *)data;

    buffer[0]=hdlt645_uint64_to_bcd(dataptr->YY);
    buffer[1]=hdlt645_uint64_to_bcd(dataptr->MM);
    buffer[2]=hdlt645_uint64_to_bcd(dataptr->DD);
    buffer[3]=hdlt645_uint64_to_bcd(dataptr->WW);

    return true;
};
static bool hdlt645_di_data_desc_yymmddww_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    hdlt645_di_data_desc_type_yymmddww_t *dataptr=(hdlt645_di_data_desc_type_yymmddww_t *)data;

    dataptr->YY=hdlt645_bcd_to_uint64(buffer[0]);
    dataptr->MM=hdlt645_bcd_to_uint64(buffer[1]);
    dataptr->DD=hdlt645_bcd_to_uint64(buffer[2]);
    dataptr->WW=hdlt645_bcd_to_uint64(buffer[3]);

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_yymmddww=
{
    "YYMMDDWW",
    4,
    HDLT645_DI_DATA_DESC_TYPE_YYMMDDWW,
    hdlt645_di_data_desc_yymmddww_set_data,
    hdlt645_di_data_desc_yymmddww_get_data
};

static bool hdlt645_di_data_desc_hhmmss_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    const hdlt645_di_data_desc_type_hhmmss_t *dataptr=(const hdlt645_di_data_desc_type_hhmmss_t *)data;

    buffer[0]=hdlt645_uint64_to_bcd(dataptr->HH);
    buffer[1]=hdlt645_uint64_to_bcd(dataptr->mm);
    buffer[2]=hdlt645_uint64_to_bcd(dataptr->ss);

    return true;
};
static bool hdlt645_di_data_desc_hhmmss_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    hdlt645_di_data_desc_type_hhmmss_t *dataptr=(hdlt645_di_data_desc_type_hhmmss_t *)data;

    dataptr->HH=hdlt645_bcd_to_uint64(buffer[0]);
    dataptr->mm=hdlt645_bcd_to_uint64(buffer[1]);
    dataptr->ss=hdlt645_bcd_to_uint64(buffer[2]);

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_hhmmss=
{
    "hhmmss",
    3,
    HDLT645_DI_DATA_DESC_TYPE_HHMMSS,
    hdlt645_di_data_desc_hhmmss_set_data,
    hdlt645_di_data_desc_hhmmss_get_data
};

static bool hdlt645_di_data_desc_yymmddhhmm_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    const hdlt645_di_data_desc_type_yymmddhhmm_t *dataptr=(const hdlt645_di_data_desc_type_yymmddhhmm_t *)data;

    buffer[0]=hdlt645_uint64_to_bcd(dataptr->YY);
    buffer[1]=hdlt645_uint64_to_bcd(dataptr->MM);
    buffer[2]=hdlt645_uint64_to_bcd(dataptr->DD);
    buffer[3]=hdlt645_uint64_to_bcd(dataptr->HH);
    buffer[4]=hdlt645_uint64_to_bcd(dataptr->mm);

    return true;
};
static bool hdlt645_di_data_desc_yymmddhhmm_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    hdlt645_di_data_desc_type_yymmddhhmm_t *dataptr=(hdlt645_di_data_desc_type_yymmddhhmm_t *)data;

    dataptr->YY=hdlt645_bcd_to_uint64(buffer[0]);
    dataptr->MM=hdlt645_bcd_to_uint64(buffer[1]);
    dataptr->DD=hdlt645_bcd_to_uint64(buffer[2]);
    dataptr->HH=hdlt645_bcd_to_uint64(buffer[3]);
    dataptr->mm=hdlt645_bcd_to_uint64(buffer[4]);

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_yymmddhhmm=
{
    "YYMMDDhhmm",
    5,
    HDLT645_DI_DATA_DESC_TYPE_YYMMDDHHMM,
    hdlt645_di_data_desc_yymmddhhmm_set_data,
    hdlt645_di_data_desc_yymmddhhmm_get_data
};

static bool hdlt645_di_data_desc_mmddhhmm_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    const hdlt645_di_data_desc_type_mmddhhmm_t *dataptr=(const hdlt645_di_data_desc_type_mmddhhmm_t *)data;

    buffer[0]=hdlt645_uint64_to_bcd(dataptr->MM);
    buffer[1]=hdlt645_uint64_to_bcd(dataptr->DD);
    buffer[2]=hdlt645_uint64_to_bcd(dataptr->HH);
    buffer[3]=hdlt645_uint64_to_bcd(dataptr->mm);

    return true;
};
static bool hdlt645_di_data_desc_mmddhhmm_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    hdlt645_di_data_desc_type_mmddhhmm_t *dataptr=(hdlt645_di_data_desc_type_mmddhhmm_t *)data;

    dataptr->MM=hdlt645_bcd_to_uint64(buffer[0]);
    dataptr->DD=hdlt645_bcd_to_uint64(buffer[1]);
    dataptr->HH=hdlt645_bcd_to_uint64(buffer[2]);
    dataptr->mm=hdlt645_bcd_to_uint64(buffer[3]);

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_mmddhhmm=
{
    "MMDDhhmm",
    4,
    HDLT645_DI_DATA_DESC_TYPE_MMDDHHMM,
    hdlt645_di_data_desc_mmddhhmm_set_data,
    hdlt645_di_data_desc_mmddhhmm_get_data
};

static bool hdlt645_di_data_desc_ddhh_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    const hdlt645_di_data_desc_type_ddhh_t *dataptr=(const hdlt645_di_data_desc_type_ddhh_t *)data;

    buffer[0]=hdlt645_uint64_to_bcd(dataptr->DD);
    buffer[1]=hdlt645_uint64_to_bcd(dataptr->HH);

    return true;
};
static bool hdlt645_di_data_desc_ddhh_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    hdlt645_di_data_desc_type_ddhh_t *dataptr=(hdlt645_di_data_desc_type_ddhh_t *)data;

    dataptr->DD=hdlt645_bcd_to_uint64(buffer[0]);
    dataptr->HH=hdlt645_bcd_to_uint64(buffer[1]);

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_ddhh=
{
    "DDhh",
    2,
    HDLT645_DI_DATA_DESC_TYPE_DDHH,
    hdlt645_di_data_desc_ddhh_set_data,
    hdlt645_di_data_desc_ddhh_get_data
};

static bool hdlt645_di_data_desc_yymmddhhmmss_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    const hdlt645_di_data_desc_type_yymmddhhmmss_t *dataptr=(const hdlt645_di_data_desc_type_yymmddhhmmss_t *)data;

    buffer[0]=hdlt645_uint64_to_bcd(dataptr->YY);
    buffer[1]=hdlt645_uint64_to_bcd(dataptr->MM);
    buffer[2]=hdlt645_uint64_to_bcd(dataptr->DD);
    buffer[3]=hdlt645_uint64_to_bcd(dataptr->HH);
    buffer[4]=hdlt645_uint64_to_bcd(dataptr->mm);
    buffer[5]=hdlt645_uint64_to_bcd(dataptr->ss);

    return true;
};
static bool hdlt645_di_data_desc_yymmddhhmmss_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    hdlt645_di_data_desc_type_yymmddhhmmss_t *dataptr=(hdlt645_di_data_desc_type_yymmddhhmmss_t *)data;

    dataptr->YY=hdlt645_bcd_to_uint64(buffer[0]);
    dataptr->MM=hdlt645_bcd_to_uint64(buffer[1]);
    dataptr->DD=hdlt645_bcd_to_uint64(buffer[2]);
    dataptr->HH=hdlt645_bcd_to_uint64(buffer[3]);
    dataptr->mm=hdlt645_bcd_to_uint64(buffer[4]);
    dataptr->ss=hdlt645_bcd_to_uint64(buffer[5]);

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_yymmddhhmmss=
{
    "YYMMDDhhmmss",
    6,
    HDLT645_DI_DATA_DESC_TYPE_YYMMDDHHMMSS,
    hdlt645_di_data_desc_yymmddhhmmss_set_data,
    hdlt645_di_data_desc_yymmddhhmmss_get_data
};

static bool hdlt645_di_data_desc_mmddnn_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    const hdlt645_di_data_desc_type_mmddnn_t *dataptr=(const hdlt645_di_data_desc_type_mmddnn_t *)data;

    buffer[0]=hdlt645_uint64_to_bcd(dataptr->MM);
    buffer[1]=hdlt645_uint64_to_bcd(dataptr->DD);
    buffer[2]=hdlt645_uint64_to_bcd(dataptr->NN);

    return true;
};
static bool hdlt645_di_data_desc_mmddnn_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    hdlt645_di_data_desc_type_mmddnn_t *dataptr=(hdlt645_di_data_desc_type_mmddnn_t *)data;

    dataptr->MM=hdlt645_bcd_to_uint64(buffer[0]);
    dataptr->DD=hdlt645_bcd_to_uint64(buffer[1]);
    dataptr->NN=hdlt645_bcd_to_uint64(buffer[2]);

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_mmddnn=
{
    "MMDDNN",
    3,
    HDLT645_DI_DATA_DESC_TYPE_MMDDNN,
    hdlt645_di_data_desc_mmddnn_set_data,
    hdlt645_di_data_desc_mmddnn_get_data
};

static bool hdlt645_di_data_desc_hhmmnn_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    const hdlt645_di_data_desc_type_hhmmnn_t *dataptr=(const hdlt645_di_data_desc_type_hhmmnn_t *)data;

    buffer[0]=hdlt645_uint64_to_bcd(dataptr->HH);
    buffer[1]=hdlt645_uint64_to_bcd(dataptr->mm);
    buffer[2]=hdlt645_uint64_to_bcd(dataptr->NN);

    return true;
};
static bool hdlt645_di_data_desc_hhmmnn_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    hdlt645_di_data_desc_type_hhmmnn_t *dataptr=(hdlt645_di_data_desc_type_hhmmnn_t *)data;

    dataptr->HH=hdlt645_bcd_to_uint64(buffer[0]);
    dataptr->mm=hdlt645_bcd_to_uint64(buffer[1]);
    dataptr->NN=hdlt645_bcd_to_uint64(buffer[2]);

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_hhmmnn=
{
    "hhmmNN",
    3,
    HDLT645_DI_DATA_DESC_TYPE_HHMMNN,
    hdlt645_di_data_desc_hhmmnn_set_data,
    hdlt645_di_data_desc_hhmmnn_get_data
};

static bool hdlt645_di_data_desc_yymmddnn_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    const hdlt645_di_data_desc_type_yymmddnn_t *dataptr=(const hdlt645_di_data_desc_type_yymmddnn_t *)data;

    buffer[0]=hdlt645_uint64_to_bcd(dataptr->YY);
    buffer[1]=hdlt645_uint64_to_bcd(dataptr->MM);
    buffer[2]=hdlt645_uint64_to_bcd(dataptr->DD);
    buffer[3]=hdlt645_uint64_to_bcd(dataptr->NN);

    return true;
};
static bool hdlt645_di_data_desc_yymmddnn_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    hdlt645_di_data_desc_type_yymmddnn_t *dataptr=(hdlt645_di_data_desc_type_yymmddnn_t *)data;

    dataptr->YY=hdlt645_bcd_to_uint64(buffer[0]);
    dataptr->MM=hdlt645_bcd_to_uint64(buffer[1]);
    dataptr->DD=hdlt645_bcd_to_uint64(buffer[2]);
    dataptr->NN=hdlt645_bcd_to_uint64(buffer[3]);

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_yymmddnn=
{
    "YYMMDDNN",
    4,
    HDLT645_DI_DATA_DESC_TYPE_YYMMDDNN,
    hdlt645_di_data_desc_yymmddnn_set_data,
    hdlt645_di_data_desc_yymmddnn_get_data
};

static bool hdlt645_di_data_desc_yymmddhhmmnn_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    const hdlt645_di_data_desc_type_yymmddhhmmnn_t *dataptr=(const hdlt645_di_data_desc_type_yymmddhhmmnn_t *)data;

    buffer[0]=hdlt645_uint64_to_bcd(dataptr->YY);
    buffer[1]=hdlt645_uint64_to_bcd(dataptr->MM);
    buffer[2]=hdlt645_uint64_to_bcd(dataptr->DD);
    buffer[3]=hdlt645_uint64_to_bcd(dataptr->HH);
    buffer[4]=hdlt645_uint64_to_bcd(dataptr->mm);
    buffer[5]=hdlt645_uint64_to_bcd(dataptr->NN);

    return true;
};
static bool hdlt645_di_data_desc_yymmddhhmmnn_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    hdlt645_di_data_desc_type_yymmddhhmmnn_t *dataptr=(hdlt645_di_data_desc_type_yymmddhhmmnn_t *)data;

    dataptr->YY=hdlt645_bcd_to_uint64(buffer[0]);
    dataptr->MM=hdlt645_bcd_to_uint64(buffer[1]);
    dataptr->DD=hdlt645_bcd_to_uint64(buffer[2]);
    dataptr->HH=hdlt645_bcd_to_uint64(buffer[3]);
    dataptr->mm=hdlt645_bcd_to_uint64(buffer[4]);
    dataptr->NN=hdlt645_bcd_to_uint64(buffer[5]);

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_yymmddhhmmnn=
{
    "YYMMDDhhmmNN",
    6,
    HDLT645_DI_DATA_DESC_TYPE_YYMMDDHHMMNN,
    hdlt645_di_data_desc_yymmddhhmmnn_set_data,
    hdlt645_di_data_desc_yymmddhhmmnn_get_data
};

static bool hdlt645_di_data_desc_ascii_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    const hdlt645_di_data_desc_type_ascii_t *dataptr=(const hdlt645_di_data_desc_type_ascii_t *)data;

    if(buffer_size > sizeof(dataptr->data))
    {
        return false;
    }

    memcpy(buffer,dataptr->data,buffer_size);

    return true;
};
static bool hdlt645_di_data_desc_ascii_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    hdlt645_di_data_desc_type_ascii_t *dataptr=(hdlt645_di_data_desc_type_ascii_t *)data;

    if(buffer_size > sizeof(dataptr->data))
    {
        return false;
    }

    memset(dataptr->data,0,sizeof(dataptr->data));
    memcpy(dataptr->data,buffer,buffer_size);

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_ascii_3=
{
    "NN...NN",
    3,
    HDLT645_DI_DATA_DESC_TYPE_ASCII,
    hdlt645_di_data_desc_ascii_set_data,
    hdlt645_di_data_desc_ascii_get_data
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_ascii_4=
{
    "XXXX",
    4,
    HDLT645_DI_DATA_DESC_TYPE_ASCII,
    hdlt645_di_data_desc_ascii_set_data,
    hdlt645_di_data_desc_ascii_get_data
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_ascii_6=
{
    "XXXXXXXXXXXX",
    6,
    HDLT645_DI_DATA_DESC_TYPE_ASCII,
    hdlt645_di_data_desc_ascii_set_data,
    hdlt645_di_data_desc_ascii_get_data
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_ascii_10=
{
    "XX...XX",
    10,
    HDLT645_DI_DATA_DESC_TYPE_ASCII,
    hdlt645_di_data_desc_ascii_set_data,
    hdlt645_di_data_desc_ascii_get_data
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_ascii_16=
{
    "XX...XX",
    16,
    HDLT645_DI_DATA_DESC_TYPE_ASCII,
    hdlt645_di_data_desc_ascii_set_data,
    hdlt645_di_data_desc_ascii_get_data
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_ascii_32=
{
    "NN...NN",
    32,
    HDLT645_DI_DATA_DESC_TYPE_ASCII,
    hdlt645_di_data_desc_ascii_set_data,
    hdlt645_di_data_desc_ascii_get_data
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_n_nnn=
{
    "N.NNN",
    2,
    HDLT645_DI_DATA_DESC_TYPE_X_XXX,
    hdlt645_di_data_desc_x_xxx_set_data,
    hdlt645_di_data_desc_x_xxx_get_data
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_nn_nnnn=
{
    "NN.NNNN",
    3,
    HDLT645_DI_DATA_DESC_TYPE_XX_XXXX,
    hdlt645_di_data_desc_xx_xxxx_set_data,
    hdlt645_di_data_desc_xx_xxxx_get_data
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_nnn_n=
{
    "NNN.N",
    2,
    HDLT645_DI_DATA_DESC_TYPE_XXX_X,
    hdlt645_di_data_desc_xxx_x_set_data,
    hdlt645_di_data_desc_xxx_x_get_data
};


static bool hdlt645_di_data_desc_status_word_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    const hdlt645_di_data_desc_type_xxxx_t *dataptr=(const hdlt645_di_data_desc_type_xxxx_t *)data;

    buffer[0]=(dataptr->data & 0xFF);
    buffer[1]=((dataptr->data >> 8) & 0xFF);

    return true;
};
static bool hdlt645_di_data_desc_status_word_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    hdlt645_di_data_desc_type_xxxx_t *dataptr=(hdlt645_di_data_desc_type_xxxx_t *)data;

    dataptr->data=buffer[0];
    dataptr->data|=(buffer[1] << 8);

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_status_word=
{
    "XXXX",
    2,
    HDLT645_DI_DATA_DESC_TYPE_XXXX,
    hdlt645_di_data_desc_status_word_set_data,
    hdlt645_di_data_desc_status_word_get_data
};

static bool hdlt645_di_data_desc_xxxx_xx_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    const hdlt645_di_data_desc_type_xxxx_xx_t *dataptr=(const hdlt645_di_data_desc_type_xxxx_xx_t *)data;

    double val=dataptr->data;

    if(val < 0)
    {
        val = -val;
    }

    /*
     * 两位小数
     */
    val *= 100;

    hdlt645_bcd_le_set(buffer,buffer_size,((uint64_t)(val + 0.5))%1000000);

    return true;
};
static bool hdlt645_di_data_desc_xxxx_xx_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    buffer_size = desc->data_length;

    hdlt645_di_data_desc_type_xxxx_xx_t *dataptr=(hdlt645_di_data_desc_type_xxxx_xx_t *)data;

    uint64_t val=hdlt645_bcd_le_get(buffer,buffer_size);

    dataptr->data=val;

    /*
     * 两位小数
     */
    dataptr->data /= 100.0;

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_xxxx_xx=
{
    "XXXX.XX",
    3,
    HDLT645_DI_DATA_DESC_TYPE_XXXX_XX,
    hdlt645_di_data_desc_xxxx_xx_set_data,
    hdlt645_di_data_desc_xxxx_xx_get_data
};

static void hdlt645_di_data_desc_event_instantaneous_set_buffer(uint8_t *buffer,const hdlt645_di_data_desc_type_event_instantaneous_t *dataptr)
{
    hdlt645_di_data_desc_xxx_x_set_data(&hdlt645_di_data_desc_xxx_x,buffer,2,&dataptr->voltage);
    buffer+=2;
    hdlt645_di_data_desc_xxx_xxx_set_data(&hdlt645_di_data_desc_xxx_xxx,buffer,3,&dataptr->current);
    buffer+=3;
    hdlt645_di_data_desc_xx_xxxx_set_data(&hdlt645_di_data_desc_xx_xxxx,buffer,3,&dataptr->active_power);
    buffer+=3;
    hdlt645_di_data_desc_xx_xxxx_set_data(&hdlt645_di_data_desc_xx_xxxx,buffer,3,&dataptr->reactive_power);
    buffer+=3;
    hdlt645_di_data_desc_x_xxx_set_data(&hdlt645_di_data_desc_x_xxx,buffer,2,&dataptr->power_factor);
};
static void hdlt645_di_data_desc_event_instantaneous_get_buffer(const uint8_t *buffer,hdlt645_di_data_desc_type_event_instantaneous_t *dataptr)
{
    hdlt645_di_data_desc_xxx_x_get_data(&hdlt645_di_data_desc_xxx_x,buffer,2,&dataptr->voltage);
    buffer+=2;
    hdlt645_di_data_desc_xxx_xxx_get_data(&hdlt645_di_data_desc_xxx_xxx,buffer,3,&dataptr->current);
    buffer+=3;
    hdlt645_di_data_desc_xx_xxxx_get_data(&hdlt645_di_data_desc_xx_xxxx,buffer,3,&dataptr->active_power);
    buffer+=3;
    hdlt645_di_data_desc_xx_xxxx_get_data(&hdlt645_di_data_desc_xx_xxxx,buffer,3,&dataptr->reactive_power);
    buffer+=3;
    hdlt645_di_data_desc_x_xxx_get_data(&hdlt645_di_data_desc_x_xxx,buffer,2,&dataptr->power_factor);
};

static bool hdlt645_di_data_desc_event_summary_abc_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    const hdlt645_di_data_desc_type_event_summary_abc_t *dataptr=(const hdlt645_di_data_desc_type_event_summary_abc_t *)data;

    uint8_t *p=buffer;

    for(size_t i=0; i<3; i++)
    {
        hdlt645_di_data_desc_xxxxxx_set_data(&hdlt645_di_data_desc_xxxxxx,p,3,&dataptr->phase[i].count);
        p+=3;
        hdlt645_di_data_desc_xxxxxx_set_data(&hdlt645_di_data_desc_xxxxxx,p,3,&dataptr->phase[i].time);
        p+=3;
    }

    return true;
};
static bool hdlt645_di_data_desc_event_summary_abc_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    hdlt645_di_data_desc_type_event_summary_abc_t *dataptr=(hdlt645_di_data_desc_type_event_summary_abc_t *)data;

    const uint8_t *p=buffer;

    for(size_t i=0; i<3; i++)
    {
        hdlt645_di_data_desc_xxxxxx_get_data(&hdlt645_di_data_desc_xxxxxx,p,3,&dataptr->phase[i].count);
        p+=3;
        hdlt645_di_data_desc_xxxxxx_get_data(&hdlt645_di_data_desc_xxxxxx,p,3,&dataptr->phase[i].time);
        p+=3;
    }

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_event_summary_abc=
{
    "NNNNNN,NNNNNN*3",
    18,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_SUMMARY_ABC,
    hdlt645_di_data_desc_event_summary_abc_set_data,
    hdlt645_di_data_desc_event_summary_abc_get_data
};

static bool hdlt645_di_data_desc_event_summary_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    const hdlt645_di_data_desc_type_event_summary_t *dataptr=(const hdlt645_di_data_desc_type_event_summary_t *)data;

    hdlt645_di_data_desc_xxxxxx_set_data(&hdlt645_di_data_desc_xxxxxx,buffer,3,&dataptr->data.count);
    hdlt645_di_data_desc_xxxxxx_set_data(&hdlt645_di_data_desc_xxxxxx,buffer+3,3,&dataptr->data.time);

    return true;
};
static bool hdlt645_di_data_desc_event_summary_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    hdlt645_di_data_desc_type_event_summary_t *dataptr=(hdlt645_di_data_desc_type_event_summary_t *)data;

    hdlt645_di_data_desc_xxxxxx_get_data(&hdlt645_di_data_desc_xxxxxx,buffer,3,&dataptr->data.count);
    hdlt645_di_data_desc_xxxxxx_get_data(&hdlt645_di_data_desc_xxxxxx,buffer+3,3,&dataptr->data.time);

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_event_summary=
{
    "NNNNNN,NNNNNN",
    6,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_SUMMARY,
    hdlt645_di_data_desc_event_summary_set_data,
    hdlt645_di_data_desc_event_summary_get_data
};

static bool hdlt645_di_data_desc_event_phase_voltage_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    const hdlt645_di_data_desc_type_event_phase_voltage_t *dataptr=(const hdlt645_di_data_desc_type_event_phase_voltage_t *)data;

    uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_set_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->start);
    p+=6;
    hdlt645_di_data_desc_yymmddhhmmss_set_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->end);
    p+=6;
    for(size_t i=0; i<4; i++)
    {
        hdlt645_di_data_desc_xx_xxxx_set_data(&hdlt645_di_data_desc_xx_xxxx,p,4,&dataptr->total_energy[i]);
        p+=4;
    }
    for(size_t i=0; i<3; i++)
    {
        for(size_t j=0; j<4; j++)
        {
            hdlt645_di_data_desc_xx_xxxx_set_data(&hdlt645_di_data_desc_xx_xxxx,p,4,&dataptr->phase[i].energy[j]);
            p+=4;
        }
        hdlt645_di_data_desc_event_instantaneous_set_buffer(p,&dataptr->phase[i].instantaneous);
        p+=13;
    }
    for(size_t i=0; i<4; i++)
    {
        hdlt645_di_data_desc_xx_xxxx_set_data(&hdlt645_di_data_desc_xx_xxxx,p,4,&dataptr->ah[i]);
        p+=4;
    }

    return true;
};
static bool hdlt645_di_data_desc_event_phase_voltage_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    hdlt645_di_data_desc_type_event_phase_voltage_t *dataptr=(hdlt645_di_data_desc_type_event_phase_voltage_t *)data;

    const uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_get_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->start);
    p+=6;
    hdlt645_di_data_desc_yymmddhhmmss_get_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->end);
    p+=6;
    for(size_t i=0; i<4; i++)
    {
        hdlt645_di_data_desc_xx_xxxx_get_data(&hdlt645_di_data_desc_xx_xxxx,p,4,&dataptr->total_energy[i]);
        p+=4;
    }
    for(size_t i=0; i<3; i++)
    {
        for(size_t j=0; j<4; j++)
        {
            hdlt645_di_data_desc_xx_xxxx_get_data(&hdlt645_di_data_desc_xx_xxxx,p,4,&dataptr->phase[i].energy[j]);
            p+=4;
        }
        hdlt645_di_data_desc_event_instantaneous_get_buffer(p,&dataptr->phase[i].instantaneous);
        p+=13;
    }
    for(size_t i=0; i<4; i++)
    {
        hdlt645_di_data_desc_xx_xxxx_get_data(&hdlt645_di_data_desc_xx_xxxx,p,4,&dataptr->ah[i]);
        p+=4;
    }

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_event_phase_voltage=
{
    "YYMMDDhhmmssYYMMDDhhmmssXXXXXX.XX*16XXX.X*3XXX.XXX*3XX.XXXX*6X.XXX*3XXXXXX.XX*4",
    131,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_PHASE_VOLTAGE,
    hdlt645_di_data_desc_event_phase_voltage_set_data,
    hdlt645_di_data_desc_event_phase_voltage_get_data
};

static bool hdlt645_di_data_desc_event_phase_current_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    const hdlt645_di_data_desc_type_event_phase_current_t *dataptr=(const hdlt645_di_data_desc_type_event_phase_current_t *)data;

    uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_set_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->start);
    p+=6;
    hdlt645_di_data_desc_yymmddhhmmss_set_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->end);
    p+=6;
    for(size_t i=0; i<4; i++)
    {
        hdlt645_di_data_desc_xx_xxxx_set_data(&hdlt645_di_data_desc_xx_xxxx,p,4,&dataptr->total_energy[i]);
        p+=4;
    }
    for(size_t i=0; i<3; i++)
    {
        for(size_t j=0; j<4; j++)
        {
            hdlt645_di_data_desc_xx_xxxx_set_data(&hdlt645_di_data_desc_xx_xxxx,p,4,&dataptr->phase[i].energy[j]);
            p+=4;
        }
        hdlt645_di_data_desc_event_instantaneous_set_buffer(p,&dataptr->phase[i].instantaneous);
        p+=13;
    }

    return true;
};
static bool hdlt645_di_data_desc_event_phase_current_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    hdlt645_di_data_desc_type_event_phase_current_t *dataptr=(hdlt645_di_data_desc_type_event_phase_current_t *)data;

    const uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_get_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->start);
    p+=6;
    hdlt645_di_data_desc_yymmddhhmmss_get_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->end);
    p+=6;
    for(size_t i=0; i<4; i++)
    {
        hdlt645_di_data_desc_xx_xxxx_get_data(&hdlt645_di_data_desc_xx_xxxx,p,4,&dataptr->total_energy[i]);
        p+=4;
    }
    for(size_t i=0; i<3; i++)
    {
        for(size_t j=0; j<4; j++)
        {
            hdlt645_di_data_desc_xx_xxxx_get_data(&hdlt645_di_data_desc_xx_xxxx,p,4,&dataptr->phase[i].energy[j]);
            p+=4;
        }
        hdlt645_di_data_desc_event_instantaneous_get_buffer(p,&dataptr->phase[i].instantaneous);
        p+=13;
    }

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_event_phase_current=
{
    "YYMMDDhhmmssYYMMDDhhmmssXXXXXX.XX*16XXX.X*3XXX.XXX*3XX.XXXX*6X.XXX*3",
    115,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_PHASE_CURRENT,
    hdlt645_di_data_desc_event_phase_current_set_data,
    hdlt645_di_data_desc_event_phase_current_get_data
};

static bool hdlt645_di_data_desc_event_reverse_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    const hdlt645_di_data_desc_type_event_reverse_t *dataptr=(const hdlt645_di_data_desc_type_event_reverse_t *)data;

    uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_set_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->start);
    p+=6;
    hdlt645_di_data_desc_yymmddhhmmss_set_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->end);
    p+=6;
    for(size_t i=0; i<16; i++)
    {
        hdlt645_di_data_desc_xx_xxxx_set_data(&hdlt645_di_data_desc_xx_xxxx,p,4,&dataptr->energy[i]);
        p+=4;
    }

    return true;
};
static bool hdlt645_di_data_desc_event_reverse_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    hdlt645_di_data_desc_type_event_reverse_t *dataptr=(hdlt645_di_data_desc_type_event_reverse_t *)data;

    const uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_get_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->start);
    p+=6;
    hdlt645_di_data_desc_yymmddhhmmss_get_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->end);
    p+=6;
    for(size_t i=0; i<16; i++)
    {
        hdlt645_di_data_desc_xx_xxxx_get_data(&hdlt645_di_data_desc_xx_xxxx,p,4,&dataptr->energy[i]);
        p+=4;
    }

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_event_reverse=
{
    "YYMMDDhhmmssYYMMDDhhmmssXXXXXX.XX*16",
    76,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_REVERSE,
    hdlt645_di_data_desc_event_reverse_set_data,
    hdlt645_di_data_desc_event_reverse_get_data
};

static bool hdlt645_di_data_desc_event_imbalance_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    const hdlt645_di_data_desc_type_event_imbalance_t *dataptr=(const hdlt645_di_data_desc_type_event_imbalance_t *)data;

    uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_set_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->start);
    p+=6;
    hdlt645_di_data_desc_yymmddhhmmss_set_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->end);
    p+=6;
    hdlt645_di_data_desc_xx_xx_set_data(&hdlt645_di_data_desc_xx_xx,p,2,&dataptr->imbalance);
    p+=2;
    for(size_t i=0; i<16; i++)
    {
        hdlt645_di_data_desc_xx_xxxx_set_data(&hdlt645_di_data_desc_xx_xxxx,p,4,&dataptr->energy[i]);
        p+=4;
    }

    return true;
};
static bool hdlt645_di_data_desc_event_imbalance_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    hdlt645_di_data_desc_type_event_imbalance_t *dataptr=(hdlt645_di_data_desc_type_event_imbalance_t *)data;

    const uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_get_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->start);
    p+=6;
    hdlt645_di_data_desc_yymmddhhmmss_get_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->end);
    p+=6;
    hdlt645_di_data_desc_xx_xx_get_data(&hdlt645_di_data_desc_xx_xx,p,2,&dataptr->imbalance);
    p+=2;
    for(size_t i=0; i<16; i++)
    {
        hdlt645_di_data_desc_xx_xxxx_get_data(&hdlt645_di_data_desc_xx_xxxx,p,4,&dataptr->energy[i]);
        p+=4;
    }

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_event_imbalance=
{
    "YYMMDDhhmmssYYMMDDhhmmssXX.XXXXXXXXXX.XX*16",
    78,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_IMBALANCE,
    hdlt645_di_data_desc_event_imbalance_set_data,
    hdlt645_di_data_desc_event_imbalance_get_data
};

static bool hdlt645_di_data_desc_event_full_voltage_loss_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    const hdlt645_di_data_desc_type_event_full_voltage_loss_t *dataptr=(const hdlt645_di_data_desc_type_event_full_voltage_loss_t *)data;

    uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_set_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->start);
    p+=6;
    hdlt645_di_data_desc_xxx_xxx_set_data(&hdlt645_di_data_desc_xxx_xxx,p,3,&dataptr->current);
    p+=3;
    hdlt645_di_data_desc_yymmddhhmmss_set_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->end);

    return true;
};
static bool hdlt645_di_data_desc_event_full_voltage_loss_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    hdlt645_di_data_desc_type_event_full_voltage_loss_t *dataptr=(hdlt645_di_data_desc_type_event_full_voltage_loss_t *)data;

    const uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_get_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->start);
    p+=6;
    hdlt645_di_data_desc_xxx_xxx_get_data(&hdlt645_di_data_desc_xxx_xxx,p,3,&dataptr->current);
    p+=3;
    hdlt645_di_data_desc_yymmddhhmmss_get_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->end);

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_event_full_voltage_loss=
{
    "YYMMDDhhmmssXXX.XXXYYMMDDhhmmss",
    15,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_FULL_VOLTAGE_LOSS,
    hdlt645_di_data_desc_event_full_voltage_loss_set_data,
    hdlt645_di_data_desc_event_full_voltage_loss_get_data
};

static bool hdlt645_di_data_desc_event_two_times_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    const hdlt645_di_data_desc_type_event_two_times_t *dataptr=(const hdlt645_di_data_desc_type_event_two_times_t *)data;

    hdlt645_di_data_desc_yymmddhhmmss_set_data(&hdlt645_di_data_desc_yymmddhhmmss,buffer,6,&dataptr->start);
    hdlt645_di_data_desc_yymmddhhmmss_set_data(&hdlt645_di_data_desc_yymmddhhmmss,buffer+6,6,&dataptr->end);

    return true;
};
static bool hdlt645_di_data_desc_event_two_times_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    hdlt645_di_data_desc_type_event_two_times_t *dataptr=(hdlt645_di_data_desc_type_event_two_times_t *)data;

    hdlt645_di_data_desc_yymmddhhmmss_get_data(&hdlt645_di_data_desc_yymmddhhmmss,buffer,6,&dataptr->start);
    hdlt645_di_data_desc_yymmddhhmmss_get_data(&hdlt645_di_data_desc_yymmddhhmmss,buffer+6,6,&dataptr->end);

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_event_two_times=
{
    "YYMMDDhhmmssYYMMDDhhmmss",
    12,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_TWO_TIMES,
    hdlt645_di_data_desc_event_two_times_set_data,
    hdlt645_di_data_desc_event_two_times_get_data
};

static bool hdlt645_di_data_desc_event_demand_exceed_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    const hdlt645_di_data_desc_type_event_demand_exceed_t *dataptr=(const hdlt645_di_data_desc_type_event_demand_exceed_t *)data;

    uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_set_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->start);
    p+=6;
    hdlt645_di_data_desc_yymmddhhmmss_set_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->end);
    p+=6;
    hdlt645_di_data_desc_xx_xxxx_set_data(&hdlt645_di_data_desc_xx_xxxx,p,3,&dataptr->demand);
    p+=3;
    hdlt645_di_data_desc_yymmddhhmm_set_data(&hdlt645_di_data_desc_yymmddhhmm,p,5,&dataptr->time);

    return true;
};
static bool hdlt645_di_data_desc_event_demand_exceed_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    hdlt645_di_data_desc_type_event_demand_exceed_t *dataptr=(hdlt645_di_data_desc_type_event_demand_exceed_t *)data;

    const uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_get_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->start);
    p+=6;
    hdlt645_di_data_desc_yymmddhhmmss_get_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->end);
    p+=6;
    hdlt645_di_data_desc_xx_xxxx_get_data(&hdlt645_di_data_desc_xx_xxxx,p,3,&dataptr->demand);
    p+=3;
    hdlt645_di_data_desc_yymmddhhmm_get_data(&hdlt645_di_data_desc_yymmddhhmm,p,5,&dataptr->time);

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_event_demand_exceed=
{
    "YYMMDDhhmmssYYMMDDhhmmssXX.XXXXYYMMDDhhmm",
    20,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_DEMAND_EXCEED,
    hdlt645_di_data_desc_event_demand_exceed_set_data,
    hdlt645_di_data_desc_event_demand_exceed_get_data
};

static bool hdlt645_di_data_desc_event_voltage_qualification_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    const hdlt645_di_data_desc_type_event_voltage_qualification_t *dataptr=(const hdlt645_di_data_desc_type_event_voltage_qualification_t *)data;

    uint8_t *p=buffer;

    hdlt645_di_data_desc_xxxxxx_set_data(&hdlt645_di_data_desc_xxxxxx,p,3,&dataptr->monitor_time);
    p+=3;
    hdlt645_di_data_desc_xxxx_xx_set_data(&hdlt645_di_data_desc_xxxx_xx,p,3,&dataptr->pass_rate);
    p+=3;
    hdlt645_di_data_desc_xxxx_xx_set_data(&hdlt645_di_data_desc_xxxx_xx,p,3,&dataptr->over_limit_rate);
    p+=3;
    hdlt645_di_data_desc_xxxxxx_set_data(&hdlt645_di_data_desc_xxxxxx,p,3,&dataptr->over_upper_time);
    p+=3;
    hdlt645_di_data_desc_xxxxxx_set_data(&hdlt645_di_data_desc_xxxxxx,p,3,&dataptr->over_lower_time);
    p+=3;
    hdlt645_di_data_desc_xxx_x_set_data(&hdlt645_di_data_desc_xxx_x,p,2,&dataptr->max_voltage);
    p+=2;
    hdlt645_di_data_desc_mmddhhmm_set_data(&hdlt645_di_data_desc_mmddhhmm,p,4,&dataptr->max_voltage_time);
    p+=4;
    hdlt645_di_data_desc_xxx_x_set_data(&hdlt645_di_data_desc_xxx_x,p,2,&dataptr->min_voltage);
    p+=2;
    hdlt645_di_data_desc_mmddhhmm_set_data(&hdlt645_di_data_desc_mmddhhmm,p,4,&dataptr->min_voltage_time);

    return true;
};
static bool hdlt645_di_data_desc_event_voltage_qualification_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    hdlt645_di_data_desc_type_event_voltage_qualification_t *dataptr=(hdlt645_di_data_desc_type_event_voltage_qualification_t *)data;

    const uint8_t *p=buffer;

    hdlt645_di_data_desc_xxxxxx_get_data(&hdlt645_di_data_desc_xxxxxx,p,3,&dataptr->monitor_time);
    p+=3;
    hdlt645_di_data_desc_xxxx_xx_get_data(&hdlt645_di_data_desc_xxxx_xx,p,3,&dataptr->pass_rate);
    p+=3;
    hdlt645_di_data_desc_xxxx_xx_get_data(&hdlt645_di_data_desc_xxxx_xx,p,3,&dataptr->over_limit_rate);
    p+=3;
    hdlt645_di_data_desc_xxxxxx_get_data(&hdlt645_di_data_desc_xxxxxx,p,3,&dataptr->over_upper_time);
    p+=3;
    hdlt645_di_data_desc_xxxxxx_get_data(&hdlt645_di_data_desc_xxxxxx,p,3,&dataptr->over_lower_time);
    p+=3;
    hdlt645_di_data_desc_xxx_x_get_data(&hdlt645_di_data_desc_xxx_x,p,2,&dataptr->max_voltage);
    p+=2;
    hdlt645_di_data_desc_mmddhhmm_get_data(&hdlt645_di_data_desc_mmddhhmm,p,4,&dataptr->max_voltage_time);
    p+=4;
    hdlt645_di_data_desc_xxx_x_get_data(&hdlt645_di_data_desc_xxx_x,p,2,&dataptr->min_voltage);
    p+=2;
    hdlt645_di_data_desc_mmddhhmm_get_data(&hdlt645_di_data_desc_mmddhhmm,p,4,&dataptr->min_voltage_time);

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_event_voltage_qualification=
{
    "NNNNNN*5XXX.X*2MMDDhhmm*2",
    27,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_VOLTAGE_QUALIFICATION,
    hdlt645_di_data_desc_event_voltage_qualification_set_data,
    hdlt645_di_data_desc_event_voltage_qualification_get_data
};

static bool hdlt645_di_data_desc_event_programming_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    const hdlt645_di_data_desc_type_event_programming_t *dataptr=(const hdlt645_di_data_desc_type_event_programming_t *)data;

    uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_set_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->time);
    p+=6;
    memcpy(p,dataptr->operator_code.c,4);
    p+=4;
    for(size_t i=0; i<10; i++)
    {
        memcpy(p,dataptr->di[i].di,4);
        p+=4;
    }

    return true;
};
static bool hdlt645_di_data_desc_event_programming_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    hdlt645_di_data_desc_type_event_programming_t *dataptr=(hdlt645_di_data_desc_type_event_programming_t *)data;

    const uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_get_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->time);
    p+=6;
    memcpy(dataptr->operator_code.c,p,4);
    p+=4;
    for(size_t i=0; i<10; i++)
    {
        memcpy(dataptr->di[i].di,p,4);
        p+=4;
    }

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_event_programming=
{
    "YYMMDDhhmmssC0C1C2C3XXXXXXXX*10",
    50,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_PROGRAMMING,
    hdlt645_di_data_desc_event_programming_set_data,
    hdlt645_di_data_desc_event_programming_get_data
};

static bool hdlt645_di_data_desc_event_meter_clear_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    const hdlt645_di_data_desc_type_event_meter_clear_t *dataptr=(const hdlt645_di_data_desc_type_event_meter_clear_t *)data;

    uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_set_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->time);
    p+=6;
    memcpy(p,dataptr->operator_code.c,4);
    p+=4;
    for(size_t i=0; i<24; i++)
    {
        hdlt645_di_data_desc_xx_xxxx_set_data(&hdlt645_di_data_desc_xx_xxxx,p,4,&dataptr->energy[i]);
        p+=4;
    }

    return true;
};
static bool hdlt645_di_data_desc_event_meter_clear_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    hdlt645_di_data_desc_type_event_meter_clear_t *dataptr=(hdlt645_di_data_desc_type_event_meter_clear_t *)data;

    const uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_get_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->time);
    p+=6;
    memcpy(dataptr->operator_code.c,p,4);
    p+=4;
    for(size_t i=0; i<24; i++)
    {
        hdlt645_di_data_desc_xx_xxxx_get_data(&hdlt645_di_data_desc_xx_xxxx,p,4,&dataptr->energy[i]);
        p+=4;
    }

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_event_meter_clear=
{
    "YYMMDDhhmmssC0C1C2C3XXXXXX.XX*24",
    106,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_METER_CLEAR,
    hdlt645_di_data_desc_event_meter_clear_set_data,
    hdlt645_di_data_desc_event_meter_clear_get_data
};

static bool hdlt645_di_data_desc_event_demand_clear_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    const hdlt645_di_data_desc_type_event_demand_clear_t *dataptr=(const hdlt645_di_data_desc_type_event_demand_clear_t *)data;

    uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_set_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->time);
    p+=6;
    memcpy(p,dataptr->operator_code.c,4);
    p+=4;
    for(size_t i=0; i<24; i++)
    {
        hdlt645_di_data_desc_xx_xxxx_set_data(&hdlt645_di_data_desc_xx_xxxx,p,3,&dataptr->demand[i].demand);
        p+=3;
        hdlt645_di_data_desc_yymmddhhmm_set_data(&hdlt645_di_data_desc_yymmddhhmm,p,5,&dataptr->demand[i].time);
        p+=5;
    }

    return true;
};
static bool hdlt645_di_data_desc_event_demand_clear_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    hdlt645_di_data_desc_type_event_demand_clear_t *dataptr=(hdlt645_di_data_desc_type_event_demand_clear_t *)data;

    const uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_get_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->time);
    p+=6;
    memcpy(dataptr->operator_code.c,p,4);
    p+=4;
    for(size_t i=0; i<24; i++)
    {
        hdlt645_di_data_desc_xx_xxxx_get_data(&hdlt645_di_data_desc_xx_xxxx,p,3,&dataptr->demand[i].demand);
        p+=3;
        hdlt645_di_data_desc_yymmddhhmm_get_data(&hdlt645_di_data_desc_yymmddhhmm,p,5,&dataptr->demand[i].time);
        p+=5;
    }

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_event_demand_clear=
{
    "YYMMDDhhmmssC0C1C2C3XX.XXXX*24YYMMDDhhmm*24",
    202,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_DEMAND_CLEAR,
    hdlt645_di_data_desc_event_demand_clear_set_data,
    hdlt645_di_data_desc_event_demand_clear_get_data
};

static bool hdlt645_di_data_desc_event_event_clear_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    const hdlt645_di_data_desc_type_event_event_clear_t *dataptr=(const hdlt645_di_data_desc_type_event_event_clear_t *)data;

    uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_set_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->time);
    p+=6;
    memcpy(p,dataptr->operator_code.c,4);
    p+=4;
    memcpy(p,dataptr->di.di,4);

    return true;
};
static bool hdlt645_di_data_desc_event_event_clear_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    hdlt645_di_data_desc_type_event_event_clear_t *dataptr=(hdlt645_di_data_desc_type_event_event_clear_t *)data;

    const uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_get_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->time);
    p+=6;
    memcpy(dataptr->operator_code.c,p,4);
    p+=4;
    memcpy(dataptr->di.di,p,4);

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_event_event_clear=
{
    "YYMMDDhhmmssC0C1C2C3XXXXXXXX",
    14,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_EVENT_CLEAR,
    hdlt645_di_data_desc_event_event_clear_set_data,
    hdlt645_di_data_desc_event_event_clear_get_data
};

static bool hdlt645_di_data_desc_event_time_sync_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    const hdlt645_di_data_desc_type_event_time_sync_t *dataptr=(const hdlt645_di_data_desc_type_event_time_sync_t *)data;

    uint8_t *p=buffer;

    memcpy(p,dataptr->operator_code.c,4);
    p+=4;
    hdlt645_di_data_desc_yymmddhhmmss_set_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->before);
    p+=6;
    hdlt645_di_data_desc_yymmddhhmmss_set_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->after);

    return true;
};
static bool hdlt645_di_data_desc_event_time_sync_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    hdlt645_di_data_desc_type_event_time_sync_t *dataptr=(hdlt645_di_data_desc_type_event_time_sync_t *)data;

    const uint8_t *p=buffer;

    memcpy(dataptr->operator_code.c,p,4);
    p+=4;
    hdlt645_di_data_desc_yymmddhhmmss_get_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->before);
    p+=6;
    hdlt645_di_data_desc_yymmddhhmmss_get_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->after);

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_event_time_sync=
{
    "C0C1C2C3YYMMDDhhmmssYYMMDDhhmmss",
    16,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_TIME_SYNC,
    hdlt645_di_data_desc_event_time_sync_set_data,
    hdlt645_di_data_desc_event_time_sync_get_data
};

static bool hdlt645_di_data_desc_event_settlement_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    const hdlt645_di_data_desc_type_event_settlement_t *dataptr=(const hdlt645_di_data_desc_type_event_settlement_t *)data;

    uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_set_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->time);
    p+=6;
    memcpy(p,dataptr->operator_code.c,4);
    p+=4;
    for(size_t i=0; i<3; i++)
    {
        hdlt645_di_data_desc_ddhh_set_data(&hdlt645_di_data_desc_ddhh,p,2,&dataptr->day[i]);
        p+=2;
    }

    return true;
};
static bool hdlt645_di_data_desc_event_settlement_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    hdlt645_di_data_desc_type_event_settlement_t *dataptr=(hdlt645_di_data_desc_type_event_settlement_t *)data;

    const uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_get_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->time);
    p+=6;
    memcpy(dataptr->operator_code.c,p,4);
    p+=4;
    for(size_t i=0; i<3; i++)
    {
        hdlt645_di_data_desc_ddhh_get_data(&hdlt645_di_data_desc_ddhh,p,2,&dataptr->day[i]);
        p+=2;
    }

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_event_settlement=
{
    "YYMMDDhhmmssC0C1C2C3DDhh*3",
    16,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_SETTLEMENT,
    hdlt645_di_data_desc_event_settlement_set_data,
    hdlt645_di_data_desc_event_settlement_get_data
};

static bool hdlt645_di_data_desc_event_time_table_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    const hdlt645_di_data_desc_type_event_time_table_t *dataptr=(const hdlt645_di_data_desc_type_event_time_table_t *)data;

    uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_set_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->time);
    p+=6;
    memcpy(p,dataptr->operator_code.c,4);
    p+=4;
    for(size_t i=0; i<2; i++)
    {
        for(size_t j=0; j<8; j++)
        {
            for(size_t k=0; k<14; k++)
            {
                hdlt645_di_data_desc_hhmmnn_set_data(&hdlt645_di_data_desc_hhmmnn,p,3,&dataptr->table[i][j][k]);
                p+=3;
            }
        }
    }

    return true;
};
static bool hdlt645_di_data_desc_event_time_table_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    hdlt645_di_data_desc_type_event_time_table_t *dataptr=(hdlt645_di_data_desc_type_event_time_table_t *)data;

    const uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_get_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->time);
    p+=6;
    memcpy(dataptr->operator_code.c,p,4);
    p+=4;
    for(size_t i=0; i<2; i++)
    {
        for(size_t j=0; j<8; j++)
        {
            for(size_t k=0; k<14; k++)
            {
                hdlt645_di_data_desc_hhmmnn_get_data(&hdlt645_di_data_desc_hhmmnn,p,3,&dataptr->table[i][j][k]);
                p+=3;
            }
        }
    }

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_event_time_table=
{
    "YYMMDDhhmmssC0C1C2C3hhmmNN*224",
    682,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_TIME_TABLE,
    hdlt645_di_data_desc_event_time_table_set_data,
    hdlt645_di_data_desc_event_time_table_get_data
};

static bool hdlt645_di_data_desc_event_timezone_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    const hdlt645_di_data_desc_type_event_timezone_t *dataptr=(const hdlt645_di_data_desc_type_event_timezone_t *)data;

    uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_set_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->time);
    p+=6;
    memcpy(p,dataptr->operator_code.c,4);
    p+=4;
    for(size_t i=0; i<2; i++)
    {
        for(size_t j=0; j<14; j++)
        {
            hdlt645_di_data_desc_mmddnn_set_data(&hdlt645_di_data_desc_mmddnn,p,3,&dataptr->table[i][j]);
            p+=3;
        }
    }

    return true;
};
static bool hdlt645_di_data_desc_event_timezone_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    hdlt645_di_data_desc_type_event_timezone_t *dataptr=(hdlt645_di_data_desc_type_event_timezone_t *)data;

    const uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_get_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->time);
    p+=6;
    memcpy(dataptr->operator_code.c,p,4);
    p+=4;
    for(size_t i=0; i<2; i++)
    {
        for(size_t j=0; j<14; j++)
        {
            hdlt645_di_data_desc_mmddnn_get_data(&hdlt645_di_data_desc_mmddnn,p,3,&dataptr->table[i][j]);
            p+=3;
        }
    }

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_event_timezone=
{
    "YYMMDDhhmmssC0C1C2C3MMDDNN*28",
    94,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_TIMEZONE,
    hdlt645_di_data_desc_event_timezone_set_data,
    hdlt645_di_data_desc_event_timezone_get_data
};

static bool hdlt645_di_data_desc_event_weekday_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    const hdlt645_di_data_desc_type_event_weekday_t *dataptr=(const hdlt645_di_data_desc_type_event_weekday_t *)data;

    uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_set_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->time);
    p+=6;
    memcpy(p,dataptr->operator_code.c,4);
    p+=4;
    hdlt645_di_data_desc_nn_set_data(&hdlt645_di_data_desc_nn,p,1,&dataptr->weekday);

    return true;
};
static bool hdlt645_di_data_desc_event_weekday_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    hdlt645_di_data_desc_type_event_weekday_t *dataptr=(hdlt645_di_data_desc_type_event_weekday_t *)data;

    const uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_get_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->time);
    p+=6;
    memcpy(dataptr->operator_code.c,p,4);
    p+=4;
    hdlt645_di_data_desc_nn_get_data(&hdlt645_di_data_desc_nn,p,1,&dataptr->weekday);

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_event_weekday=
{
    "YYMMDDhhmmssC0C1C2C3NN",
    11,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_WEEKDAY,
    hdlt645_di_data_desc_event_weekday_set_data,
    hdlt645_di_data_desc_event_weekday_get_data
};

static bool hdlt645_di_data_desc_event_holiday_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    const hdlt645_di_data_desc_type_event_holiday_t *dataptr=(const hdlt645_di_data_desc_type_event_holiday_t *)data;

    uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_set_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->time);
    p+=6;
    memcpy(p,dataptr->operator_code.c,4);
    p+=4;
    for(size_t i=0; i<254; i++)
    {
        hdlt645_di_data_desc_yymmddnn_set_data(&hdlt645_di_data_desc_yymmddnn,p,4,&dataptr->holiday[i]);
        p+=4;
    }

    return true;
};
static bool hdlt645_di_data_desc_event_holiday_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    hdlt645_di_data_desc_type_event_holiday_t *dataptr=(hdlt645_di_data_desc_type_event_holiday_t *)data;

    const uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_get_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->time);
    p+=6;
    memcpy(dataptr->operator_code.c,p,4);
    p+=4;
    for(size_t i=0; i<254; i++)
    {
        hdlt645_di_data_desc_yymmddnn_get_data(&hdlt645_di_data_desc_yymmddnn,p,4,&dataptr->holiday[i]);
        p+=4;
    }

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_event_holiday=
{
    "YYMMDDhhmmssC0C1C2C3YYMMDDNN*254",
    1026,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_HOLIDAY,
    hdlt645_di_data_desc_event_holiday_set_data,
    hdlt645_di_data_desc_event_holiday_get_data
};

static bool hdlt645_di_data_desc_event_combination_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    const hdlt645_di_data_desc_type_event_combination_t *dataptr=(const hdlt645_di_data_desc_type_event_combination_t *)data;

    uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_set_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->time);
    p+=6;
    memcpy(p,dataptr->operator_code.c,4);
    p+=4;
    hdlt645_di_data_desc_nn_set_data(&hdlt645_di_data_desc_nn,p,1,&dataptr->mode);

    return true;
};
static bool hdlt645_di_data_desc_event_combination_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    hdlt645_di_data_desc_type_event_combination_t *dataptr=(hdlt645_di_data_desc_type_event_combination_t *)data;

    const uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_get_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->time);
    p+=6;
    memcpy(dataptr->operator_code.c,p,4);
    p+=4;
    hdlt645_di_data_desc_nn_get_data(&hdlt645_di_data_desc_nn,p,1,&dataptr->mode);

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_event_combination=
{
    "YYMMDDhhmmssC0C1C2C3NN",
    11,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_COMBINATION,
    hdlt645_di_data_desc_event_combination_set_data,
    hdlt645_di_data_desc_event_combination_get_data
};

static bool hdlt645_di_data_desc_event_cover_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    const hdlt645_di_data_desc_type_event_cover_t *dataptr=(const hdlt645_di_data_desc_type_event_cover_t *)data;

    uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_set_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->start);
    p+=6;
    hdlt645_di_data_desc_yymmddhhmmss_set_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->end);
    p+=6;
    for(size_t i=0; i<12; i++)
    {
        hdlt645_di_data_desc_xx_xxxx_set_data(&hdlt645_di_data_desc_xx_xxxx,p,4,&dataptr->energy[i]);
        p+=4;
    }

    return true;
};
static bool hdlt645_di_data_desc_event_cover_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    hdlt645_di_data_desc_type_event_cover_t *dataptr=(hdlt645_di_data_desc_type_event_cover_t *)data;

    const uint8_t *p=buffer;

    hdlt645_di_data_desc_yymmddhhmmss_get_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->start);
    p+=6;
    hdlt645_di_data_desc_yymmddhhmmss_get_data(&hdlt645_di_data_desc_yymmddhhmmss,p,6,&dataptr->end);
    p+=6;
    for(size_t i=0; i<12; i++)
    {
        hdlt645_di_data_desc_xx_xxxx_get_data(&hdlt645_di_data_desc_xx_xxxx,p,4,&dataptr->energy[i]);
        p+=4;
    }

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_event_cover=
{
    "YYMMDDhhmmssYYMMDDhhmmssXXXXXX.XX*12",
    60,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_COVER,
    hdlt645_di_data_desc_event_cover_set_data,
    hdlt645_di_data_desc_event_cover_get_data
};

static bool hdlt645_di_data_desc_freeze_variable_set_data(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    const hdlt645_di_data_desc_type_freeze_variable_t *dataptr=(const hdlt645_di_data_desc_type_freeze_variable_t *)data;

    uint8_t *p=buffer;

    for(size_t i=0; i<8; i++)
    {
        hdlt645_di_data_desc_xx_xxxx_set_data(&hdlt645_di_data_desc_xx_xxxx,p,3,&dataptr->data[i]);
        p+=3;
    }

    return true;
};
static bool hdlt645_di_data_desc_freeze_variable_get_data(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data)
{
    if(desc == NULL || buffer == NULL || buffer_size < desc->data_length || data == NULL)
    {
        return false;
    }

    hdlt645_di_data_desc_type_freeze_variable_t *dataptr=(hdlt645_di_data_desc_type_freeze_variable_t *)data;

    const uint8_t *p=buffer;

    for(size_t i=0; i<8; i++)
    {
        hdlt645_di_data_desc_xx_xxxx_get_data(&hdlt645_di_data_desc_xx_xxxx,p,3,&dataptr->data[i]);
        p+=3;
    }

    return true;
};

HDEFAULTS_RO_ATTRIBUTE
const hdlt645_di_data_desc_t hdlt645_di_data_desc_freeze_variable=
{
    "XX.XXXX*8",
    24,
    HDLT645_DI_DATA_DESC_TYPE_FREEZE_VARIABLE,
    hdlt645_di_data_desc_freeze_variable_set_data,
    hdlt645_di_data_desc_freeze_variable_get_data
};

const hdlt645_di_data_desc_t * hdlt645_di_data_desc_get(hdlt645_data_di_t *di)
{
    const hdlt645_di_data_desc_t *ret=NULL;
    if(di==NULL)
    {
        return ret;
    }

    /*
     * 数据标识中任意字节为0xFF表示数据块(通配符)，由用户自行处理
     */
    for(size_t i=0; i<sizeof(di->di); i++)
    {
        if(di->di[i] == HDLT645_DATA_DI_WILDCARD)
        {
            return ret;
        }
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
    case 0x02:
    {
        /*
         * DL/T 645 A.3 变量数据标识编码表
         */
        switch(di->di[2])
        {
        case 0x01:
        {
            ret=&hdlt645_di_data_desc_xxx_x;
        }
        break;
        case 0x02:
        {
            ret=&hdlt645_di_data_desc_xxx_xxx_signed;
        }
        break;
        case 0x03:
        case 0x04:
        case 0x05:
        {
            ret=&hdlt645_di_data_desc_xx_xxxx_signed;
        }
        break;
        case 0x06:
        {
            ret=&hdlt645_di_data_desc_x_xxx_signed;
        }
        break;
        case 0x07:
        {
            ret=&hdlt645_di_data_desc_xxx_x;
        }
        break;
        case 0x08:
        case 0x09:
        case 0x0A:
        case 0x0B:
        {
            ret=&hdlt645_di_data_desc_xx_xx;
        }
        break;
        case 0x80:
        {
            switch(di->di[0])
            {
            case 0x01:
            {
                ret=&hdlt645_di_data_desc_xxx_xxx_signed;
            }
            break;
            case 0x02:
            {
                ret=&hdlt645_di_data_desc_xx_xx;
            }
            break;
            case 0x03:
            {
                ret=&hdlt645_di_data_desc_xx_xxxx;
            }
            break;
            case 0x04:
            case 0x05:
            case 0x06:
            {
                ret=&hdlt645_di_data_desc_xx_xxxx_signed;
            }
            break;
            case 0x07:
            {
                ret=&hdlt645_di_data_desc_xxx_x_signed;
            }
            break;
            case 0x08:
            case 0x09:
            {
                ret=&hdlt645_di_data_desc_xx_xx;
            }
            break;
            case 0x0A:
            {
                ret=&hdlt645_di_data_desc_xxxxxxxx;
            }
            break;
            default:
            {

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
    }
    break;
    case 0x03:
    {
        /*
         * DL/T 645 A.4 事件记录数据标识编码表
         */
        switch(di->di[2])
        {
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        {
            if(di->di[1] == 0x00)
            {
                ret=&hdlt645_di_data_desc_event_summary_abc;
            }
            else
            {
                ret=&hdlt645_di_data_desc_event_phase_voltage;
            }
        }
        break;
        case 0x05:
        {
            if(di->di[0] == 0x00)
            {
                ret=&hdlt645_di_data_desc_event_summary;
            }
            else
            {
                ret=&hdlt645_di_data_desc_event_full_voltage_loss;
            }
        }
        break;
        case 0x06:
        {
            if(di->di[0] == 0x00)
            {
                ret=&hdlt645_di_data_desc_event_summary;
            }
            else
            {
                ret=&hdlt645_di_data_desc_event_two_times;
            }
        }
        break;
        case 0x07:
        case 0x08:
        {
            if(di->di[0] == 0x00)
            {
                ret=&hdlt645_di_data_desc_event_summary;
            }
            else
            {
                ret=&hdlt645_di_data_desc_event_reverse;
            }
        }
        break;
        case 0x09:
        case 0x0A:
        {
            if(di->di[0] == 0x00)
            {
                ret=&hdlt645_di_data_desc_event_summary;
            }
            else
            {
                ret=&hdlt645_di_data_desc_event_imbalance;
            }
        }
        break;
        case 0x0B:
        case 0x0C:
        case 0x0D:
        {
            if(di->di[1] == 0x00)
            {
                ret=&hdlt645_di_data_desc_event_summary_abc;
            }
            else
            {
                ret=&hdlt645_di_data_desc_event_phase_current;
            }
        }
        break;
        case 0x0E:
        case 0x0F:
        {
            if(di->di[1] == 0x00)
            {
                ret=&hdlt645_di_data_desc_event_summary_abc;
            }
            else
            {
                ret=&hdlt645_di_data_desc_event_reverse;
            }
        }
        break;
        case 0x10:
        {
            ret=&hdlt645_di_data_desc_event_voltage_qualification;
        }
        break;
        case 0x11:
        {
            if(di->di[0] == 0x00)
            {
                ret=&hdlt645_di_data_desc_xxxxxx;
            }
            else
            {
                ret=&hdlt645_di_data_desc_event_two_times;
            }
        }
        break;
        case 0x12:
        {
            if(di->di[1] == 0x00)
            {
                ret=&hdlt645_di_data_desc_event_summary_abc;
            }
            else
            {
                ret=&hdlt645_di_data_desc_event_demand_exceed;
            }
        }
        break;
        case 0x30:
        {
            switch(di->di[1])
            {
            case 0x00:
            {
                if(di->di[0] == 0x00)
                {
                    ret=&hdlt645_di_data_desc_xxxxxx;
                }
                else
                {
                    ret=&hdlt645_di_data_desc_event_programming;
                }
            }
            break;
            case 0x01:
            {
                if(di->di[0] == 0x00)
                {
                    ret=&hdlt645_di_data_desc_xxxxxx;
                }
                else
                {
                    ret=&hdlt645_di_data_desc_event_meter_clear;
                }
            }
            break;
            case 0x02:
            {
                if(di->di[0] == 0x00)
                {
                    ret=&hdlt645_di_data_desc_xxxxxx;
                }
                else
                {
                    ret=&hdlt645_di_data_desc_event_demand_clear;
                }
            }
            break;
            case 0x03:
            {
                if(di->di[0] == 0x00)
                {
                    ret=&hdlt645_di_data_desc_xxxxxx;
                }
                else
                {
                    ret=&hdlt645_di_data_desc_event_event_clear;
                }
            }
            break;
            case 0x04:
            {
                if(di->di[0] == 0x00)
                {
                    ret=&hdlt645_di_data_desc_xxxxxx;
                }
                else
                {
                    ret=&hdlt645_di_data_desc_event_time_sync;
                }
            }
            break;
            case 0x05:
            {
                if(di->di[0] == 0x00)
                {
                    ret=&hdlt645_di_data_desc_xxxxxx;
                }
                else
                {
                    ret=&hdlt645_di_data_desc_event_time_table;
                }
            }
            break;
            case 0x06:
            {
                if(di->di[0] == 0x00)
                {
                    ret=&hdlt645_di_data_desc_xxxxxx;
                }
                else
                {
                    ret=&hdlt645_di_data_desc_event_timezone;
                }
            }
            break;
            case 0x07:
            {
                if(di->di[0] == 0x00)
                {
                    ret=&hdlt645_di_data_desc_xxxxxx;
                }
                else
                {
                    ret=&hdlt645_di_data_desc_event_weekday;
                }
            }
            break;
            case 0x08:
            {
                if(di->di[0] == 0x00)
                {
                    ret=&hdlt645_di_data_desc_xxxxxx;
                }
                else
                {
                    ret=&hdlt645_di_data_desc_event_holiday;
                }
            }
            break;
            case 0x09:
            case 0x0A:
            case 0x0B:
            {
                if(di->di[0] == 0x00)
                {
                    ret=&hdlt645_di_data_desc_xxxxxx;
                }
                else
                {
                    ret=&hdlt645_di_data_desc_event_combination;
                }
            }
            break;
            case 0x0C:
            {
                if(di->di[0] == 0x00)
                {
                    ret=&hdlt645_di_data_desc_xxxxxx;
                }
                else
                {
                    ret=&hdlt645_di_data_desc_event_settlement;
                }
            }
            break;
            case 0x0D:
            case 0x0E:
            {
                if(di->di[0] == 0x00)
                {
                    ret=&hdlt645_di_data_desc_xxxxxx;
                }
                else
                {
                    ret=&hdlt645_di_data_desc_event_cover;
                }
            }
            break;
            default:
            {

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
    }
    break;
    case 0x04:
    {
        /*
         * DL/T 645 A.5 参变量数据标识编码表
         */
        switch(di->di[2])
        {
        case 0x00:
        {
            switch(di->di[1])
            {
            case 0x01:
            {
                switch(di->di[0])
                {
                case 0x01:
                {
                    ret=&hdlt645_di_data_desc_yymmddww;
                }
                break;
                case 0x02:
                {
                    ret=&hdlt645_di_data_desc_hhmmss;
                }
                break;
                case 0x03:
                case 0x04:
                {
                    ret=&hdlt645_di_data_desc_nn;
                }
                break;
                case 0x05:
                {
                    ret=&hdlt645_di_data_desc_xxxx;
                }
                break;
                case 0x06:
                case 0x07:
                {
                    ret=&hdlt645_di_data_desc_yymmddhhmm;
                }
                break;
                default:
                {

                }
                break;
                }
            }
            break;
            case 0x02:
            {
                switch(di->di[0])
                {
                case 0x01:
                case 0x02:
                case 0x03:
                case 0x04:
                case 0x06:
                {
                    ret=&hdlt645_di_data_desc_nn;
                }
                break;
                case 0x05:
                {
                    ret=&hdlt645_di_data_desc_nnnn;
                }
                break;
                default:
                {

                }
                break;
                }
            }
            break;
            case 0x03:
            {
                switch(di->di[0])
                {
                case 0x01:
                case 0x02:
                case 0x03:
                case 0x04:
                case 0x05:
                {
                    ret=&hdlt645_di_data_desc_nn;
                }
                break;
                default:
                {

                }
                break;
                }
            }
            break;
            case 0x04:
            {
                switch(di->di[0])
                {
                case 0x01:
                case 0x02:
                {
                    ret=&hdlt645_di_data_desc_nnnnnnnnnnnn;
                }
                break;
                case 0x03:
                {
                    ret=&hdlt645_di_data_desc_ascii_32;
                }
                break;
                case 0x04:
                case 0x05:
                case 0x06:
                {
                    ret=&hdlt645_di_data_desc_ascii_6;
                }
                break;
                case 0x07:
                case 0x08:
                {
                    ret=&hdlt645_di_data_desc_ascii_4;
                }
                break;
                case 0x09:
                case 0x0A:
                {
                    ret=&hdlt645_di_data_desc_xxxxxx;
                }
                break;
                case 0x0B:
                case 0x0C:
                {
                    ret=&hdlt645_di_data_desc_ascii_10;
                }
                break;
                case 0x0D:
                {
                    ret=&hdlt645_di_data_desc_ascii_16;
                }
                break;
                default:
                {

                }
                break;
                }
            }
            break;
            case 0x05:
            {
                switch(di->di[0])
                {
                case 0x01:
                case 0x02:
                case 0x03:
                case 0x04:
                case 0x05:
                case 0x06:
                case 0x07:
                {
                    ret=&hdlt645_di_data_desc_status_word;
                }
                break;
                default:
                {

                }
                break;
                }
            }
            break;
            case 0x06:
            {
                switch(di->di[0])
                {
                case 0x01:
                case 0x02:
                case 0x03:
                {
                    ret=&hdlt645_di_data_desc_nn;
                }
                break;
                default:
                {

                }
                break;
                }
            }
            break;
            case 0x07:
            {
                switch(di->di[0])
                {
                case 0x01:
                case 0x02:
                case 0x03:
                case 0x04:
                case 0x05:
                {
                    ret=&hdlt645_di_data_desc_nn;
                }
                break;
                default:
                {

                }
                break;
                }
            }
            break;
            case 0x08:
            {
                switch(di->di[0])
                {
                case 0x01:
                case 0x02:
                {
                    ret=&hdlt645_di_data_desc_nn;
                }
                break;
                default:
                {

                }
                break;
                }
            }
            break;
            case 0x09:
            {
                switch(di->di[0])
                {
                case 0x01:
                case 0x02:
                {
                    ret=&hdlt645_di_data_desc_nn;
                }
                break;
                default:
                {

                }
                break;
                }
            }
            break;
            case 0x0A:
            {
                switch(di->di[0])
                {
                case 0x01:
                {
                    ret=&hdlt645_di_data_desc_mmddhhmm;
                }
                break;
                case 0x02:
                case 0x03:
                case 0x04:
                case 0x05:
                case 0x06:
                case 0x07:
                {
                    ret=&hdlt645_di_data_desc_nnnn;
                }
                break;
                default:
                {

                }
                break;
                }
            }
            break;
            case 0x0B:
            {
                switch(di->di[0])
                {
                case 0x01:
                case 0x02:
                case 0x03:
                {
                    ret=&hdlt645_di_data_desc_ddhh;
                }
                break;
                default:
                {

                }
                break;
                }
            }
            break;
            case 0x0C:
            {
                switch(di->di[0])
                {
                case 0x01:
                case 0x02:
                case 0x03:
                case 0x04:
                case 0x05:
                case 0x06:
                case 0x07:
                case 0x08:
                case 0x09:
                case 0x0A:
                {
                    ret=&hdlt645_di_data_desc_nnnnnnnn;
                }
                break;
                default:
                {

                }
                break;
                }
            }
            break;
            case 0x0D:
            {
                switch(di->di[0])
                {
                case 0x01:
                case 0x02:
                case 0x03:
                case 0x04:
                case 0x05:
                case 0x06:
                case 0x07:
                case 0x08:
                case 0x09:
                case 0x0A:
                case 0x0B:
                case 0x0C:
                {
                    ret=&hdlt645_di_data_desc_n_nnn;
                }
                break;
                default:
                {

                }
                break;
                }
            }
            break;
            case 0x0E:
            {
                switch(di->di[0])
                {
                case 0x01:
                case 0x02:
                {
                    ret=&hdlt645_di_data_desc_nn_nnnn;
                }
                break;
                case 0x03:
                case 0x04:
                {
                    ret=&hdlt645_di_data_desc_nnn_n;
                }
                break;
                default:
                {

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
        }
        break;
        case 0x01:
        case 0x02:
        {
            switch(di->di[1])
            {
            case 0x00:
            {
                if(di->di[0] == 0x00)
                {
                    ret=&hdlt645_di_data_desc_mmddnn;
                }
                else
                {
                    ret=&hdlt645_di_data_desc_hhmmnn;
                }
            }
            break;
            default:
            {

            }
            break;
            }
        }
        break;
        case 0x03:
        {
            ret=&hdlt645_di_data_desc_yymmddnn;
        }
        break;
        case 0x04:
        {
            switch(di->di[1])
            {
            case 0x01:
            case 0x02:
            {
                ret=&hdlt645_di_data_desc_nnnnnnnn;
            }
            break;
            default:
            {

            }
            break;
            }
        }
        break;
        case 0x80:
        {
            switch(di->di[0])
            {
            case 0x01:
            case 0x02:
            case 0x03:
            {
                ret=&hdlt645_di_data_desc_ascii_32;
            }
            break;
            default:
            {

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
    }
    break;
    case 0x05:
    {
        /*
         * DL/T 645 A.6 冻结数据标识编码表
         * 电能、最大需量等为单个成员，多个成员由用户自行分离与组装
         */
        switch(di->di[1])
        {
        case 0x00:
        {
            ret=&hdlt645_di_data_desc_yymmddhhmm;
        }
        break;
        case 0x01:
        case 0x02:
        {
            ret=&hdlt645_di_data_desc_xxxxxx_xx;
        }
        break;
        case 0x03:
        case 0x04:
        {
            ret=&hdlt645_di_data_desc_xxxxxx_xx_signed;
        }
        break;
        case 0x05:
        case 0x06:
        case 0x07:
        case 0x08:
        {
            ret=&hdlt645_di_data_desc_xxxxxx_xx;
        }
        break;
        case 0x09:
        case 0x0A:
        {
            ret=&hdlt645_di_data_desc_xx_xxxx_yymmddhhmm;
        }
        break;
        case 0x10:
        {
            ret=&hdlt645_di_data_desc_freeze_variable;
        }
        break;
        default:
        {

        }
        break;
        }
    }
    break;
    case 0x06:
    {
        /*
         * DL/T 645 A.7 负荷记录数据标识编码表
         */
        switch(di->di[0])
        {
        case 0x00:
        case 0x02:
        {
            ret=&hdlt645_di_data_desc_nn;
        }
        break;
        case 0x01:
        {
            ret=&hdlt645_di_data_desc_yymmddhhmmnn;
        }
        break;
        default:
        {

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
    &hdlt645_di_data_desc_xxx_x,
    &hdlt645_di_data_desc_xxx_x_signed,
    &hdlt645_di_data_desc_xx_xx,
    &hdlt645_di_data_desc_xx_xx_signed,
    &hdlt645_di_data_desc_x_xxx,
    &hdlt645_di_data_desc_x_xxx_signed,
    &hdlt645_di_data_desc_xxx_xxx,
    &hdlt645_di_data_desc_xxx_xxx_signed,
    &hdlt645_di_data_desc_xx_xxxx,
    &hdlt645_di_data_desc_xx_xxxx_signed,
    &hdlt645_di_data_desc_xxxxxxxx,
    &hdlt645_di_data_desc_xxxxxxxx_signed,
    &hdlt645_di_data_desc_nn,
    &hdlt645_di_data_desc_nnnn,
    &hdlt645_di_data_desc_xxxx,
    &hdlt645_di_data_desc_xxxxxx,
    &hdlt645_di_data_desc_nnnnnnnn,
    &hdlt645_di_data_desc_nnnnnnnnnnnn,
    &hdlt645_di_data_desc_yymmddww,
    &hdlt645_di_data_desc_hhmmss,
    &hdlt645_di_data_desc_yymmddhhmm,
    &hdlt645_di_data_desc_mmddhhmm,
    &hdlt645_di_data_desc_ddhh,
    &hdlt645_di_data_desc_yymmddhhmmss,
    &hdlt645_di_data_desc_mmddnn,
    &hdlt645_di_data_desc_hhmmnn,
    &hdlt645_di_data_desc_yymmddnn,
    &hdlt645_di_data_desc_yymmddhhmmnn,
    &hdlt645_di_data_desc_ascii_3,
    &hdlt645_di_data_desc_ascii_4,
    &hdlt645_di_data_desc_ascii_6,
    &hdlt645_di_data_desc_ascii_10,
    &hdlt645_di_data_desc_ascii_16,
    &hdlt645_di_data_desc_ascii_32,
    &hdlt645_di_data_desc_n_nnn,
    &hdlt645_di_data_desc_nn_nnnn,
    &hdlt645_di_data_desc_nnn_n,
    &hdlt645_di_data_desc_status_word,
    &hdlt645_di_data_desc_xxxx_xx,
    &hdlt645_di_data_desc_event_summary_abc,
    &hdlt645_di_data_desc_event_summary,
    &hdlt645_di_data_desc_event_phase_voltage,
    &hdlt645_di_data_desc_event_phase_current,
    &hdlt645_di_data_desc_event_reverse,
    &hdlt645_di_data_desc_event_imbalance,
    &hdlt645_di_data_desc_event_full_voltage_loss,
    &hdlt645_di_data_desc_event_two_times,
    &hdlt645_di_data_desc_event_demand_exceed,
    &hdlt645_di_data_desc_event_voltage_qualification,
    &hdlt645_di_data_desc_event_programming,
    &hdlt645_di_data_desc_event_meter_clear,
    &hdlt645_di_data_desc_event_demand_clear,
    &hdlt645_di_data_desc_event_event_clear,
    &hdlt645_di_data_desc_event_time_sync,
    &hdlt645_di_data_desc_event_settlement,
    &hdlt645_di_data_desc_event_time_table,
    &hdlt645_di_data_desc_event_timezone,
    &hdlt645_di_data_desc_event_weekday,
    &hdlt645_di_data_desc_event_holiday,
    &hdlt645_di_data_desc_event_combination,
    &hdlt645_di_data_desc_event_cover,
    &hdlt645_di_data_desc_freeze_variable,
    NULL
};

const hdlt645_di_data_desc_t * const * hdlt645_di_data_desc_get_all(void)
{
    return hdlt645_di_data_desc;
}

