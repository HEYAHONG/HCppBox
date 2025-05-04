/***************************************************************
 * Name:      hgetenv.c
 * Purpose:   实现hgetenv接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-04
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hgetenv.h"
#include "hdefaults.h"

#ifdef HGETENV
extern char * HGETENV(const char *name);
#endif // HGETENV
extern char * getenv(const char *name);

char * hgetenv(const char *name)
{
#if defined(HGETENV)
    return HGETENV(name);
#else
    return getenv(name);
#endif
}


