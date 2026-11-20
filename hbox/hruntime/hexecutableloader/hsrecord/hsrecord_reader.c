/***************************************************************
 * Name:      hsrecord_reader.c
 * Purpose:   实现hsrecord_reader接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hsrecord_reader.h"

/** \brief 设置读取错误(保留首个错误)
 *
 * \param reader hsrecord_reader_t* 读取者指针
 * \param error hsrecord_reader_error_t 错误
 *
 */
static void hsrecord_reader_set_error(hsrecord_reader_t *reader,hsrecord_reader_error_t error)
{
    if(reader!=NULL && reader->error==HSRECORD_READER_ERROR_NONE)
    {
        reader->error=error;
    }
}

/** \brief 将单个十六进制字符转化为数值
 *
 * \param code uint8_t 字符
 * \return int 数值,无效字符返回-1
 *
 */
static int hsrecord_reader_hex_value(uint8_t code)
{
    if(code>='0' && code<='9')
    {
        return code-'0';
    }
    if(code>='A' && code<='F')
    {
        return code-'A'+0xA;
    }
    if(code>='a' && code<='f')
    {
        return code-'a'+0xa;
    }
    return -1;
}

/** \brief 处理单行
 *
 * \param reader hsrecord_reader_t* 读取者指针
 *
 */
static void hsrecord_reader_process_line(hsrecord_reader_t *reader)
{
    if(reader==NULL)
    {
        return;
    }

    uint8_t *line=reader->buffer.line;
    size_t line_length=reader->buffer.line_length;

    if(line_length==0)
    {
        return;
    }

    reader->line_number++;

    if(line_length < 2)
    {
        hsrecord_reader_set_error(reader,HSRECORD_READER_ERROR_INVALID_LENGTH);
        return;
    }

    int tag=line[1]-'0';
    if(tag < 0 || tag > 9 || tag==HSRECORD_TYPE_RESERVED)
    {
        hsrecord_reader_set_error(reader,HSRECORD_READER_ERROR_INVALID_RECORD_TYPE);
        return;
    }

    size_t hex_length=line_length-2;
    if((hex_length%2)!=0 || (hex_length/2) > sizeof(reader->buffer.line_binary))
    {
        hsrecord_reader_set_error(reader,HSRECORD_READER_ERROR_INVALID_HEX);
        return;
    }

    uint8_t *line_binary=reader->buffer.line_binary;
    size_t line_binary_len=hex_length/2;

    for(size_t i=0; i<line_binary_len; i++)
    {
        int high=hsrecord_reader_hex_value(line[2+i*2]);
        int low=hsrecord_reader_hex_value(line[3+i*2]);
        if(high < 0 || low < 0)
        {
            hsrecord_reader_set_error(reader,HSRECORD_READER_ERROR_INVALID_HEX);
            return;
        }
        line_binary[i]=(uint8_t)((high<<4)|low);
    }

    if(line_binary_len < 2)
    {
        hsrecord_reader_set_error(reader,HSRECORD_READER_ERROR_INVALID_LENGTH);
        return;
    }

    //长度字段包含地址、数据与校验
    if(((size_t)line_binary[0]+1) != line_binary_len)
    {
        hsrecord_reader_set_error(reader,HSRECORD_READER_ERROR_INVALID_LENGTH);
        return;
    }

    //校验:S记录使用反码校验,所有字节(含校验)之和低8位为0xFF
    {
        uint8_t sum=0;
        for(size_t i=0; i < (line_binary_len-1); i++)
        {
            sum+=line_binary[i];
        }
        if((uint8_t)(sum+line_binary[line_binary_len-1]) != 0xFF)
        {
            hsrecord_reader_set_error(reader,HSRECORD_READER_ERROR_INVALID_CHECKSUM);
            return;
        }
    }

    //payload长度为地址与数据的总长度,即长度字段减1
    size_t payload_len=(size_t)line_binary[0]-1;

    hsrecord_reader_operate_t op=HSRECORD_READER_OPERATE_DATA;
    size_t address_length=2;
    switch(tag)
    {
    case HSRECORD_TYPE_HEADER:
    {
        op=HSRECORD_READER_OPERATE_HEADER;
        address_length=2;
        if(payload_len < address_length)
        {
            //部分程序写入较短的头部
            address_length=payload_len;
        }
    }
    break;
    case HSRECORD_TYPE_DATA_16BIT:
    {
        op=HSRECORD_READER_OPERATE_DATA;
        address_length=2;
    }
    break;
    case HSRECORD_TYPE_DATA_24BIT:
    {
        op=HSRECORD_READER_OPERATE_DATA;
        address_length=3;
    }
    break;
    case HSRECORD_TYPE_DATA_32BIT:
    {
        op=HSRECORD_READER_OPERATE_DATA;
        address_length=4;
    }
    break;
    case HSRECORD_TYPE_DATA_COUNT_16BIT:
    {
        op=HSRECORD_READER_OPERATE_DATA_COUNT;
        address_length=2;
        if(payload_len >= 2 && payload_len <= 4)
        {
            address_length=payload_len;
        }
    }
    break;
    case HSRECORD_TYPE_DATA_COUNT_24BIT:
    {
        op=HSRECORD_READER_OPERATE_DATA_COUNT;
        address_length=3;
        if(payload_len==4)
        {
            address_length=payload_len;
        }
    }
    break;
    case HSRECORD_TYPE_START_ADDRESS_32BIT:
    {
        op=HSRECORD_READER_OPERATE_START_ADDRESS;
        address_length=4;
    }
    break;
    case HSRECORD_TYPE_START_ADDRESS_24BIT:
    {
        op=HSRECORD_READER_OPERATE_START_ADDRESS;
        address_length=3;
    }
    break;
    case HSRECORD_TYPE_START_ADDRESS_16BIT:
    {
        op=HSRECORD_READER_OPERATE_START_ADDRESS;
        address_length=2;
    }
    break;
    default:
    {
        hsrecord_reader_set_error(reader,HSRECORD_READER_ERROR_INVALID_RECORD_TYPE);
        return;
    }
    }

    if(payload_len < address_length)
    {
        hsrecord_reader_set_error(reader,HSRECORD_READER_ERROR_INVALID_LENGTH);
        return;
    }

    uint32_t address=hsrecord_common_decode_big_endian(&line_binary[1],address_length);
    const uint8_t *data=&line_binary[1+address_length];
    size_t data_len=payload_len-address_length;

    switch(op)
    {
    case HSRECORD_READER_OPERATE_HEADER:
    {
        //头部记录地址无效,固定为0
        address=0;
    }
    break;
    case HSRECORD_READER_OPERATE_DATA:
    {
        reader->data_count++;
    }
    break;
    case HSRECORD_READER_OPERATE_DATA_COUNT:
    {
        uint32_t mask=0xFFFF;
        while(address > mask)
        {
            mask=(mask << 8) | 0xFF;
        }
        mask&=reader->data_count;
        if(address != mask)
        {
            hsrecord_reader_set_error(reader,HSRECORD_READER_ERROR_DATA_COUNT_MISMATCH);
        }
    }
    break;
    case HSRECORD_READER_OPERATE_START_ADDRESS:
    {
        //起始地址记录中的数据无效
        data_len=0;
    }
    break;
    default:
    break;
    }

    if(reader->op!=NULL)
    {
        reader->op(reader,op,address,data,data_len);
    }
}

void hsrecord_reader_init(hsrecord_reader_t *reader,hsrecord_reader_operate_callback_t op,void *usr)
{
    if(reader!=NULL)
    {
        memset(reader,0,sizeof(hsrecord_reader_t));
        hsrecord_common_buffer_init(&reader->buffer);
        reader->op=op;
        reader->usr=usr;
        reader->error=HSRECORD_READER_ERROR_NONE;
    }
}

void hsrecord_reader_input(hsrecord_reader_t *reader,const char *data,size_t data_len)
{
    if(reader==NULL || data == NULL || data_len == 0)
    {
        return;
    }

    uint8_t *line=reader->buffer.line;

    for(size_t i=0; i<data_len; i++)
    {
        uint8_t c=(uint8_t)data[i];

        if(c=='\r' || c=='\n')
        {
            if(reader->buffer.line_length > 0)
            {
                line[reader->buffer.line_length]='\0';
                hsrecord_reader_process_line(reader);
                reader->buffer.line_length=0;
                line[0]='\0';
            }
            continue;
        }

        if(c=='S' || c=='s')
        {
            //开始一条新记录,丢弃未完成的旧记录
            line[0]='S';
            reader->buffer.line_length=1;
            continue;
        }

        if(reader->buffer.line_length==0)
        {
            //忽略记录之前的多余字符
            continue;
        }

        if(reader->buffer.line_length >= (sizeof(reader->buffer.line)-1))
        {
            hsrecord_reader_set_error(reader,HSRECORD_READER_ERROR_LINE_TOO_LONG);
            reader->buffer.line_length=0;
            continue;
        }

        line[reader->buffer.line_length++]=c;
    }

    line[reader->buffer.line_length]='\0';
}

void hsrecord_reader_finish(hsrecord_reader_t *reader)
{
    if(reader==NULL)
    {
        return;
    }
    if(reader->buffer.line_length > 0)
    {
        reader->buffer.line[reader->buffer.line_length]='\0';
        hsrecord_reader_process_line(reader);
        reader->buffer.line_length=0;
        reader->buffer.line[0]='\0';
    }
}
