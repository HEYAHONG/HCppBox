/***************************************************************
 * Name:      hcompiler.c
 * Purpose:   实现编译器特性相关函数
 * Author:    HYH (hyhsystem.cn)
 * Created:   2023-07-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include "hcompiler.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "string.h"

long hcompiler_get_stdc_version(void)
{
#ifdef __STDC_VERSION__
    return __STDC_VERSION__;
#else
    return 199409L;
#endif // __STDC_VERSION__
}

static bool hcompiler_is_number(const char c)
{
    return c >= '0' && c <= '9';
}

static size_t hcompiler_get_number(const char *str,size_t index)
{
    if(str==NULL)
    {
        return 0;
    }
    size_t str_len=strlen(str);
    const char *number_str=NULL;
    size_t      number_len=0;
    for(size_t i=0; i<str_len; i++)
    {
        if(hcompiler_is_number(str[i]))
        {
            if(number_str==NULL)
            {
                number_len=1;
                number_str=&str[i];
            }
            else
            {
                number_len++;
            }
        }
        else
        {
            if(number_str!=NULL)
            {
                if(index!=0)
                {
                    number_str=NULL;
                    index--;
                }
                else
                {
                    break;
                }
            }
        }
    }

    if(index!=0)
    {
        number_str=NULL;
        number_len=0;
    }

    if(number_str!=NULL && number_len > 0)
    {
        size_t ret=0;
        for(size_t i=0; i<number_len; i++)
        {
            ret*=10;
            ret+=(number_str[i]-'0');
        }
        return ret;
    }

    return 0;
}

const char * hcompiler_get_date(void)
{
    return __DATE__;
}


const char * hcompiler_get_time(void)
{
    return __TIME__;
}

static const char * const hcompiler_month_name[12] =
{
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "Jul",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dec"
};

static int hcompiler_month_strcmp(const char *str1,const char *str2)
{
    if(str1==NULL || str2==NULL)
    {
        return -1;
    }
    for(size_t i=0;; i++)
    {
        if(str1[i]=='\0')
        {
            break;
        }
        if(str2[i]=='\0')
        {
            break;
        }
        if(str1[i]!=str2[i])
        {
            return -1;
        }
    }
    return 0;
}

int hcompiler_get_date_year(void)
{
    const char *date=hcompiler_get_date();
    return hcompiler_get_number(date,1);
}


int hcompiler_get_date_month(void)
{
    for(size_t i=0; i<sizeof(hcompiler_month_name)/sizeof(hcompiler_month_name[0]); i++)
    {
        if(hcompiler_month_strcmp(hcompiler_month_name[i],hcompiler_get_date())==0)
        {
            return i+1;
        }
    }
    return -1;
}


int hcompiler_get_date_day(void)
{
    const char *date=hcompiler_get_date();
    return hcompiler_get_number(date,0);
}

int hcompiler_get_time_hour(void)
{
    const char *timestr=hcompiler_get_time();
    return hcompiler_get_number(timestr,0);
}


int hcompiler_get_time_minute(void)
{
    const char *timestr=hcompiler_get_time();
    return hcompiler_get_number(timestr,1);
}


int hcompiler_get_time_second(void)
{
    const char *timestr=hcompiler_get_time();
    return hcompiler_get_number(timestr,2);
}
