/***************************************************************
 * Name:      hfread.c
 * Purpose:   实现hfread接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-22
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hfread.h"
#include "hdefaults.h"
#include "stdlib.h"

#ifdef HFREAD
extern size_t HFREAD(void*buffer,size_t obj_size,size_t obj_count,FILE * stream );
#endif // HFREAD

size_t hfread(void*buffer,size_t obj_size,size_t obj_count,FILE * stream)
{
#if defined(HFREAD)
    return HFREAD(buffer,obj_size,obj_count,stream);
#else
    return fread(buffer,obj_size,obj_count,stream);
#endif
}


