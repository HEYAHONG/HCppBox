/***************************************************************
 * Name:      hthrd_windows.c
 * Purpose:   实现hthrd_windows接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-27
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "stdlib.h"

struct hthrd_windows
{
    HANDLE      hThread;
    DWORD       ThreadId;
};
typedef struct hthrd_windows hthrd_windows_t;

struct hthrd_windows_create_context
{
    hthrd_start_t func;
    void *arg;
};
typedef struct hthrd_windows_create_context hthrd_windows_create_context_t;

static DWORD WINAPI hthrd_windows_thread_proc(LPVOID lpParameter)
{
    hthrd_windows_create_context_t *ctx=(hthrd_windows_create_context_t*)lpParameter;
    if(ctx==NULL)
    {
        return -1;
    }

    hthrd_start_t   func=ctx->func;
    void *          arg=ctx->arg;

    hfree(ctx);

    if(func!=NULL)
    {
        return func(arg);
    }


    return -1;
}



int hthrd_windows_create(hthrd_t *thr,hthrd_start_t func,void *arg )
{
    int ret=hthrd_error;
    if(sizeof(hthrd_t) < sizeof(hthrd_windows_t))
    {
        return ret;
    }
    if(func==NULL)
    {
        return ret;
    }
    hthrd_windows_t thr_ctx= {0};
    hthrd_windows_create_context_t *thr_start_ctx=(hthrd_windows_create_context_t *)hmalloc(sizeof(hthrd_windows_create_context_t));
    if(thr_start_ctx==NULL)
    {
        return ret;
    }
    thr_start_ctx->func=func;
    thr_start_ctx->arg=arg;
    thr_ctx.hThread=CreateThread(NULL,0,hthrd_windows_thread_proc,thr_start_ctx,0,&thr_ctx.ThreadId);
    if(thr_ctx.hThread==NULL)
    {
        hfree(thr_start_ctx);
    }
    else
    {
        ret=hthrd_success;
    }

    if(thr!=NULL)
    {
        (*(hthrd_windows_t *)thr)=thr_ctx;
    }
    else
    {
        if(ret==hthrd_success)
        {
            CloseHandle(thr_ctx.hThread);
        }
    }

    return ret;
}


int hthrd_windows_equal(hthrd_t lhs,hthrd_t rhs )
{
    int ret=-1;
    if(sizeof(hthrd_t) < sizeof(hthrd_windows_t))
    {
        return ret;
    }
    if((*(hthrd_windows_t *)&lhs).hThread!=NULL && (*(hthrd_windows_t *)&rhs).hThread!=NULL)
    {
        if((*(hthrd_windows_t *)&lhs).ThreadId==(*(hthrd_windows_t *)&rhs).ThreadId)
        {
            ret=0;
        }
    }

    return ret;
}


hthrd_t hthrd_windows_current(void)
{
    hthrd_t ret= {0};

    if(sizeof(hthrd_t) < sizeof(hthrd_windows_t))
    {
        return ret;
    }

    hthrd_windows_t thr_ctx= {0};
    thr_ctx.hThread=GetCurrentThread();
    thr_ctx.ThreadId=GetCurrentThreadId();

    (*(hthrd_windows_t *)&ret)=thr_ctx;

    return ret;
}


int hthrd_windows_sleep(const htimespec_t* duration,htimespec_t * remaining)
{
    int ret=0;

    htimespec_t current= {0};
    if(duration!=NULL)
    {
        current=(*duration);
    }

    while(current.tv_sec--)
    {
        Sleep(1000);
    }

    while(current.tv_nsec > 1000000)
    {
        current.tv_nsec-=1000000;
        Sleep(1);
    }

    if(remaining!=NULL)
    {
        (*remaining)=current;
    }

    return ret;
}

void hthrd_windows_yield(void)
{
    Sleep(0);
}


void hthrd_windows_exit(int res)
{
    ExitThread(res);
}


int hthrd_windows_detach(hthrd_t thr)
{
    int ret=hthrd_error;
    if(sizeof(hthrd_t) < sizeof(hthrd_windows_t))
    {
        return ret;
    }
    if((*(hthrd_windows_t *)&thr).hThread!=NULL)
    {
        CloseHandle((*(hthrd_windows_t *)&thr).hThread);
        ret=hthrd_success;
    }

    return ret;
}


int hthrd_windows_join(hthrd_t thr, int *res)
{
    int ret=hthrd_error;
    if(sizeof(hthrd_t) < sizeof(hthrd_windows_t))
    {
        return ret;
    }
    if((*(hthrd_windows_t *)&thr).hThread!=NULL)
    {
        WaitForSingleObject((*(hthrd_windows_t *)&thr).hThread,INFINITE);
        if(res!=NULL)
        {
            DWORD result=0;
            if(GetExitCodeThread((*(hthrd_windows_t *)&thr).hThread,&result))
            {
                (*res)=result;
            }
        }
        CloseHandle((*(hthrd_windows_t *)&thr).hThread);
        ret=hthrd_success;
    }

    return ret;
}

