/***************************************************************
 * Name:      hfclose.c
 * Purpose:   实现hfclose接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-02-13
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hfclose.h"
#include "hdefaults.h"
#include "stdlib.h"

#ifdef HFCLOSE
extern int HFCLOSE(FILE * stream );
#endif // HFCLOSE

int hfclose(FILE * stream)
{
#if defined(HFCLOSE)
    return HFCLOSE(stream);
#elif ((HDEFAULTS_LIBC_OPTIMIZE_LEVEL) > 0) && !defined(HDEFAULTS_LIBC_TINY) && !defined(HLIBC_NO_STDIO)
    return hlibc_fclose((hfile_t *)stream);
#else
    return fclose(stream);
#endif
}


