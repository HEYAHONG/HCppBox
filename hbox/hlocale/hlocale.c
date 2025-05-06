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


