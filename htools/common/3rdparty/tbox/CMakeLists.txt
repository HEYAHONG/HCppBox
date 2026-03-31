cmake_minimum_required(VERSION 3.15)

# First pass: detect compiler (MSVC variable becomes available after project())
project(tbox VERSION 1.8.0 LANGUAGES C CXX)

# Enable ASM support based on detected compiler:
# MSVC uses MASM (ml.exe / ml64.exe), GCC/Clang uses GNU ASM
if(MSVC)
    enable_language(ASM_MASM)
else()
    enable_language(ASM)
endif()

include(FindPkgConfig)
include(CheckIncludeFile)
include(CheckCSourceCompiles)

# ============================================================
# Options (equivalent to xmake options in src/xmake.lua)
# ============================================================
option(TBOX_BUILD_DEMO      "Enable or disable the demo module."                        ON)
option(TBOX_SMALL           "Enable the small compile mode and disable all modules."    ON)
option(TBOX_MICRO           "Compile micro core library for the embed system."          OFF)
option(TBOX_FLOAT           "Enable or disable the float type."                         ON)
option(TBOX_WCHAR           "Enable wchar_t support."                                   ON)
option(TBOX_INFO            "Enable or disable version/info."                           ON)
option(TBOX_EXCEPTION       "Enable or disable the exception."                          OFF)
option(TBOX_DEPRECATED      "Enable or disable the deprecated interfaces."              OFF)
option(TBOX_FORCE_UTF8      "Forcely regard all tb_char* as utf-8."                     OFF)
option(TBOX_BUILD_SHARED    "Build tbox as a shared library (default: static)."         OFF)

# modules
option(TBOX_MODULE_XML      "The xml module."       ON)
option(TBOX_MODULE_ZIP      "The zip module."       ON)
option(TBOX_MODULE_HASH     "The hash module."      ON)
option(TBOX_MODULE_REGEX    "The regex module."     ON)
option(TBOX_MODULE_OBJECT   "The object module."    ON)
option(TBOX_MODULE_CHARSET  "The charset module."   ON)
option(TBOX_MODULE_DATABASE "The database module."  ON)
option(TBOX_MODULE_COROUTINE "The coroutine module." ON)

# packages
option(TBOX_USE_ZLIB        "Enable the zlib package."      ON)
option(TBOX_USE_MYSQL       "Enable the mysql package."     ON)
option(TBOX_USE_SQLITE3     "Enable the sqlite3 package."   ON)
option(TBOX_USE_OPENSSL     "Enable the openssl package."   ON)
option(TBOX_USE_POLARSSL    "Enable the polarssl package."  ON)
option(TBOX_USE_MBEDTLS     "Enable the mbedtls package."   ON)
option(TBOX_USE_PCRE2       "Enable the pcre2 package."     ON)
option(TBOX_USE_PCRE        "Enable the pcre package."      ON)

# ============================================================
# micro/small mode disables modules and float
# ============================================================
if(TBOX_MICRO OR TBOX_SMALL)
    set(TBOX_MODULE_XML      OFF)
    set(TBOX_MODULE_ZIP      OFF)
    set(TBOX_MODULE_HASH     OFF)
    set(TBOX_MODULE_REGEX    OFF)
    set(TBOX_MODULE_OBJECT   OFF)
    set(TBOX_MODULE_CHARSET  OFF)
    set(TBOX_MODULE_DATABASE OFF)
    set(TBOX_MODULE_COROUTINE OFF)
    set(TBOX_INFO            OFF)
    
    set(TBOX_USE_ZLIB        OFF)
    set(TBOX_USE_MYSQL       OFF)
    set(TBOX_USE_SQLITE3     OFF)
    set(TBOX_USE_OPENSSL     OFF)
    set(TBOX_USE_POLARSSL    OFF)
    set(TBOX_USE_MBEDTLS     OFF)
    set(TBOX_USE_PCRE2       OFF)
    set(TBOX_USE_PCRE        OFF)
endif()

if(TBOX_MICRO)
    set(TBOX_FLOAT OFF)
endif()

# ============================================================
# Global compile flags (equivalent to root xmake.lua)
# ============================================================
# Global compile flags (equivalent to root xmake.lua)
# ============================================================
# Non-MSVC: use C11 and C++11
if(NOT MSVC)
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_CXX_STANDARD 11)
endif()

# These must be defined at compile time, not in config header
# (equivalent to xmake's set_configvar("_GNU_SOURCE", 1))
add_compile_definitions(_GNU_SOURCE=1)
if(NOT MSVC)
    add_compile_definitions(_REENTRANT=1)
endif()

if(MSVC)
    # MSVC: use /std:c++latest for C++ and /std:clatest for C to enable more features (VLA, etc.)
    # Note: /std:clatest and /std:c++latest must be set separately for C and C++ compilation
    string(APPEND CMAKE_C_FLAGS_INIT "  /TC  /std:clatest")
    string(APPEND CMAKE_CXX_FLAGS_INIT " /std:c++latest")
    add_compile_options(/W3 /wd4996 /wd4133 /wd4090 /wd4214)
    add_compile_definitions(NOCRYPT NOGDI)
else()
    add_compile_options(
        -Wno-error=deprecated-declarations
        -fno-strict-aliasing
        -Wno-error=expansion-to-defined
        -Wno-error=empty-body
    )
endif()

if(TBOX_SMALL OR TBOX_MICRO)
    add_compile_definitions(__tb_small__)
    if(NOT MSVC)
        add_compile_options(-fno-stack-protector)
    endif()
endif()

# Build type defines
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(__tb_debug__)
endif()

# Platform-specific system links and MSVC runtime
if(WIN32)
    add_compile_definitions(NOCRYPT NOGDI)
    set(TBOX_PLATFORM_LIBS ws2_32 user32)
    # MSVC runtime: MTd for Debug, MT for Release
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreadedDebug)
    else()
        set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded)
    endif()
elseif(ANDROID)
    set(TBOX_PLATFORM_LIBS m c)
elseif(MINGW OR MSYS OR CYGWIN)
    set(TBOX_PLATFORM_LIBS ws2_32 user32 pthread m)
elseif(HAIKU)
    set(TBOX_PLATFORM_LIBS pthread network m c)
else()
    set(TBOX_PLATFORM_LIBS pthread dl m c)
endif()

# ============================================================
# Find optional packages
# ============================================================
# Rules:
# - TBOX_USE_XXX=ON: find REQUIRED, set TB_CONFIG_PACKAGE_HAVE_XXX=1 only if found
# - TBOX_USE_XXX=OFF: find QUIET (no error), never set TB_CONFIG_PACKAGE_HAVE_XXX=1

# ZLIB
set(TB_CONFIG_PACKAGE_HAVE_ZLIB 0)
if(TBOX_USE_ZLIB)
    find_package(ZLIB REQUIRED)
    if(ZLIB_FOUND)
        set(TB_CONFIG_PACKAGE_HAVE_ZLIB 1)
    endif()
else()
    find_package(ZLIB QUIET)
endif()

# SQLite3
set(TB_CONFIG_PACKAGE_HAVE_SQLITE3 0)
if(TBOX_USE_SQLITE3)
    find_package(SQLite3 REQUIRED)
    if(SQLite3_FOUND)
        set(TB_CONFIG_PACKAGE_HAVE_SQLITE3 1)
    endif()
else()
    find_package(SQLite3 QUIET)
endif()

# MySQL
set(TB_CONFIG_PACKAGE_HAVE_MYSQL 0)
if(TBOX_USE_MYSQL)
    pkg_check_modules(MYSQL REQUIRED IMPORTED_TARGET mysqlclient)
    if(MYSQL_FOUND)
        set(TB_CONFIG_PACKAGE_HAVE_MYSQL 1)
    endif()
else()
    pkg_check_modules(MYSQL IMPORTED_TARGET mysqlclient)
endif()

# mbedTLS - use pkg_check_modules with mbedcrypto.pc / mbedtls.pc / mbedx509.pc
set(TB_CONFIG_PACKAGE_HAVE_MBEDTLS 0)
if(TBOX_USE_MBEDTLS)
    pkg_check_modules(MBEDCRYPTO IMPORTED_TARGET mbedcrypto)
    pkg_check_modules(MBEDTLS    IMPORTED_TARGET mbedtls)
    pkg_check_modules(MBEDX509   IMPORTED_TARGET mbedx509)
    if(MBEDCRYPTO_FOUND AND MBEDTLS_FOUND AND MBEDX509_FOUND)
        set(CMAKE_REQUIRED_INCLUDES ${MBEDTLS_INCLUDE_DIRS})
        check_include_file("mbedtls/certs.h" _MBEDTLS_CERTS_H)
        unset(CMAKE_REQUIRED_INCLUDES)
        if(_MBEDTLS_CERTS_H)
            set(TB_CONFIG_PACKAGE_HAVE_MBEDTLS 1)
        else()
            message(STATUS "mbedTLS found but mbedtls/certs.h is missing, trying next SSL backend")
        endif()
    else()
        message(STATUS "mbedTLS requested but not found, trying next SSL backend")
    endif()
else()
    pkg_check_modules(MBEDCRYPTO IMPORTED_TARGET mbedcrypto)
    pkg_check_modules(MBEDTLS    IMPORTED_TARGET mbedtls)
    pkg_check_modules(MBEDX509   IMPORTED_TARGET mbedx509)
endif()

# OpenSSL (only if mbedTLS not found)
set(TB_CONFIG_PACKAGE_HAVE_OPENSSL 0)
if(NOT TB_CONFIG_PACKAGE_HAVE_MBEDTLS)
    if(TBOX_USE_OPENSSL)
        find_package(OpenSSL REQUIRED)
        if(OPENSSL_FOUND)
            set(TB_CONFIG_PACKAGE_HAVE_OPENSSL 1)
        endif()
    else()
        find_package(OpenSSL QUIET)
    endif()
endif()

# PolarSSL (old name of mbedTLS, only if mbedTLS and OpenSSL not found)
set(TB_CONFIG_PACKAGE_HAVE_POLARSSL 0)
if(NOT TB_CONFIG_PACKAGE_HAVE_MBEDTLS AND NOT TB_CONFIG_PACKAGE_HAVE_OPENSSL)
    if(TBOX_USE_POLARSSL)
        pkg_check_modules(POLARSSL REQUIRED IMPORTED_TARGET libpolarssl)
        if(POLARSSL_FOUND)
            set(TB_CONFIG_PACKAGE_HAVE_POLARSSL 1)
        endif()
    else()
        pkg_check_modules(POLARSSL IMPORTED_TARGET libpolarssl)
    endif()
endif()

# PCRE2 - try libpcre2-8, libpcre2-16, libpcre2-32 in order, pick first found
# Sets PCRE2_CODE_UNIT_WIDTH to 8/16/32 accordingly
set(TB_CONFIG_PACKAGE_HAVE_PCRE2 0)
set(PCRE2_CODE_UNIT_WIDTH 0)
if(TBOX_USE_PCRE2)
    pkg_check_modules(PCRE2 REQUIRED IMPORTED_TARGET libpcre2-8)
    if(PCRE2_FOUND)
        set(TB_CONFIG_PACKAGE_HAVE_PCRE2 1)
        set(PCRE2_CODE_UNIT_WIDTH 8)
    else()
        pkg_check_modules(PCRE2 REQUIRED IMPORTED_TARGET libpcre2-16)
        if(PCRE2_FOUND)
            set(TB_CONFIG_PACKAGE_HAVE_PCRE2 1)
            set(PCRE2_CODE_UNIT_WIDTH 16)
        else()
            pkg_check_modules(PCRE2 REQUIRED IMPORTED_TARGET libpcre2-32)
            if(PCRE2_FOUND)
                set(TB_CONFIG_PACKAGE_HAVE_PCRE2 1)
                set(PCRE2_CODE_UNIT_WIDTH 32)
            else()
                message(FATAL_ERROR "TBOX_USE_PCRE2 is ON but no libpcre2-8/16/32 found via pkg-config")
            endif()
        endif()
    endif()
else()
    pkg_check_modules(PCRE2 IMPORTED_TARGET libpcre2-8)
    if(PCRE2_FOUND)
        set(PCRE2_CODE_UNIT_WIDTH 8)
    else()
        pkg_check_modules(PCRE2 IMPORTED_TARGET libpcre2-16)
        if(PCRE2_FOUND)
            set(PCRE2_CODE_UNIT_WIDTH 16)
        else()
            pkg_check_modules(PCRE2 IMPORTED_TARGET libpcre2-32)
            if(PCRE2_FOUND)
                set(PCRE2_CODE_UNIT_WIDTH 32)
            endif()
        endif()
    endif()
endif()
if(TB_CONFIG_PACKAGE_HAVE_PCRE2)
    message(STATUS "Found PCRE2 with code unit width: ${PCRE2_CODE_UNIT_WIDTH}")
endif()

# PCRE (only if PCRE2 not found)
set(TB_CONFIG_PACKAGE_HAVE_PCRE 0)
if(NOT TB_CONFIG_PACKAGE_HAVE_PCRE2)
    if(TBOX_USE_PCRE)
        pkg_check_modules(PCRE REQUIRED IMPORTED_TARGET libpcre)
        if(PCRE_FOUND)
            set(TB_CONFIG_PACKAGE_HAVE_PCRE 1)
        endif()
    else()
        pkg_check_modules(PCRE IMPORTED_TARGET libpcre)
    endif()
endif()

# ============================================================
# Config variables for tb_config.h.cmake.in
# ============================================================
if(TBOX_SMALL)
    set(TB_CONFIG_SMALL 1)
else()
    set(TB_CONFIG_SMALL 0)
endif()

if(TBOX_MICRO)
    set(TB_CONFIG_MICRO_ENABLE 1)
else()
    set(TB_CONFIG_MICRO_ENABLE 0)
endif()

# Detect wchar_t
check_c_source_compiles( [[
    typedef wchar_t test_wchar_t;
    test_wchar_t a=0;
    int main()
    {
        return a;
    }
]] HAVE_WCHAR_T)
if(HAVE_WCHAR_T AND TBOX_WCHAR)
    set(TB_CONFIG_TYPE_HAVE_WCHAR 1)
else()
    set(TB_CONFIG_TYPE_HAVE_WCHAR 0)
endif()

if(TBOX_FLOAT)
    set(TB_CONFIG_TYPE_HAVE_FLOAT 1)
else()
    set(TB_CONFIG_TYPE_HAVE_FLOAT 0)
endif()

if(TBOX_FORCE_UTF8)
    set(TB_CONFIG_FORCE_UTF8 1)
    if(MSVC)
        add_compile_options(/utf-8)
    endif()
else()
    set(TB_CONFIG_FORCE_UTF8 0)
endif()

if(TBOX_DEPRECATED)
    set(TB_CONFIG_API_HAVE_DEPRECATED 1)
else()
    set(TB_CONFIG_API_HAVE_DEPRECATED 0)
endif()

if(TBOX_EXCEPTION)
    set(TB_CONFIG_EXCEPTION_ENABLE 1)
else()
    set(TB_CONFIG_EXCEPTION_ENABLE 0)
endif()

if(TBOX_INFO)
    set(TB_CONFIG_INFO_HAVE_VERSION 1)
    set(TB_CONFIG_INFO_TRACE_MORE   1)
else()
    set(TB_CONFIG_INFO_HAVE_VERSION 0)
    set(TB_CONFIG_INFO_TRACE_MORE   0)
endif()

# Module flags
foreach(_mod XML ZIP HASH REGEX OBJECT CHARSET DATABASE COROUTINE)
    string(TOUPPER "${_mod}" _MOD)
    if(TBOX_MODULE_${_MOD})
        set(TB_CONFIG_MODULE_HAVE_${_MOD} 1)
    else()
        set(TB_CONFIG_MODULE_HAVE_${_MOD} 0)
    endif()
endforeach()

# OS detection
if(WIN32)
    set(TB_CONFIG_OS "WINDOWS")
elseif(APPLE)
    if(IOS)
        set(TB_CONFIG_OS "IOS")
    else()
        set(TB_CONFIG_OS "MACOSX")
    endif()
elseif(ANDROID)
    set(TB_CONFIG_OS "ANDROID")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(TB_CONFIG_OS "LINUX")
elseif(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    set(TB_CONFIG_OS "BSD")
elseif(EMSCRIPTEN)
    set(TB_CONFIG_OS "WASM")
else()
    set(TB_CONFIG_OS "UNIX")
endif()

# Version components
set(TB_CONFIG_VERSION       "${PROJECT_VERSION}")
set(TB_CONFIG_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(TB_CONFIG_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(TB_CONFIG_VERSION_ALTER "${PROJECT_VERSION_PATCH}")
string(TIMESTAMP TB_CONFIG_VERSION_BUILD "%Y%m%d")

# ============================================================
# Run interface checks
# ============================================================
include(${CMAKE_CURRENT_LIST_DIR}/xmake/check_interfaces.cmake)

# ============================================================
# Generate tbox.config.h
# ============================================================
set(TBOX_CONFIG_DIR "${CMAKE_BINARY_DIR}/tbox_config")
file(MAKE_DIRECTORY "${TBOX_CONFIG_DIR}")
configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/src/tbox/tbox.config.h.cmake.in"
    "${TBOX_CONFIG_DIR}/tbox.config.h"
    @ONLY
)

# ============================================================
# Subdirectories
# ============================================================
add_subdirectory(src)
