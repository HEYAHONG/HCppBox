/***************************************************************
 * Name:      hstdatomic_common.c
 * Purpose:   实现hstdatomic_common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-25
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hstdatomic_common.h"
#include "stdlib.h"

int hmemory_order_cstd(hmemory_order_t __order)
{
#if !defined(HDEFAULTS_LIBC_HAVE_STDATOMIC)
    return (int)(__order);
#else
    int ret=0;
    switch(__order)
    {
    case hmemory_order_relaxed:
    {
        ret = (int)memory_order_relaxed;
    }
    break;
    case hmemory_order_consume:
    {
        ret = (int)memory_order_consume;
    }
    break;
    case hmemory_order_acquire:
    {
        ret = (int)memory_order_acquire;
    }
    break;
    case hmemory_order_release:
    {
        ret = (int)memory_order_release;
    }
    break;
    case hmemory_order_acq_rel:
    {
        ret = (int)memory_order_acq_rel;
    }
    break;
    case hmemory_order_seq_cst:
    {
        ret = (int)memory_order_seq_cst;
    }
    break;
    default:
    {

    }
    break;
    }
    return ret;
#endif
}
