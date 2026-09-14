/***************************************************************
 * Name:      hsimplegui_font.c
 * Purpose:   实现hsimplegui_font接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-09-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include "hsimplegui_font.h"

static const uint8_t ASCII_font_8_bytes[]=
{
#include "font/ASCII_font_8.txt"
};

static const uint8_t ASCII_font_12_bytes[]=
{
#include "font/ASCII_font_12.txt"
};

static const uint8_t ASCII_font_16_bytes[]=
{
#include "font/ASCII_font_16.txt"
};



static const struct
{
    uint32_t uiCode;
    uint32_t uiAddr;
    uint8_t  uiSize;
    uint8_t  uiWidth;
} ASCII_font_8_fonts[] =
{
#include "font/ASCII_font_8.c"
};

static const struct
{
    uint32_t uiCode;
    uint32_t uiAddr;
    uint8_t  uiSize;
    uint8_t  uiWidth;
} ASCII_font_12_fonts[] =
{
#include "font/ASCII_font_12.c"
};

static const struct
{
    uint32_t uiCode;
    uint32_t uiAddr;
    uint8_t  uiSize;
    uint8_t  uiWidth;
} ASCII_font_16_fonts[] =
{
#include "font/ASCII_font_16.c"
};

static SGUI_BOOL       hsimplegui_font_utf8_is_full_width(SGUI_UINT32 uiCode)
{
    SGUI_BOOL  bReturn=SGUI_FALSE;
    if(uiCode < 0x7F)
    {
        bReturn = SGUI_FALSE;
    }
    else
    {
        bReturn = SGUI_TRUE;
    }
    return bReturn;
}

static SGUI_CSZSTR     hsimplegui_font_utf8_step_next(SGUI_CSZSTR cszSrc, SGUI_UINT32* puiCode)
{
    if(puiCode == NULL || cszSrc == NULL)
    {
        return cszSrc;
    }
    size_t char_size=1;
    uint8_t first_byte=(uint8_t)cszSrc[0];
    if(first_byte < 0x7F)
    {
        (*puiCode)=first_byte;
    }
    else if(first_byte < 0xC0)
    {
        /*
         * 中间字符
         */
        for(size_t i=0; ((uint8_t)cszSrc[i]) >= 0x80 && ((uint8_t)cszSrc[i]) < 0xC0 ; i++)
        {
            char_size=i+1;
        }
        (*puiCode)=0;
    }
    else if(first_byte < 0xE0)
    {
        /*
         * 双字节
         */
        uint8_t second_byte=(uint8_t)cszSrc[1];
        char_size=2;
        (*puiCode)=(((uint32_t)first_byte&0x1F) << 6) + (second_byte & 0x3F);

    }
    else if(first_byte < 0xF0)
    {
        /*
         * 三字节
         */
        uint8_t second_byte=(uint8_t)cszSrc[1];
        uint8_t third_byte=(uint8_t)cszSrc[2];
        char_size=3;
        (*puiCode)=(((uint32_t)first_byte&0x0F) << 12) + (((uint32_t)second_byte & 0x3F) << 6) +  (third_byte & 0x3F);
    }
    else
    {
        /*
         * 四字节
         */
        uint8_t second_byte=(uint8_t)cszSrc[1];
        uint8_t third_byte=(uint8_t)cszSrc[2];
        uint8_t fourth_byte=(uint8_t)cszSrc[3];
        char_size=4;
        (*puiCode)=(((uint32_t)first_byte&0x07) << 18) + (((uint32_t)second_byte & 0x3F) << 12) +  (((uint32_t)third_byte & 0x3F) << 6) + (fourth_byte & 0x3F);
    }
    return cszSrc+char_size;
}

/*
 * 使用此变量传参,不使用原函数的sStartAddr
 */
static uintptr_t  ASCII_font_8_start_addr=0;


static SGUI_INT        hsimplegui_font_ascii_8_get_char_index(SGUI_UINT32 uiCode)
{
    bool is_found=false;
    for(size_t i=0; i<sizeof(ASCII_font_8_fonts)/sizeof(ASCII_font_8_fonts[0]); i++)
    {
        if(ASCII_font_8_fonts[i].uiCode==uiCode)
        {
            ASCII_font_8_start_addr=ASCII_font_8_fonts[i].uiAddr;
            is_found=true;
            break;
        }
    }
    if(!is_found)
    {
        return SGUI_INVALID_INDEX;
    }
    return 0;
}


static SGUI_SIZE       hsimplegui_font_ascii_8_get_font_data(SGUI_SIZE sStartAddr, SGUI_BYTE* pDataBuffer, SGUI_SIZE sReadSize)
{
    /*
     * 使用全局变量传参
     */
    sStartAddr=ASCII_font_8_start_addr;
    if(sStartAddr+sReadSize > sizeof(ASCII_font_8_bytes))
    {
        sReadSize=sizeof(ASCII_font_8_bytes)-sStartAddr;
    }

    memcpy(pDataBuffer,ASCII_font_8_bytes+sStartAddr,sReadSize);

    return sReadSize;
}



const SGUI_FONT_RES hsimplegui_font_ascii_8=
{
    4,
    8,
    8,
    hsimplegui_font_ascii_8_get_char_index,
    hsimplegui_font_ascii_8_get_font_data,
    hsimplegui_font_utf8_step_next,
    hsimplegui_font_utf8_is_full_width
};


/*
 * 使用此变量传参,不使用原函数的sStartAddr
 */
static uintptr_t  ASCII_font_12_start_addr=0;


static SGUI_INT        hsimplegui_font_ascii_12_get_char_index(SGUI_UINT32 uiCode)
{
    bool is_found=false;
    for(size_t i=0; i<sizeof(ASCII_font_12_fonts)/sizeof(ASCII_font_12_fonts[0]); i++)
    {
        if(ASCII_font_12_fonts[i].uiCode==uiCode)
        {
            ASCII_font_12_start_addr=ASCII_font_12_fonts[i].uiAddr;
            is_found=true;
            break;
        }
    }
    if(!is_found)
    {
        return SGUI_INVALID_INDEX;
    }
    return 0;
}


static SGUI_SIZE       hsimplegui_font_ascii_12_get_font_data(SGUI_SIZE sStartAddr, SGUI_BYTE* pDataBuffer, SGUI_SIZE sReadSize)
{
    /*
     * 使用全局变量传参
     */
    sStartAddr=ASCII_font_12_start_addr;
    if(sStartAddr+sReadSize > sizeof(ASCII_font_12_bytes))
    {
        sReadSize=sizeof(ASCII_font_12_bytes)-sStartAddr;
    }

    memcpy(pDataBuffer,ASCII_font_12_bytes+sStartAddr,sReadSize);

    return sReadSize;
}



const SGUI_FONT_RES hsimplegui_font_ascii_12=
{
    6,
    12,
    12,
    hsimplegui_font_ascii_12_get_char_index,
    hsimplegui_font_ascii_12_get_font_data,
    hsimplegui_font_utf8_step_next,
    hsimplegui_font_utf8_is_full_width
};


/*
 * 使用此变量传参,不使用原函数的sStartAddr
 */
static uintptr_t  ASCII_font_16_start_addr=0;


static SGUI_INT        hsimplegui_font_ascii_16_get_char_index(SGUI_UINT32 uiCode)
{
    bool is_found=false;
    for(size_t i=0; i<sizeof(ASCII_font_16_fonts)/sizeof(ASCII_font_16_fonts[0]); i++)
    {
        if(ASCII_font_16_fonts[i].uiCode==uiCode)
        {
            ASCII_font_16_start_addr=ASCII_font_16_fonts[i].uiAddr;
            is_found=true;
            break;
        }
    }
    if(!is_found)
    {
        return SGUI_INVALID_INDEX;
    }
    return 0;
}


static SGUI_SIZE       hsimplegui_font_ascii_16_get_font_data(SGUI_SIZE sStartAddr, SGUI_BYTE* pDataBuffer, SGUI_SIZE sReadSize)
{
    /*
     * 使用全局变量传参
     */
    sStartAddr=ASCII_font_16_start_addr;
    if(sStartAddr+sReadSize > sizeof(ASCII_font_16_bytes))
    {
        sReadSize=sizeof(ASCII_font_16_bytes)-sStartAddr;
    }

    memcpy(pDataBuffer,ASCII_font_16_bytes+sStartAddr,sReadSize);

    return sReadSize;
}



const SGUI_FONT_RES hsimplegui_font_ascii_16=
{
    8,
    16,
    16,
    hsimplegui_font_ascii_16_get_char_index,
    hsimplegui_font_ascii_16_get_font_data,
    hsimplegui_font_utf8_step_next,
    hsimplegui_font_utf8_is_full_width
};




