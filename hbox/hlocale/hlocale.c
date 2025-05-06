/***************************************************************
 * Name:      hlocale.c
 * Purpose:   实现hlocale接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-06
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hlocale.h"
#include "hdefaults.h"
#include "h3rdparty.h"

const char *hlocale_locale_get(void)
{
    const char *locale=NULL;
#if defined(HDEFAULTS_OS_WINDOWS)
    {
        static char locale_codepage[11+1]= {0};
        if(locale_codepage[0]=='\0')
        {
            int codepage=GetACP();
            hsprintf(locale_codepage,"CP%d",codepage);
        }
        locale=locale_codepage;
    }
#else

    if(locale==NULL || locale[0]=='\0')
    {
        locale=hgetenv("LC_ALL");
    }

    if(locale==NULL || locale[0]=='\0')
    {
        locale=hgetenv("LC_CTYPE");
    }

    if(locale==NULL || locale[0]=='\0')
    {
        locale=hgetenv("LANG");
    }

#endif

    //默认采用C.UTF-8
    if(locale==NULL || locale[0]=='\0')
    {
        locale="C.UTF-8";
    }
    return locale;
}

bool hlocale_charset_is_utf8(void)
{
    const char *locale=hlocale_locale_get();
    if(locale!=NULL && locale[0]!='\0')
    {
        /*
         * windows下根据代码页判断
         */
        if(strstr(locale,"CP65001")!=NULL || strstr(locale,"cp65001")!=NULL)
        {
            return true;
        }
        /*
         * 其它系统通过UTF-8字符串判断
         */
        if(strstr(locale,"UTF-8")!=NULL || strstr(locale,"utf-8")!=NULL || strstr(locale,"UTF8")!=NULL || strstr(locale,"utf8")!=NULL)
        {
            return true;
        }
    }
    return false;
}

bool hlocale_charset_is_gb2312(void)
{
    const char *locale=hlocale_locale_get();
    if(locale!=NULL && locale[0]!='\0')
    {
        /*
         * windows下根据代码页判断
         */
        if(strstr(locale,"CP936")!=NULL || strstr(locale,"cp936")!=NULL)
        {
            return true;
        }
        if(strstr(locale,"CP20936")!=NULL || strstr(locale,"cp20936")!=NULL)
        {
            return true;
        }
        //FreeBSD
        if(strstr(locale,"eucCN")!=NULL)
        {
            return true;
        }
        //其它环境下
        if(strstr(locale,"GB2312")!=NULL || strstr(locale,"gb2312")!=NULL)
        {
            return true;
        }
        if(strstr(locale,"GBK")!=NULL || strstr(locale,"gbk")!=NULL)
        {
            return true;
        }
    }
    return false;
}
