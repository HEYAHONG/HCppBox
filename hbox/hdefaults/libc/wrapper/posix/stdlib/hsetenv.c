/***************************************************************
 * Name:      hsetenv.c
 * Purpose:   实现hsetenv接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-08
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hsetenv.h"
#include "hdefaults.h"


#ifdef HSETENV
extern int HSETENV(const char *envname, const char *envval, int overwrite);
#endif // HSETENV

int hsetenv(const char *envname, const char *envval, int overwrite)
{
#if defined(HSETENV)
    return HSETENV(envname,envval,overwrite);
#elif defined(HDEFAULTS_OS_UNIX)
    return setenv(envname,envval,overwrite);
#elif defined(HDEFAULTS_OS_WINDOWS)
    {
        const char *old_envval=hgetenv(envname);
        if(old_envval!=NULL && old_envval[0]!='\0')
        {
            if(overwrite==0)
            {
                /*
                 * 不改变原有变量
                 */
                return 0;
            }
        }
        if(SetEnvironmentVariable(envname,envval))
        {
            return 0;
        }
    }
    return -1;
#else
    {
        const char *old_envval=hgetenv(envname);
        if(old_envval!=NULL && old_envval[0]!='\0')
        {
            if(overwrite==0)
            {
                /*
                 * 不改变原有变量
                 */
                return 0;
            }
        }
        return hlibc_env_setenv(envname,envval,overwrite);
    }
#endif
}


