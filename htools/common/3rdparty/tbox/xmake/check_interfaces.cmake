# check_interfaces.cmake
# Equivalent of xmake/check_interfaces.lua
# Checks for available C functions and sets TB_CONFIG_* variables accordingly.
# Usage: include(check_interfaces.cmake) after project() and before target definition.

include(CheckFunctionExists)
include(CheckSymbolExists)
include(CheckCSourceCompiles)

# Helper: suppress common warnings during checks
if(MSVC)
    set(_TB_CHECK_CFLAGS "-D_CRT_SECURE_NO_WARNINGS")
else()
    set(_TB_CHECK_CFLAGS "-Wno-error=unused-variable")
endif()

# Save and restore CMAKE_REQUIRED_* around each group
macro(tb_check_cfunc VARNAME FUNC HEADERS)
    set(_tb_includes_code "")
    foreach(_hdr ${HEADERS})
        string(APPEND _tb_includes_code "#include <${_hdr}>\n")
    endforeach()
    set(CMAKE_REQUIRED_FLAGS "${_TB_CHECK_CFLAGS}")
    set(CMAKE_REQUIRED_DEFINITIONS "")
    check_c_source_compiles("${_tb_includes_code}
int main(void){ (void)${FUNC}; return 0; }
" ${VARNAME})
    if(${VARNAME})
        set(${VARNAME} 1)
    else()
        set(${VARNAME} 0)
    endif()
endmacro()

macro(tb_check_csnippet VARNAME SNIPPET HEADERS)
    set(_tb_includes_code "")
    foreach(_hdr ${HEADERS})
        string(APPEND _tb_includes_code "#include <${_hdr}>\n")
    endforeach()
    set(CMAKE_REQUIRED_FLAGS "${_TB_CHECK_CFLAGS}")
    # Check if snippet already has a main() definition; if not, append an empty main
    if("${SNIPPET}" MATCHES "main[[:space:]]*\\(")
        # Snippet already defines main -> use as-is
        set(_tb_snippet_code "${_tb_includes_code}\n${SNIPPET}\n")
    else()
        # No main -> append a trivial main at the end
        set(_tb_snippet_code "${_tb_includes_code}\n${SNIPPET}\nint main(void){ return 0; }\n")
    endif()
    check_c_source_compiles("${_tb_snippet_code}" ${VARNAME})
    if(${VARNAME})
        set(${VARNAME} 1)
    else()
        set(${VARNAME} 0)
    endif()
endmacro()

# ============================================================
# libc - string.h / stdlib.h
# ============================================================
foreach(_func
    memcpy memset memmove memcmp memmem
    strcat strncat strcpy strncpy strlcpy
    strlen strnlen strstr strchr strrchr strcasestr
    strcmp strcasecmp strncmp strncasecmp strupr strlwr)
    string(TOUPPER "${_func}" _FUNC)
    tb_check_cfunc(TB_CONFIG_LIBC_HAVE_${_FUNC} "${_func}" "string.h;stdlib.h")
endforeach()

# libc - wchar.h / wctype.h / stdlib.h
foreach(_func
    wcscat wcsncat wcscpy wcsncpy wcslcpy
    wcslen wcsnlen wcsstr wcscasestr
    wcscmp wcscasecmp wcsncmp wcsncasecmp
    wcstombs mbstowcs towlower towupper wcsupr wcslwr)
    string(TOUPPER "${_func}" _FUNC)
    tb_check_cfunc(TB_CONFIG_LIBC_HAVE_${_FUNC} "${_func}" "wchar.h;wctype.h;stdlib.h")
endforeach()

# libc - time.h
foreach(_func gmtime mktime localtime)
    string(TOUPPER "${_func}" _FUNC)
    tb_check_cfunc(TB_CONFIG_LIBC_HAVE_${_FUNC} "${_func}" "time.h")
endforeach()

# libc - sys/time.h
tb_check_cfunc(TB_CONFIG_LIBC_HAVE_GETTIMEOFDAY "gettimeofday" "sys/time.h")

# libc - signal.h / setjmp.h
foreach(_func signal setjmp kill)
    string(TOUPPER "${_func}" _FUNC)
    tb_check_cfunc(TB_CONFIG_LIBC_HAVE_${_FUNC} "${_func}" "signal.h;setjmp.h")
endforeach()
tb_check_csnippet(TB_CONFIG_LIBC_HAVE_SIGSETJMP
    "void test(void){ sigjmp_buf buf; sigsetjmp(buf, 0); }"
    "signal.h;setjmp.h")

# libc - execinfo.h
tb_check_cfunc(TB_CONFIG_LIBC_HAVE_BACKTRACE "backtrace" "execinfo.h")

# libc - locale.h
tb_check_cfunc(TB_CONFIG_LIBC_HAVE_SETLOCALE "setlocale" "locale.h")

# libc - stdio.h
foreach(_func fputs fgets fgetc ungetc fputc fread fwrite)
    string(TOUPPER "${_func}" _FUNC)
    tb_check_cfunc(TB_CONFIG_LIBC_HAVE_${_FUNC} "${_func}" "stdio.h")
endforeach()

# libc - stdlib.h (random)
foreach(_func srandom random)
    string(TOUPPER "${_func}" _FUNC)
    tb_check_cfunc(TB_CONFIG_LIBC_HAVE_${_FUNC} "${_func}" "stdlib.h")
endforeach()

# ============================================================
# libm - math.h
# ============================================================
foreach(_func
    sincos sincosf log2 log2f sqrt sqrtf
    acos acosf asin asinf pow powf
    fmod fmodf tan tanf atan atanf
    atan2 atan2f cos cosf sin sinf exp expf)
    string(TOUPPER "${_func}" _FUNC)
    tb_check_cfunc(TB_CONFIG_LIBM_HAVE_${_FUNC} "${_func}" "math.h")
endforeach()

# ============================================================
# posix (non-Windows only)
# ============================================================
if(NOT WIN32)
    tb_check_cfunc(TB_CONFIG_POSIX_HAVE_POLL          "poll"          "poll.h;sys/socket.h")
    tb_check_cfunc(TB_CONFIG_POSIX_HAVE_SELECT        "select"        "sys/select.h")

    foreach(_func
        pthread_mutex_init pthread_create
        pthread_setspecific pthread_getspecific
        pthread_key_create pthread_key_delete
        pthread_setaffinity_np)
        string(TOUPPER "${_func}" _FUNC)
        tb_check_cfunc(TB_CONFIG_POSIX_HAVE_${_FUNC} "${_func}" "pthread.h")
    endforeach()

    tb_check_cfunc(TB_CONFIG_POSIX_HAVE_SOCKET        "socket"        "sys/socket.h;fcntl.h")
    tb_check_cfunc(TB_CONFIG_POSIX_HAVE_OPENDIR       "opendir"       "dirent.h")
    tb_check_cfunc(TB_CONFIG_POSIX_HAVE_DLOPEN        "dlopen"        "dlfcn.h")

    foreach(_func open stat64 lstat64)
        string(TOUPPER "${_func}" _FUNC)
        tb_check_cfunc(TB_CONFIG_POSIX_HAVE_${_FUNC} "${_func}" "sys/stat.h;fcntl.h")
    endforeach()

    tb_check_cfunc(TB_CONFIG_POSIX_HAVE_GETHOSTNAME   "gethostname"   "unistd.h")
    tb_check_cfunc(TB_CONFIG_POSIX_HAVE_GETIFADDRS    "getifaddrs"    "ifaddrs.h;net/if_dl.h")
    tb_check_cfunc(TB_CONFIG_POSIX_HAVE_GETPAGESIZE   "getpagesize"   "unistd.h")
    tb_check_cfunc(TB_CONFIG_POSIX_HAVE_SYSCONF       "sysconf"       "unistd.h")

    foreach(_func sched_yield sched_setaffinity)
        string(TOUPPER "${_func}" _FUNC)
        tb_check_cfunc(TB_CONFIG_POSIX_HAVE_${_FUNC} "${_func}" "sched.h")
    endforeach()

    foreach(_func regcomp regexec)
        string(TOUPPER "${_func}" _FUNC)
        tb_check_cfunc(TB_CONFIG_POSIX_HAVE_${_FUNC} "${_func}" "regex.h")
    endforeach()

    foreach(_func readv writev preadv pwritev)
        string(TOUPPER "${_func}" _FUNC)
        tb_check_cfunc(TB_CONFIG_POSIX_HAVE_${_FUNC} "${_func}" "sys/uio.h")
    endforeach()

    foreach(_func pread64 pwrite64 fdatasync execvp execvpe fork vfork getdtablesize pipe pipe2)
        string(TOUPPER "${_func}" _FUNC)
        tb_check_cfunc(TB_CONFIG_POSIX_HAVE_${_FUNC} "${_func}" "unistd.h")
    endforeach()

    tb_check_cfunc(TB_CONFIG_POSIX_HAVE_COPYFILE      "copyfile"      "copyfile.h")
    tb_check_cfunc(TB_CONFIG_POSIX_HAVE_SENDFILE      "sendfile"      "sys/sendfile.h")

    foreach(_func epoll_create epoll_wait)
        string(TOUPPER "${_func}" _FUNC)
        tb_check_cfunc(TB_CONFIG_POSIX_HAVE_${_FUNC} "${_func}" "sys/epoll.h")
    endforeach()

    tb_check_cfunc(TB_CONFIG_POSIX_HAVE_WAITPID       "waitpid"       "sys/wait.h")
    tb_check_cfunc(TB_CONFIG_POSIX_HAVE_GETRLIMIT     "getrlimit"     "sys/resource.h")

    foreach(_func getaddrinfo getnameinfo gethostbyname gethostbyaddr)
        string(TOUPPER "${_func}" _FUNC)
        tb_check_cfunc(TB_CONFIG_POSIX_HAVE_${_FUNC} "${_func}" "netdb.h")
    endforeach()

    tb_check_cfunc(TB_CONFIG_POSIX_HAVE_FCNTL         "fcntl"         "fcntl.h")
    tb_check_cfunc(TB_CONFIG_POSIX_HAVE_MKFIFO        "mkfifo"        "sys/stat.h")
    tb_check_cfunc(TB_CONFIG_POSIX_HAVE_MMAP          "mmap"          "sys/mman.h")

    foreach(_func futimens utimensat)
        string(TOUPPER "${_func}" _FUNC)
        tb_check_cfunc(TB_CONFIG_POSIX_HAVE_${_FUNC} "${_func}" "sys/stat.h")
    endforeach()
endif()

# posix - non-Windows and non-WASM
if(NOT WIN32 AND NOT EMSCRIPTEN)
    foreach(_func posix_spawnp posix_spawn_file_actions_addchdir_np)
        string(TOUPPER "${_func}" _FUNC)
        tb_check_cfunc(TB_CONFIG_POSIX_HAVE_${_FUNC} "${_func}" "spawn.h")
    endforeach()
    tb_check_cfunc(TB_CONFIG_POSIX_HAVE_SEM_INIT      "sem_init"      "semaphore.h")
endif()

# ============================================================
# windows intrinsics (MSVC only)
# ============================================================
if(WIN32 AND MSVC)
    foreach(_mo "" "_nf" "_acq" "_rel")
        string(TOUPPER "${_mo}" _MO_UPPER)
        string(REPLACE "-" "_" _MO_UPPER "${_MO_UPPER}")

        tb_check_csnippet(TB_CONFIG_WINDOWS_HAVE__INTERLOCKEDEXCHANGE${_MO_UPPER}
            "LONG _InterlockedExchange${_mo}(LONG volatile* Destination, LONG Value);
#pragma intrinsic(_InterlockedExchange${_mo})
void test(void){ _InterlockedExchange${_mo}(0, 0); }"
            "windows.h")

        tb_check_csnippet(TB_CONFIG_WINDOWS_HAVE__INTERLOCKEDEXCHANGE8${_MO_UPPER}
            "CHAR _InterlockedExchange8${_mo}(CHAR volatile* Destination, CHAR Value);
#pragma intrinsic(_InterlockedExchange8${_mo})
void test(void){ _InterlockedExchange8${_mo}(0, 0); }"
            "windows.h")

        tb_check_csnippet(TB_CONFIG_WINDOWS_HAVE__INTERLOCKEDOR8${_MO_UPPER}
            "CHAR _InterlockedOr8${_mo}(CHAR volatile* Destination, CHAR Value);
#pragma intrinsic(_InterlockedOr8${_mo})
void test(void){ _InterlockedOr8${_mo}(0, 0); }"
            "windows.h")

        tb_check_csnippet(TB_CONFIG_WINDOWS_HAVE__INTERLOCKEDEXCHANGEADD${_MO_UPPER}
            "LONG _InterlockedExchangeAdd${_mo}(LONG volatile* Destination, LONG Value);
#pragma intrinsic(_InterlockedExchangeAdd${_mo})
void test(void){ _InterlockedExchangeAdd${_mo}(0, 0); }"
            "windows.h")

        tb_check_csnippet(TB_CONFIG_WINDOWS_HAVE__INTERLOCKEDEXCHANGEADD64${_MO_UPPER}
            "__int64 _InterlockedExchangeAdd64${_mo}(__int64 volatile* Destination, __int64 Value);
#pragma intrinsic(_InterlockedExchangeAdd64${_mo})
void test(void){ _InterlockedExchangeAdd64${_mo}(0, 0); }"
            "windows.h")

        tb_check_csnippet(TB_CONFIG_WINDOWS_HAVE__INTERLOCKEDCOMPAREEXCHANGE${_MO_UPPER}
            "LONG _InterlockedCompareExchange${_mo}(LONG volatile* Destination, LONG Exchange, LONG Comperand);
#pragma intrinsic(_InterlockedCompareExchange${_mo})
void test(void){ _InterlockedCompareExchange${_mo}(0, 0, 0); }"
            "windows.h")

        tb_check_csnippet(TB_CONFIG_WINDOWS_HAVE__INTERLOCKEDCOMPAREEXCHANGE64${_MO_UPPER}
            "__int64 _InterlockedCompareExchange64${_mo}(__int64 volatile* Destination, __int64 Exchange, __int64 Comperand);
#pragma intrinsic(_InterlockedCompareExchange64${_mo})
void test(void){ _InterlockedCompareExchange64${_mo}(0, 0, 0); }"
            "windows.h")
    endforeach()
endif()

# ============================================================
# bsd
# ============================================================
if(NOT WIN32)
    tb_check_cfunc(TB_CONFIG_BSD_HAVE_FLOCK "flock" "sys/file.h;fcntl.h")
endif()

# ============================================================
# systemv
# ============================================================
if(NOT WIN32 AND NOT EMSCRIPTEN)
    foreach(_func semget semtimedop)
        string(TOUPPER "${_func}" _FUNC)
        tb_check_cfunc(TB_CONFIG_SYSTEMV_HAVE_${_FUNC} "${_func}" "sys/sem.h;sys/ipc.h")
    endforeach()
endif()

# ============================================================
# linux
# ============================================================
if(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR ANDROID)
    tb_check_cfunc(TB_CONFIG_LINUX_HAVE_INOTIFY_INIT "inotify_init" "sys/inotify.h")
    tb_check_csnippet(TB_CONFIG_LINUX_HAVE_IFADDRS
        "void test(void){}"
        "linux/if.h;linux/netlink.h")
endif()

# ============================================================
# valgrind
# ============================================================
tb_check_csnippet(TB_CONFIG_VALGRIND_HAVE_VALGRIND_STACK_REGISTER
    "void test(void){ VALGRIND_STACK_REGISTER(0, 0); }"
    "valgrind/valgrind.h")

# ============================================================
# keywords
# ============================================================
set(CMAKE_REQUIRED_LIBRARIES "pthread")
tb_check_csnippet(TB_CONFIG_KEYWORD_HAVE__thread
    "__thread int a = 0; int main(void){ return a; }"
    "")
tb_check_csnippet(TB_CONFIG_KEYWORD_HAVE_Thread_local
    "_Thread_local int a = 0; int main(void){ return a; }"
    "")
set(CMAKE_REQUIRED_LIBRARIES "")

# ============================================================
# features
# ============================================================
tb_check_csnippet(TB_CONFIG_FEATURE_HAVE_ANONYMOUS_UNION
    "void test(void){ struct __st { union { int dummy; }; } a; a.dummy = 1; }"
    "")
