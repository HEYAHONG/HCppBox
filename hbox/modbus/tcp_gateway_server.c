#include "modbus.h"

static bool tcp_reply(uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    modbus_tcp_gateway_server_context_t *ctx=(modbus_tcp_gateway_server_context_t *)usr;
    if(ctx->reply!=NULL)
    {
        if(ctx->tcp_buffer!=NULL)
        {
            uint8_t *tcp_buffer=ctx->tcp_buffer;
            size_t modbus_tcp_len=modbus_tcp_set_pdu_to_adu(tcp_buffer,MODBUS_TCP_MAX_ADU_LENGTH,ctx->TId,node_address,pdu,pdu_length);
            ctx->reply(ctx,tcp_buffer,modbus_tcp_len);
        }
        else
        {
            uint8_t tcp_buffer[MODBUS_TCP_MAX_ADU_LENGTH];
            size_t modbus_tcp_len=modbus_tcp_set_pdu_to_adu(tcp_buffer,MODBUS_TCP_MAX_ADU_LENGTH,ctx->TId,node_address,pdu,pdu_length);
            ctx->reply(ctx,tcp_buffer,modbus_tcp_len);
        }
        return true;
    }
    else
    {
        return false;
    }
}


static bool rtu_reply(modbus_tcp_gateway_server_context_t* ctx,const uint8_t *adu,size_t adu_length)
{
    if(ctx==NULL || adu == NULL || adu_length == 0 || ctx->reply == NULL)
    {
        return false;
    }
    return modbus_rtu_get_pdu_from_adu(adu,adu_length,tcp_reply,ctx);
}

static bool pdu_process(uint16_t TId,uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    modbus_tcp_gateway_server_context_t *ctx=(modbus_tcp_gateway_server_context_t *)usr;
    ctx->TId=TId;
    bool ret=false;
    if(pdu!=NULL && pdu_length > 0)
    {
        if(ctx->rtu_buffer!=NULL)
        {
            uint8_t *rtu_buffer=ctx->rtu_buffer;
            size_t rtu_adu_len=modbus_rtu_set_pdu_to_adu(rtu_buffer,MODBUS_RTU_MAX_ADU_LENGTH,node_address,pdu,pdu_length);
            if(rtu_adu_len!=0)
            {
                ret=ctx->rtu_request(ctx,rtu_buffer,rtu_adu_len,rtu_reply);
            }
        }
        else
        {
            uint8_t rtu_buffer[MODBUS_RTU_MAX_ADU_LENGTH];
            size_t rtu_adu_len=modbus_rtu_set_pdu_to_adu(rtu_buffer,MODBUS_RTU_MAX_ADU_LENGTH,node_address,pdu,pdu_length);
            if(rtu_adu_len!=0)
            {
                ret=ctx->rtu_request(ctx,rtu_buffer,rtu_adu_len,rtu_reply);
            }
        }
    }

    if(!ret)
    {
        //失败后，由网关返回异常
        if(ctx->reply!=NULL)
        {
            if(ctx->tcp_buffer!=NULL)
            {
                uint8_t *tcp_buffer=ctx->tcp_buffer;
                size_t len=modbus_tcp_set_exception_pdu_to_adu(tcp_buffer,MODBUS_TCP_MAX_ADU_LENGTH,TId,node_address,pdu[0],MODBUS_EXCEPTION_GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND);
                ctx->reply(ctx,tcp_buffer,len);
            }
            else
            {
                uint8_t tcp_buffer[MODBUS_TCP_MAX_ADU_LENGTH];
                size_t len=modbus_tcp_set_exception_pdu_to_adu(tcp_buffer,MODBUS_TCP_MAX_ADU_LENGTH,TId,node_address,pdu[0],MODBUS_EXCEPTION_GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND);
                ctx->reply(ctx,tcp_buffer,len);
            }
        }
    }

    return ret;
}

/** \brief  modbus gateway server解析输入并返回
 *          注意:本操作未加锁，应当避免在多个线程中使用同一个上下文调用此函数。
 *
 * \param ctx modbus_tcp_gateway_server_context_t*上下文指针
 * \param adu uint8_t* modbus tcp请求数据包地址
 * \param adu_length size_t modbus tcp请求数据包长度
 * \return bool 是否成功处理
 *
 */
bool modbus_tcp_gateway_server_parse_input(modbus_tcp_gateway_server_context_t* ctx,uint8_t *adu,size_t adu_length)
{
    if(ctx == NULL || ctx->rtu_request == NULL || adu == NULL || adu_length == 0)
    {
        return false;
    }

    return modbus_tcp_get_pdu_from_adu(adu,adu_length,pdu_process,ctx);
}


