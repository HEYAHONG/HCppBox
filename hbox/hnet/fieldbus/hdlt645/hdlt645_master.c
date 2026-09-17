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

    switch(fct)
    {
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

        switch(ctx->cmd.fct)
        {
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

            ctx->status =  HDLT645_MASTER_CTX_STATUS_WAIT_REPLY;
        }
    }
    break;
    HDLT645_MASTER_CTX_STATUS_WAIT_REPLY:
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
