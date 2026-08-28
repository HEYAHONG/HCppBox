/***************************************************************
 * Name:      hdlt645_common.c
 * Purpose:   实现hdlt645_common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-08-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include "hdlt645_common.h"

bool hdlt645_bcd_addr_match(hdlt645_bcd_addr_t *addr_src,hdlt645_bcd_addr_t *addr_dst)
{
    if(addr_src == NULL || addr_dst == NULL)
    {
        return false;
    }
    bool ret=true;

    if(addr_src->A[0]!=addr_dst->A[0] && addr_src->A[0]!=HDLT645_FRAME_ADDR_WILDCARD_BYTE && addr_dst->A[0]!=HDLT645_FRAME_ADDR_WILDCARD_BYTE )
    {
        ret=false;
    }

    if(addr_src->A[1]!=addr_dst->A[1] && addr_src->A[1]!=HDLT645_FRAME_ADDR_WILDCARD_BYTE && addr_dst->A[1]!=HDLT645_FRAME_ADDR_WILDCARD_BYTE )
    {
        ret=false;
    }

    if(addr_src->A[2]!=addr_dst->A[2] && addr_src->A[2]!=HDLT645_FRAME_ADDR_WILDCARD_BYTE && addr_dst->A[2]!=HDLT645_FRAME_ADDR_WILDCARD_BYTE )
    {
        ret=false;
    }

    if(addr_src->A[3]!=addr_dst->A[3] && addr_src->A[3]!=HDLT645_FRAME_ADDR_WILDCARD_BYTE && addr_dst->A[3]!=HDLT645_FRAME_ADDR_WILDCARD_BYTE )
    {
        ret=false;
    }

    if(addr_src->A[4]!=addr_dst->A[4] && addr_src->A[4]!=HDLT645_FRAME_ADDR_WILDCARD_BYTE && addr_dst->A[4]!=HDLT645_FRAME_ADDR_WILDCARD_BYTE )
    {
        ret=false;
    }

    if(addr_src->A[5]!=addr_dst->A[5] && addr_src->A[5]!=HDLT645_FRAME_ADDR_WILDCARD_BYTE && addr_dst->A[5]!=HDLT645_FRAME_ADDR_WILDCARD_BYTE )
    {
        ret=false;
    }

    return ret;
}

void hdlt645_bcd_addr_set(hdlt645_bcd_addr_t *addr,uint64_t bcd_addr)
{
    if(addr==NULL)
    {
        return;
    }

    addr->A[0]=((bcd_addr >> (0))&0xFF);
    addr->A[1]=((bcd_addr >> (8))&0xFF);
    addr->A[2]=((bcd_addr >> (16))&0xFF);
    addr->A[3]=((bcd_addr >> (24))&0xFF);
    addr->A[4]=((bcd_addr >> (32))&0xFF);
    addr->A[5]=((bcd_addr >> (40))&0xFF);

}

uint64_t hdlt645_bcd_addr_get(hdlt645_bcd_addr_t *addr)
{
    uint64_t ret=0;
    if(addr!=NULL)
    {
        ret+=addr->A[5];
        ret<<=8;
        ret+=addr->A[4];
        ret<<=8;
        ret+=addr->A[3];
        ret<<=8;
        ret+=addr->A[2];
        ret<<=8;
        ret+=addr->A[1];
        ret<<=8;
        ret+=addr->A[0];
    }
    return ret;
}

hdlt645_control_t hdlt645_control_decode(uint8_t control_code)
{
    hdlt645_control_t ret;

    if((control_code&HDLT645_FRAME_CONTROL_DIR_MASK)==HDLT645_FRAME_CONTROL_DIR_SLAVE)
    {
        ret.dir=1;
    }
    else
    {
        ret.dir=0;
    }

    if((control_code&HDLT645_FRAME_CONTROL_ACK_MASK)==HDLT645_FRAME_CONTROL_ACK_ERROR)
    {
        ret.ack=1;
    }
    else
    {
        ret.ack=0;
    }

    if((control_code&HDLT645_FRAME_CONTROL_EXT_MASK)==HDLT645_FRAME_CONTROL_EXT_ON)
    {
        ret.ext=1;
    }
    else
    {
        ret.ext=0;
    }

    ret.fct=(control_code&HDLT645_FRAME_CONTROL_FCT_MASK);

    return ret;
}

uint8_t hdlt645_control_encode(hdlt645_control_t control)
{
    uint8_t ret=0;
    if(control.dir==1)
    {
        ret+=HDLT645_FRAME_CONTROL_DIR_SLAVE;
    }
    else
    {
        ret+=HDLT645_FRAME_CONTROL_DIR_MASTER;
    }

    if(control.ack==1)
    {
        ret+=HDLT645_FRAME_CONTROL_ACK_ERROR;
    }
    else
    {
        ret+=HDLT645_FRAME_CONTROL_ACK_NORMAL;
    }

    if(control.ext==1)
    {
        ret+=HDLT645_FRAME_CONTROL_EXT_ON;
    }
    else
    {
        ret+=HDLT645_FRAME_CONTROL_EXT_OFF;
    }

    ret+= control.fct;

    return ret;
}

void hdlt645_data_pack(uint8_t *data,size_t datalen)
{
    if(data!=NULL &&datalen > 0)
    {
        for(size_t i=0; i<datalen; i++)
        {
            data[i]+=0x33;
        }
    }
}

void hdlt645_data_unpack(uint8_t *data,size_t datalen)
{
    if(data!=NULL &&datalen > 0)
    {
        for(size_t i=0; i<datalen; i++)
        {
            data[i]-=0x33;
        }
    }
}

uint8_t hdlt645_checksum_calculate(const uint8_t *frame,size_t frame_check_len)
{
    uint8_t ret=0;
    if(frame!=NULL &&frame_check_len > 0)
    {
        for(size_t i=0; i<frame_check_len; i++)
        {
            ret+=frame[i];
        }
    }
    return ret;
}

