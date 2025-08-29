#include "modbussessiontcpgatewayserialport.h"

typedef struct
{
    modbus_tcp_gateway_server_context_t mb_ctx;
    wxSharedPtr<wxSocketServer> server;
    wxSharedPtr<wxSocketBase> client;
} ModbusSessionTCPGatewaySerialportContext_t;

ModbusSessionTCPGatewaySerialport::ModbusSessionTCPGatewaySerialport():serialport_config(NULL),ModbusRTURequestTimeout(100)
{
    sp_new_config(&serialport_config);
    sp_set_config_baudrate(serialport_config,115200);
    sp_set_config_bits(serialport_config,8);
    sp_set_config_parity(serialport_config,SP_PARITY_NONE);
    sp_set_config_stopbits(serialport_config,1);
    sp_set_config_flowcontrol(serialport_config,SP_FLOWCONTROL_NONE);
    SetOnData([&](HSocketServer&,wxSharedPtr<wxSocketServer> server,wxSharedPtr<wxSocketBase> client)
    {
        if(client==NULL || !client->IsData())
        {
            return;
        }
        ModbusSessionTCPGatewaySerialportContext_t context= {modbus_tcp_gateway_server_context_default(),server,client};
        context.mb_ctx.reply=[](modbus_tcp_gateway_server_context_t* ctx,const uint8_t *adu,size_t adu_length)
        {
            const ModbusSessionTCPGatewaySerialportContext_t *context=GET_STRUCT_PTR_BY_MEMBER_PTR(ctx,ModbusSessionTCPGatewaySerialportContext_t,mb_ctx);
            if(context==NULL || context->client==NULL)
            {
                return;
            }
            context->client->WaitForWrite();
            context->client->Write(adu,adu_length);
            context->client->WaitForWrite();
        };
        context.mb_ctx.usr=this;
        context.mb_ctx.rtu_request=[](modbus_tcp_gateway_server_context_t *ctx,const uint8_t *adu,size_t adu_length,bool (*rtu_reply)(modbus_tcp_gateway_server_context_t* ctx,const uint8_t *adu,size_t adu_length))->bool
        {
            if(ctx==NULL || ctx->usr==NULL || adu==NULL || adu_length==0)
            {
                return false;
            }

            ModbusSessionTCPGatewaySerialport &serialport=*(ModbusSessionTCPGatewaySerialport *)ctx->usr;


            std::string reply=serialport.ModbusRTURequest(std::string((char *)adu,adu_length));

            if(rtu_reply!=NULL && reply.length() > 0)
            {
                return rtu_reply(ctx,(const uint8_t *)reply.c_str(),reply.length());
            }

            return false;
        };

        client->WaitForRead(0,5);
        uint8_t adu[MODBUS_TCP_MAX_ADU_LENGTH]= {0};
        client->Read(adu,sizeof(adu));
        modbus_tcp_gateway_server_parse_input(&context.mb_ctx,adu,client->LastReadCount());
        client->Discard();
    });
}

ModbusSessionTCPGatewaySerialport::~ModbusSessionTCPGatewaySerialport()
{
    if(serialport_config!=NULL)
    {
        sp_free_config(serialport_config);
    }
}

struct sp_port_config & ModbusSessionTCPGatewaySerialport::GetSerialportConfig()
{
    return *serialport_config;
};

std::string &ModbusSessionTCPGatewaySerialport::GetSerialportName()
{
    return serialport_name;
}

std::string ModbusSessionTCPGatewaySerialport::ModbusRTURequest(std::string request,unsigned int timeout_ms)
{
    std::string ret;

    std::string portname=GetSerialportName();

    OnModbusRTUBegin();

    if(portname.empty())
    {
        OnModbusRTUEnd();
        return ret;
    }

    struct sp_port *port=NULL;

    sp_get_port_by_name(portname.c_str(),&port);

    if(port==NULL)
    {
        OnModbusRTUEnd();
        return ret;
    }

    if(SP_OK!=sp_open(port,SP_MODE_READ_WRITE))
    {
        sp_free_port(port);
        OnModbusRTUEnd();
        return ret;
    }

    if(SP_OK!=sp_set_config(port,&GetSerialportConfig()))
    {
        sp_close(port);
        sp_free_port(port);
        OnModbusRTUEnd();
        return ret;
    }

    sp_flush(port,SP_BUF_BOTH);

    size_t byte_time_us=0;
    {
        int baudrate=115200;
        sp_get_config_baudrate(&GetSerialportConfig(),&baudrate);
        int bits=8;
        sp_get_config_bits(&GetSerialportConfig(),&bits);
        int stop_bits=1;
        sp_get_config_stopbits(&GetSerialportConfig(),&stop_bits);
        byte_time_us=1000000/(baudrate/(1+bits+stop_bits));
        if(byte_time_us < 1000)
        {
            /*
             * 至少1ms
             */
            byte_time_us=1000;
        }
    }

    if(request.length()!=sp_blocking_write(port,request.c_str(),request.length(),ModbusRTURequestTimeout+request.length()*byte_time_us/1000))
    {
        sp_close(port);
        sp_free_port(port);
        OnModbusRTUEnd();
        return ret;
    }

    OnModbusRTUSend(request);

    uint8_t buffer[MODBUS_RTU_MAX_ADU_LENGTH]= {0};
    if(timeout_ms < sizeof(buffer)*byte_time_us*15/10/1000)
    {
        timeout_ms=sizeof(buffer)*byte_time_us*15/10/1000;
    }

    int bytes_read=0;
    if((bytes_read=sp_blocking_read(port,buffer,sizeof(buffer),ModbusRTURequestTimeout+timeout_ms)) <=0)
    {
        sp_close(port);
        sp_free_port(port);
        OnModbusRTUEnd();
        return ret;
    }

    ret=std::string((char *)buffer,bytes_read);

    sp_close(port);
    sp_free_port(port);

    OnModbusRTUReceive(ret);

    OnModbusRTUEnd();
    return ret;
}

void ModbusSessionTCPGatewaySerialport::LocalLog(wxString format,...)
{

}

void ModbusSessionTCPGatewaySerialport::OnModbusRTUBegin()
{

}

void ModbusSessionTCPGatewaySerialport::OnModbusRTUSend(std::string request)
{

}
void ModbusSessionTCPGatewaySerialport::OnModbusRTUReceive(std::string reply)
{

}

void ModbusSessionTCPGatewaySerialport::OnModbusRTUEnd()
{

}
