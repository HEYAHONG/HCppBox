/***************************************************************
 * Name:      hethernet.c
 * Purpose:   实现hethernet接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-09-30
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hethernet.h"
#include "hcrypto.h"

hethernet_hwaddr_i_g_t hethernet_hwaddr_i_g(hethernet_hwaddr_t *addr,hethernet_hwaddr_i_g_t *new_value)
{
    hethernet_hwaddr_i_g_t ret=HETHERNET_HWADDR_I_G_I;
    if(addr == NULL)
    {
        return ret;
    }

    if((addr->addr[0]&0x01)!=0)
    {
        ret=HETHERNET_HWADDR_I_G_G;
    }

    if(new_value!=NULL)
    {
        if((*new_value)!=HETHERNET_HWADDR_I_G_I)
        {
            addr->addr[0] |= 0x01;
        }
        else
        {
            addr->addr[0] &= (~(0x01));
        }
    }


    return ret;
}

hethernet_hwaddr_u_l_t hethernet_hwaddr_u_l(hethernet_hwaddr_t *addr,hethernet_hwaddr_u_l_t *new_value)
{
    hethernet_hwaddr_u_l_t ret=HETHERNET_HWADDR_U_L_U;
    if(addr == NULL)
    {
        return ret;
    }

    if((addr->addr[0]&0x02)!=0)
    {
        ret=HETHERNET_HWADDR_U_L_L;
    }

    if(new_value!=NULL)
    {
        if((*new_value)!=HETHERNET_HWADDR_U_L_U)
        {
            addr->addr[0] |= 0x02;
        }
        else
        {
            addr->addr[0] &= (~(0x02));
        }
    }


    return ret;
}

bool hethernet_hwaddr_is_broadcast(hethernet_hwaddr_t *addr)
{
    if(addr==NULL)
    {
        return false;
    }
    bool ret=true;

    if(addr->addr[0]!=HETHERNET_HWADDR_BROADCAST_ADDR0)
    {
        ret=false;
    }

    if(addr->addr[1]!=HETHERNET_HWADDR_BROADCAST_ADDR1)
    {
        ret=false;
    }

    if(addr->addr[2]!=HETHERNET_HWADDR_BROADCAST_ADDR2)
    {
        ret=false;
    }

    if(addr->addr[3]!=HETHERNET_HWADDR_BROADCAST_ADDR3)
    {
        ret=false;
    }

    if(addr->addr[4]!=HETHERNET_HWADDR_BROADCAST_ADDR4)
    {
        ret=false;
    }

    if(addr->addr[5]!=HETHERNET_HWADDR_BROADCAST_ADDR5)
    {
        ret=false;
    }

    return ret;
}

bool hethernet_hwaddr_is_ipv4_multicast(hethernet_hwaddr_t *addr)
{
    if(addr==NULL)
    {
        return false;
    }
    bool ret=true;

    if(addr->addr[0]!=HETHERNET_HWADDR_IPV4_MULTICAST_ADDR0)
    {
        ret=false;
    }

    if(addr->addr[1]!=HETHERNET_HWADDR_IPV4_MULTICAST_ADDR1)
    {
        ret=false;
    }

    if(addr->addr[2]!=HETHERNET_HWADDR_IPV4_MULTICAST_ADDR2)
    {
        ret=false;
    }

    return ret;
}

bool hethernet_hwaddr_is_ipv6_multicast(hethernet_hwaddr_t *addr)
{
    if(addr==NULL)
    {
        return false;
    }
    bool ret=true;

    if(addr->addr[0]!=HETHERNET_HWADDR_IPV6_MULTICAST_ADDR0)
    {
        ret=false;
    }

    if(addr->addr[1]!=HETHERNET_HWADDR_IPV6_MULTICAST_ADDR1)
    {
        ret=false;
    }

    return ret;
}

uint16_t hethernet_length_type_decode(const uint8_t data[2])
{
    if(data==NULL)
    {
        return 0;
    }
    uint16_t ret=data[0];
    ret <<= 8;
    ret+=data[1];
    return ret;
}

void hethernet_length_type_encode(uint8_t data[2],uint16_t length_type)
{
    if(data!=NULL)
    {
        data[0] = ((length_type >> 8)&0xFF);
        data[1] = ((length_type >> 0)&0xFF);
    }
}

uint32_t hethernet_crc32_calculate(const uint8_t *frame,size_t frame_len)
{
    return hcrc_crc32_default_table_calculate(frame,frame_len);
}
