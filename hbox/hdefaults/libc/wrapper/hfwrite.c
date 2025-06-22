/***************************************************************
 * Name:      hfwrite.c
 * Purpose:   实现hfwrite接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-22
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hfwrite.h"
#include "hdefaults.h"
#include "stdlib.h"

#ifdef HFWRITE
extern size_t HFWRITE(const void*buffer,size_t obj_size,size_t obj_count,FILE * stream );
#endif // HFWRITE

size_t hfwrite(const void*buffer,size_t obj_size,size_t obj_count,FILE * stream)
{
#if defined(HFWRITE)
    return HFWRITE(buffer,obj_size,obj_count,stream);
#else
    return fwrite(buffer,obj_size,obj_count,stream);
#endif
}


