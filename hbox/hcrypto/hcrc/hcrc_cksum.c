/***************************************************************
 * Name:      hcrc_cksum.c
 * Purpose:   实现hcrc_cksum接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-12-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hcrc_cksum.h"

hcrc_cksum_context_t hcrc_cksum_starts(void)
{
    hcrc_cksum_context_t ret={0};
    ret.byte_count=0;
    ret.crc_ctx=hcrc_crc32_cksum_table_starts();
    return ret;
}


hcrc_cksum_context_t hcrc_cksum_update(hcrc_cksum_context_t ctx,const uint8_t *data,size_t datalen)
{
    ctx.crc_ctx=hcrc_crc32_cksum_table_update(ctx.crc_ctx,data,datalen);
    ctx.byte_count+=datalen;
    return ctx;
}


uint32_t hcrc_cksum_finish(hcrc_cksum_context_t ctx)
{
    uint64_t temp=ctx.byte_count;
    while(temp!=0)
    {
        uint8_t byte[1]={0};
        byte[0]=(temp&0xFF);
        ctx.crc_ctx=hcrc_crc32_cksum_table_update(ctx.crc_ctx,byte,sizeof(byte));
        temp>>=8;
    }
    return hcrc_crc32_cksum_table_finish(ctx.crc_ctx);
}


uint32_t hcrc_cksum_calculate(const uint8_t *data,size_t datalen)
{
    hcrc_cksum_context_t ctx=hcrc_cksum_starts();
    ctx=hcrc_cksum_update(ctx,data,datalen);
    return hcrc_cksum_finish(ctx);
}


bool hcrc_cksum_check(const uint8_t *data,size_t datalen,uint32_t check)
{
    return check==hcrc_cksum_calculate(data,datalen);
}
