/***************************************************************
 * Name:      hbase16.c
 * Purpose:   声明base16实现。
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-04-23
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hbase16.h"

static uint8_t hbase16_decode_single_char(char  code)
{
    if(code >= '0' && code <= '9')
    {
        return code-'0'+0;
    }
    if(code >= 'A' && code <= 'F')
    {
        return code-'A'+0xA;
    }
    if(code >= 'a' && code <= 'f')
    {
        return code-'a'+0xa;
    }
    return 0;
}
static uint8_t hbase16_decode_single_data(const char * code,size_t code_len)
{
    uint8_t ret=0;
    if(code!=NULL &&code_len>0)
    {
        for(size_t i=0; i<code_len; i++)
        {
            ret<<=4;
            ret+=hbase16_decode_single_char(code[i]);
        }
    }
    return ret;
}

size_t hbase16_decode(uint8_t *value,size_t value_length,const char *encoding,size_t encoding_length)
{
    if(encoding==NULL || encoding_length==0)
    {
        return 0;
    }
    size_t len=0;
    size_t single_data_start=0;
    size_t single_data_length=0;
    for(size_t i=0; i<encoding_length; i++)
    {
        if(encoding[i]=='0' && (encoding[i+1]=='x' || encoding[i+1]=='X'))
        {
            if(single_data_length>0)
            {
                if(value !=NULL && value_length > len)
                {
                    value[len]=hbase16_decode_single_data(&encoding[single_data_start],single_data_length);
                }
                len++;
            }
            //跳过0x,0X
            i+=2;
            single_data_start=i;
            single_data_length=0;
            continue;
        }
        if(encoding[i]==' ')
        {
            //跳过空格
            if(single_data_length>0)
            {
                if(value !=NULL && value_length > len)
                {
                    value[len]=hbase16_decode_single_data(&encoding[single_data_start],single_data_length);
                }
                len++;
            }
            i++;
            single_data_start=i;
            single_data_length=0;
            continue;
        }
        if(single_data_length >=1)
        {
            /*
             * 已经满足两个有效字符
             */
            single_data_length++;
            {
                if(value !=NULL && value_length > len)
                {
                    value[len]=hbase16_decode_single_data(&encoding[single_data_start],single_data_length);
                }
                len++;
            }
            i++;
            single_data_start=i;
            single_data_length=0;
        }
        single_data_length++;
    }
    return len;
}

static char hbase16_encode_single_char(uint8_t code)
{
    if(code >= 0x10)
    {
        //异常情况不处理，返回空格
        return ' ';
    }
    if(code < 0xA)
    {
        return code-0x0+'0';
    }
    else
    {
        return code-0xA+'A';
    }
}

size_t hbase16_encode(char *encoding,size_t encoding_length,const uint8_t*value,size_t value_length)
{
    if(value==NULL || value_length==0)
    {
        return 0;
    }
    size_t bytes_encode=0;
    for(size_t i=0; i<value_length; i++)
    {
        if(encoding!=NULL)
        {
            if((bytes_encode+1)*2 >= encoding_length)
            {
                break;
            }
            encoding[i*2+0]=hbase16_encode_single_char((value[i]>>4)&0xF);
            encoding[i*2+1]=hbase16_encode_single_char((value[i]>>0)&0xF);
        }
        bytes_encode++;
    }
    return bytes_encode*2;//每个字符编码为2个字符
}

size_t hbase16_encode_with_null_terminator(char *encoding,size_t encoding_length,const uint8_t*value,size_t value_length)
{
    size_t len=hbase16_encode(encoding,encoding_length,value,value_length);
    len+=1;//添加'\0'字符长度
    if(len > encoding_length)
    {
        len=encoding_length;
    }
    if(encoding!=NULL)
    {
        encoding[len-1]='\0';
    }
    return len;
}
