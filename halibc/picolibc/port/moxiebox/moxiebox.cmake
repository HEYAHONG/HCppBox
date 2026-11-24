#***************************************************************
# Name:      moxiebox.cmake
# Purpose:   moxiebox专用picolibc CMake配置
# Author:    HYH (hyhsystem.cn)
# Created:   2026-11-24
# Copyright: HYH (hyhsystem.cn)
# License:   MIT
#**************************************************************/

if(NOT DEFINED __THREAD_LOCAL_STORAGE)
    set(__THREAD_LOCAL_STORAGE OFF CACHE BOOL
        "use thread local storage" FORCE)
endif()
