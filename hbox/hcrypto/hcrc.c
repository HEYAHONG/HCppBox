/***************************************************************
 * Name:      hcrc.c
 * Purpose:   实现hcrc接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-13
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hcrc.h"

const hcrc_crc8_t  hcrc_crc8_default=
{
    0,
    0x07,
    0,
    false,
    false
};

const hcrc_crc8_t  hcrc_crc8_itu=
{
    0,
    0x07,
    0x55,
    false,
    false
};

const hcrc_crc8_t  hcrc_crc8_rohc=
{
    0xFF,
    0x07,
    0x0,
    true,
    true
};

const hcrc_crc8_t  hcrc_crc8_maxim=
{
    0,
    0x31,
    0,
    true,
    true
};

/*
 * 此函数可优化
 */
static uint8_t hcrc_crc8_table_reversal(const hcrc_crc8_t *config,uint8_t index,uint8_t poly_reversal)
{
    if(config==NULL)
    {
        config=&hcrc_crc8_default;
    }
    uint8_t crc=index;
    for(size_t i=0; i<8; i++)
    {
        if((crc&0x1)>0)
        {
            crc = ((crc >> 1)^poly_reversal);
        }
        else
        {
            crc = (crc >> 1);
        }
    }
    return crc;
}

/*
 * 当输入与输出均反转时，采用反转表达式的做法
 */
static uint8_t hcrc_crc8_calculate_reversal(const hcrc_crc8_t *config,const uint8_t *data,size_t datalen)
{
    if(config==NULL)
    {
        return 0;
    }
    uint8_t crc=config->init;
    uint8_t poly=config->poly;
    {
        uint8_t temp=0;
        for(size_t i=0; i < sizeof(poly)*8 ; i++)
        {
            if(poly & (1U << (i)))
            {
                temp |= (1U << (sizeof(poly)*8-1-i));
            }
        }
        poly=temp;
    }
    if(data!=NULL && datalen > 0)
    {
        for(size_t i=0; i<datalen; i++)
        {
            uint8_t current_data=data[i];

            crc = hcrc_crc8_table_reversal(config,current_data^crc,poly);
        }
    }



    /*
     * 异或输出
     */
    crc ^= config->xorout;

    return crc;

}

/*
 * 此函数可优化
 */
static uint8_t hcrc_crc8_table(const hcrc_crc8_t *config,uint8_t index)
{
    if(config==NULL)
    {
        config=&hcrc_crc8_default;
    }
    uint8_t crc=index;
    for(size_t i=0; i<8; i++)
    {
        if((crc&0x80)>0)
        {
            crc = ((crc << 1)^config->poly);
        }
        else
        {
            crc = (crc << 1);
        }
    }
    return crc;
}

uint8_t hcrc_crc8_calculate(const hcrc_crc8_t *config,const uint8_t *data,size_t datalen)
{
    if(config==NULL)
    {
        config=&hcrc_crc8_default;
    }
    if(config->refin && config->refout)
    {
        /*
         * 使用替换的算法
         */
        return hcrc_crc8_calculate_reversal(config,data,datalen);
    }
    uint8_t crc=config->init;
    if(data!=NULL && datalen > 0)
    {
        for(size_t i=0; i<datalen; i++)
        {
            uint8_t current_data=data[i];
            /*
             * 输入数据反转
             */
            if(config->refin)
            {
                uint8_t temp=0;
                for(size_t i=0; i < sizeof(current_data)*8 ; i++)
                {
                    if(current_data & (1U << (i)))
                    {
                        temp |= (1U << (sizeof(current_data)*8-1-i));
                    }
                }
                current_data=temp;
            }

            crc = hcrc_crc8_table(config,current_data^crc);
        }
    }

    /*
     * 输出结果反转
     */
    if(config->refout)
    {
        uint8_t temp=0;
        for(size_t i=0; i < sizeof(crc)*8 ; i++)
        {
            if(crc & (1U << (i)))
            {
                temp |= (1U << (sizeof(crc)*8-1-i));
            }
        }
        crc=temp;
    }

    /*
     * 异或输出
     */
    crc ^= config->xorout;

    return crc;
}

bool hcrc_crc8_check(const hcrc_crc8_t *config,const uint8_t *data,size_t datalen,uint8_t check)
{
    return check==hcrc_crc8_calculate(config,data,datalen);
}

const hcrc_crc16_t  hcrc_crc16_ibm=
{
    0x0000,
    0x8005,
    0x0000,
    true,
    true
};
const hcrc_crc16_t  hcrc_crc16_maxim=
{
    0x0000,
    0x8005,
    0xFFFF,
    true,
    true
};
const hcrc_crc16_t  hcrc_crc16_usb=
{
    0xFFFF,
    0x8005,
    0xFFFF,
    true,
    true
};
const hcrc_crc16_t  hcrc_crc16_modbus=
{
    0xFFFF,
    0x8005,
    0x0000,
    true,
    true
};
const hcrc_crc16_t  hcrc_crc16_ccitt=
{
    0x0000,
    0x1021,
    0x0000,
    true,
    true
};
const hcrc_crc16_t  hcrc_crc16_ccitt_false=
{
    0xFFFF,
    0x1021,
    0x0000,
    false,
    false
};
const hcrc_crc16_t  hcrc_crc16_x25=
{
    0xFFFF,
    0x1021,
    0xFFFF,
    true,
    true
};
const hcrc_crc16_t  hcrc_crc16_xmodem=
{
    0x0000,
    0x1021,
    0x0000,
    false,
    false
};
const hcrc_crc16_t  hcrc_crc16_dnp=
{
    0x0000,
    0x3D65,
    0xFFFF,
    true,
    true
};

/*
 * 当输入与输出均反转时，采用反转表达式的做法
 */
static uint16_t hcrc_crc16_calculate_reversal(const hcrc_crc16_t *config,const uint8_t *data,size_t datalen)
{
    if(config==NULL)
    {
        return 0;
    }
    uint16_t crc=config->init;
    /*
     * 反转表达式
     */
    uint16_t poly=config->poly;
    {
        uint16_t temp=0;
        for(size_t i=0; i < sizeof(poly)*8 ; i++)
        {
            if(poly & (1U << (i)))
            {
                temp |= (1U << (sizeof(poly)*8-1-i));
            }
        }
        poly=temp;
    }
    if(data!=NULL && datalen > 0)
    {
        for(size_t i=0; i<datalen; i++)
        {
            uint8_t current_data=data[i];
            crc ^= (((uint16_t)current_data));

            for(size_t j=0; j<8; j++)
            {
                if((crc & 1) > 0)
                {
                    crc = ((crc >> 1) ^ poly);
                }
                else
                {
                    crc = (crc >> 1);
                }
            }
        }
    }


    /*
     * 异或输出
     */
    crc ^= config->xorout;

    return crc;

}

uint16_t hcrc_crc16_calculate(const hcrc_crc16_t *config,const uint8_t *data,size_t datalen)
{
    if(config==NULL)
    {
        config=&hcrc_crc16_modbus;
    }
    if(config->refin && config->refout)
    {
        /*
         * 使用替换的算法
         */
        return  hcrc_crc16_calculate_reversal(config,data,datalen);
    }
    uint16_t crc=config->init;
    if(data!=NULL && datalen > 0)
    {
        for(size_t i=0; i<datalen; i++)
        {
            uint8_t current_data=data[i];
            /*
             * 输入数据反转
             */
            if(config->refin)
            {
                uint8_t temp=0;
                for(size_t i=0; i < sizeof(current_data)*8 ; i++)
                {
                    if(current_data & (1U << (i)))
                    {
                        temp |= (1U << (sizeof(current_data)*8-1-i));
                    }
                }
                current_data=temp;
            }

            crc ^= (((uint16_t)current_data) <<8);

            for(size_t j=0; j<8; j++)
            {
                if((crc & 0x8000) > 0)
                {
                    crc = ((crc << 1) ^ config->poly);
                }
                else
                {
                    crc = (crc << 1);
                }
            }
        }
    }

    /*
     * 输出结果反转
     */
    if(config->refout)
    {
        uint16_t temp=0;
        for(size_t i=0; i < sizeof(crc)*8 ; i++)
        {
            if(crc & (1U << (i)))
            {
                temp |= (1U << (sizeof(crc)*8-1-i));
            }
        }
        crc=temp;
    }

    /*
     * 异或输出
     */
    crc ^= config->xorout;

    return crc;

}

bool hcrc_crc16_check(const hcrc_crc16_t *config,const uint8_t *data,size_t datalen,uint16_t check)
{
    return check==hcrc_crc16_calculate(config,data,datalen);
}


const hcrc_crc32_t hcrc_crc32_default=
{
    0xFFFFFFFF,
    0x04C11DB7,
    0xFFFFFFFF,
    true,
    true
};

const hcrc_crc32_t hcrc_crc32_mpeg_2=
{
    0xFFFFFFFF,
    0x04C11DB7,
    0x00000000,
    false,
    false
};

/*
 * 当输入与输出均反转时，采用反转表达式的做法
 */
static uint32_t hcrc_crc32_calculate_reversal(const hcrc_crc32_t *config,const uint8_t *data,size_t datalen)
{
    if(config == NULL)
    {
        return 0;
    }
    uint32_t crc=config->init;
    uint32_t poly=config->poly;
    /*
     * 反转表达式
     */
    {
        uint32_t temp=0;
        for(size_t i=0; i < sizeof(poly)*8 ; i++)
        {
            if(poly & (1U << (i)))
            {
                temp |= (1U << (sizeof(poly)*8-1-i));
            }
        }
        poly=temp;
    }

    if(data!=NULL && datalen > 0)
    {
        for(size_t i=0; i<datalen; i++)
        {
            uint8_t current_data=data[i];

            crc ^= ((uint32_t)current_data);

            for(size_t j=0; j<8; j++)
            {
                if((crc & 1) > 0)
                {
                    crc = ((crc >> 1) ^ poly);
                }
                else
                {
                    crc = (crc >> 1);
                }
            }
        }
    }



    /*
     * 异或输出
     */
    crc ^= config->xorout;

    return crc;

}

uint32_t hcrc_crc32_calculate(const hcrc_crc32_t *config,const uint8_t *data,size_t datalen)
{
    if(config==NULL)
    {
        config=&hcrc_crc32_default;
    }

    if(config->refin && config ->refout)
    {
        /*
         * 使用替换的算法
         */
        return hcrc_crc32_calculate_reversal(config,data,datalen);
    }

    uint32_t crc=config->init;
    if(data!=NULL && datalen > 0)
    {
        for(size_t i=0; i<datalen; i++)
        {
            uint8_t current_data=data[i];
            /*
             * 输入数据反转
             */
            if(config->refin)
            {
                uint8_t temp=0;
                for(size_t i=0; i < sizeof(current_data)*8 ; i++)
                {
                    if(current_data & (1U << (i)))
                    {
                        temp |= (1U << (sizeof(current_data)*8-1-i));
                    }
                }
                current_data=temp;
            }

            crc ^= (((uint32_t)current_data) <<24);

            for(size_t j=0; j<8; j++)
            {
                if((crc & 0x80000000) > 0)
                {
                    crc = ((crc << 1) ^ config->poly);
                }
                else
                {
                    crc = (crc << 1);
                }
            }
        }
    }

    /*
     * 输出结果反转
     */
    if(config->refout)
    {
        uint32_t temp=0;
        for(size_t i=0; i < sizeof(crc)*8 ; i++)
        {
            if(crc & (1U << (i)))
            {
                temp |= (1U << (sizeof(crc)*8-1-i));
            }
        }
        crc=temp;
    }

    /*
     * 异或输出
     */
    crc ^= config->xorout;

    return crc;

}

bool hcrc_crc32_check(const hcrc_crc32_t *config,const uint8_t *data,size_t datalen,uint32_t check)
{
    return check==hcrc_crc32_calculate(config,data,datalen);
}

static const uint16_t hcrc_crc16_modbus_table[256]=
{
    0x0000,0xC0C1,0xC181,0x0140,0xC301,0x03C0,0x0280,0xC241,
    0xC601,0x06C0,0x0780,0xC741,0x0500,0xC5C1,0xC481,0x0440,
    0xCC01,0x0CC0,0x0D80,0xCD41,0x0F00,0xCFC1,0xCE81,0x0E40,
    0x0A00,0xCAC1,0xCB81,0x0B40,0xC901,0x09C0,0x0880,0xC841,
    0xD801,0x18C0,0x1980,0xD941,0x1B00,0xDBC1,0xDA81,0x1A40,
    0x1E00,0xDEC1,0xDF81,0x1F40,0xDD01,0x1DC0,0x1C80,0xDC41,
    0x1400,0xD4C1,0xD581,0x1540,0xD701,0x17C0,0x1680,0xD641,
    0xD201,0x12C0,0x1380,0xD341,0x1100,0xD1C1,0xD081,0x1040,
    0xF001,0x30C0,0x3180,0xF141,0x3300,0xF3C1,0xF281,0x3240,
    0x3600,0xF6C1,0xF781,0x3740,0xF501,0x35C0,0x3480,0xF441,
    0x3C00,0xFCC1,0xFD81,0x3D40,0xFF01,0x3FC0,0x3E80,0xFE41,
    0xFA01,0x3AC0,0x3B80,0xFB41,0x3900,0xF9C1,0xF881,0x3840,
    0x2800,0xE8C1,0xE981,0x2940,0xEB01,0x2BC0,0x2A80,0xEA41,
    0xEE01,0x2EC0,0x2F80,0xEF41,0x2D00,0xEDC1,0xEC81,0x2C40,
    0xE401,0x24C0,0x2580,0xE541,0x2700,0xE7C1,0xE681,0x2640,
    0x2200,0xE2C1,0xE381,0x2340,0xE101,0x21C0,0x2080,0xE041,
    0xA001,0x60C0,0x6180,0xA141,0x6300,0xA3C1,0xA281,0x6240,
    0x6600,0xA6C1,0xA781,0x6740,0xA501,0x65C0,0x6480,0xA441,
    0x6C00,0xACC1,0xAD81,0x6D40,0xAF01,0x6FC0,0x6E80,0xAE41,
    0xAA01,0x6AC0,0x6B80,0xAB41,0x6900,0xA9C1,0xA881,0x6840,
    0x7800,0xB8C1,0xB981,0x7940,0xBB01,0x7BC0,0x7A80,0xBA41,
    0xBE01,0x7EC0,0x7F80,0xBF41,0x7D00,0xBDC1,0xBC81,0x7C40,
    0xB401,0x74C0,0x7580,0xB541,0x7700,0xB7C1,0xB681,0x7640,
    0x7200,0xB2C1,0xB381,0x7340,0xB101,0x71C0,0x7080,0xB041,
    0x5000,0x90C1,0x9181,0x5140,0x9301,0x53C0,0x5280,0x9241,
    0x9601,0x56C0,0x5780,0x9741,0x5500,0x95C1,0x9481,0x5440,
    0x9C01,0x5CC0,0x5D80,0x9D41,0x5F00,0x9FC1,0x9E81,0x5E40,
    0x5A00,0x9AC1,0x9B81,0x5B40,0x9901,0x59C0,0x5880,0x9841,
    0x8801,0x48C0,0x4980,0x8941,0x4B00,0x8BC1,0x8A81,0x4A40,
    0x4E00,0x8EC1,0x8F81,0x4F40,0x8D01,0x4DC0,0x4C80,0x8C41,
    0x4400,0x84C1,0x8581,0x4540,0x8701,0x47C0,0x4680,0x8641,
    0x8201,0x42C0,0x4380,0x8341,0x4100,0x81C1,0x8081,0x4040
};

uint16_t hcrc_crc16_modbus_calculate(const uint8_t *data,size_t datalen)
{
    uint16_t crc=0xFFFF;//modbus初始值
    if(data!=NULL && datalen > 0)
    {
        for(size_t i=0; i<datalen; i++)
        {
            uint8_t current_data=data[i];
            crc= (crc>>8)^hcrc_crc16_modbus_table[((uint16_t)current_data ^ crc)&0xFF];
        }
    }
    return crc;
}

bool hcrc_crc16_modbus_check(const uint8_t *data,size_t datalen,uint16_t check)
{
    return check==hcrc_crc16_modbus_calculate(data,datalen);
}

