/***************************************************************
 * Name:      rtu_slave_tiny.c
 * Purpose:   modbus rtu 从机（精简版）
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-09-02
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hmodbus.h"



static bool modbus_rtu_slave_tiny_pdu_process_broadcast(modbus_rtu_slave_tiny_context_t* ctx,const uint8_t *pdu,size_t pdu_length)
{
    //广播操作，一般只允许写寄存器
    if(pdu_length < 1)
    {
        return false;
    }
    bool ret=false;
    uint8_t function_code=pdu[0];
    switch(function_code)
    {
    case MODBUS_FC_WRITE_SINGLE_COIL:
    {
        modbus_data_address_t addr=modbus_data_get_uint16_t(pdu,1,pdu_length);
        modbus_data_register_t value=modbus_data_get_uint16_t(pdu,3,pdu_length);
        if(ctx->write_coil!=NULL)
        {
            ret=true;
            if(value==0xFF00)
            {
                ctx->write_coil(ctx,addr,true);
            }
            if(value==0x0000)
            {
                ctx->write_coil(ctx,addr,false);
            }
        }
    }
    break;
    case MODBUS_FC_WRITE_SINGLE_REGISTER:
    {
        modbus_data_address_t addr=modbus_data_get_uint16_t(pdu,1,pdu_length);
        modbus_data_register_t value=modbus_data_get_uint16_t(pdu,3,pdu_length);
        if(ctx->write_holding_register!=NULL)
        {
            ret=true;
            ctx->write_holding_register(ctx,addr,value);
        }
    }
    break;
    case MODBUS_FC_WRITE_MULTIPLE_COILS:
    {
        modbus_data_address_t addr=modbus_data_get_uint16_t(pdu,1,pdu_length);
        modbus_data_register_t cnt=modbus_data_get_uint16_t(pdu,3,pdu_length);
        if(cnt <= MODBUS_MAX_WRITE_BITS && cnt > 0 && ctx->write_coil != NULL)
        {
            uint8_t byte_cnt=pdu[5];
            if((byte_cnt+6)!=pdu_length)
            {
                break;
            }
            ret=true;
            for(size_t i=0; i<cnt; i++)
            {
                if((pdu[6+i/8]&(0x01<<(i%8)))!=0)
                {
                    ctx->write_coil(ctx,addr+i,true);
                }
                else
                {
                    ctx->write_coil(ctx,addr+i,false);
                }
            }
        }
    }
    break;
    case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
    {
        modbus_data_address_t addr=modbus_data_get_uint16_t(pdu,1,pdu_length);
        modbus_data_register_t cnt=modbus_data_get_uint16_t(pdu,3,pdu_length);
        uint8_t length=pdu[5];
        if((cnt !=0 ) && (ctx->write_holding_register!=NULL) && (cnt <= MODBUS_MAX_WRITE_REGISTERS) && (cnt*2 ==length))
        {
            ret=true;
            for(size_t i=0; i<cnt; i++)
            {
                ctx->write_holding_register(ctx,addr+i,modbus_data_get_uint16_t(pdu,6+2*i,pdu_length));
            }
        }
    }
    break;
    case MODBUS_FC_MASK_WRITE_REGISTER:
    {
        modbus_data_address_t  addr=modbus_data_get_uint16_t(pdu,1,pdu_length);
        modbus_data_register_t and_mask=modbus_data_get_uint16_t(pdu,3,pdu_length);
        modbus_data_register_t or_mask=modbus_data_get_uint16_t(pdu,5,pdu_length);
        if(ctx->read_holding_register!=NULL && ctx->write_holding_register!=NULL )
        {
            ret=true;
            modbus_data_register_t value=ctx->read_holding_register(ctx,addr);
            ctx->write_holding_register(ctx,addr,(value & and_mask)|(or_mask & (~and_mask)));
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

static bool modbus_rtu_slave_tiny_pdu_process_unicast(modbus_rtu_slave_tiny_context_t* ctx,uint8_t node_address,const uint8_t *pdu,size_t pdu_length,uint8_t *buffer,size_t buffer_length)
{
    if(pdu_length < 1 || node_address != ctx->addr)
    {
        return false;
    }
    bool ret=false;
    uint8_t function_code=pdu[0];
    switch(function_code)
    {
    case MODBUS_FC_READ_COILS:
    {
        modbus_data_address_t addr=modbus_data_get_uint16_t(pdu,1,pdu_length);
        modbus_data_register_t cnt=modbus_data_get_uint16_t(pdu,3,pdu_length);
        if(cnt <= MODBUS_MAX_READ_BITS && cnt > 0 && ctx->read_coil != NULL && buffer_length >= (cnt/8+5+((cnt%8!=0)?1:0)))
        {
            uint8_t *reply_pdu=&buffer[1];
            reply_pdu[0]=function_code;
            uint8_t byte_cnt=cnt/8+((cnt%8!=0)?1:0);
            reply_pdu[1]=byte_cnt;
            for(size_t i=0; i<cnt; i++)
            {
                if(i%8 == 0)
                {
                    reply_pdu[2+i/8]=0xFF;
                }

                if(ctx->read_coil(ctx,addr+i))
                {
                    reply_pdu[2+i/8]|=(0x01 << (i%8));
                }
                else
                {
                    reply_pdu[2+i/8]&=(~(0x01 << (i%8)));
                }
            }
            modbus_rtu_set_pdu_to_adu(buffer,buffer_length,node_address,reply_pdu,byte_cnt+2);
            if(ctx->reply!=NULL)
            {
                ret=true;
                ctx->reply(ctx,buffer,byte_cnt+5);
            }
        }
    }
    break;
    case MODBUS_FC_READ_DISCRETE_INPUTS:
    {
        modbus_data_address_t addr=modbus_data_get_uint16_t(pdu,1,pdu_length);
        modbus_data_register_t cnt=modbus_data_get_uint16_t(pdu,3,pdu_length);
        if(cnt <= MODBUS_MAX_READ_BITS && cnt > 0 && ctx->read_discrete_input != NULL && buffer_length >= (cnt/8+5+((cnt%8!=0)?1:0)))
        {
            uint8_t *reply_pdu=&buffer[1];
            reply_pdu[0]=function_code;
            uint8_t byte_cnt=cnt/8+((cnt%8!=0)?1:0);
            reply_pdu[1]=byte_cnt;
            for(size_t i=0; i<cnt; i++)
            {
                if(i%8 == 0)
                {
                    reply_pdu[2+i/8]=0xFF;
                }

                if(ctx->read_discrete_input(ctx,addr+i))
                {
                    reply_pdu[2+i/8]|=(0x01 << (i%8));
                }
                else
                {
                    reply_pdu[2+i/8]&=(~(0x01 << (i%8)));
                }
            }
            modbus_rtu_set_pdu_to_adu(buffer,buffer_length,node_address,reply_pdu,byte_cnt+2);
            if(ctx->reply!=NULL)
            {
                ret=true;
                ctx->reply(ctx,buffer,byte_cnt+5);
            }
        }
    }
    break;
    case MODBUS_FC_READ_HOLDING_REGISTERS:
    {
        modbus_data_address_t addr=modbus_data_get_uint16_t(pdu,1,pdu_length);
        modbus_data_register_t cnt=modbus_data_get_uint16_t(pdu,3,pdu_length);
        if(cnt <= MODBUS_MAX_READ_REGISTERS && cnt > 0 && ctx->read_holding_register != NULL && buffer_length >= (cnt*2+5))
        {
            uint8_t *reply_pdu=&buffer[1];
            reply_pdu[0]=function_code;
            uint8_t byte_cnt=cnt*2;
            reply_pdu[1]=byte_cnt;
            for(size_t i=0; i<cnt; i++)
            {
                modbus_data_set_uint16_t(reply_pdu,2+i*2,buffer_length-1,ctx->read_holding_register(ctx,addr+i));
            }
            modbus_rtu_set_pdu_to_adu(buffer,buffer_length,node_address,reply_pdu,byte_cnt+2);
            if(ctx->reply!=NULL)
            {
                ret=true;
                ctx->reply(ctx,buffer,byte_cnt+5);
            }
        }
    }
    break;
    case MODBUS_FC_READ_INPUT_REGISTERS:
    {
        modbus_data_address_t addr=modbus_data_get_uint16_t(pdu,1,pdu_length);
        modbus_data_register_t cnt=modbus_data_get_uint16_t(pdu,3,pdu_length);
        if(cnt <= MODBUS_MAX_READ_REGISTERS && cnt > 0 && ctx->read_input_register != NULL && buffer_length >= (cnt*2+5))
        {
            uint8_t *reply_pdu=&buffer[1];
            reply_pdu[0]=function_code;
            uint8_t byte_cnt=cnt*2;
            reply_pdu[1]=byte_cnt;
            for(size_t i=0; i<cnt; i++)
            {
                modbus_data_set_uint16_t(reply_pdu,2+i*2,buffer_length-1,ctx->read_input_register(ctx,addr+i));
            }
            modbus_rtu_set_pdu_to_adu(buffer,buffer_length,node_address,reply_pdu,byte_cnt+2);
            if(ctx->reply!=NULL)
            {
                ret=true;
                ctx->reply(ctx,buffer,byte_cnt+5);
            }
        }
    }
    break;
    case MODBUS_FC_WRITE_SINGLE_COIL:
    {
        modbus_data_address_t addr=modbus_data_get_uint16_t(pdu,1,pdu_length);
        modbus_data_register_t value=modbus_data_get_uint16_t(pdu,3,pdu_length);
        if(ctx->write_coil!=NULL && buffer_length >=(pdu_length+3))
        {
            ret=true;
            if(value==0xFF00)
            {
                ctx->write_coil(ctx,addr,true);
            }
            if(value==0x0000)
            {
                ctx->write_coil(ctx,addr,false);
            }
            modbus_rtu_set_pdu_to_adu(buffer,buffer_length,node_address,pdu,pdu_length);
            if(ctx->reply!=NULL)
            {
                ctx->reply(ctx,buffer,pdu_length+3);
            }
        }
    }
    break;
    case MODBUS_FC_WRITE_SINGLE_REGISTER:
    {
        modbus_data_address_t addr=modbus_data_get_uint16_t(pdu,1,pdu_length);
        modbus_data_register_t value=modbus_data_get_uint16_t(pdu,3,pdu_length);
        if(ctx->write_holding_register!=NULL && buffer_length >=(pdu_length+3))
        {
            ret=true;
            ctx->write_holding_register(ctx,addr,value);
            modbus_rtu_set_pdu_to_adu(buffer,buffer_length,node_address,pdu,pdu_length);
            if(ctx->reply!=NULL)
            {
                ctx->reply(ctx,buffer,pdu_length+3);
            }
        }
    }
    break;
    case MODBUS_FC_WRITE_MULTIPLE_COILS:
    {
        modbus_data_address_t addr=modbus_data_get_uint16_t(pdu,1,pdu_length);
        modbus_data_register_t cnt=modbus_data_get_uint16_t(pdu,3,pdu_length);
        if(cnt <= MODBUS_MAX_WRITE_BITS && cnt > 0 && ctx->write_coil != NULL && buffer_length >= (8))
        {
            uint8_t byte_cnt=pdu[5];
            if((byte_cnt+6)!=pdu_length)
            {
                break;
            }
            ret=true;
            for(size_t i=0; i<cnt; i++)
            {
                if((pdu[6+i/8]&(0x01<<(i%8)))!=0)
                {
                    ctx->write_coil(ctx,addr+i,true);
                }
                else
                {
                    ctx->write_coil(ctx,addr+i,false);
                }
            }
            modbus_rtu_set_pdu_to_adu(buffer,buffer_length,node_address,pdu,5);
            if(ctx->reply!=NULL)
            {
                ctx->reply(ctx,buffer,8);
            }
        }
    }
    break;
    case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
    {
        modbus_data_address_t addr=modbus_data_get_uint16_t(pdu,1,pdu_length);
        modbus_data_register_t cnt=modbus_data_get_uint16_t(pdu,3,pdu_length);
        if(cnt <= MODBUS_MAX_WRITE_REGISTERS && cnt > 0 && ctx->write_holding_register != NULL && buffer_length >= (8))
        {
            uint8_t byte_cnt=pdu[5];
            if((byte_cnt+6)!=pdu_length)
            {
                break;
            }
            ret=true;
            for(size_t i=0; i<cnt; i++)
            {
                ctx->write_holding_register(ctx,addr+i,modbus_data_get_uint16_t(pdu,6+2*i,pdu_length));
            }
            modbus_rtu_set_pdu_to_adu(buffer,buffer_length,node_address,pdu,5);
            if(ctx->reply!=NULL)
            {
                ctx->reply(ctx,buffer,8);
            }
        }
    }
    break;
    case MODBUS_FC_MASK_WRITE_REGISTER:
    {
        modbus_data_address_t  addr=modbus_data_get_uint16_t(pdu,1,pdu_length);
        modbus_data_register_t and_mask=modbus_data_get_uint16_t(pdu,3,pdu_length);
        modbus_data_register_t or_mask=modbus_data_get_uint16_t(pdu,5,pdu_length);
        if(ctx->read_holding_register!=NULL && ctx->write_holding_register!=NULL && buffer_length >=(pdu_length+3))
        {
            ret=true;
            modbus_data_register_t value=ctx->read_holding_register(ctx,addr);
            ctx->write_holding_register(ctx,addr,(value & and_mask)|(or_mask & (~and_mask)));
            modbus_rtu_set_pdu_to_adu(buffer,buffer_length,node_address,pdu,pdu_length);
            if(ctx->reply!=NULL)
            {
                ctx->reply(ctx,buffer,pdu_length+3);
            }
        }
    }
    break;
    case MODBUS_FC_WRITE_AND_READ_REGISTERS:
    {

        {
            //写寄存器
            modbus_data_address_t addr=modbus_data_get_uint16_t(pdu,5,pdu_length);
            modbus_data_register_t cnt=modbus_data_get_uint16_t(pdu,7,pdu_length);
            if(cnt <= MODBUS_MAX_WR_WRITE_REGISTERS && cnt > 0 && ctx->write_holding_register != NULL)
            {
                uint8_t byte_cnt=pdu[9];
                if((byte_cnt+10)!=pdu_length)
                {
                    break;
                }
                for(size_t i=0; i<cnt; i++)
                {
                    ctx->write_holding_register(ctx,addr+i,modbus_data_get_uint16_t(pdu,10+2*i,pdu_length));
                }
            }
        }
        {
            //读寄存器
            modbus_data_address_t addr=modbus_data_get_uint16_t(pdu,1,pdu_length);
            modbus_data_register_t read_cnt=modbus_data_get_uint16_t(pdu,3,pdu_length);
            if(read_cnt <= MODBUS_MAX_WR_READ_REGISTERS && read_cnt > 0 && ctx->read_holding_register != NULL && buffer_length >= (read_cnt*2+5))
            {
                uint8_t *reply_pdu=&buffer[1];
                reply_pdu[0]=function_code;
                uint8_t byte_cnt=read_cnt*2;
                reply_pdu[1]=byte_cnt;
                for(size_t i=0; i<read_cnt; i++)
                {
                    modbus_data_set_uint16_t(reply_pdu,2+i*2,buffer_length-1,ctx->read_holding_register(ctx,addr+i));
                }
                modbus_rtu_set_pdu_to_adu(buffer,buffer_length,node_address,reply_pdu,byte_cnt+2);
                if(ctx->reply!=NULL)
                {
                    ret=true;
                    ctx->reply(ctx,buffer,byte_cnt+5);
                }

            }
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

static bool modbus_rtu_slave_tiny_pdu_process(modbus_rtu_slave_tiny_context_t* ctx,uint8_t node_address,const uint8_t *pdu,size_t pdu_length,uint8_t *buffer,size_t buffer_length)
{
    if(node_address==MODBUS_ANYCAST_ADDRESS)
    {
        node_address=ctx->addr;
    }
    if(node_address==MODBUS_ANYCAST_WITH_CONDITION_ADDRESS)
    {
        if(ctx->check_anycast_condition!=NULL)
        {
            if(ctx->check_anycast_condition(ctx))
            {
                node_address=ctx->addr;
            }
            else
            {
                //仅当有检查函数且检查失败才返回false
                return false;
            }
        }
    }
    if(node_address==MODBUS_BROADCAST_ADDRESS)
    {
        return modbus_rtu_slave_tiny_pdu_process_broadcast(ctx,pdu,pdu_length);
    }
    else
    {
        return modbus_rtu_slave_tiny_pdu_process_unicast(ctx,node_address,pdu,pdu_length,buffer,buffer_length);
    }
}

static bool modbus_rtu_slave_tiny_modbus_rtu_pdu(uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    modbus_rtu_slave_tiny_context_t* ctx=(modbus_rtu_slave_tiny_context_t*)usr;
    if(ctx->buffer==NULL)
    {
        uint8_t  buffer[MODBUS_RTU_MAX_ADU_LENGTH];
        return modbus_rtu_slave_tiny_pdu_process(ctx,node_address,pdu,pdu_length,buffer,MODBUS_RTU_MAX_ADU_LENGTH);
    }
    else
    {
        return modbus_rtu_slave_tiny_pdu_process(ctx,node_address,pdu,pdu_length,ctx->buffer,MODBUS_RTU_MAX_ADU_LENGTH);
    }
}

bool modbus_rtu_slave_tiny_parse_input(modbus_rtu_slave_tiny_context_t* ctx,uint8_t *adu,size_t adu_length)
{
    if(ctx==NULL || adu == NULL || adu_length == 0 || adu_length > MODBUS_RTU_MAX_ADU_LENGTH)
    {
        return false;
    }
    return modbus_rtu_get_pdu_from_adu(adu,adu_length,modbus_rtu_slave_tiny_modbus_rtu_pdu,ctx);
}


static bool    modbus_rtu_slave_tiny_read_coil(modbus_rtu_slave_tiny_context_t* ctx,modbus_data_address_t addr)
{
    (void)ctx;
    //返回地址是否奇数
    return addr%2!=0;
}
static bool    modbus_rtu_slave_tiny_read_discrete_input(modbus_rtu_slave_tiny_context_t* ctx,modbus_data_address_t addr)
{
    (void)ctx;
    //返回地址是否偶数
    return addr%2==0;
}
static modbus_data_register_t  modbus_rtu_slave_tiny_read_holding_register(modbus_rtu_slave_tiny_context_t* ctx,modbus_data_address_t addr)
{
    (void)ctx;
    //返回 0xDEADBEEF;
    if(addr%2==0)
    {
        return 0xDEAD;
    }
    else
    {
        return 0xBEEF;
    }
}
static modbus_data_register_t  modbus_rtu_slave_tiny_read_input_register(modbus_rtu_slave_tiny_context_t* ctx,modbus_data_address_t addr)
{
    (void)ctx;
    //返回 0xBEEFDEAD;
    if(addr%2!=0)
    {
        return 0xDEAD;
    }
    else
    {
        return 0xBEEF;
    }
}
static void    modbus_rtu_slave_tiny_write_coil(modbus_rtu_slave_tiny_context_t* ctx,modbus_data_address_t addr,bool value)
{
    (void)ctx;
    (void)addr;
    (void)value;
    //啥也不做
}
static void    modbus_rtu_slave_tiny_write_holding_register(modbus_rtu_slave_tiny_context_t* ctx,modbus_data_address_t addr,modbus_data_register_t value)
{
    (void)ctx;
    (void)addr;
    (void)value;
    //啥也不做
}
static bool    modbus_rtu_slave_tiny_check_anycast_condition(modbus_rtu_slave_tiny_context_t* ctx)
{
    (void)ctx;
    //默认不回应
    return false;
}

modbus_rtu_slave_tiny_context_t modbus_rtu_slave_tiny_context_default(void)
{
    modbus_rtu_slave_tiny_context_t ctx= {0};
    ctx.addr=MODBUS_NODE_ADDRESS_DEFAULT;
    ctx.read_coil=modbus_rtu_slave_tiny_read_coil;
    ctx.read_discrete_input=modbus_rtu_slave_tiny_read_discrete_input;
    ctx.read_holding_register=modbus_rtu_slave_tiny_read_holding_register;
    ctx.read_input_register=modbus_rtu_slave_tiny_read_input_register;
    ctx.write_coil=modbus_rtu_slave_tiny_write_coil;
    ctx.write_holding_register=modbus_rtu_slave_tiny_write_holding_register;
    ctx.check_anycast_condition=modbus_rtu_slave_tiny_check_anycast_condition;
    ctx.buffer=NULL;
    return ctx;
}

