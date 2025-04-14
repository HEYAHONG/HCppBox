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

static const uint8_t hcrc_crc8_fast_table[256]=
{
    0x00,0x07,0x0E,0x09,0x1C,0x1B,0x12,0x15,
    0x38,0x3F,0x36,0x31,0x24,0x23,0x2A,0x2D,
    0x70,0x77,0x7E,0x79,0x6C,0x6B,0x62,0x65,
    0x48,0x4F,0x46,0x41,0x54,0x53,0x5A,0x5D,
    0xE0,0xE7,0xEE,0xE9,0xFC,0xFB,0xF2,0xF5,
    0xD8,0xDF,0xD6,0xD1,0xC4,0xC3,0xCA,0xCD,
    0x90,0x97,0x9E,0x99,0x8C,0x8B,0x82,0x85,
    0xA8,0xAF,0xA6,0xA1,0xB4,0xB3,0xBA,0xBD,
    0xC7,0xC0,0xC9,0xCE,0xDB,0xDC,0xD5,0xD2,
    0xFF,0xF8,0xF1,0xF6,0xE3,0xE4,0xED,0xEA,
    0xB7,0xB0,0xB9,0xBE,0xAB,0xAC,0xA5,0xA2,
    0x8F,0x88,0x81,0x86,0x93,0x94,0x9D,0x9A,
    0x27,0x20,0x29,0x2E,0x3B,0x3C,0x35,0x32,
    0x1F,0x18,0x11,0x16,0x03,0x04,0x0D,0x0A,
    0x57,0x50,0x59,0x5E,0x4B,0x4C,0x45,0x42,
    0x6F,0x68,0x61,0x66,0x73,0x74,0x7D,0x7A,
    0x89,0x8E,0x87,0x80,0x95,0x92,0x9B,0x9C,
    0xB1,0xB6,0xBF,0xB8,0xAD,0xAA,0xA3,0xA4,
    0xF9,0xFE,0xF7,0xF0,0xE5,0xE2,0xEB,0xEC,
    0xC1,0xC6,0xCF,0xC8,0xDD,0xDA,0xD3,0xD4,
    0x69,0x6E,0x67,0x60,0x75,0x72,0x7B,0x7C,
    0x51,0x56,0x5F,0x58,0x4D,0x4A,0x43,0x44,
    0x19,0x1E,0x17,0x10,0x05,0x02,0x0B,0x0C,
    0x21,0x26,0x2F,0x28,0x3D,0x3A,0x33,0x34,
    0x4E,0x49,0x40,0x47,0x52,0x55,0x5C,0x5B,
    0x76,0x71,0x78,0x7F,0x6A,0x6D,0x64,0x63,
    0x3E,0x39,0x30,0x37,0x22,0x25,0x2C,0x2B,
    0x06,0x01,0x08,0x0F,0x1A,0x1D,0x14,0x13,
    0xAE,0xA9,0xA0,0xA7,0xB2,0xB5,0xBC,0xBB,
    0x96,0x91,0x98,0x9F,0x8A,0x8D,0x84,0x83,
    0xDE,0xD9,0xD0,0xD7,0xC2,0xC5,0xCC,0xCB,
    0xE6,0xE1,0xE8,0xEF,0xFA,0xFD,0xF4,0xF3
};

uint8_t hcrc_crc8_fast_calculate(const uint8_t *data,size_t datalen)
{
    uint8_t crc=0;//crc8初始值
    if(data!=NULL && datalen > 0)
    {
        for(size_t i=0; i<datalen; i++)
        {
            uint8_t current_data=data[i];
            crc= hcrc_crc8_fast_table[(current_data ^ crc)&0xFF];
        }
    }
    return crc;
}

bool hcrc_crc8_fast_check(const uint8_t *data,size_t datalen,uint8_t check)
{
    return check==hcrc_crc8_fast_calculate(data,datalen);
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

