# src/demo/CMakeLists.txt
# Equivalent of src/demo/xmake.lua

# ============================================================
# Source files
# ============================================================
set(_DEMO_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/demo.c"
)

file(GLOB _DEMO_LIBC_SRCS    "${CMAKE_CURRENT_SOURCE_DIR}/libc/*.c")
file(GLOB _DEMO_UTILS_SRCS   "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.c")
list(FILTER _DEMO_UTILS_SRCS EXCLUDE REGEX ".*/option\\.c$")

list(APPEND _DEMO_SRCS
    ${_DEMO_LIBC_SRCS}
    "${CMAKE_CURRENT_SOURCE_DIR}/libm/integer.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/math/random.c"
    ${_DEMO_UTILS_SRCS}
)

file(GLOB _DEMO_OTHER_SRCS   "${CMAKE_CURRENT_SOURCE_DIR}/other/*.c"
                              "${CMAKE_CURRENT_SOURCE_DIR}/other/*.cpp")
list(FILTER _DEMO_OTHER_SRCS EXCLUDE REGEX ".*/charset\\.c$")
list(APPEND _DEMO_SRCS ${_DEMO_OTHER_SRCS})

file(GLOB _DEMO_STRING_SRCS  "${CMAKE_CURRENT_SOURCE_DIR}/string/*.c")
file(GLOB_RECURSE _DEMO_MEM_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/memory/*.c")
file(GLOB _DEMO_PLAT_SRCS    "${CMAKE_CURRENT_SOURCE_DIR}/platform/*.c")
list(FILTER _DEMO_PLAT_SRCS EXCLUDE REGEX ".*/exception\\.c$")
list(FILTER _DEMO_PLAT_SRCS EXCLUDE REGEX ".*/context\\.c$")
file(GLOB _DEMO_CONTAINER_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/container/*.c")
file(GLOB _DEMO_ALGO_SRCS    "${CMAKE_CURRENT_SOURCE_DIR}/algorithm/*.c")

list(APPEND _DEMO_SRCS
    ${_DEMO_STRING_SRCS}
    ${_DEMO_MEM_SRCS}
    ${_DEMO_PLAT_SRCS}
    ${_DEMO_CONTAINER_SRCS}
    ${_DEMO_ALGO_SRCS}
    "${CMAKE_CURRENT_SOURCE_DIR}/stream/stream.c"
)

file(GLOB _DEMO_STREAM_SRCS  "${CMAKE_CURRENT_SOURCE_DIR}/stream/stream/*.c")
file(GLOB_RECURSE _DEMO_NET_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/network/*.c")
list(APPEND _DEMO_SRCS ${_DEMO_STREAM_SRCS} ${_DEMO_NET_SRCS})

# ============================================================
# Optional modules
# ============================================================
if(TBOX_MODULE_HASH)
    file(GLOB _DEMO_HASH_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/hash/*.c")
    list(APPEND _DEMO_SRCS ${_DEMO_HASH_SRCS})
endif()

if(TBOX_FLOAT)
    list(APPEND _DEMO_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/math/fixed.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/libm/float.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/libm/double.c")
endif()

if(TBOX_MODULE_COROUTINE)
    file(GLOB_RECURSE _DEMO_CORO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/coroutine/*.c")
    list(APPEND _DEMO_SRCS ${_DEMO_CORO_SRCS})
    list(APPEND _DEMO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/platform/context.c")
endif()

if(TBOX_EXCEPTION)
    list(APPEND _DEMO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/platform/exception.c")
endif()

if(TBOX_MODULE_XML)
    file(GLOB _DEMO_XML_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/xml/*.c")
    list(APPEND _DEMO_SRCS ${_DEMO_XML_SRCS})
endif()

if(TBOX_MODULE_REGEX)
    file(GLOB _DEMO_REGEX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/regex/*.c")
    list(APPEND _DEMO_SRCS ${_DEMO_REGEX_SRCS})
endif()

if(TBOX_MODULE_OBJECT)
    file(GLOB _DEMO_OBJ_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/object/*.c")
    list(APPEND _DEMO_SRCS ${_DEMO_OBJ_SRCS})
    list(APPEND _DEMO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/utils/option.c")
endif()

if(TBOX_MODULE_CHARSET)
    list(APPEND _DEMO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/other/charset.c")
endif()

if(TBOX_MODULE_DATABASE)
    list(APPEND _DEMO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/database/sql.c")
endif()

# ============================================================
# Target
# ============================================================
add_executable(demo ${_DEMO_SRCS})

target_compile_definitions(demo PRIVATE __tb_prefix__="demo")

target_link_libraries(demo PRIVATE tbox)

# Windows subsystem compatibility
if(WIN32 AND MSVC)
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        target_link_options(demo PRIVATE /subsystem:console,5.01)
    else()
        target_link_options(demo PRIVATE /subsystem:console,5.02)
    endif()
endif()

# MinGW/MSYS/Cygwin: static libgcc
if(MINGW OR MSYS OR CYGWIN)
    target_link_options(demo PRIVATE -static-libgcc)
endif()

# BSD: execinfo
if(CMAKE_SYSTEM_NAME MATCHES "BSD")
    target_link_libraries(demo PRIVATE execinfo)
endif()
