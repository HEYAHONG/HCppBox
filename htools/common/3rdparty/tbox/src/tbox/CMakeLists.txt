# src/tbox/CMakeLists.txt
# Equivalent of src/tbox/xmake.lua

# ============================================================
# Library type
# ============================================================
if(TBOX_BUILD_SHARED)
    set(_TBOX_LIB_TYPE SHARED)
else()
    set(_TBOX_LIB_TYPE STATIC)
endif()

# ============================================================
# Source files (common, always compiled)
# ============================================================
file(GLOB _TBOX_ROOT_SRCS         "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
file(GLOB _TBOX_HASH_CORE_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/hash/bkdr.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/hash/fnv32.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/hash/adler32.c")
file(GLOB_RECURSE _TBOX_MATH_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/math/*.c")
file(GLOB_RECURSE _TBOX_LIBC_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/libc/*.c")
# exclude string/impl/**
file(GLOB_RECURSE _TBOX_LIBC_EXCL "${CMAKE_CURRENT_SOURCE_DIR}/libc/string/impl/*.c")
list(REMOVE_ITEM _TBOX_LIBC_SRCS ${_TBOX_LIBC_EXCL})

file(GLOB _TBOX_UTILS_SRCS        "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.c")
list(FILTER _TBOX_UTILS_SRCS EXCLUDE REGEX ".*/option\\.c$")

file(GLOB_RECURSE _TBOX_PREFIX_SRCS   "${CMAKE_CURRENT_SOURCE_DIR}/prefix/*.c")
file(GLOB_RECURSE _TBOX_MEMORY_SRCS   "${CMAKE_CURRENT_SOURCE_DIR}/memory/*.c")
file(GLOB_RECURSE _TBOX_STRING_SRCS   "${CMAKE_CURRENT_SOURCE_DIR}/string/*.c")

file(GLOB_RECURSE _TBOX_STREAM_SRCS   "${CMAKE_CURRENT_SOURCE_DIR}/stream/*.c")
file(GLOB         _TBOX_STREAM_EXCL1  "${CMAKE_CURRENT_SOURCE_DIR}/stream/impl/filter/charset.c"
                                      "${CMAKE_CURRENT_SOURCE_DIR}/stream/impl/filter/zip.c")
list(REMOVE_ITEM _TBOX_STREAM_SRCS ${_TBOX_STREAM_EXCL1})

file(GLOB_RECURSE _TBOX_NETWORK_SRCS  "${CMAKE_CURRENT_SOURCE_DIR}/network/*.c")
file(GLOB_RECURSE _TBOX_NET_SSL_EXCL  "${CMAKE_CURRENT_SOURCE_DIR}/network/impl/ssl/*.c")
list(REMOVE_ITEM _TBOX_NETWORK_SRCS ${_TBOX_NET_SSL_EXCL})

file(GLOB_RECURSE _TBOX_ALGORITHM_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/algorithm/*.c")

file(GLOB_RECURSE _TBOX_CONTAINER_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/container/*.c")
list(FILTER _TBOX_CONTAINER_SRCS EXCLUDE REGEX ".*/element/obj\\.c$")

set(_TBOX_LIBM_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/libm/impl/libm.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/libm/idivi8.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/libm/ilog2i.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/libm/isqrti.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/libm/isqrti64.c")

file(GLOB _TBOX_PLATFORM_SRCS     "${CMAKE_CURRENT_SOURCE_DIR}/platform/*.c")
list(FILTER _TBOX_PLATFORM_SRCS EXCLUDE REGEX ".*/context\\.c$")
list(FILTER _TBOX_PLATFORM_SRCS EXCLUDE REGEX ".*/exception\\.c$")
file(GLOB _TBOX_PLATFORM_IMPL_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/platform/impl/*.c")
list(FILTER _TBOX_PLATFORM_IMPL_SRCS EXCLUDE REGEX ".*/charset\\.c$")
list(FILTER _TBOX_PLATFORM_IMPL_SRCS EXCLUDE REGEX ".*/poller_fwatcher\\.c$")

set(_TBOX_SRCS
    ${_TBOX_ROOT_SRCS}
    ${_TBOX_HASH_CORE_SRCS}
    ${_TBOX_MATH_SRCS}
    ${_TBOX_LIBC_SRCS}
    ${_TBOX_UTILS_SRCS}
    ${_TBOX_PREFIX_SRCS}
    ${_TBOX_MEMORY_SRCS}
    ${_TBOX_STRING_SRCS}
    ${_TBOX_STREAM_SRCS}
    ${_TBOX_NETWORK_SRCS}
    ${_TBOX_ALGORITHM_SRCS}
    ${_TBOX_CONTAINER_SRCS}
    ${_TBOX_LIBM_SRCS}
    ${_TBOX_PLATFORM_SRCS}
    ${_TBOX_PLATFORM_IMPL_SRCS}
)

# ============================================================
# Optional: float
# ============================================================
if(TBOX_FLOAT)
    file(GLOB _TBOX_LIBM_FLOAT_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/libm/*.c")
    # exclude already-added impl/libm.c and integer files
    list(FILTER _TBOX_LIBM_FLOAT_SRCS EXCLUDE REGEX ".*/impl/.*")
    list(FILTER _TBOX_LIBM_FLOAT_SRCS EXCLUDE REGEX ".*/idivi8\\.c$")
    list(FILTER _TBOX_LIBM_FLOAT_SRCS EXCLUDE REGEX ".*/ilog2i\\.c$")
    list(FILTER _TBOX_LIBM_FLOAT_SRCS EXCLUDE REGEX ".*/isqrti\\.c$")
    list(FILTER _TBOX_LIBM_FLOAT_SRCS EXCLUDE REGEX ".*/isqrti64\\.c$")
    list(APPEND _TBOX_SRCS ${_TBOX_LIBM_FLOAT_SRCS})
endif()

# ============================================================
# Optional: xml
# ============================================================
if(TBOX_MODULE_XML)
    file(GLOB_RECURSE _TBOX_XML_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/xml/*.c")
    list(APPEND _TBOX_SRCS ${_TBOX_XML_SRCS})
endif()

# ============================================================
# Optional: regex
# ============================================================
if(TBOX_MODULE_REGEX)
    file(GLOB _TBOX_REGEX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/regex/*.c")
    list(APPEND _TBOX_SRCS ${_TBOX_REGEX_SRCS})
endif()

# ============================================================
# Optional: hash
# ============================================================
if(TBOX_MODULE_HASH)
    file(GLOB _TBOX_HASH_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/hash/*.c")
    list(APPEND _TBOX_SRCS ${_TBOX_HASH_SRCS})
    # CRC32 assembly (non-Windows, non-cosmocc)
    if(NOT WIN32)
        list(APPEND _TBOX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/hash/arch/crc32.S")
    endif()
endif()

# ============================================================
# Optional: coroutine
# ============================================================
if(TBOX_MODULE_COROUTINE)
    list(APPEND _TBOX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/platform/context.c")
    if(WIN32 AND MSVC)
        # e.g. x86 or x64
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            list(APPEND _TBOX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/platform/arch/x64/context.asm")
        else()
            list(APPEND _TBOX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/platform/arch/x86/context.asm")
        endif()
    else()
        list(APPEND _TBOX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/platform/arch/context.S")
    endif()
    file(GLOB_RECURSE _TBOX_COROUTINE_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/coroutine/*.c")
    list(APPEND _TBOX_SRCS ${_TBOX_COROUTINE_SRCS})
endif()

# ============================================================
# Optional: exception
# ============================================================
if(TBOX_EXCEPTION)
    list(APPEND _TBOX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/platform/exception.c")
endif()

# ============================================================
# Optional: object
# ============================================================
if(TBOX_MODULE_OBJECT)
    file(GLOB_RECURSE _TBOX_OBJECT_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/object/*.c")
    # exclude xml-dependent files initially
    list(FILTER _TBOX_OBJECT_SRCS EXCLUDE REGEX ".*/impl/reader/xml\\.c$")
    list(FILTER _TBOX_OBJECT_SRCS EXCLUDE REGEX ".*/impl/reader/xplist\\.c$")
    list(FILTER _TBOX_OBJECT_SRCS EXCLUDE REGEX ".*/impl/writer/xml\\.c$")
    list(FILTER _TBOX_OBJECT_SRCS EXCLUDE REGEX ".*/impl/writer/xplist\\.c$")
    list(APPEND _TBOX_SRCS ${_TBOX_OBJECT_SRCS})
    list(APPEND _TBOX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/utils/option.c")
    list(APPEND _TBOX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/container/element/obj.c")
    if(TBOX_MODULE_XML)
        list(APPEND _TBOX_SRCS
            "${CMAKE_CURRENT_SOURCE_DIR}/object/impl/reader/xml.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/object/impl/reader/xplist.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/object/impl/writer/xml.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/object/impl/writer/xplist.c")
    endif()
endif()

# ============================================================
# Optional: charset
# ============================================================
if(TBOX_MODULE_CHARSET)
    file(GLOB_RECURSE _TBOX_CHARSET_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/charset/*.c")
    list(APPEND _TBOX_SRCS ${_TBOX_CHARSET_SRCS})
    list(APPEND _TBOX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/platform/impl/charset.c")
    list(APPEND _TBOX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/stream/impl/filter/charset.c")
endif()

# ============================================================
# Optional: zip
# ============================================================
if(TBOX_MODULE_ZIP)
    file(GLOB_RECURSE _TBOX_ZIP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/zip/*.c")
    list(FILTER _TBOX_ZIP_SRCS EXCLUDE REGEX ".*/gzip\\.c$")
    list(FILTER _TBOX_ZIP_SRCS EXCLUDE REGEX ".*/zlib\\.c$")
    list(FILTER _TBOX_ZIP_SRCS EXCLUDE REGEX ".*/zlibraw\\.c$")
    list(FILTER _TBOX_ZIP_SRCS EXCLUDE REGEX ".*/lzsw\\.c$")
    list(APPEND _TBOX_SRCS ${_TBOX_ZIP_SRCS})
    list(APPEND _TBOX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/stream/impl/filter/zip.c")
    if(TBOX_USE_ZLIB AND ZLIB_FOUND)
        list(APPEND _TBOX_SRCS
            "${CMAKE_CURRENT_SOURCE_DIR}/zip/gzip.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/zip/zlib.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/zip/zlibraw.c")
    endif()
endif()

# ============================================================
# Optional: database
# ============================================================
if(TBOX_MODULE_DATABASE)
    file(GLOB _TBOX_DB_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/database/*.c")
    list(APPEND _TBOX_SRCS ${_TBOX_DB_SRCS})
    if(TB_CONFIG_PACKAGE_HAVE_MYSQL)
        list(APPEND _TBOX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/database/impl/mysql.c")
    endif()
    if(TB_CONFIG_PACKAGE_HAVE_SQLITE3)
        list(APPEND _TBOX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/database/impl/sqlite3.c")
    endif()
endif()

# ============================================================
# SSL backend
# ============================================================
if(TB_CONFIG_PACKAGE_HAVE_MBEDTLS)
    list(APPEND _TBOX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/network/impl/ssl/mbedtls.c")
elseif(TB_CONFIG_PACKAGE_HAVE_POLARSSL)
    list(APPEND _TBOX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/network/impl/ssl/polarssl.c")
elseif(TB_CONFIG_PACKAGE_HAVE_OPENSSL)
    list(APPEND _TBOX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/network/impl/ssl/openssl.c")
endif()

# ============================================================
# Platform-specific sources
# ============================================================
if(WIN32)
    list(APPEND _TBOX_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/platform/windows/windows.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/platform/windows/iocp_object.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/platform/windows/socket_pool.c")
    file(GLOB _TBOX_WIN_IFACE_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/platform/windows/interface/*.c")
    list(APPEND _TBOX_SRCS ${_TBOX_WIN_IFACE_SRCS})
endif()

if(IOS)
    list(APPEND _TBOX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/platform/mach/ios/directory.m")
endif()

if(ANDROID)
    file(GLOB _TBOX_ANDROID_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/platform/android/*.c")
    list(APPEND _TBOX_SRCS ${_TBOX_ANDROID_SRCS})
endif()

# ============================================================
# Create target
# ============================================================
add_library(tbox ${_TBOX_LIB_TYPE} ${_TBOX_SRCS})

target_compile_definitions(tbox PRIVATE __tb_prefix__="tbox")

# Export all symbols for Windows shared build
if(WIN32 AND TBOX_BUILD_SHARED)
    set_target_properties(tbox PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Include directories
target_include_directories(tbox
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "${TBOX_CONFIG_DIR}"
)

# Link libraries
target_link_libraries(tbox PUBLIC ${TBOX_PLATFORM_LIBS})

if(TB_CONFIG_PACKAGE_HAVE_ZLIB)
    target_link_libraries(tbox PUBLIC ZLIB::ZLIB)
endif()
if(TB_CONFIG_PACKAGE_HAVE_SQLITE3)
    target_link_libraries(tbox PUBLIC SQLite::SQLite3)
endif()
if(TB_CONFIG_PACKAGE_HAVE_MYSQL)
    target_link_libraries(tbox PUBLIC PkgConfig::MYSQL)
endif()
if(TB_CONFIG_PACKAGE_HAVE_MBEDTLS)
    target_link_libraries(tbox PUBLIC PkgConfig::MBEDTLS PkgConfig::MBEDCRYPTO PkgConfig::MBEDX509)
elseif(TB_CONFIG_PACKAGE_HAVE_POLARSSL AND NOT TB_CONFIG_PACKAGE_HAVE_MBEDTLS)
    if(POLARSSL_FOUND)
        target_link_libraries(tbox PUBLIC PkgConfig::POLARSSL)
    else()
        find_library(POLARSSL_LIBRARY NAMES polarssl REQUIRED)
        target_link_libraries(tbox PUBLIC ${POLARSSL_LIBRARY})
    endif()
elseif(TB_CONFIG_PACKAGE_HAVE_OPENSSL)
    target_link_libraries(tbox PUBLIC OpenSSL::SSL OpenSSL::Crypto)
endif()
if(TB_CONFIG_PACKAGE_HAVE_PCRE2)
    target_compile_definitions(tbox PUBLIC PCRE2_CODE_UNIT_WIDTH=${PCRE2_CODE_UNIT_WIDTH})
    if(PCRE2_FOUND)
        target_link_libraries(tbox PUBLIC PkgConfig::PCRE2)
    endif()
elseif(TB_CONFIG_PACKAGE_HAVE_PCRE)
    if(PCRE_FOUND)
        target_link_libraries(tbox PUBLIC PkgConfig::PCRE)
    else()
        find_library(PCRE_LIBRARY NAMES pcre REQUIRED)
        target_link_libraries(tbox PUBLIC ${PCRE_LIBRARY})
    endif()
endif()

# macOS frameworks
if(APPLE AND NOT IOS)
    target_link_libraries(tbox PUBLIC "-framework CoreFoundation" "-framework CoreServices")
endif()

# Linux debug: rdynamic
if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_options(tbox PUBLIC -rdynamic)
endif()

# Suppress "missing .note.GNU-stack" warning from assembler files on Linux/ELF
# This warning appears when .S files don't declare stack non-executable
if(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR (UNIX AND NOT APPLE))
    target_link_options(tbox PRIVATE -Wl,-z,noexecstack)
endif()
