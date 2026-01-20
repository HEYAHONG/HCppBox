/***************************************************************
 * Name:      hunsetenv.c
 * Purpose:   实现hunsetenv接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-08
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hunsetenv.h"
#include "hdefaults.h"
#include "stdlib.h"


#ifdef HUNSETENV
extern int HUNSETENV(const char *envname);
#endif // HUNSETENV

int hunsetenv(const char *envname)
{
#if defined(HUNSETENV)
    return HUNSETENV(envname);
#elif defined(HDEFAULTS_OS_UNIX)
    return unsetenv(envname);
#elif defined(HDEFAULTS_OS_WINDOWS)
    {
        const char *old_envval=hgetenv(envname);
        if(old_envval==NULL || old_envval[0]=='\0')
        {
            /*
             * 视为成功
             */
            return 0;
        }
        if(SetEnvironmentVariable(envname,NULL))
        {
            return 0;
        }
    }
    return -1;
#elif !defined(HLIBC_NO_IMPLEMENTATION) && !defined(HLIBC_NO_ENV)
    {
        const char *old_envval=hgetenv(envname);
        if(old_envval==NULL || old_envval[0]=='\0')
        {
            /*
             * 视为成功
             */
            return 0;
        }
        return hlibc_env_unsetenv(envname);
    }
#else
    {
        const char *old_envval=hgetenv(envname);
        if(old_envval==NULL || old_envval[0]=='\0')
        {
            /*
             * 视为成功
             */
            return 0;
        }
        return -1;
    }
#endif
}


