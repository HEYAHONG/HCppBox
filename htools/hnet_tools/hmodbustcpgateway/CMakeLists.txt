cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME hmodbustcpgateway)
project(${PROJECT_NAME}
	VERSION 1.0.0.0
	LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)
if(WIN32)
    if(MSVC)
        add_compile_options(-utf-8)
    endif()
endif()


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../common/3rdparty/astyle/ ${CMAKE_CURRENT_SOURCE_DIR}/ )
file(GLOB C_CXX_FILES *.h *.cpp *.c *.hpp *.cxx
)
add_executable(${PROJECT_NAME}  ${C_CXX_FILES})
#添加线程库
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME}  ${CMAKE_THREAD_LIBS_INIT})

check_include_file(syslog.h HAVE_SYSLOG_H)
if(HAVE_SYSLOG_H)
target_compile_definitions(${PROJECT_NAME}  PUBLIC  HAVE_SYSLOG_H=1)
endif()


#设定HCppBox使用纯C++
set(HCPPBOX_USE_PURE_CPLUSPLUS ON)

#设定HRC文件系统目录为fs
set(HRC_FS_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/fs/)
#添加3rdparty/HCppBox
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../  HCppBox EXCLUDE_FROM_ALL)
#启用HCppBox、HRC
hcppbox_enable(${PROJECT_NAME})
hrc_enable(${PROJECT_NAME})

include(FindPkgConfig)
pkg_check_modules(LIBSERIALPORT QUIET IMPORTED_TARGET libserialport)
target_link_libraries(${PROJECT_NAME}  PkgConfig::LIBSERIALPORT)
pkg_check_modules(LIBEVENT QUIET IMPORTED_TARGET libevent)
target_link_libraries(${PROJECT_NAME}  PkgConfig::LIBEVENT)
pkg_check_modules(LIBEVENT_CORE QUIET IMPORTED_TARGET libevent_core)
target_link_libraries(${PROJECT_NAME}  PkgConfig::LIBEVENT_CORE)
pkg_check_modules(LIBEVENT_EXTRA QUIET IMPORTED_TARGET libevent_extra)
target_link_libraries(${PROJECT_NAME}  PkgConfig::LIBEVENT_EXTRA)


