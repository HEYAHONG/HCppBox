/***************************************************************
 * Name:      hexit.c
 * Purpose:   实现hexit接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hexit.h"
#include "hdefaults.h"
#include "stdlib.h"

#ifdef HEXIT
extern void HEXIT(int exit_code);
#endif // HEXIT

void hexit(int exit_code)
{
#if defined(HEXIT)
    HEXIT(exit_code);
#else
    exit(exit_code);
#endif
}


