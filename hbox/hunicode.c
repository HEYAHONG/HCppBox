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
static size_t char_zero_bit(char c)
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
static size_t utf8_char_length(const char *utf8str)
{
    size_t len=0;
    if(utf8str!=NULL)
    {
        uint8_t zero_bit=char_zero_bit(utf8str[0]);
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
            uint8_t utf8_char_len=utf8_char_length(&str[index]);
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
                    if(char_zero_bit(utf8_char[i])!=1)
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

