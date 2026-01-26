/***************************************************************
 * Name:      hthreads_common.c
 * Purpose:   实现hthreads_common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-26
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hthreads_common.h"
#include "stdlib.h"

int hthrd_code_cstd(int wrapper_code)
{
    int ret=wrapper_code;
#if defined(HDEFAULTS_LIBC_HAVE_THREADS)
    switch(ret)
    {
    case hthrd_success:
    {
        ret=thrd_success;
    }
    break;
    case hthrd_busy:
    {
        ret=thrd_busy;
    }
    break;
    case hthrd_error:
    {
        ret=thrd_error;
    }
    break;
    case hthrd_nomem:
    {
        ret=thrd_nomem;
    }
    break;
    case hthrd_timedout:
    {
        ret=thrd_timedout;
    }
    break;
    default:
    {

    }
    break;
    }
#endif
    return ret;
}

int hthrd_code_wrapper(int cstd_code)
{
    int ret=cstd_code;
#if defined(HDEFAULTS_LIBC_HAVE_THREADS)
    switch(ret)
    {
    case thrd_success:
    {
        ret=hthrd_success;
    }
    break;
    case thrd_busy:
    {
        ret=hthrd_busy;
    }
    break;
    case thrd_error:
    {
        ret=hthrd_error;
    }
    break;
    case thrd_nomem:
    {
        ret=hthrd_nomem;
    }
    break;
    case thrd_timedout:
    {
        ret=hthrd_timedout;
    }
    break;
    default:
    {

    }
    break;
    }
#endif
    return ret;
}

int hmtx_type_cstd(int wrapper_type)
{
#if defined(HDEFAULTS_LIBC_HAVE_THREADS)
    int ret=0;
    if((wrapper_type&hmtx_recursive)!=0)
    {
        ret|=mtx_recursive;
    }
    if((wrapper_type&hmtx_timed)!=0)
    {
        ret|=mtx_timed;
    }
    if(ret==0)
    {
        ret|=mtx_plain;
    }
    return ret;
#else
    return wrapper_type;
#endif
}
