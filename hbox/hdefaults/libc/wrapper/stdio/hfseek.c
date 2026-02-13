/***************************************************************
 * Name:      hfseek.c
 * Purpose:   实现hfseek接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-02-13
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hfseek.h"
#include "hdefaults.h"
#include "stdlib.h"

#ifdef HFSEEK
extern int64_t HFSEEK(FILE * stream,int64_t offset,int whence);
#endif // HFSEEK

int64_t hfseek(FILE * stream,int64_t offset,int whence)
{
#if defined(HFSEEK)
    return HFSEEK(stream,offset,whence);
#elif ((HDEFAULTS_LIBC_OPTIMIZE_LEVEL) > 0) && !defined(HDEFAULTS_LIBC_TINY) && !defined(HLIBC_NO_STDIO)
    return hlibc_fseek((hfile_t *)stream,offset,whence);
#else
    return fseek(stream,offset,whence);
#endif
}


