/***************************************************************
 * Name:      hdlt645_slave.c
 * Purpose:   实现hdlt645_slave接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-08-31
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include "hdlt645_slave.h"
#include "hdlt645_utils.h"

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

#if !defined(HDLT645_SLAVE_TIME_SYNC)
#include "hdefaults.h"

void hdlt645_slave_time_sync_default(const hdlt645_slave_time_t *time,uint8_t ss,uint8_t mm,uint8_t hh,uint8_t DD,uint8_t MM,uint8_t YY)
{
    htimeval_t tv= {0};
    {
        htm_t tm= {0};
        tm.tm_sec=hdlt645_bcd_to_uint64(ss);
        tm.tm_min=hdlt645_bcd_to_uint64(mm);
        tm.tm_hour=hdlt645_bcd_to_uint64(hh);
        tm.tm_mday=hdlt645_bcd_to_uint64(DD);
        tm.tm_mon=hdlt645_bcd_to_uint64(MM)-1;
        tm.tm_year=hdlt645_bcd_to_uint64(YY)+2000;
        tv.tv_sec=hlibc_mktime(&tm);
    }
    hsettimeofday(&tv,NULL);
}

const hdlt645_slave_time_t hdlt645_slave_time_default=
{
    hdlt645_slave_time_sync_default,
    0
};

#define HDLT645_SLAVE_TIME_SYNC (&hdlt645_slave_time_default)
#endif

#if !defined(HDLT645_SLAVE_DI_TABLE)
#define HDLT645_SLAVE_DI_TABLE NULL
#endif
#if !defined(HDLT645_SLAVE_DI_TABLE_SIZE)
#define HDLT645_SLAVE_DI_TABLE_SIZE 0
#endif

#if !defined(HDLT645_SLAVE_READ_DI_TABLE)
#define HDLT645_SLAVE_READ_DI_TABLE HDLT645_SLAVE_DI_TABLE
#endif
#if !defined(HDLT645_SLAVE_READ_DI_TABLE_SIZE)
#define HDLT645_SLAVE_READ_DI_TABLE_SIZE HDLT645_SLAVE_DI_TABLE_SIZE
#endif

#if !defined(HDLT645_SLAVE_WRITE_DI_TABLE)
#define HDLT645_SLAVE_WRITE_DI_TABLE HDLT645_SLAVE_DI_TABLE
#endif
#if !defined(HDLT645_SLAVE_WRITE_DI_TABLE_SIZE)
#define HDLT645_SLAVE_WRITE_DI_TABLE_SIZE HDLT645_SLAVE_DI_TABLE_SIZE
#endif

#if !defined(HDLT645_SLAVE_WRITEADDR)
#define HDLT645_SLAVE_WRITEADDR NULL
#endif

#if !defined(HDLT645_SLAVE_FREEZE)
#define HDLT645_SLAVE_FREEZE NULL
#endif


static const hdlt645_slave_io_ctx_cmd_t hdlt645_slave_io_ctx_cmd_default[]=
{
    HDLT645_SLAVE_IO_CTX_CMD_TIME(hdlt645_slave_io_ctx_cmd_time_process,HDLT645_SLAVE_TIME_SYNC),
    HDLT645_SLAVE_IO_CTX_CMD_READ(hdlt645_slave_io_ctx_cmd_read_process,HDLT645_SLAVE_READ_DI_TABLE,HDLT645_SLAVE_READ_DI_TABLE_SIZE),
    HDLT645_SLAVE_IO_CTX_CMD_READEXT(hdlt645_slave_io_ctx_cmd_readext_process,HDLT645_SLAVE_READ_DI_TABLE,HDLT645_SLAVE_READ_DI_TABLE_SIZE),
    HDLT645_SLAVE_IO_CTX_CMD_READADDR(hdlt645_slave_io_ctx_cmd_readaddr_process),
    HDLT645_SLAVE_IO_CTX_CMD_WRITE(hdlt645_slave_io_ctx_cmd_write_process,HDLT645_SLAVE_WRITE_DI_TABLE,HDLT645_SLAVE_WRITE_DI_TABLE_SIZE),
    HDLT645_SLAVE_IO_CTX_CMD_WRITEADDR(hdlt645_slave_io_ctx_cmd_writeaddr_process,HDLT645_SLAVE_WRITEADDR),
    HDLT645_SLAVE_IO_CTX_CMD_FREEZE(hdlt645_slave_io_ctx_cmd_freeze_process,HDLT645_SLAVE_FREEZE),
    HDLT645_SLAVE_IO_CTX_CMD_END(),
};

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

    if(ctx->cmd_table==NULL)
    {
        ctx->cmd_table=hdlt645_slave_io_ctx_cmd_default;
    }

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
            if(cmd_table->fct==0 && cmd_table->process==NULL)
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

size_t hdlt645_slave_di_count(const hdlt645_slave_di_t *di_table,size_t di_table_len,uint32_t di_dst_num,size_t max_len)
{
    if(di_table==NULL || di_table_len == 0 || max_len == 0)
    {
        return 0;
    }

    size_t len=0;

    size_t ret=0;

    for(size_t i=0; i<di_table_len; i++)
    {
        if(di_table[i].getlen==NULL || di_table[i].read == NULL || di_table[i].write == NULL)
        {
            break;
        }
        hdlt645_data_di_t di_src;
        hdlt645_data_di_set(&di_src,di_table[i].di_num);
        hdlt645_data_di_t di_dst;
        hdlt645_data_di_set(&di_dst,di_dst_num);
        if(!hdlt645_data_di_match(&di_src,&di_src))
        {
            continue;
        }

        size_t di_len=di_table[i].getlen(&di_table[i]);

        if(di_len + len > max_len)
        {
            len=di_len;
            ret++;
        }
        else
        {
            len+=di_len;
        }
    }

    if(len != 0)
    {
        ret++;
    }
    return ret;
}

size_t hdlt645_slave_di_read(const hdlt645_slave_di_t *di_table,size_t di_table_len,uint32_t di_dst_num,size_t index,uint8_t *data,size_t datalen)
{
    size_t ret=0;

    if(di_table==NULL || di_table_len == 0 || data == NULL || datalen == 0 || index >=  hdlt645_slave_di_count(di_table,di_table_len,di_dst_num,datalen))
    {
        return 0;
    }

    size_t index_dst=0;
    size_t index_dst_len=0;

    for(size_t i=0; i<di_table_len; i++)
    {
        if(di_table[i].getlen==NULL || di_table[i].read == NULL || di_table[i].write == NULL)
        {
            break;
        }
        hdlt645_data_di_t di_src;
        hdlt645_data_di_set(&di_src,di_table[i].di_num);
        hdlt645_data_di_t di_dst;
        hdlt645_data_di_set(&di_dst,di_dst_num);
        if(!hdlt645_data_di_match(&di_src,&di_src))
        {
            continue;
        }

        size_t read_len=di_table[i].getlen(&di_table[i]);

        if(read_len + index_dst_len > datalen)
        {
            index_dst_len=read_len;
            index_dst++;
        }
        else
        {
            index_dst_len+=read_len;
        }

        if(index!=index_dst)
        {
            continue;
        }

        if((datalen-ret) < read_len)
        {
            read_len=datalen-ret;
        }

        read_len=di_table[i].read(&di_table[i],&data[ret],read_len);

        ret+=read_len;

    }

    return ret;


}

size_t hdlt645_slave_di_write(const hdlt645_slave_di_t *di_table,size_t di_table_len,uint32_t di_dst_num,const uint8_t *data,size_t datalen)
{
    size_t ret=0;

    if(di_table==NULL || di_table_len == 0 || data == NULL || datalen == 0)
    {
        return 0;
    }

    for(size_t i=0; i<di_table_len; i++)
    {
        if(di_table[i].getlen==NULL || di_table[i].read == NULL || di_table[i].write == NULL)
        {
            break;
        }
        hdlt645_data_di_t di_src;
        hdlt645_data_di_set(&di_src,di_table[i].di_num);
        hdlt645_data_di_t di_dst;
        hdlt645_data_di_set(&di_dst,di_dst_num);
        if(!hdlt645_data_di_match(&di_src,&di_src))
        {
            continue;
        }

        size_t write_len=di_table[i].getlen(&di_table[i]);

        if((datalen-ret) < write_len)
        {
            write_len=datalen-ret;
        }

        write_len=di_table[i].write(&di_table[i],&data[ret],write_len);

        ret+=write_len;

    }

    return ret;
}

bool hdlt645_slave_io_ctx_cmd_time_process(hdlt645_slave_io_ctx_t *ctx,hdlt645_slave_io_t *io,const hdlt645_slave_io_ctx_cmd_t *cmd,uint8_t *data,size_t datalen,uint8_t *reply_buffer,size_t reply_buffer_len)
{
    if(ctx==NULL || io == NULL || cmd == NULL || data == NULL || datalen < 6 || reply_buffer == NULL || reply_buffer_len < 12)
    {
        return false;
    }

    const hdlt645_slave_time_t *m_time=(const hdlt645_slave_time_t *)cmd->usr[0];

    if(m_time!=NULL)
    {
        if(m_time->time_sync!=NULL)
        {
            m_time->time_sync(m_time,data[0],data[1],data[2],data[3],data[4],data[5]);
        }
    }

    bool ret=false;

    return ret;
}


bool hdlt645_slave_io_ctx_cmd_read_process(hdlt645_slave_io_ctx_t *ctx,hdlt645_slave_io_t *io,const hdlt645_slave_io_ctx_cmd_t *cmd,uint8_t *data,size_t datalen,uint8_t *reply_buffer,size_t reply_buffer_len)
{
    if(ctx==NULL || io == NULL || cmd == NULL || data == NULL || datalen < 4 || reply_buffer == NULL || reply_buffer_len < 12+4)
    {
        return false;
    }

    const hdlt645_slave_di_t *di_table=(const hdlt645_slave_di_t *)cmd->usr[0];
    size_t di_table_len=cmd->usr[1];
    size_t reply_data_buffer_len=reply_buffer_len-12-4;
    uint8_t *reply_data_buffer=&(hdlt645_frame_get_data(reply_buffer,reply_buffer_len)[4]);
    hdlt645_data_di_t *reply_di=(hdlt645_data_di_t *)&(hdlt645_frame_get_data(reply_buffer,reply_buffer_len)[0]);

    hdlt645_control_t c=hdlt645_control_decode(0);

    c.dir=1;

    c.fct=cmd->fct;

    hdlt645_data_di_t *di_src=(hdlt645_data_di_t *)data;
    if(reply_di!=NULL)
    {
        memcpy(reply_di,di_src,sizeof(*di_src));
    }

    size_t index=0;

    if(datalen >= 5)
    {
        index=data[4];
    }

    if(datalen >=10)
    {
        if(di_table != NULL)
        {
            for(size_t i=0; i<di_table_len; i++)
            {
                if(di_table[i].set_time!=NULL)
                {
                    hdlt645_data_di_t di_dst;
                    hdlt645_data_di_set(&di_dst,di_table[i].di_num);
                    if(!hdlt645_data_di_match(di_src,&di_dst))
                    {
                        continue;
                    }
                    di_table[i].set_time(&di_table[i],data[5],data[6],data[7],data[8],data[9]);
                }
            }
        }

    }
    else
    {
        if(di_table != NULL)
        {
            for(size_t i=0; i<di_table_len; i++)
            {
                if(di_table[i].set_time!=NULL)
                {
                    hdlt645_data_di_t di_dst;
                    hdlt645_data_di_set(&di_dst,di_table[i].di_num);
                    if(!hdlt645_data_di_match(di_src,&di_dst))
                    {
                        continue;
                    }
                    di_table[i].reset_time(&di_table[i]);
                }
            }
        }
    }

    if(hdlt645_slave_di_count(di_table,di_table_len,hdlt645_data_di_get(di_src),reply_data_buffer_len) > index+1)
    {
        c.ext=1;
    }

    bool ret=true;

    /*
     * 读取数据
     */
    reply_data_buffer_len=hdlt645_slave_di_read(di_table,di_table_len,hdlt645_data_di_get(di_src),index,reply_data_buffer,reply_data_buffer_len);

    /*
     * 设置控制码
     */
    (*hdlt645_frame_get_c(reply_buffer,reply_buffer_len))=hdlt645_control_encode(c);


    /*
     * 设置数据长度
     */
    size_t l=reply_data_buffer_len+sizeof(*di_src);
    (*hdlt645_frame_get_datalen(reply_buffer,reply_buffer_len))=l;

    return ret;
}

bool hdlt645_slave_io_ctx_cmd_readext_process(hdlt645_slave_io_ctx_t *ctx,hdlt645_slave_io_t *io,const hdlt645_slave_io_ctx_cmd_t *cmd,uint8_t *data,size_t datalen,uint8_t *reply_buffer,size_t reply_buffer_len)
{
    if(ctx==NULL || io == NULL || cmd == NULL || data == NULL || datalen < 4 || reply_buffer == NULL || reply_buffer_len < 12)
    {
        return false;
    }

    bool ret=true;

    /*
     *  在本协议栈中，读后续数据中序号等效于读数据的记录块数。注意：这是非标实现，用户如需其它实现请自行实现处理函数
     */
    ret=hdlt645_slave_io_ctx_cmd_read_process(ctx,io,cmd,data,datalen,reply_buffer,reply_buffer_len);

    return ret;
}

bool hdlt645_slave_io_ctx_cmd_readaddr_process(hdlt645_slave_io_ctx_t *ctx,hdlt645_slave_io_t *io,const hdlt645_slave_io_ctx_cmd_t *cmd,uint8_t *data,size_t datalen,uint8_t *reply_buffer,size_t reply_buffer_len)
{
    if(ctx==NULL || io == NULL || cmd == NULL || reply_buffer == NULL || reply_buffer_len < 12+6)
    {
        return false;
    }

    bool ret=true;

    hdlt645_control_t c=hdlt645_control_decode(0);

    c.dir=1;

    c.fct=cmd->fct;


    /*
    * 设置控制码
    */
    (*hdlt645_frame_get_c(reply_buffer,reply_buffer_len))=hdlt645_control_encode(c);

    /*
     * 读取地址
     */
    memcpy(hdlt645_frame_get_data(reply_buffer,reply_buffer_len),&ctx->addr,sizeof(ctx->addr));


    /*
     * 设置数据长度
     */
    size_t l=sizeof(hdlt645_bcd_addr_t);
    (*hdlt645_frame_get_datalen(reply_buffer,reply_buffer_len))=l;

    return ret;
}

bool hdlt645_slave_io_ctx_cmd_write_process(hdlt645_slave_io_ctx_t *ctx,hdlt645_slave_io_t *io,const hdlt645_slave_io_ctx_cmd_t *cmd,uint8_t *data,size_t datalen,uint8_t *reply_buffer,size_t reply_buffer_len)
{
    if(ctx==NULL || io == NULL || cmd == NULL || data == NULL || datalen < 12 || reply_buffer == NULL || reply_buffer_len < 12)
    {
        return false;
    }

    const hdlt645_slave_di_t *di_table=(const hdlt645_slave_di_t *)cmd->usr[0];
    size_t di_table_len=cmd->usr[1];

    hdlt645_control_t c=hdlt645_control_decode(0);

    c.dir=1;

    c.fct=cmd->fct;

    hdlt645_data_di_t *di_src=(hdlt645_data_di_t *)data;
    hdlt645_data_p_t *p_src=(hdlt645_data_p_t *)&data[4];
    hdlt645_data_c_t *c_src=(hdlt645_data_c_t *)&data[8];
    const uint8_t *data_src=(const uint8_t *)&data[12];
    size_t data_src_len=datalen-12;


    bool ret=true;

    /*
     * 写入数据
     */
    {
        /*
         * 写使能
         */
        if(di_table != NULL)
        {
            for(size_t i=0; i<di_table_len; i++)
            {
                if(di_table[i].set_time!=NULL)
                {
                    hdlt645_data_di_t di_dst;
                    hdlt645_data_di_set(&di_dst,di_table[i].di_num);
                    if(!hdlt645_data_di_match(di_src,&di_dst))
                    {
                        continue;
                    }
                    di_table[i].write_enable(&di_table[i],p_src,c_src);
                }
            }
        }

    }

    hdlt645_slave_di_write(di_table,di_table_len,hdlt645_data_di_get(di_src),data_src,data_src_len);

    {
        /*
         * 写使能（关闭）
         */
        if(di_table != NULL)
        {
            for(size_t i=0; i<di_table_len; i++)
            {
                if(di_table[i].set_time!=NULL)
                {
                    hdlt645_data_di_t di_dst;
                    hdlt645_data_di_set(&di_dst,di_table[i].di_num);
                    if(!hdlt645_data_di_match(di_src,&di_dst))
                    {
                        continue;
                    }
                    di_table[i].write_disable(&di_table[i]);
                }
            }
        }
    }

    /*
     * 设置控制码
     */
    (*hdlt645_frame_get_c(reply_buffer,reply_buffer_len))=hdlt645_control_encode(c);


    /*
     * 设置数据长度
     */
    size_t l=0;
    (*hdlt645_frame_get_datalen(reply_buffer,reply_buffer_len))=l;

    return ret;
}

bool hdlt645_slave_io_ctx_cmd_writeaddr_process(hdlt645_slave_io_ctx_t *ctx,hdlt645_slave_io_t *io,const hdlt645_slave_io_ctx_cmd_t *cmd,uint8_t *data,size_t datalen,uint8_t *reply_buffer,size_t reply_buffer_len)
{
    if(ctx==NULL || io == NULL || cmd == NULL || data == NULL || datalen < 6 || reply_buffer == NULL || reply_buffer_len < 12)
    {
        return false;
    }

    hdlt645_control_t c=hdlt645_control_decode(0);

    c.dir=1;

    c.fct=cmd->fct;

    hdlt645_bcd_addr_t *addr=(hdlt645_bcd_addr_t *)data;

    hdlt645_slave_writeaddr_t *writeaddr=(hdlt645_slave_writeaddr_t *)cmd->usr[0];

    bool ret=true;

    /*
    * 设置控制码
    */
    (*hdlt645_frame_get_c(reply_buffer,reply_buffer_len))=hdlt645_control_encode(c);

    /*
     * 写地址
     */
    if(writeaddr!=NULL)
    {
        if(writeaddr->write!=NULL)
        {
            ret=writeaddr->write(writeaddr,ctx,addr);

            if(ret)
            {
                memcpy(&ctx->addr,addr,sizeof(*addr));
            }

        }
    }

    /*
     * 设置数据长度
     */
    size_t l=0;
    (*hdlt645_frame_get_datalen(reply_buffer,reply_buffer_len))=l;

    return ret;
}

bool hdlt645_slave_io_ctx_cmd_freeze_process(hdlt645_slave_io_ctx_t *ctx,hdlt645_slave_io_t *io,const hdlt645_slave_io_ctx_cmd_t *cmd,uint8_t *data,size_t datalen,uint8_t *reply_buffer,size_t reply_buffer_len)
{
    if(ctx==NULL || io == NULL || cmd == NULL || data == NULL || datalen < 4 || reply_buffer == NULL || reply_buffer_len < 12)
    {
        return false;
    }

    hdlt645_control_t c=hdlt645_control_decode(0);

    c.dir=1;

    c.fct=cmd->fct;

    hdlt645_slave_freeze_t *freeze=(hdlt645_slave_freeze_t *)cmd->usr[0];

    bool ret=true;

    /*
    * 设置控制码
    */
    (*hdlt645_frame_get_c(reply_buffer,reply_buffer_len))=hdlt645_control_encode(c);

    if(freeze!=NULL)
    {
        freeze->freeze(freeze,data[0],data[1],data[2],data[3]);
    }

    /*
     * 设置数据长度
     */
    size_t l=0;
    (*hdlt645_frame_get_datalen(reply_buffer,reply_buffer_len))=l;

    return ret;
}
