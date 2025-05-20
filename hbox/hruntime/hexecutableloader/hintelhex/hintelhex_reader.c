/***************************************************************
 * Name:      hintelhex_reader.c
 * Purpose:   实现hintelhex_reader接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-13
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hintelhex_reader.h"
#include "hcrypto.h"

void hintelhex_reader_init(hintelhex_reader_t *reader,hintelhex_reader_operate_callback_t op,void *usr)
{
    if(reader!=NULL)
    {
        memset(reader,0,sizeof(hintelhex_reader_t));
        hintelhex_common_buffer_init(&reader->buffer);
        reader->op=op;
        reader->usr=usr;
    }
}

void hintelhex_reader_input(hintelhex_reader_t *reader,const char *data,size_t data_len)
{
    if(reader==NULL || reader->op==NULL || data == NULL || data_len == 0)
    {
        return;
    }

    {
        /*
         *  防止strlen出错
         */
        reader->buffer.line[sizeof(reader->buffer.line)-1]='\0';
    }

    size_t line_index=strlen((const char *)reader->buffer.line);
    uint8_t *line=reader->buffer.line;
    for(size_t i=0; i<data_len; i++)
    {
        if(data[i]=='\r' || data[i]=='\n' || line_index >= sizeof(reader->buffer.line))
        {
            line[line_index]='\0';
            {
                //处理单行
                uint8_t *line_binary=reader->buffer.line_binary;
                size_t line_binary_len=hbase16_decode(line_binary,sizeof(reader->buffer.line_binary),(char *)&line[1],line_index-1);
                if(hlrc_check(line_binary,line_binary_len-1,line_binary[line_binary_len-1]))
                {
                    uint8_t len=line_binary[0];
                    uint16_t byte_address=line_binary[1]*256+line_binary[2];
                    uint8_t type=line_binary[3];
                    switch(type)
                    {
                    case HINTELHEX_RECORD_TYPE_DATA_RECORD:
                    {
                        uint32_t address_offset=reader->buffer.address+(reader->buffer.segment << 4);
                        reader->op(reader,HINTELHEX_READER_OPERATE_DATA,address_offset+byte_address,&line_binary[4],len);
                    }
                    break;
                    case HINTELHEX_RECORD_TYPE_END_OF_FILE_RECORD:
                    {
                        uint32_t address_offset=reader->buffer.address+(reader->buffer.segment << 4);
                        reader->op(reader,HINTELHEX_READER_OPERATE_END_OF_FILE,address_offset+byte_address,&line_binary[4],len);
                    }
                    break;
                    case HINTELHEX_RECORD_TYPE_EXTENDED_SEGMENT_ADDRESS_RECORD:
                    {
                        reader->buffer.segment=line_binary[4]*256+line_binary[5];
                        reader->op(reader,HINTELHEX_READER_OPERATE_EXTENDED_SEGMENT_ADDRESS,reader->buffer.segment,&line_binary[4],len);
                    }
                    break;
                    case HINTELHEX_RECORD_TYPE_START_SEGMENT_ADDRESS_RECORD:
                    {
                        uint32_t cs_ip=line_binary[4]*(1UL << 24)+line_binary[5]*(1UL << 16)+line_binary[6]*(1UL << 8)+line_binary[7]*(1UL << 0);
                        reader->op(reader,HINTELHEX_READER_OPERATE_START_SEGMENT_ADDRESS,cs_ip,&line_binary[4],len);
                    }
                    break;
                    case HINTELHEX_RECORD_TYPE_EXTENDED_LINEAR_ADDRESS_RECORD:
                    {
                        reader->buffer.address=line_binary[4]*(1UL << 24)+line_binary[5]*(1UL << 16);
                        reader->op(reader,HINTELHEX_READER_OPERATE_EXTENDED_LINEAR_ADDRESS,reader->buffer.address,&line_binary[4],len);
                    }
                    break;
                    case HINTELHEX_RECORD_TYPE_START_LINEAR_ADDRESS_RECORD:
                    {
                        uint32_t eip=line_binary[4]*(1UL << 24)+line_binary[5]*(1UL << 16)+line_binary[6]*(1UL << 8)+line_binary[7]*(1UL << 0);
                        reader->op(reader,HINTELHEX_READER_OPERATE_START_LINEAR_ADDRESS,eip,&line_binary[4],len);
                    }
                    break;
                    default:
                    {

                    }
                    break;
                    }
                }
            }
            line_index=0;
            line[0]='\0';
            continue;
        }
        if(line[0]!=':')
        {
            line[0]=data[i];
            line_index=0;
            if(line[0]==':')
            {
                line_index++;
            }
        }
        else
        {
            line[line_index++]=data[i];
        }
    }
}
