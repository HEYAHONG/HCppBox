/***************************************************************
 * Name:      hunicode.c
 * Purpose:   实现hunicode接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-08-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hunicode.h"

/** \brief 获取在字符第几位为0（从高到低）
 *
 * \param c char 字符
 * \return size_t 第几位为0
 *
 */
static size_t hunicode_char_zero_bit(char c)
{
    uint8_t u_c=(uint8_t)c;
    for(size_t i=0; i<8; i++)
    {
        if((u_c&(0x80>>i))==0)
        {
            return i;
        }
    }
    //不会走到此处
    return 0;
}
//获取一个字符在UTF-8在编码长度中的长度，返回0表示出错
static size_t hunicode_utf8_char_length(const char *utf8str)
{
    size_t len=0;
    if(utf8str!=NULL)
    {
        uint8_t zero_bit=hunicode_char_zero_bit(utf8str[0]);
        if(zero_bit==0)
        {
            //单个字符
            return 1;
        }
        if(zero_bit>1)
        {
            return zero_bit;
        }
    }
    return len;
}

bool hunicode_cchar_string_is_utf8(const char *str)
{
    bool ret=(str!=NULL);
    if(str!=NULL)
    {
        size_t index=0;
        while(str[index]!='\0')
        {
            uint8_t utf8_char_len=hunicode_utf8_char_length(&str[index]);
            if(utf8_char_len==0)
            {
                ret=false;
                break;
            }
            if(utf8_char_len==1)
            {
                index+=utf8_char_len;
                continue;
            }
            if(utf8_char_len>1)
            {
                const char *utf8_char=&str[index];
                for(size_t i=1; i<utf8_char_len; i++)
                {
                    if(hunicode_char_zero_bit(utf8_char[i])!=1)
                    {
                        ret=false;
                        break;
                    }
                }
                if(!ret)
                {
                    break;
                }
                index+=utf8_char_len;
            }
        }
    }
    return ret;
}

bool hunicode_cchar_string_is_ascii(const char *str)
{
    bool ret=(str!=NULL);
    if(str!=NULL)
    {
        size_t index=0;
        while(str[index]!='\0')
        {
            if(((uint8_t)str[index]) >= 0x80)
            {
                ret=false;
                break;
            }
            index++;
        }
    }
    return ret;
}


size_t hunicode_cchar_utf8_string_length(const char *utf8_str)
{
    size_t length=0;
    if(utf8_str!=NULL)
    {
        size_t index=0;
        while(utf8_str[index]!='\0')
        {
            const char *utf8_char=&utf8_str[index];
            size_t utf8_char_len=hunicode_utf8_char_length(utf8_char);
            if(utf8_char_len==0)
            {
                break;
            }
            if(utf8_char_len==1)
            {
                index++;
                length++;
                continue;
            }
            if(utf8_char_len>1)
            {
                for(size_t i=1; i<utf8_char_len; i++)
                {
                    if(utf8_char[i]=='\0')
                    {
                        index+=i;
                        break;
                    }
                    if(hunicode_char_zero_bit(utf8_char[i])==1)
                    {
                        if(i==(utf8_char_len-1))
                        {
                            index+=utf8_char_len;
                            length++;
                        }
                        continue;
                    }
                    else
                    {
                        index+=i;
                        break;
                    }
                }
            }
        }
    }
    return length;
}

size_t hunicode_cchar_string_length(const char *str)
{
    size_t length=0;
    if(str!=NULL)
    {
        while(str[length]!='\0')
        {
            length++;
        }
    }
    return length;
}

size_t hunicode_wchar_t_string_length(const wchar_t *str)
{
    size_t length=0;
    if(str!=NULL)
    {
        while(str[length]!='\0')
        {
            length++;
        }
    }
    return length;
}

size_t hunicode_char_string_length(const hunicode_char_t *str)
{
    size_t length=0;
    if(str!=NULL)
    {
        while(str[length]!=0)
        {
            length++;
        }
    }
    return length;
}

void hunicode_char_from_wchar(hunicode_char_t *dest,size_t dest_length,const wchar_t *src,size_t src_length)
{
    if(dest==NULL || dest_length==0||src==0||src_length==0)
    {
        return;
    }
    for(size_t i=0; i<src_length; i++)
    {
        if(i<dest_length)
        {
            dest[i]=(hunicode_char_t)src[i];
        }
        else
        {
            break;
        }
    }
}

void hunicode_char_from_wchar_string(hunicode_char_t *dest,size_t dest_length,const wchar_t *src)
{
    size_t length=hunicode_wchar_t_string_length(src);
    hunicode_char_from_wchar(dest,dest_length,src,length);
    if(length < dest_length)
    {
        if(dest!=NULL)
        {
            dest[length]=0;
        }
    }
}

void hunicode_char_from_utf8_string(hunicode_char_t *dest,size_t dest_length,const char *src)
{
    if(dest == NULL || dest_length == 0 || src == NULL)
    {
        return;
    }

    size_t length=0;
    if(src!=NULL)
    {
        size_t index=0;
        while(src[index]!='\0')
        {
            const char *utf8_char=&src[index];
            size_t utf8_char_len=hunicode_utf8_char_length(utf8_char);
            if(utf8_char_len==0)
            {
                break;
            }
            if(utf8_char_len==1)
            {
                index++;
                if(length<dest_length)
                {
                    dest[length]=(hunicode_char_t)(*utf8_char);
                }
                length++;
                continue;
            }
            if(utf8_char_len>1)
            {
                hunicode_char_t temp=(utf8_char[0]&((0x80>>utf8_char_len)-1));
                for(size_t i=1; i<utf8_char_len; i++)
                {
                    if(utf8_char[i]=='\0')
                    {
                        index+=i;
                        break;
                    }
                    if(hunicode_char_zero_bit(utf8_char[i])==1)
                    {
                        temp<<=6;
                        temp|=(utf8_char[i]&0x3F);
                        if(i==(utf8_char_len-1))
                        {
                            index+=utf8_char_len;
                            if(length<dest_length)
                            {
                                dest[length]=temp;
                            }
                            length++;
                        }
                        continue;
                    }
                    else
                    {
                        index+=i;
                        break;
                    }
                }
            }
        }
    }
}

void hunicode_char_to_wchar(wchar_t *dest,size_t dest_length,const hunicode_char_t *src,size_t src_length)
{
    if(dest==NULL || dest_length==0||src==0||src_length==0)
    {
        return;
    }
    for(size_t i=0; i<src_length; i++)
    {
        if(i<dest_length)
        {
            dest[i]=(wchar_t)src[i];
        }
        else
        {
            break;
        }
    }
}

void hunicode_char_string_to_wchar(wchar_t *dest,size_t dest_length,const hunicode_char_t *src)
{
    size_t length=hunicode_char_string_length(src);
    hunicode_char_to_wchar(dest,dest_length,src,length);
    if(length < dest_length)
    {
        if(dest!=NULL)
        {
            dest[length]=0;
        }
    }
}

void hunicode_char_to_utf8(char *dest,size_t dest_length,const hunicode_char_t *src,size_t src_length)
{
    if(dest == NULL || dest_length == 0 || src == NULL || src_length == 0)
    {
        return;
    }
    size_t dest_index=0;
    for(size_t i=0; i<src_length; i++)
    {
        if(src[i] < 0x80)
        {
            //单字符
            if(dest_index < dest_length)
            {
                dest[dest_index]=(char)src[i];
                dest_index++;
            }
            else
            {
                //存储空间不足
                break;
            }
            continue;
        }
        {
            //utf8字符所需位
            size_t utf8_char_bits=0;
            {
                for(size_t bit=0; bit<32; bit++)
                {
                    if((src[i]&(0x01ULL<<(32-bit-1)))!=0)
                    {
                        utf8_char_bits=32-bit;
                        break;
                    }
                }
            }
            if(utf8_char_bits >31)
            {
                //非法字符
                break;
            }
            //获取utf8字符所需空间
            size_t utf8_char_bytes=(utf8_char_bits/6)+1;
            if((utf8_char_bits%6)>(8-utf8_char_bytes-1))
            {
                utf8_char_bytes+=1;
            }
            if(dest_index+utf8_char_bytes > dest_length)
            {
                //空间不够
                break;
            }
            {
                //写入utf8字符
                uint8_t *utf8_char=(uint8_t *)&dest[dest_index];
                utf8_char[0]=(((0x1ULL<<utf8_char_bytes)-1)<<(8-utf8_char_bytes))+(src[i]>>(6*(utf8_char_bytes-1)));
                for(size_t j=1; j<utf8_char_bytes; j++)
                {
                    utf8_char[j]=0x80+(0x3F&(src[i]>>(6*(utf8_char_bytes-1-j))));
                }
            }
            dest_index+=utf8_char_bytes;
        }
    }
}


void hunicode_char_string_to_utf8(char *dest,size_t dest_length,const hunicode_char_t *src)
{
    size_t length=hunicode_char_string_length(src);
    //多拷贝一个\0字符
    hunicode_char_to_utf8(dest,dest_length,src,length+1);
}

