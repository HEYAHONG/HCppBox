/***************************************************************
 * Name:      hgui_gui_xpm.c
 * Purpose:   实现xpm接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-11-30
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hgui_gui_xpm.h"
/**
 *  本组件支持的XPM格式,注意:标准的XPM格式变量定义为static char*  <variable_name>[],会占用过多的内存空间不利于嵌入式使用。
 *const char* const <variable_name>[] = {
 *<Values>
 *<Colors>
 *<Pixels>
 *<Extensions>
 *};
 */

/*
 * 获取一行里以空格分割的相应字段
 */
static const char *parse_line(const char *line,size_t i,size_t *len)
{
    if(line==NULL)
    {
        return line;
    }
    while(i-- > 0)
    {
        size_t len=0;
        line=parse_line(line,0,&len);
        line+=len;
    }
    if(line==NULL || line[0] == '\0')
    {
        return line;
    }
    while((*line)!='\0')
    {
        if((*line)==' ')
        {
            line++;
        }
        else
        {
            break;
        }
    }

    if(len!=NULL)
    {
        size_t i=0;
        while(((*(line+i))!='\0') && ((*(line+i))!=' '))
        {
            i++;
        }
        (*len)=i;
    }

    return line;
}

static size_t dec_string_to_number(const char *number_string,size_t len)
{
    size_t number=0;
    if(number_string!=NULL)
    {

        size_t start=0;
        while((number_string[start]!='\0') && (start<len))
        {
            if((number_string[start]<'0') || (number_string[start] >'9'))
            {
                start++;
            }
            else
            {
                break;
            }
        }
        size_t end=start;
        while((number_string[end]!='\0') && (end<=len))
        {
            if((number_string[end]<'0') || (number_string[end] >'9'))
            {
                break;
            }
            else
            {
                end++;
            }
        }
        for(size_t i=start; i<end; i++)
        {
            number*=10;
            number+=(number_string[i]-'0');
        }
    }
    return number;
}

/**
 * 格式：<width> <height> <ncolors> <cpp> [<x_hotspot> <y_hotspot>] [XPMEXT]
 */
hgui_gui_xpm_header_t hgui_gui_xpm_header_get(const char *const *xpm)
{
    hgui_gui_xpm_header_t header= {0};
    if(xpm!=NULL && xpm[0]!=NULL)
    {
        const char *line=xpm[0];
        {
            size_t len=0;
            const char *string=parse_line(line,0,&len);
            header.width=dec_string_to_number(string,len);
        }
        {
            size_t len=0;
            const char *string=parse_line(line,1,&len);
            header.height=dec_string_to_number(string,len);
        }
        {
            size_t len=0;
            const char *string=parse_line(line,2,&len);
            header.ncolors=dec_string_to_number(string,len);
        }
        {
            size_t len=0;
            const char *string=parse_line(line,3,&len);
            header.cpp=dec_string_to_number(string,len);
        }
        {
            size_t len=0;
            const char *string=parse_line(line,4,&len);
            if(string!=NULL  && string[0]=='\0')
            {
                if(string[0]=='X' || string[0]=='x' )
                {
                    header.XPMEXT=true;
                }
                else
                {
                    header.x_hotspot=dec_string_to_number(string,len);
                    string=parse_line(line,5,&len);
                    header.y_hotspot=dec_string_to_number(string,len);
                    string=parse_line(line,6,&len);
                    if(string!=NULL  && string[0]=='\0')
                    {
                        if(string[0]=='X' || string[0]=='x' )
                        {
                            header.XPMEXT=true;
                        }
                    }
                }
            }
        }
    }
    return header;
}

const char *hgui_gui_xpm_pixel_color_get(const char *const *xpm,size_t x,size_t y)
{
    hgui_gui_xpm_header_t header=hgui_gui_xpm_header_get(xpm);
    if(x<header.width && y<header.height && header.cpp!=0)
    {
        //像素点
        const char *pixel=&xpm[1+header.ncolors+y][header.cpp*x];
        for(const char *const*color=&xpm[1]; color!=&xpm[1+header.ncolors]; color++)
        {
            const char *color_string=(*color);
            bool is_match=true;
            for(size_t i=0; i<header.cpp; i++)
            {
                if(color_string[i]!=pixel[i])
                {
                    is_match=false;
                    break;
                }
            }
            if(is_match)
            {
                return &color_string[header.cpp];
            }
        }
    }
    return NULL;
}

static bool is_hex(char c)
{
    return ((c>='0') && (c<='9')) || ((c>='a') && (c<='f')) || ((c>='A') && (c<='F'));
}

static uint8_t hex_to_number(char c)
{
    if((c>='0') && (c<='9'))
    {
        return c-'0';
    }
    if((c>='a') && (c<='f'))
    {
        return c-'a'+10;
    }
    if((c>='A') && (c<='F'))
    {
        return c-'A'+10;
    }
    return 0;
}

static size_t hex_string_to_number(const char *number_string,size_t len)
{
    size_t number=0;
    if(number_string!=NULL)
    {

        size_t start=0;
        while((number_string[start]!='\0') && (start<len))
        {
            if(!is_hex(number_string[start]))
            {
                start++;
            }
            else
            {
                break;
            }
        }
        size_t end=start;
        while((number_string[end]!='\0') && (end<=len))
        {
            if(!is_hex(number_string[end]))
            {
                break;
            }
            else
            {
                end++;
            }
        }
        for(size_t i=start; i<end; i++)
        {
            number*=16;
            number+= hex_to_number(number_string[i]);
        }
    }
    return number;
}

static uint32_t decode_color_string(const char *color_string)
{
    uint32_t ret=0;
    {
        size_t i=0;
        while(true)
        {
            size_t len=0;
            const char *string=parse_line(color_string,i,&len);
            if(string==NULL || string[0]=='\0')
            {
                break;
            }
            if(string[0]=='#')
            {
                ret=hex_string_to_number(string,len);
                break;
            }
            i++;
        }
    }
    return ret;
}


bool hgui_gui_xpm_draw_color(const char *const *xpm,size_t x,size_t y,hgui_gui_xpm_draw_color_pixel_t draw_pixel,void *usr)
{
    if(draw_pixel==NULL)
    {
        return false;
    }
    hgui_gui_xpm_header_t header=hgui_gui_xpm_header_get(xpm);
    for(size_t i=0; i<header.width; i++)
    {
        for(size_t j=0; j<header.height; j++)
        {
            const char *color=hgui_gui_xpm_pixel_color_get(xpm,i,j);
            if(color==NULL || color[0]=='\0')
            {
                return false;
            }
            if(!draw_pixel(x+i,y+j,decode_color_string(color),usr))
            {
                return false;
            }
        }
    }
    return true;
}

//包含一些测试图像
#include "hgui_gui_xpm_xpm.xpm"
