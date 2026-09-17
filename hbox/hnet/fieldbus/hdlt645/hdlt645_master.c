/***************************************************************
 * Name:      hdlt645_master.c
 * Purpose:   实现hdlt645_master接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-09-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include "hdefaults.h"
#include "hdlt645_master.h"
#include "hdlt645_utils.h"

#if defined(HDLT645_MASTER_EXTERN_SOURCE_FILENAME)
#include HDLT645_MASTER_EXTERN_SOURCE_FILENAME
#endif


void hdlt645_master_io_init(hdlt645_master_io_t *io,hdlt645_master_io_cb_timeout_t timeout,hdlt645_master_io_cb_send_t send,void *usr)
{
    if(io!=NULL)
    {
        memset(io,0,sizeof(*io));

        io->timeout=timeout;
        io->send=send;
        io->usr=usr;
    }
}

hdlt645_master_io_status_t hdlt645_master_io_status(hdlt645_master_io_t *io)
{
    if(io==NULL)
    {
        return HDLT645_MASTER_IO_STATUS_ERROR;
    }

    if(io->timeout!=NULL)
    {
        if(io->timeout(io))
        {
            return HDLT645_MASTER_IO_STATUS_TIMEOUT;
        }
    }

    hdlt645_master_io_status_t status=HDLT645_MASTER_IO_STATUS_IDLE;

    switch(io->rx_buffer_index)
    {
    case 0:
    {

    }
    break;
    case 1:
    {
        status=HDLT645_MASTER_IO_STATUS_SOF1;
    }
    break;
    case 8:
    {
        status=HDLT645_MASTER_IO_STATUS_SOF2;
    }
    break;
    case 9:
    {
        status=HDLT645_MASTER_IO_STATUS_C;
    }
    break;
    case 10:
    {
        status=HDLT645_MASTER_IO_STATUS_DATALEN;
    }
    break;
    default:
    {
        size_t i=io->rx_buffer_index;
        if(i >= 2 && i < 8)
        {
            status=HDLT645_MASTER_IO_STATUS_ADDR;
            break;
        }

        if(i >= 11)
        {
            uint8_t datalen=io->rx_buffer[9];
            if(datalen > HDLT645_FRAME_DATALENGTH_MAX_WRITE)
            {
                status=HDLT645_MASTER_IO_STATUS_ERROR;
                break;
            }

            if(i< 10+datalen+1)
            {
                status=HDLT645_MASTER_IO_STATUS_DATA;
                break;
            }
            else if(i == 10+datalen+1)
            {
                status=HDLT645_MASTER_IO_STATUS_CKSUM;
                break;
            }
            else if(i >= 10+datalen+2)
            {
                status=HDLT645_MASTER_IO_STATUS_EOF;
                break;
            }
        }
        status=HDLT645_MASTER_IO_STATUS_ERROR;
    }
    break;
    }

    return status;
}

void hdlt645_master_io_rx_reset(hdlt645_master_io_t *io)
{
    if(io!=NULL)
    {
        memset(io->rx_buffer,0,sizeof(io->rx_buffer));
        io->rx_buffer_index=0;
    }
}

size_t hdlt645_master_io_rx_input(hdlt645_master_io_t *io,uint8_t *data,size_t datalen)
{
    if(io==NULL || data == NULL || datalen == 0)
    {
        return 0;
    }

    size_t ret=0;

    for(size_t i=0; i<datalen; i++)
    {
        if(io->rx_buffer_index==0)
        {
            if(data[i]==HDLT645_FRAME_PREAMBLE)
            {
                /*
                 * 跳过前导字节
                 */
                continue;
            }
        }

        if(io->rx_buffer_index >= sizeof(io->rx_buffer))
        {
            break;
        }

        io->rx_buffer[io->rx_buffer_index++]=data[i];

        ret++;
    }

    return ret;
}

bool hdlt645_master_ctx_init(hdlt645_master_ctx_t *ctx,int fct,void *cmd_ctx,size_t cmd_ctx_size)
{
    bool ret=false;

    if(ctx==NULL)
    {
        return ret;
    }

    ctx->cmd.fct=fct;
    ctx->cmd.ctx=cmd_ctx;
    ctx->cmd.ctx_size=cmd_ctx_size;

    switch(fct)
    {
    case HDLT645_FRAME_CONTROL_FCT_TIME:
    {
        if(ctx->cmd.ctx == NULL || ctx->cmd.ctx_size != sizeof(hdlt645_master_ctx_cmd_time_t))
        {
            ret=false;
        }
    }
    break;
    case HDLT645_FRAME_CONTROL_FCT_READ:
    {
        if(ctx->cmd.ctx == NULL || ctx->cmd.ctx_size != sizeof(hdlt645_master_ctx_cmd_read_t))
        {
            ret=false;
        }
    }
    break;
    case HDLT645_FRAME_CONTROL_FCT_READEXT:
    {
        if(ctx->cmd.ctx == NULL || ctx->cmd.ctx_size != sizeof(hdlt645_master_ctx_cmd_readext_t))
        {
            ret=false;
        }
    }
    break;
    default:
    {
        ret=true;
    }
    break;
    }

    if(ret)
    {
        ctx->status=HDLT645_MASTER_CTX_STATUS_INIT;
    }

    return ret;
}


hdlt645_master_ctx_status_t hdlt645_master_ctx_status(hdlt645_master_ctx_t *ctx)
{
    if(ctx == NULL)
    {
        return HDLT645_MASTER_CTX_STATUS_ERROR;
    }

    return (hdlt645_master_ctx_status_t)ctx->status;

}

hdlt645_master_ctx_status_t hdlt645_master_ctx_process(hdlt645_master_ctx_t *ctx,hdlt645_master_io_t *io)
{
    if(ctx==NULL || io == NULL)
    {
        return HDLT645_MASTER_CTX_STATUS_ERROR;
    }

    switch(ctx->status)
    {
    case HDLT645_MASTER_CTX_STATUS_INIT:
    {
        /*
         * 此步骤用于检查参数
         */
        switch(ctx->cmd.fct)
        {
        case HDLT645_FRAME_CONTROL_FCT_TIME:
        {
            if(ctx->cmd.ctx == NULL || ctx->cmd.ctx_size != sizeof(hdlt645_master_ctx_cmd_time_t))
            {
                ctx->status=HDLT645_MASTER_CTX_STATUS_ERROR;
            }
        }
        break;
        case HDLT645_FRAME_CONTROL_FCT_READ:
        {
            if(ctx->cmd.ctx == NULL || ctx->cmd.ctx_size != sizeof(hdlt645_master_ctx_cmd_read_t))
            {
                ctx->status=HDLT645_MASTER_CTX_STATUS_ERROR;
            }
        }
        break;
        case HDLT645_FRAME_CONTROL_FCT_READEXT:
        {
            if(ctx->cmd.ctx == NULL || ctx->cmd.ctx_size != sizeof(hdlt645_master_ctx_cmd_readext_t))
            {
                ctx->status=HDLT645_MASTER_CTX_STATUS_ERROR;
            }
        }
        break;
        default:
        {
            ctx->status=HDLT645_MASTER_CTX_STATUS_ERROR;
        }
        break;
        }

        if(hdlt645_master_ctx_status(ctx) == HDLT645_MASTER_CTX_STATUS_INIT)
        {
            hdlt645_master_io_rx_reset(io);
            ctx->status=HDLT645_MASTER_CTX_STATUS_SEND_REQUEST;
        }
    }
    break;
    case HDLT645_MASTER_CTX_STATUS_SEND_REQUEST:
    {
        /*
         * 此步骤用于发送数据
         */
#if !defined(HDLT645_MASTER_IO_NO_TX_BUFFER)
        uint8_t *buffer=io->buffer;
        size_t   buffer_size=sizeof(io->buffer);
#else
        uint8_t buffer[12+HDLT645_FRAME_DATALENGTH_MAX_WRITE]= {0};
        size_t   buffer_size=sizeof(buffer);
#endif

        bool need_reply=true;

        switch(ctx->cmd.fct)
        {
        case HDLT645_FRAME_CONTROL_FCT_TIME:
        {
            need_reply=false;
            hdlt645_master_ctx_cmd_time_t *cmd=(hdlt645_master_ctx_cmd_time_t *)ctx->cmd.ctx;

            {
                /*
                 * 设置地址
                 */
                hdlt645_bcd_addr_t *frame_addr=hdlt645_frame_get_bcd_addr(buffer,buffer_size);
                if(frame_addr!=NULL)
                {
                    hdlt645_bcd_addr_set(frame_addr,HDLT645_FRAME_BOARDCAST_BCD_ADDR);
                }
            }

            {
                /*
                 * 设置数据
                 */
                uint8_t *data=hdlt645_frame_get_data(buffer,buffer_size);
                uint8_t *datalen=hdlt645_frame_get_datalen(buffer,buffer_size);
                if(data != NULL &&datalen != NULL)
                {
                    (*datalen)=6;
                    htime_t current_time=hdlt645_master_ctx_cmd_time_current(cmd);
                    htm_t current_tm;
                    hlibc_localtime_r(&current_time,&current_tm);
                    data[0]=hdlt645_uint64_to_bcd(current_tm.tm_sec);
                    data[1]=hdlt645_uint64_to_bcd(current_tm.tm_min);
                    data[2]=hdlt645_uint64_to_bcd(current_tm.tm_hour);
                    data[3]=hdlt645_uint64_to_bcd(current_tm.tm_mday);
                    data[4]=hdlt645_uint64_to_bcd(current_tm.tm_mon+1);
                    data[5]=hdlt645_uint64_to_bcd(current_tm.tm_year%100);
                }
            }
        }
        break;
        case HDLT645_FRAME_CONTROL_FCT_READ:
        {
            hdlt645_master_ctx_cmd_read_t *cmd=(hdlt645_master_ctx_cmd_read_t *)ctx->cmd.ctx;

            {
                /*
                 * 设置地址
                 */
                hdlt645_bcd_addr_t *frame_addr=hdlt645_frame_get_bcd_addr(buffer,buffer_size);
                if(frame_addr!=NULL)
                {
                    memcpy(frame_addr,&cmd->addr,sizeof(cmd->addr));
                }
            }

            {
                /*
                 * 设置数据
                 */
                uint8_t *data=hdlt645_frame_get_data(buffer,buffer_size);
                uint8_t *datalen=hdlt645_frame_get_datalen(buffer,buffer_size);
                if(data != NULL &&datalen != NULL)
                {
                    (*datalen)=cmd->request_buffer_length;
                    memcpy(data,cmd->request_buffer,(*datalen));
                }
            }
        }
        break;
        case HDLT645_FRAME_CONTROL_FCT_READEXT:
        {
            hdlt645_master_ctx_cmd_readext_t *cmd=(hdlt645_master_ctx_cmd_readext_t *)ctx->cmd.ctx;

            {
                /*
                 * 设置地址
                 */
                hdlt645_bcd_addr_t *frame_addr=hdlt645_frame_get_bcd_addr(buffer,buffer_size);
                if(frame_addr!=NULL)
                {
                    memcpy(frame_addr,&cmd->addr,sizeof(cmd->addr));
                }
            }

            {
                /*
                 * 设置数据
                 */
                uint8_t *data=hdlt645_frame_get_data(buffer,buffer_size);
                uint8_t *datalen=hdlt645_frame_get_datalen(buffer,buffer_size);
                if(data != NULL &&datalen != NULL)
                {
                    (*datalen)=5;
                    memcpy(&data[0],&cmd->di,sizeof(cmd->di));
                    data[4]=cmd->seq;
                }
            }
        }
        break;
        default:
        {
            ctx->status=HDLT645_MASTER_CTX_STATUS_ERROR;
        }
        break;
        }

        if(hdlt645_master_ctx_status(ctx) == HDLT645_MASTER_CTX_STATUS_SEND_REQUEST)
        {
            {
                /*
                 * 检查控制码
                 */
                hdlt645_control_t c;
                c.fct=ctx->cmd.fct;
                (*hdlt645_frame_get_c(buffer,buffer_size))=hdlt645_control_encode(c);
            }

            {
                /*
                 * 检查数据长度
                 */
                uint8_t data_len=(*hdlt645_frame_get_datalen(buffer,buffer_size));
                if(data_len+12 > buffer_size)
                {
                    ctx->status=HDLT645_MASTER_CTX_STATUS_ERROR;
                    break;
                }
                buffer_size=data_len+12;
            }

            if(!hdlt645_frame_pack(buffer,buffer_size))
            {
                ctx->status=HDLT645_MASTER_CTX_STATUS_ERROR;
                break;
            }

            if(io->send == NULL || io->send(io,buffer,buffer_size) <= 0)
            {
                ctx->status=HDLT645_MASTER_CTX_STATUS_ERROR;
                break;
            }

            if(need_reply)
            {
                ctx->status =  HDLT645_MASTER_CTX_STATUS_WAIT_REPLY;
            }
            else
            {
                ctx->status =  HDLT645_MASTER_CTX_STATUS_FINISHED;
            }
        }
    }
    break;
    case HDLT645_MASTER_CTX_STATUS_WAIT_REPLY:
    {
        /*
         * 此步骤用于接收数据并处理数据
         */
        hdlt645_master_io_status_t status=hdlt645_master_io_status(io);
        if(status != HDLT645_MASTER_IO_STATUS_TIMEOUT && status != HDLT645_MASTER_IO_STATUS_EOF)
        {
            break;
        }

        uint8_t *buffer=io->rx_buffer;
        size_t   buffer_size=io->rx_buffer_index;
        if(!hdlt645_frame_unpack(buffer,buffer_size))
        {
            ctx->status=HDLT645_MASTER_CTX_STATUS_ERROR;
            break;
        }

        switch(ctx->cmd.fct)
        {
        case HDLT645_FRAME_CONTROL_FCT_READ:
        {
            hdlt645_master_ctx_cmd_read_t *cmd=(hdlt645_master_ctx_cmd_read_t *)ctx->cmd.ctx;

            {
                /*
                 * 读取地址
                 */
                hdlt645_bcd_addr_t *frame_addr=hdlt645_frame_get_bcd_addr(buffer,buffer_size);
                if(frame_addr!=NULL)
                {
                    memcpy(&cmd->addr,frame_addr,sizeof(cmd->addr));
                }
            }

            hdlt645_control_t c;

            {
                c=hdlt645_control_decode(*hdlt645_frame_get_c(buffer,buffer_size));

                if(c.ext == 1)
                {
                    cmd->need_readext=true;
                }
            }

            {
                /*
                 * 读取数据
                 */
                uint8_t *data=hdlt645_frame_get_data(buffer,buffer_size);
                uint8_t *datalen=hdlt645_frame_get_datalen(buffer,buffer_size);
                if(data != NULL &&datalen != NULL)
                {
                    if(c.ack==1)
                    {
                        if((*datalen) >= 1 &&cmd->error!=NULL)
                        {
                            cmd->error(cmd,data[0]);
                        }
                    }
                    else
                    {
                        if((*datalen) >= 4 &&cmd->read!=NULL)
                        {
                            cmd->read(cmd,(hdlt645_data_di_t *)&data[0],&data[sizeof(hdlt645_data_di_t)],(*datalen)-sizeof(hdlt645_data_di_t));
                        }
                    }
                }
            }
        }
        break;
        case HDLT645_FRAME_CONTROL_FCT_READEXT:
        {
            hdlt645_master_ctx_cmd_readext_t *cmd=(hdlt645_master_ctx_cmd_readext_t *)ctx->cmd.ctx;

            {
                /*
                 * 读取地址
                 */
                hdlt645_bcd_addr_t *frame_addr=hdlt645_frame_get_bcd_addr(buffer,buffer_size);
                if(frame_addr!=NULL)
                {
                    memcpy(&cmd->addr,frame_addr,sizeof(cmd->addr));
                }
            }

            hdlt645_control_t c;

            {
                c=hdlt645_control_decode(*hdlt645_frame_get_c(buffer,buffer_size));

                if(c.ext == 1)
                {
                    cmd->need_readext=true;
                }
            }

            {
                /*
                 * 读取数据
                 */
                uint8_t *data=hdlt645_frame_get_data(buffer,buffer_size);
                uint8_t *datalen=hdlt645_frame_get_datalen(buffer,buffer_size);
                if(data != NULL &&datalen != NULL)
                {
                    if(c.ack==1)
                    {
                        if((*datalen) >= 1 &&cmd->error!=NULL)
                        {
                            cmd->error(cmd,data[0]);
                        }
                    }
                    else
                    {
                        if((*datalen) >= 5 &&cmd->read!=NULL)
                        {
                            cmd->read(cmd,(hdlt645_data_di_t *)&data[0],&data[sizeof(hdlt645_data_di_t)],(*datalen)-sizeof(hdlt645_data_di_t)-1,data[(*datalen)-1]);
                        }
                    }
                }
            }
        }
        break;
        default:
        {
            ctx->status=HDLT645_MASTER_CTX_STATUS_ERROR;
        }
        break;
        }

        hdlt645_master_io_rx_reset(io);

        if(hdlt645_master_ctx_status(ctx) == HDLT645_MASTER_CTX_STATUS_WAIT_REPLY)
        {
            ctx->status=HDLT645_MASTER_CTX_STATUS_FINISHED;
        }
    }
    break;
    default:
    {

    }
    break;
    }

    return hdlt645_master_ctx_status(ctx);

}

void hdlt645_master_ctx_cmd_time_init(hdlt645_master_ctx_cmd_time_t *cmd,hdlt645_master_ctx_cmd_time_callback_t cb,void *usr)
{
    if(cmd!=NULL)
    {
        cmd->cb=cb;
        cmd->usr=(uintptr_t)usr;
    }
}

htime_t hdlt645_master_ctx_cmd_time_current(hdlt645_master_ctx_cmd_time_t *cmd)
{
    if(cmd == NULL || cmd->cb == NULL)
    {
        return htime(NULL);
    }

    return cmd->cb(cmd);
}

bool hdlt645_master_ctx_cmd_read_init1(hdlt645_master_ctx_cmd_read_t *cmd,hdlt645_bcd_addr_t *dst_addr,hdlt645_data_di_t *di,hdlt645_master_ctx_cmd_read_callback_t read,hdlt645_master_ctx_cmd_read_error_callback_t error,void *usr)
{
    if(cmd==NULL)
    {
        return false;
    }

    memset(cmd,0,sizeof(*cmd));

    bool ret=true;

    if(dst_addr != NULL)
    {
        memcpy(&cmd->addr,dst_addr,sizeof(*dst_addr));
    }
    else
    {
        hdlt645_bcd_addr_set(&cmd->addr,HDLT645_FRAME_ANYCAST_BCD_ADDR);
    }

    if(di!=NULL)
    {
        memcpy(cmd->request_buffer,di,sizeof(*di));
        cmd->request_buffer_length=sizeof(*di);
    }

    cmd->read=read;
    cmd->error=error;
    cmd->usr=(uintptr_t)usr;

    return ret;
}

bool hdlt645_master_ctx_cmd_read_init2(hdlt645_master_ctx_cmd_read_t *cmd,hdlt645_bcd_addr_t *dst_addr,hdlt645_data_di_t *di,uint8_t n,hdlt645_master_ctx_cmd_read_callback_t read,hdlt645_master_ctx_cmd_read_error_callback_t error,void *usr)
{
    if(cmd==NULL)
    {
        return false;
    }

    memset(cmd,0,sizeof(*cmd));

    bool ret=true;

    if(dst_addr != NULL)
    {
        memcpy(&cmd->addr,dst_addr,sizeof(*dst_addr));
    }
    else
    {
        hdlt645_bcd_addr_set(&cmd->addr,HDLT645_FRAME_ANYCAST_BCD_ADDR);
    }

    if(di!=NULL)
    {
        memcpy(cmd->request_buffer,di,sizeof(*di));
    }

    cmd->request_buffer[sizeof(*di)]=n;

    cmd->request_buffer_length=sizeof(*di)+sizeof(n);

    cmd->read=read;
    cmd->error=error;
    cmd->usr=(uintptr_t)usr;

    return ret;
}

bool hdlt645_master_ctx_cmd_read_init3(hdlt645_master_ctx_cmd_read_t *cmd,hdlt645_bcd_addr_t *dst_addr,hdlt645_data_di_t *di,uint8_t n,htime_t timestamp,hdlt645_master_ctx_cmd_read_callback_t read,hdlt645_master_ctx_cmd_read_error_callback_t error,void *usr)
{
    if(cmd==NULL)
    {
        return false;
    }

    memset(cmd,0,sizeof(*cmd));

    bool ret=true;

    if(dst_addr != NULL)
    {
        memcpy(&cmd->addr,dst_addr,sizeof(*dst_addr));
    }
    else
    {
        hdlt645_bcd_addr_set(&cmd->addr,HDLT645_FRAME_ANYCAST_BCD_ADDR);
    }

    if(di!=NULL)
    {
        memcpy(cmd->request_buffer,di,sizeof(*di));
    }

    cmd->request_buffer[sizeof(*di)]=n;

    {
        htm_t timestamp_tm;
        hlibc_localtime_r(&timestamp,&timestamp_tm);
        cmd->request_buffer[sizeof(*di)+sizeof(n)+0]=hdlt645_uint64_to_bcd(timestamp_tm.tm_min);
        cmd->request_buffer[sizeof(*di)+sizeof(n)+1]=hdlt645_uint64_to_bcd(timestamp_tm.tm_hour);
        cmd->request_buffer[sizeof(*di)+sizeof(n)+2]=hdlt645_uint64_to_bcd(timestamp_tm.tm_mday);
        cmd->request_buffer[sizeof(*di)+sizeof(n)+3]=hdlt645_uint64_to_bcd(timestamp_tm.tm_mon+1);
        cmd->request_buffer[sizeof(*di)+sizeof(n)+4]=hdlt645_uint64_to_bcd(timestamp_tm.tm_year%100);

    }
    cmd->request_buffer_length=sizeof(*di)+sizeof(n)+5;

    cmd->read=read;
    cmd->error=error;
    cmd->usr=(uintptr_t)usr;

    return ret;
}

bool hdlt645_master_ctx_cmd_readext_init(hdlt645_master_ctx_cmd_readext_t *cmd,hdlt645_bcd_addr_t *dst_addr,hdlt645_data_di_t *di,uint8_t seq,hdlt645_master_ctx_cmd_readext_callback_t read,hdlt645_master_ctx_cmd_readext_error_callback_t error,void *usr)
{
    if(cmd==NULL)
    {
        return false;
    }

    memset(cmd,0,sizeof(*cmd));

    bool ret=true;

    if(dst_addr != NULL)
    {
        memcpy(&cmd->addr,dst_addr,sizeof(*dst_addr));
    }
    else
    {
        hdlt645_bcd_addr_set(&cmd->addr,HDLT645_FRAME_ANYCAST_BCD_ADDR);
    }

    if(di!=NULL)
    {
        memcpy(&cmd->di,di,sizeof(*di));
    }

    if(seq==0)
    {
        seq=1;
    }
    cmd->seq=seq;

    cmd->read=read;
    cmd->error=error;
    cmd->usr=(uintptr_t)usr;

    return ret;
}
