/***************************************************************
 * Name:      hbase64.c
 * Purpose:   声明base64实现，参考RFC2045～RFC2049。
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-09-02
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hbase64.h"
static uint8_t hbase64_get_value_by_encoding(char encoding)
{
    if(encoding >= 'A' && encoding <= 'Z')
    {
        return 0+encoding-'A';
    }
    if(encoding >= 'a' && encoding <= 'z')
    {
        return 26+encoding-'a';
    }
    if(encoding >= '0' && encoding <= '9')
    {
        return 52+encoding-'0';
    }
    if(encoding == '+')
    {
        return 62;
    }
    if(encoding == '/')
    {
        return 63;
    }
    return 0;
}

static char hbase64_get_encoding_by_value(uint8_t value)
{
    if(value < 26)
    {
        return 'A'+value;
    }
    if(value < 52)
    {
        return 'a'+value-26;
    }
    if(value < 62)
    {
        return '0'+value-52;
    }
    if(value == 62)
    {
        return '+';
    }
    if(value == 63)
    {
        return '/';
    }
    return '=';
}

/** \brief 单次解码
 *
 * \param value uint8_t* 待解码数据的缓冲区
 * \param value_length size_t 待解码数据的缓冲区长度
 * \param encoding const char* 编码数据缓冲区
 * \param encoding_length size_t 解码数据的缓冲区长度
 * \return size_t 已解码长度
 */
static size_t hbase64_single_decode(uint8_t *value,size_t value_length,const char *encoding,size_t encoding_length)
{
    size_t len=3;
    if(encoding[3]=='=')
    {
        len=2;
        if(encoding[2]=='=')
        {
            len=1;
        }
    }
    value[0]=(hbase64_get_value_by_encoding(encoding[0])<<2)|(hbase64_get_value_by_encoding(encoding[1])>>4);
    if(len>1)
    {
        value[1]=(hbase64_get_value_by_encoding(encoding[1])<<4)|(hbase64_get_value_by_encoding(encoding[2])>>2);
    }
    if(len>2)
    {
        value[2]=(hbase64_get_value_by_encoding(encoding[2])<<6)|(hbase64_get_value_by_encoding(encoding[3]));
    }
    return len;
}

/** \brief 单次编码
 *
 * \param encoding char* 编码数据缓冲区
 * \param encoding_length size_t 编码数据缓冲区长度
 * \param uint8_t*value const 待编码数据缓冲区
 * \param value_length size_t 待编码数据缓冲区长长度
 * \return size_t 已编码长度
 *
 */
static size_t hbase64_single_encode(char *encoding,size_t encoding_length,const uint8_t*value,size_t value_length)
{
    size_t len=4;
    if(value_length > 0)
    {
        encoding[0]=hbase64_get_encoding_by_value(value[0]>>2);
        if(value_length > 1)
        {
            encoding[1]=hbase64_get_encoding_by_value(((value[0]&0x3)<<4)|(value[1]>>4));
            if(value_length>2)
            {
                encoding[2]=hbase64_get_encoding_by_value(((value[1]&0xF)<<2)|(value[2]>>6));
                encoding[3]=hbase64_get_encoding_by_value((value[2]&0x3F));
            }
            else
            {
                encoding[2]=hbase64_get_encoding_by_value((value[1]&0xF)<<2);
                encoding[3]='=';
            }
        }
        else
        {
            encoding[1]=hbase64_get_encoding_by_value((value[0]&0x3)<<4);
            encoding[2]='=';
            encoding[3]='=';
        }
    }
    return len;
}



size_t hbase64_decode(uint8_t *value,size_t value_length,const char *encoding,size_t encoding_length)
{
    if(encoding==NULL || encoding_length==0)
    {
        return 0;
    }
    size_t max_cnt=encoding_length/4;
    size_t cur_cnt=0;
    size_t len=0;
    for(size_t i=0; i<max_cnt; i++)
    {
        if(value!=NULL && ((cur_cnt+1)*3 <= value_length))
        {
            hbase64_single_decode(&value[cur_cnt*3],value_length-(cur_cnt*3),&encoding[cur_cnt*4],encoding_length-cur_cnt*4);
        }
        len+=3;
        if(encoding[cur_cnt*4+3]=='=')
        {
            len-=1;
            if(encoding[cur_cnt*4+2]=='=')
            {
                len-=1;
            }
        }
        cur_cnt++;
    }
    return len;
}


size_t hbase64_encode(char *encoding,size_t encoding_length,const uint8_t*value,size_t value_length)
{
    if(value==NULL || value_length==0)
    {
        return 0;
    }
    size_t max_cnt=value_length/3;
    if(value_length%3!=0)
    {
        max_cnt++;
    }
    size_t cur_cnt=0;
    for(size_t i=0; i<max_cnt; i++)
    {
        if(encoding!=NULL && ((cur_cnt+1)*4 <= encoding_length))
        {
            hbase64_single_encode(&encoding[cur_cnt*4],encoding_length-(cur_cnt*4),&value[cur_cnt*3],value_length-cur_cnt*3);
        }
        cur_cnt++;
    }
    return cur_cnt*4;
}

size_t hbase64_encode_with_null_terminator(char *encoding,size_t encoding_length,const uint8_t*value,size_t value_length)
{
    size_t len=hbase64_encode(encoding,encoding_length,value,value_length);
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

size_t hbase64_get_encoding_length(const char *encoding)
{
    size_t ret=0;
    if(encoding!=NULL)
    {
        while(((*encoding)!='\0') && ((*encoding)!='\n') && ((*encoding)!='\r'))
        {
            encoding++;
            ret++;
        }
    }
    return ret;
}
