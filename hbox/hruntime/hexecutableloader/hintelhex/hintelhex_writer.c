/***************************************************************
 * Name:      hintelhex_writer.c
 * Purpose:   实现hintelhex_writer接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-13
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hintelhex_writer.h"
#include "hcrypto.h"

void hintelhex_writer_init(hintelhex_writer_t *writer,hintelhex_writer_writeline_t writeline,void *usr)
{
    if(writer==NULL || writeline==NULL)
    {
        return;
    }
    memset(writer,0,sizeof(hintelhex_writer_t));
    hintelhex_common_buffer_init(&writer->buffer);
    writer->writeline=writeline;
    writer->usr=usr;
}

void hintelhex_writer_write_data(hintelhex_writer_t *writer,uint32_t address,const uint8_t *data,size_t data_len)
{
    if(writer==NULL || writer->writeline == NULL)
    {
        return;
    }

    if(data==NULL || data_len == 0)
    {
        return;
    }

    while(data_len > 0)
    {
        size_t bytes_to_write=data_len;
        if(bytes_to_write > HINTELHEX_WRITER_PAGE_SIZE)
        {
            bytes_to_write=HINTELHEX_WRITER_PAGE_SIZE;
        }

        size_t address_offset=writer->buffer.address+(writer->buffer.segment << 4);

        if(address < address_offset || address >= (address_offset+0x10000) )
        {
            //地址需要修正
            if(writer->buffer.segment!=0)
            {
                uint16_t segment=((address&0xFFFF0) >> 4);
                if(segment==writer->buffer.segment)
                {
                    //使用线性地址
                    hintelhex_writer_write_extended_linear_address(writer,address >> 16);
                    hintelhex_writer_write_extended_segment_address(writer,0);
                }
                else
                {
                    //使用段地址
                    hintelhex_writer_write_extended_segment_address(writer,segment);
                }
            }
            else
            {
                //使用线性地址
                hintelhex_writer_write_extended_linear_address(writer,address >> 16);
            }
            continue;
        }

        if(address+bytes_to_write > (address_offset+0x10000))
        {
            bytes_to_write=(address_offset+0x10000)-address;
        }

        if(bytes_to_write > 0xFF)
        {
            bytes_to_write=0xFF;
        }

        {
            memset(writer->buffer.line_binary,0,sizeof(writer->buffer.line_binary));

            uint8_t *line_binary=writer->buffer.line_binary;

            //数据长度为0
            line_binary[0]=bytes_to_write;

            //字节地址
            size_t byte_address=address-address_offset;
            line_binary[1]=((byte_address>>8)&0xFF);
            line_binary[2]=((byte_address>>0)&0xFF);

            //类型为DATA
            line_binary[3]=HINTELHEX_RECORD_TYPE_DATA_RECORD;

            //填写数据
            memcpy(&line_binary[4],data,bytes_to_write);

            size_t line_binary_len=line_binary[0]+5;

            //添加校验
            line_binary[line_binary_len-1]=hlrc_calculate(line_binary,line_binary_len-1);

            char *line=(char *)writer->buffer.line;

            line[0]=':';

            size_t line_len=1+hbase16_encode(&line[1],sizeof(writer->buffer.line)-1,line_binary,line_binary_len);

            line[line_len]='\0';

            //写入行
            writer->writeline(writer,line,line_len);
        }


        address+=bytes_to_write;
        data+=bytes_to_write;
        data_len-=bytes_to_write;
    }
}

void hintelhex_writer_write_end_of_file(hintelhex_writer_t *writer)
{
    if(writer==NULL || writer->writeline == NULL)
    {
        return;
    }

    memset(writer->buffer.line_binary,0,sizeof(writer->buffer.line_binary));

    uint8_t *line_binary=writer->buffer.line_binary;

    //数据长度为0
    line_binary[0]=0;

    //字节地址为0
    line_binary[1]=0;
    line_binary[2]=0;

    //类型为end_of_file
    line_binary[3]=HINTELHEX_RECORD_TYPE_END_OF_FILE_RECORD;

    size_t line_binary_len=line_binary[0]+5;

    //添加校验
    line_binary[line_binary_len-1]=hlrc_calculate(line_binary,line_binary_len-1);

    char *line=(char *)writer->buffer.line;

    line[0]=':';

    size_t line_len=1+hbase16_encode(&line[1],sizeof(writer->buffer.line)-1,line_binary,line_binary_len);

    line[line_len]='\0';

    //写入行
    writer->writeline(writer,line,line_len);
}

void hintelhex_writer_write_extended_segment_address(hintelhex_writer_t *writer,uint16_t segment_address)
{
    if(writer==NULL || writer->writeline == NULL)
    {
        return;
    }

    if(writer==NULL || writer->writeline == NULL)
    {
        return;
    }

    memset(writer->buffer.line_binary,0,sizeof(writer->buffer.line_binary));

    uint8_t *line_binary=writer->buffer.line_binary;

    //数据长度为0
    line_binary[0]=sizeof(segment_address);

    //字节地址为0
    line_binary[1]=0;
    line_binary[2]=0;

    //类型为 extended_segment_address
    line_binary[3]=HINTELHEX_RECORD_TYPE_EXTENDED_SEGMENT_ADDRESS_RECORD;

    //写入数据
    line_binary[4]=((segment_address>>8)&0xFF);
    line_binary[5]=((segment_address>>0)&0xFF);

    size_t line_binary_len=line_binary[0]+5;

    //添加校验
    line_binary[line_binary_len-1]=hlrc_calculate(line_binary,line_binary_len-1);

    char *line=(char *)writer->buffer.line;

    line[0]=':';

    size_t line_len=1+hbase16_encode(&line[1],sizeof(writer->buffer.line)-1,line_binary,line_binary_len);

    line[line_len]='\0';

    //写入行
    writer->writeline(writer,line,line_len);

    //更改buffer段地址
    writer->buffer.segment=segment_address;
}

void hintelhex_writer_write_start_segment_address(hintelhex_writer_t *writer,uint32_t cs_ip)
{
    if(writer==NULL || writer->writeline == NULL)
    {
        return;
    }

    memset(writer->buffer.line_binary,0,sizeof(writer->buffer.line_binary));

    uint8_t *line_binary=writer->buffer.line_binary;

    //数据长度为0
    line_binary[0]=sizeof(cs_ip);

    //字节地址为0
    line_binary[1]=0;
    line_binary[2]=0;

    //类型为start_sgement_address
    line_binary[3]=HINTELHEX_RECORD_TYPE_START_SEGMENT_ADDRESS_RECORD;

    //写入数据
    line_binary[4]=((cs_ip>>24)&0xFF);
    line_binary[5]=((cs_ip>>16)&0xFF);
    line_binary[6]=((cs_ip>>8)&0xFF);
    line_binary[7]=((cs_ip>>0)&0xFF);

    size_t line_binary_len=line_binary[0]+5;

    //添加校验
    line_binary[line_binary_len-1]=hlrc_calculate(line_binary,line_binary_len-1);

    char *line=(char *)writer->buffer.line;

    line[0]=':';

    size_t line_len=1+hbase16_encode(&line[1],sizeof(writer->buffer.line)-1,line_binary,line_binary_len);

    line[line_len]='\0';

    //写入行
    writer->writeline(writer,line,line_len);

}

void hintelhex_writer_write_extended_linear_address(hintelhex_writer_t *writer,uint16_t address_high)
{
    if(writer==NULL || writer->writeline == NULL)
    {
        return;
    }

    memset(writer->buffer.line_binary,0,sizeof(writer->buffer.line_binary));

    uint8_t *line_binary=writer->buffer.line_binary;

    //数据长度为0
    line_binary[0]=sizeof(address_high);

    //字节地址为0
    line_binary[1]=0;
    line_binary[2]=0;

    //类型为 extended_segment_address
    line_binary[3]=HINTELHEX_RECORD_TYPE_EXTENDED_LINEAR_ADDRESS_RECORD;

    //写入数据
    line_binary[4]=((address_high>>8)&0xFF);
    line_binary[5]=((address_high>>0)&0xFF);

    size_t line_binary_len=line_binary[0]+5;

    //添加校验
    line_binary[line_binary_len-1]=hlrc_calculate(line_binary,line_binary_len-1);

    char *line=(char *)writer->buffer.line;

    line[0]=':';

    size_t line_len=1+hbase16_encode(&line[1],sizeof(writer->buffer.line)-1,line_binary,line_binary_len);

    line[line_len]='\0';

    //写入行
    writer->writeline(writer,line,line_len);

    //更改buffer线性地址
    writer->buffer.address=(((uint32_t)address_high) << 16);
}

void hintelhex_writer_write_start_linear_address(hintelhex_writer_t *writer,uint32_t eip)
{
    if(writer==NULL || writer->writeline == NULL)
    {
        return;
    }

    memset(writer->buffer.line_binary,0,sizeof(writer->buffer.line_binary));

    uint8_t *line_binary=writer->buffer.line_binary;

    //数据长度为0
    line_binary[0]=sizeof(eip);

    //字节地址为0
    line_binary[1]=0;
    line_binary[2]=0;

    //类型为start_sgement_address
    line_binary[3]=HINTELHEX_RECORD_TYPE_START_LINEAR_ADDRESS_RECORD;

    //写入数据
    line_binary[4]=((eip>>24)&0xFF);
    line_binary[5]=((eip>>16)&0xFF);
    line_binary[6]=((eip>>8)&0xFF);
    line_binary[7]=((eip>>0)&0xFF);

    size_t line_binary_len=line_binary[0]+5;

    //添加校验
    line_binary[line_binary_len-1]=hlrc_calculate(line_binary,line_binary_len-1);

    char *line=(char *)writer->buffer.line;

    line[0]=':';

    size_t line_len=1+hbase16_encode(&line[1],sizeof(writer->buffer.line)-1,line_binary,line_binary_len);

    line[line_len]='\0';

    //写入行
    writer->writeline(writer,line,line_len);
}
