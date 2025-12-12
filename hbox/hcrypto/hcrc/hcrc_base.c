/***************************************************************
 * Name:      hcrc_base.c
 * Purpose:   实现hcrc接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-12-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hcrc_base.h"

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

hcrc_crc8_context_t hcrc_crc8_starts(const hcrc_crc8_t *config)
{
    if(config==NULL)
    {
        config=&hcrc_crc8_default;
    }
    return config->init;
}

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

hcrc_crc8_context_t hcrc_crc8_update(const hcrc_crc8_t *config,hcrc_crc8_context_t ctx,const uint8_t *data,size_t datalen)
{
    if(config==NULL)
    {
        config=&hcrc_crc8_default;
    }
    uint8_t crc=ctx;
    if(config->refin && config->refout)
    {
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

    }
    else
    {
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
    }

    return crc;
}

uint8_t hcrc_crc8_finish(const hcrc_crc8_t *config,hcrc_crc8_context_t ctx)
{
    if(config==NULL)
    {
        config=&hcrc_crc8_default;
    }
    uint8_t crc=ctx;
    if(!(config->refin && config->refout))
    {
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
    }

    /*
    * 异或输出
    */
    crc ^= config->xorout;

    return crc;
}

uint8_t hcrc_crc8_calculate(const hcrc_crc8_t *config,const uint8_t *data,size_t datalen)
{
    hcrc_crc8_context_t ctx=hcrc_crc8_starts(config);
    ctx=hcrc_crc8_update(config,ctx,data,datalen);
    return hcrc_crc8_finish(config,ctx);
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

hcrc_crc16_context_t hcrc_crc16_starts(const hcrc_crc16_t *config)
{
    if(config==NULL)
    {
        config=&hcrc_crc16_modbus;
    }
    return config->init;
}


hcrc_crc16_context_t hcrc_crc16_update(const hcrc_crc16_t *config,hcrc_crc16_context_t ctx,const uint8_t *data,size_t datalen)
{
    if(config==NULL)
    {
        config=&hcrc_crc16_modbus;
    }
    uint16_t crc=ctx;
    if(config->refin && config->refout)
    {
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

    }
    else
    {
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
    }

    return crc;
}

uint16_t hcrc_crc16_finish(const hcrc_crc16_t *config,hcrc_crc16_context_t ctx)
{
    if(config==NULL)
    {
        config=&hcrc_crc16_modbus;
    }
    uint16_t crc=ctx;
    if(!(config->refin && config->refout))
    {
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
    }

    /*
    * 异或输出
    */
    crc ^= config->xorout;

    return crc;
}

uint16_t hcrc_crc16_calculate(const hcrc_crc16_t *config,const uint8_t *data,size_t datalen)
{
    hcrc_crc16_context_t ctx=hcrc_crc16_starts(config);
    ctx=hcrc_crc16_update(config,ctx,data,datalen);
    return hcrc_crc16_finish(config,ctx);
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

const hcrc_crc32_t hcrc_crc32_bzip2=
{
    0xFFFFFFFF,
    0x04C11DB7,
    0xFFFFFFFF,
    false,
    false
};

const hcrc_crc32_t hcrc_crc32_cksum=
{
    0x00000000,
    0x04C11DB7,
    0xFFFFFFFF,
    false,
    false
};

const hcrc_crc32_t hcrc_crc32_32c=
{
    0xFFFFFFFF,
    0x1EDC6F41,
    0xFFFFFFFF,
    true,
    true
};

const hcrc_crc32_t hcrc_crc32_32d=
{
    0xFFFFFFFF,
    0xA833982B,
    0xFFFFFFFF,
    true,
    true
};

const hcrc_crc32_t hcrc_crc32_32q=
{
    0x00000000,
    0x814141AB,
    0x00000000,
    false,
    false
};

hcrc_crc32_context_t hcrc_crc32_starts(const hcrc_crc32_t *config)
{
    if(config==NULL)
    {
        config=&hcrc_crc32_default;
    }
    return config->init;
}


hcrc_crc32_context_t hcrc_crc32_update(const hcrc_crc32_t *config,hcrc_crc32_context_t ctx,const uint8_t *data,size_t datalen)
{
    if(config==NULL)
    {
        config=&hcrc_crc32_default;
    }
    uint32_t crc=ctx;
    if(config->refin && config->refout)
    {
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

    }
    else
    {
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

    }

    return crc;
}

uint32_t hcrc_crc32_finish(const hcrc_crc32_t *config,hcrc_crc32_context_t ctx)
{
    if(config==NULL)
    {
        config=&hcrc_crc32_default;
    }
    uint32_t crc=ctx;
    if(!(config->refin && config->refout))
    {
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
    }

    /*
    * 异或输出
    */
    crc ^= config->xorout;

    return crc;
}

uint32_t hcrc_crc32_calculate(const hcrc_crc32_t *config,const uint8_t *data,size_t datalen)
{
    hcrc_crc32_context_t ctx=hcrc_crc32_starts(config);
    ctx=hcrc_crc32_update(config,ctx,data,datalen);
    return hcrc_crc32_finish(config,ctx);
}

bool hcrc_crc32_check(const hcrc_crc32_t *config,const uint8_t *data,size_t datalen,uint32_t check)
{
    return check==hcrc_crc32_calculate(config,data,datalen);
}
