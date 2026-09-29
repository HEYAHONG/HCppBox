/***************************************************************
 * Name:      hsntp.c
 * Purpose:   实现hsntp接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-09-29
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hsntp.h"

/* 1970  Feb 7, 2036 06:28:16 UTC (epoch 1) 之间的秒数 */
#define HSNTP_DIFF_SEC_1970_2036          ((uint32_t)2085978496L)

/*
 * hsntp纪元，用于解决2036问题
 */
HDEFAULTS_ZI_ATTRIBUTE
static int32_t hsntp_era=0;

int32_t hsntp_era_get(void)
{
    return hsntp_era;
}

void hsntp_era_set(int32_t era)
{
    hsntp_era=era;
}

htime_t hsntp_timestamp_to_time_t(uint32_t timestamp_seconds)
{
    int64_t ret=(int64_t)timestamp_seconds+hsntp_era_get()*(1ULL<<32);;
    ret+=HSNTP_DIFF_SEC_1970_2036;
    ret-=(1ULL<<32);
    return (htime_t)ret;
}

uint32_t hsntp_time_t_to_timestamp(htime_t time_seconds)
{
    uint64_t ret=time_seconds;
    ret+=(1ULL<<32);
    ret-=HSNTP_DIFF_SEC_1970_2036;
    return (uint32_t)ret;
}

void hsntp_packet_encode(hsntp_packet_t *packet)
{
    if(packet == NULL)
    {
        return;
    }

    {
        uint8_t li_vn_mode=packet->li;
        li_vn_mode <<= 3;
        li_vn_mode+=packet->vn;
        li_vn_mode <<= 3;
        li_vn_mode+=packet->mode;
        packet->msg[0]=li_vn_mode;
    }

    packet->rootdealy=hbe32toh(packet->rootdealy);
    packet->rootdisp=hbe32toh(packet->rootdisp);
    packet->refid=hbe32toh(packet->refid);
    packet->reference_timestamp=hbe64toh(packet->reference_timestamp);
    packet->origin_timestamp=hbe64toh(packet->origin_timestamp);
    packet->receive_timestamp=hbe64toh(packet->receive_timestamp);
    packet->transmit_timestamp=hbe64toh(packet->transmit_timestamp);
}

void hsntp_packet_decode(hsntp_packet_t *packet)
{
    if(packet == NULL)
    {
        return;
    }

    {
        uint8_t li_vn_mode=packet->msg[0];
        packet->li=((li_vn_mode>>6)&0x3);
        packet->vn=((li_vn_mode>>3)&0x7);
        packet->mode=((li_vn_mode>>0)&0x7);
    }

    packet->rootdealy=hhtobe32(packet->rootdealy);
    packet->rootdisp=hhtobe32(packet->rootdisp);
    packet->refid=hhtobe32(packet->refid);
    packet->reference_timestamp=hhtobe64(packet->reference_timestamp);
    packet->origin_timestamp=hhtobe64(packet->origin_timestamp);
    packet->receive_timestamp=hhtobe64(packet->receive_timestamp);
    packet->transmit_timestamp=hhtobe64(packet->transmit_timestamp);
}

hsntp_packet_t hsntp_packet_default(void)
{
    hsntp_packet_t ret;
    memset(&ret,0,sizeof(ret));
    ret.li=HSNTP_LI_UNKNOWN;
    ret.vn=HSNTP_VN_NTPV4;
    ret.mode=HSNTP_MODE_CLIENT;
    return ret;
}

hsntp_short_format_t hsntp_short_format_decode(uint32_t data)
{
    hsntp_short_format_t ret;
    ret.seconds=(data>>16)&0xFFFF;
    ret.fraction=(data&0xFFFF);
    return ret;
}

uint32_t hsntp_short_format_encode(hsntp_short_format_t data)
{
    uint32_t ret=0;
    ret=data.seconds;
    ret<<=16;
    ret+=data.fraction;
    return ret;
}

hsntp_timestamp_format_t hsntp_timestamp_format_decode(uint64_t data)
{
    hsntp_timestamp_format_t ret;
    ret.seconds=(data>>32)&0xFFFFFFFF;
    ret.fraction=(data&0xFFFFFFFF);
    return ret;
}

uint64_t hsntp_timestamp_format_encode(hsntp_timestamp_format_t data)
{
    uint64_t ret=0;
    ret=data.seconds;
    ret<<=32;
    ret+=data.fraction;
    return ret;
}

void hsntp_ntp_client_init(hsntp_ntp_client_t *ntp,hsntp_ntp_client_sendpacket_t sendpacket,hsntp_ntp_client_receivepacket_t receivepacket,hsntp_ntp_client_settimeofday_t  settimeofday,void *usr)
{
    if(ntp==NULL)
    {
        return;
    }

    ntp->sendpacket=sendpacket;
    ntp->receivepacket=receivepacket;
    ntp->settimeofday=settimeofday;
    ntp->usr=(uintptr_t)usr;
}

bool hsntp_ntp_client_sendpacket(hsntp_ntp_client_t *ntp,hsntp_packet_t *packet,size_t packet_size)
{
    if(ntp==NULL || ntp->sendpacket ==NULL)
    {
        return false;
    }

    bool ret=true;

    if(packet==NULL || packet_size == 0)
    {
        hsntp_packet_t packet=hsntp_packet_default();
        hsntp_packet_encode(&packet);
        ntp->sendpacket(ntp,packet.msg,sizeof(packet.msg));
    }
    else if(packet_size >= sizeof(hsntp_packet_t))
    {
        hsntp_packet_encode(packet);
        ntp->sendpacket(ntp,packet->msg,packet_size);
        hsntp_packet_decode(packet);
    }
    else
    {
        ret=false;
    }

    return ret;
}

bool hsntp_ntp_client_receivepacket(hsntp_ntp_client_t *ntp,const uint8_t *packet,size_t packet_size)
{
    bool ret=false;
    if(ntp == NULL)
    {
        return ret;
    }

    hsntp_packet_t sntp_packet;

    if(packet!=NULL && packet_size !=0)
    {
        if(packet_size < sizeof(sntp_packet.msg))
        {
            return ret;
        }
        else
        {
            memcpy(sntp_packet.msg,packet,sizeof(sntp_packet.msg));
            ret=true;
        }
    }
    else
    {
        if(ntp->receivepacket!=NULL)
        {
            ret=(sizeof(sntp_packet.msg) <= ntp->receivepacket(ntp,sntp_packet.msg,sizeof(sntp_packet.msg)));
        }
    }

    if(ret)
    {
        hsntp_packet_decode(&sntp_packet);
        hsntp_timestamp_format_t timestamp=hsntp_timestamp_format_decode(sntp_packet.transmit_timestamp);
        htimeval_t tv= {};
        tv.tv_sec=hsntp_timestamp_to_time_t(timestamp.seconds);
        tv.tv_usec=(((uint64_t)1000000ULL*timestamp.fraction)>>32);
        if(ntp->settimeofday!=NULL)
        {
            ret=(0==ntp->settimeofday(ntp,&tv));
        }
    }

    return ret;
}


