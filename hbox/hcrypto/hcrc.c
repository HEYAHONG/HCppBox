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
 *
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
