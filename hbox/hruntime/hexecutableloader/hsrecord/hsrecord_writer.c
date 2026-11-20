/***************************************************************
 * Name:      hsrecord_writer.c
 * Purpose:   实现hsrecord_writer接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hsrecord_writer.h"

/** \brief 设置写入错误(保留首个错误)
 *
 * \param writer hsrecord_writer_t* 写入者指针
 * \param error hsrecord_writer_error_t 错误
 *
 */
static void hsrecord_writer_set_error(hsrecord_writer_t *writer,hsrecord_writer_error_t error)
{
    if(writer!=NULL && writer->error==HSRECORD_WRITER_ERROR_NONE)
    {
        writer->error=error;
    }
}

/** \brief 写入一行S记录
 *
 * \param writer hsrecord_writer_t* 写入者指针
 * \param tag uint8_t 记录类型
 * \param address uint32_t 地址
 * \param address_length size_t 地址字节数
 * \param data const uint8_t* 数据
 * \param data_len size_t 数据长度
 *
 */
static void hsrecord_writer_write_inner(hsrecord_writer_t *writer,uint8_t tag,uint32_t address,size_t address_length,const uint8_t *data,size_t data_len)
{
    if(writer==NULL || writer->writeline==NULL)
    {
        return;
    }

    if(address_length==0 || address_length > 4)
    {
        hsrecord_writer_set_error(writer,HSRECORD_WRITER_ERROR_INVALID_PARAM);
        return;
    }

    if(data_len > HSRECORD_MAX_DATA_LEN)
    {
        hsrecord_writer_set_error(writer,HSRECORD_WRITER_ERROR_DATA_TOO_LONG);
        return;
    }

    //长度字段(1字节)+地址+数据+校验(1字节)
    size_t line_binary_len=1+address_length+data_len+1;
    if(line_binary_len > sizeof(writer->buffer.line_binary))
    {
        hsrecord_writer_set_error(writer,HSRECORD_WRITER_ERROR_DATA_TOO_LONG);
        return;
    }

    uint8_t *line_binary=writer->buffer.line_binary;
    memset(line_binary,0,sizeof(writer->buffer.line_binary));

    //长度字段包含地址、数据与校验
    line_binary[0]=(uint8_t)(address_length+data_len+1);

    //地址(大端序)
    hsrecord_common_encode_big_endian(&line_binary[1],address,address_length);

    //数据
    if(data!=NULL && data_len > 0)
    {
        memcpy(&line_binary[1+address_length],data,data_len);
    }

    //校验(反码,S记录所有字节(含校验)之和低8位为0xFF)
    {
        uint8_t sum=0;
        for(size_t i=0; i < (line_binary_len-1); i++)
        {
            sum+=line_binary[i];
        }
        line_binary[line_binary_len-1]=(uint8_t)(~sum);
    }

    //转化为文本
    {
        static const char hex_table[]="0123456789ABCDEF";
        uint8_t *line=writer->buffer.line;
        size_t line_len=0;

        line[line_len++]='S';
        line[line_len++]=(uint8_t)('0'+tag);
        for(size_t i=0; i<line_binary_len; i++)
        {
            line[line_len++]=(uint8_t)hex_table[(line_binary[i]>>4)&0xF];
            line[line_len++]=(uint8_t)hex_table[(line_binary[i]>>0)&0xF];
        }
        line[line_len]='\0';

        writer->writeline(writer,(const char *)line,line_len);
    }
}

void hsrecord_writer_init(hsrecord_writer_t *writer,hsrecord_writer_writeline_t writeline,void *usr)
{
    if(writer==NULL || writeline==NULL)
    {
        return;
    }
    memset(writer,0,sizeof(hsrecord_writer_t));
    hsrecord_common_buffer_init(&writer->buffer);
    writer->writeline=writeline;
    writer->usr=usr;
    writer->page_size=HSRECORD_WRITER_PAGE_SIZE;
    if(writer->page_size > HSRECORD_MAX_DATA_LEN)
    {
        writer->page_size=HSRECORD_MAX_DATA_LEN;
    }
    if(writer->page_size==0)
    {
        writer->page_size=1;
    }
    writer->address_length=2;
    writer->enable_data_count=true;
    writer->error=HSRECORD_WRITER_ERROR_NONE;
}

void hsrecord_writer_set_page_size(hsrecord_writer_t *writer,size_t page_size)
{
    if(writer==NULL)
    {
        return;
    }
    if(page_size==0)
    {
        page_size=1;
    }
    if(page_size > HSRECORD_MAX_DATA_LEN)
    {
        page_size=HSRECORD_MAX_DATA_LEN;
    }
    writer->page_size=page_size;
}

void hsrecord_writer_set_address_length(hsrecord_writer_t *writer,size_t address_length)
{
    if(writer==NULL)
    {
        return;
    }
    if(address_length < 2)
    {
        address_length=2;
    }
    if(address_length > 4)
    {
        address_length=4;
    }
    writer->address_length=address_length;
}

void hsrecord_writer_set_address_type(hsrecord_writer_t *writer,hsrecord_writer_address_type_t address_type)
{
    hsrecord_writer_set_address_length(writer,(size_t)address_type);
}

void hsrecord_writer_write_header(hsrecord_writer_t *writer,const uint8_t *data,size_t data_len)
{
    if(writer==NULL || writer->writeline==NULL)
    {
        return;
    }
    if(data==NULL)
    {
        data_len=0;
    }
    if(data_len > HSRECORD_MAX_DATA_LEN)
    {
        data_len=HSRECORD_MAX_DATA_LEN;
        hsrecord_writer_set_error(writer,HSRECORD_WRITER_ERROR_DATA_TOO_LONG);
    }
    hsrecord_writer_write_inner(writer,HSRECORD_TYPE_HEADER,0,2,data,data_len);
}

void hsrecord_writer_write_data(hsrecord_writer_t *writer,uint32_t address,const uint8_t *data,size_t data_len)
{
    if(writer==NULL || writer->writeline==NULL)
    {
        return;
    }
    if(data==NULL || data_len==0)
    {
        return;
    }

    while(data_len > 0)
    {
        size_t address_length=writer->address_length;
        uint8_t tag=HSRECORD_TYPE_DATA_16BIT;

        if(address_length < 2)
        {
            address_length=2;
        }
        if(address_length > 4)
        {
            address_length=4;
        }

        if(address < (1UL << 16) && address_length <= 2)
        {
            address_length=2;
            tag=HSRECORD_TYPE_DATA_16BIT;
        }
        else if(address < (1UL << 24) && address_length <= 3)
        {
            address_length=3;
            tag=HSRECORD_TYPE_DATA_24BIT;
        }
        else
        {
            address_length=4;
            tag=HSRECORD_TYPE_DATA_32BIT;
        }

        size_t bytes_to_write=writer->page_size;
        if(bytes_to_write==0)
        {
            bytes_to_write=HSRECORD_WRITER_PAGE_SIZE;
        }
        //长度字段最大值255,即地址+数据+1不大于255
        size_t max_bytes_to_write=254-address_length;
        if(bytes_to_write > max_bytes_to_write)
        {
            bytes_to_write=max_bytes_to_write;
        }
        if(bytes_to_write > data_len)
        {
            bytes_to_write=data_len;
        }
        if(bytes_to_write==0)
        {
            hsrecord_writer_set_error(writer,HSRECORD_WRITER_ERROR_DATA_TOO_LONG);
            break;
        }

        hsrecord_writer_write_inner(writer,tag,address,address_length,data,bytes_to_write);

        writer->data_count++;
        writer->data_count_written=false;

        address+=(uint32_t)bytes_to_write;
        data+=bytes_to_write;
        data_len-=bytes_to_write;
    }
}

void hsrecord_writer_write_data_count(hsrecord_writer_t *writer)
{
    if(writer==NULL || writer->writeline==NULL)
    {
        return;
    }
    if(writer->data_count_written)
    {
        return;
    }

    if(writer->enable_data_count)
    {
        if(writer->data_count < (1UL << 16))
        {
            hsrecord_writer_write_inner(writer,HSRECORD_TYPE_DATA_COUNT_16BIT,writer->data_count,2,NULL,0);
        }
        else
        {
            hsrecord_writer_write_inner(writer,HSRECORD_TYPE_DATA_COUNT_24BIT,writer->data_count,3,NULL,0);
        }
    }

    writer->data_count_written=true;
    writer->data_count=0;
}

void hsrecord_writer_write_start_address(hsrecord_writer_t *writer,uint32_t address)
{
    if(writer==NULL || writer->writeline==NULL)
    {
        return;
    }

    //与srecord一致,在写入起始地址之前写入数据记录计数
    hsrecord_writer_write_data_count(writer);

    size_t address_length=writer->address_length;
    if(address_length < 2)
    {
        address_length=2;
    }
    if(address_length > 4)
    {
        address_length=4;
    }

    if(address < (1UL << 16) && address_length <= 2)
    {
        hsrecord_writer_write_inner(writer,HSRECORD_TYPE_START_ADDRESS_16BIT,address,2,NULL,0);
    }
    else if(address < (1UL << 24) && address_length <= 3)
    {
        hsrecord_writer_write_inner(writer,HSRECORD_TYPE_START_ADDRESS_24BIT,address,3,NULL,0);
    }
    else
    {
        hsrecord_writer_write_inner(writer,HSRECORD_TYPE_START_ADDRESS_32BIT,address,4,NULL,0);
    }
}

void hsrecord_writer_write_end_of_file(hsrecord_writer_t *writer)
{
    hsrecord_writer_write_start_address(writer,0);
}
