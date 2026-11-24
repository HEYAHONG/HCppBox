if(NOT DEFINED __THREAD_LOCAL_STORAGE)
    set(__THREAD_LOCAL_STORAGE OFF CACHE BOOL
        "use thread local storage" FORCE)
endif()
