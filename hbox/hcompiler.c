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

long hcompiler_get_stdc_version(void)
{
#ifdef __STDC_VERSION__
    return __STDC_VERSION__;
#else
    return 199409L;
#endif // __STDC_VERSION__
}


const char * hcompiler_get_date(void)
{
    return __DATE__;
}


const char * hcompiler_get_time(void)
{
    return __TIME__;
}

static const char * const Month_Name[12] =
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

static int month_strcmp(const char *str1,const char *str2)
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
    return (date[7]-'0')*1000+(date[8]-'0')*100+(date[9]-'0')*10+(date[10]-'0');
}


int hcompiler_get_date_month(void)
{
    for(size_t i=0; i<sizeof(Month_Name)/sizeof(Month_Name[0]); i++)
    {
        if(month_strcmp(Month_Name[i],hcompiler_get_date())==0)
        {
            return i+1;
        }
    }
    return -1;
}


int hcompiler_get_date_day(void)
{
    const char *date=hcompiler_get_date();
    return (date[4]-'0')*10+(date[5]-'0');
}

int hcompiler_get_time_hour(void)
{
    const char *timestr=hcompiler_get_time();
    return (timestr[0]-'0')*10+(timestr[1]-'0');
}


int hcompiler_get_time_minute(void)
{
    const char *timestr=hcompiler_get_time();
    return (timestr[3]-'0')*10+(timestr[4]-'0');
}


int hcompiler_get_time_second(void)
{
    const char *timestr=hcompiler_get_time();
    return (timestr[6]-'0')*10+(timestr[7]-'0');
}
