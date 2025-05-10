/***************************************************************
 * Name:      hdefaults_libdl_port.c
 * Purpose:   实现hdefaults_libdl_port接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-10
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hruntime.h"

#ifdef HAVE_DLFCN_H
#include "dlfcn.h"
#endif // HAVE_DLFCN_H

#ifdef HDLOPEN
void *  HDLOPEN(const char *file, int mode);
#endif // HDLOPEN
void *hdlopen(const char *file, int mode)
{
#if defined(HDLOPEN)
    return HDLOPEN(file,mode);
#elif  defined(HAVE_DLFCN_H)
    return dlopen(file,mode);
#elif defined(HDEFAULTS_OS_WINDOWS)
    {
        //本组件中Windows下不支持模式(mode)
        if(file==NULL)
        {
            return GetModuleHandleA(NULL);
        }
        else
        {
            char lpFileName[MAX_PATH]= {0};
            {
                size_t len=strlen(file);
                if(len > sizeof(lpFileName)-1)
                {
                    return NULL;
                }
                for(size_t i=0; i < len; i++)
                {
                    lpFileName[i]=file[i];
                    if(lpFileName[i]=='/')
                    {
                        lpFileName[i]='\\';
                    }
                }
            }
            return LoadLibraryExA( lpFileName, NULL, LOAD_WITH_ALTERED_SEARCH_PATH );
        }
    }
#else
    return NULL;
#endif
}
#ifdef HDLCLOSE
int HDLCLOSE(void *handle);
#endif // HDLCLOSE
int hdlclose(void *handle)
{
#if defined(HDLCLOSE)
    return HDLCLOSE(handle);
#elif  defined(HAVE_DLFCN_H)
    return dlclose(handle);
#elif defined(HDEFAULTS_OS_WINDOWS)
    {
        HMODULE hModule = (HMODULE) handle;
        if( hModule == GetModuleHandleA(NULL))
        {
            return 0;
        }
        int ret=!FreeLibrary(hModule);
        return ret;
    }
#else
    return 0;
#endif

}



#ifdef HDLSYM
void *HDLSYM(void *handle, const char *name);
#endif // HDLSYM
void *hdlsym(void *handle, const char *name)
{
    if(handle==NULL)
    {
        //优先使用hruntime组件
        const hruntime_symbol_t * symbol=hruntime_symbol_find(name);
        if(symbol!=NULL)
        {
            return (void *)symbol->symbol_addr;
        }
    }
#if defined(HDLSYM)
    return HDLSYM(handle,name);
#elif defined(HAVE_DLFCN_H)
    return dlsym(handle,name);
#elif defined(HDEFAULTS_OS_WINDOWS)
    {
        HMODULE hModule = (HMODULE) handle;
        if(handle==NULL)
        {
            hModule=GetModuleHandleA(NULL);
        }
        return GetProcAddress( (HMODULE) hModule, name );
    }
#else
    return NULL;
#endif

}


