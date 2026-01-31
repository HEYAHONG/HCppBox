/***************************************************************
 * Name:      herrno.c
 * Purpose:   实现herrno接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-02-01
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "herrno.h"
#include "hdefaults.h"
#include "errno.h"

herrno_t * herrno_get()
{
#if defined(HERRNO)
    return &(HERRNO);
#else
    return &(errno);
#endif
}

