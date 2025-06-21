/***************************************************************
 * Name:      habort.c
 * Purpose:   实现habort接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "habort.h"
#include "hdefaults.h"
#include "stdlib.h"

#ifdef HABORT
extern void HABORT(void);
#endif // HABORT

void habort(void)
{
#if defined(HABORT)
    HABORT();
#else
    abort();
#endif
}


