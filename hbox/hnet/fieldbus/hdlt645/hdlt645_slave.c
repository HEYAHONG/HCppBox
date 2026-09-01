/***************************************************************
 * Name:      hdlt645_slave.c
 * Purpose:   实现hdlt645_slave接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-08-31
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include "hdlt645_slave.h"

void hdlt645_slave_io_init(hdlt645_slave_io_t *io,hdlt645_slave_io_cb_timeout_t timeout,hdlt645_slave_io_cb_reply_t reply,void *usr)
{
    if(io!=NULL)
    {
        memset(io,0,sizeof(*io));

        io->timeout=timeout;
        io->reply=reply;
        io->usr=usr;
    }
}

hdlt645_slave_io_status_t hdlt645_slave_io_status(hdlt645_slave_io_t *io)
{
    if(io==NULL)
    {
        return HDLT645_SLAVE_IO_STATUS_ERROR;
    }

    if(io->timeout!=NULL)
    {
        if(io->timeout(io))
        {
            return HDLT645_SLAVE_IO_STATUS_TIMEOUT;
        }
    }

    hdlt645_slave_io_status_t status=HDLT645_SLAVE_IO_STATUS_IDLE;

    switch(io->rx_buffer_index)
    {
    case 0:
    {

    }
    break;
    case 1:
    {
        status=HDLT645_SLAVE_IO_STATUS_SOF1;
    }
    break;
    case 8:
    {
        status=HDLT645_SLAVE_IO_STATUS_SOF2;
    }
    break;
    case 9:
    {
        status=HDLT645_SLAVE_IO_STATUS_C;
    }
    break;
    case 10:
    {
        status=HDLT645_SLAVE_IO_STATUS_DATALEN;
    }
    break;
    default:
    {
        size_t i=io->rx_buffer_index;
        if(i >= 2 && i < 8)
        {
            status=HDLT645_SLAVE_IO_STATUS_ADDR;
            break;
        }

        if(i >= 11)
        {
            uint8_t datalen=io->buffer[9];
            if(datalen > HDLT645_FRAME_DATALENGTH_MAX_WRITE)
            {
                status=HDLT645_SLAVE_IO_STATUS_ERROR;
                break;
            }

            if(i< 10+datalen+1)
            {
                status=HDLT645_SLAVE_IO_STATUS_DATA;
                break;
            }
            else if(i == 10+datalen+1)
            {
                status=HDLT645_SLAVE_IO_STATUS_CKSUM;
                break;
            }
            else if(i == 10+datalen+2)
            {
                status=HDLT645_SLAVE_IO_STATUS_EOF;
                break;
            }
        }
        status=HDLT645_SLAVE_IO_STATUS_ERROR;
    }
    break;
    }

    return HDLT645_SLAVE_IO_STATUS_IDLE;
}

void hdlt645_slave_io_rx_reset(hdlt645_slave_io_t *io)
{
    if(io!=NULL)
    {
        memset(io->rx_buffer,0,sizeof(io->rx_buffer));
        io->rx_buffer_index=0;
    }
}

size_t hdlt645_slave_io_rx_input(hdlt645_slave_io_t *io,uint8_t *data,size_t datalen)
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

void hdlt645_slave_io_ctx_init(hdlt645_slave_io_ctx_t *ctx,hdlt645_bcd_addr_t *addr,hdlt645_slave_io_ctx_cmd_t *cmd_table,void *usr)
{
    if(ctx==NULL)
    {
        return;
    }

    memset(ctx,0,sizeof(*ctx));

    if(addr!=NULL)
    {
        ctx->addr=(*addr);
    }

    ctx->cmd_table=cmd_table;

    ctx->usr=usr;
}

void hdlt645_slave_io_ctx_process_io(hdlt645_slave_io_ctx_t *ctx,hdlt645_slave_io_t *io)
{
    if(ctx==NULL || io==NULL)
    {
        return;
    }

    size_t frame_len=io->rx_buffer_index;

    if(frame_len < 12)
    {
        return;
    }

    hdlt645_slave_io_status_t status=hdlt645_slave_io_status(io);

    if(status != HDLT645_SLAVE_IO_STATUS_TIMEOUT && status != HDLT645_SLAVE_IO_STATUS_EOF)
    {
        return;
    }

    frame_len=io->rx_buffer_index;

    if(!hdlt645_frame_unpack(io->rx_buffer,frame_len))
    {
        hdlt645_slave_io_rx_reset(io);
        return;
    }

#if !defined(HDLT645_SLAVE_IO_NO_TX_BUFFER)
    uint8_t *reply_buffer=io->buffer;
    size_t reply_buffer_len=sizeof(io->buffer);
#else
    uint8_t buffer[12+HDLT645_FRAME_DATALENGTH_MAX_READ]= {0};                                      /**< 从机发送数据缓冲（受限于最大读取长度） */
    uint8_t *reply_buffer=buffer;
    size_t reply_buffer_len=sizeof(buffer);
#endif

    bool reply=true;

    memset(reply_buffer,0,reply_buffer_len);

    uint8_t *frame=io->rx_buffer;



    {
        /*
         * 检查地址
         */
        hdlt645_bcd_addr_t public_bcd_addr;
        hdlt645_bcd_addr_set(&public_bcd_addr,HDLT645_FRAME_BOARDCAST_BCD_ADDR);

        hdlt645_bcd_addr_t *frame_addr=hdlt645_frame_get_bcd_addr(frame,frame_len);

        if(frame_addr==NULL)
        {
            hdlt645_slave_io_rx_reset(io);
            return;
        }

        if(hdlt645_bcd_addr_match(frame_addr,&public_bcd_addr))
        {
            reply=false;
        }
        else if(!hdlt645_bcd_addr_match(frame_addr,&ctx->addr))
        {
            hdlt645_slave_io_rx_reset(io);
            return;
        }
    }

    {
        /*
         * 设定回复地址
         */
        memcpy(hdlt645_frame_get_bcd_addr(reply_buffer,reply_buffer_len),&ctx->addr,sizeof(ctx->addr));
    }

    hdlt645_control_t c;
    {
        /*
         * 控制码
         */
        uint8_t *c_num_ptr=hdlt645_frame_get_c(frame,frame_len);
        if(c_num_ptr==NULL)
        {
            hdlt645_slave_io_rx_reset(io);
            return;
        }

        c=hdlt645_control_decode(*c_num_ptr);
    }

    {
        /*
         * 设定回复控制码及数据长度
         */
        hdlt645_control_t reply_c=c;
        reply_c.dir=1;
        (*hdlt645_frame_get_c(reply_buffer,reply_buffer_len))=hdlt645_control_encode(reply_c);
        (*hdlt645_frame_get_datalen(reply_buffer,reply_buffer_len))=0;
    }

    {
        /*
         * 根据控制码处理命令
         */
        if(ctx->cmd_table==NULL)
        {
            hdlt645_slave_io_rx_reset(io);
            return;
        }
        const hdlt645_slave_io_ctx_cmd_t *cmd_table=ctx->cmd_table;
        while(true)
        {
            if(cmd_table->fct==0)
            {
                break;
            }

            if(cmd_table->fct == c.fct)
            {
                if(cmd_table->process!=NULL)
                {
                    if(!cmd_table->process(ctx,io,cmd_table,hdlt645_frame_get_data(frame,frame_len),*hdlt645_frame_get_datalen(frame,frame_len),reply_buffer,reply_buffer_len))
                    {
                        reply=false;
                    }
                }
                break;
            }

            cmd_table++;
        }
    }

    if(reply)
    {
        size_t reply_len=hdlt645_frame_pack(reply_buffer,reply_buffer_len);
        if(io->reply!=NULL)
        {
            io->reply(io,reply_buffer,reply_len);
        }
    }

    hdlt645_slave_io_rx_reset(io);
}

