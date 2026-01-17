/***************************************************************
 * Name:      hgetchar.c
 * Purpose:   实现hgetchar接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-02
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hgetchar.h"
#include "hdefaults.h"

#ifdef HGETCHAR
extern int HGETCHAR(void);
#endif // HGETCHAR


int hgetchar(void)
{
#if defined(HGETCHAR)
    return HGETCHAR();
#else
    return getchar();
#endif
}


