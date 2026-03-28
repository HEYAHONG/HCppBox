cmake_minimum_required(VERSION 3.16)



if(MSVC)
    add_compile_options(-utf-8 )
endif()

add_library(HBox STATIC EXCLUDE_FROM_ALL)
file(GLOB  HBOX_C_CPP_FILES *.cpp *.hpp *.c *.h cpp/*.cpp cpp/*.hpp cpp/*.c cpp/*.h h3rdparty/*.cpp h3rdparty/*.hpp h3rdparty/*.c h3rdparty/*.h )
target_sources(HBox PRIVATE ${HBOX_C_CPP_FILES})
target_include_directories(HBox PUBLIC ./ )
target_include_directories(HBox PUBLIC ./cpp/ )


if(MSVC)
    target_compile_definitions(HBox PRIVATE _CRT_SECURE_NO_WARNINGS=1 )
    add_compile_options(/experimental:c11atomics )
endif()

#设置属性
set_property(TARGET HBox PROPERTY CXX_STANDARD 11)
#虽然最低支持C99，但此处仍然设置为C11
set_property(TARGET HBox PROPERTY C_STANDARD 11)

#启用HBox
function(hbox_enable targetname)
target_link_libraries(${targetname} HBox)
target_compile_definitions(${targetname} PUBLIC  HBOX_ENABLE=1)
endfunction()

include(CheckIncludeFile)
#查找libdl库
check_include_file(dlfcn.h HAVE_DLFCN_H)
if(HAVE_DLFCN_H)
target_link_libraries(HBox dl)
target_compile_definitions(HBox PUBLIC  HAVE_DLFCN_H=1)
endif()



#查找sys/mman.h
check_include_file(sys/mman.h HAVE_SYS_MMAN_H)
if(HAVE_SYS_MMAN_H)
target_compile_definitions(HBox PUBLIC  HAVE_SYS_MMAN_H=1)
endif()

#查找sys/ioctl.h
check_include_file(sys/ioctl.h HAVE_SYS_IOCTL_H)
if(HAVE_SYS_IOCTL_H)
target_compile_definitions(HBox PUBLIC  HAVE_SYS_IOCTL_H=1)
endif()


#查找unistd.h
check_include_file(unistd.h HAVE_UNISTD_H)
if(HAVE_UNISTD_H)
target_compile_definitions(HBox PUBLIC  HAVE_UNISTD_H=1)
endif()

#查找fcntl.h
check_include_file(fcntl.h HAVE_FCNTL_H)
if(HAVE_FCNTL_H)
target_compile_definitions(HBox PUBLIC  HAVE_FCNTL_H=1)
endif()

#查找threads.h
check_include_file(threads.h HAVE_THREADS_H)
if(HAVE_THREADS_H)
target_compile_definitions(HBox PUBLIC  HAVE_THREADS_H=1)

#查找threads库(c11)
find_package(Threads)
if(Threads_FOUND)
target_link_libraries(HBox Threads::Threads)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "FreeBSD")
target_link_libraries(HBox stdthreads)
endif()

endif()

endif()

#查找stdatomic.h
check_include_file(stdatomic.h HAVE_STDATOMIC_H)
if(HAVE_STDATOMIC_H)
target_compile_definitions(HBox PUBLIC  HAVE_STDATOMIC_H=1)
endif()

include(CheckSourceCompiles)

#thread_local
set(CMAKE_C_STANDARD 11)
if(HAVE_THREADS_H)
check_source_compiles(C [[
	#include "threads.h"
	thread_local int a=0;
	int main()
	{
		return 0;
	}
]] HAVE_C_THREAD_LOCAL)
else()
check_source_compiles(C [[
	#include "stdlib.h"
	#if defined(__STDC_VERSION__) && ((__STDC_VERSION__) < 202311L) && !defined(thread_local)
	#define thread_local _Thread_local
	#endif
	thread_local int a=0;
	int main()
	{
		return 0;
	}
]] HAVE_C_THREAD_LOCAL)
endif()
if(HAVE_C_THREAD_LOCAL)
target_compile_definitions(HBox PUBLIC  HAVE_C_THREAD_LOCAL=1)
endif()

set(CMAKE_CXX_STANDARD  11)
check_source_compiles(CXX [[
	#include "thread"
 	thread_local int a=0;
	int main()
	{
		return 0;
	}
]] HAVE_CXX_THREAD_LOCAL)

if(HAVE_CXX_THREAD_LOCAL)
target_compile_definitions(HBox PUBLIC  HAVE_CXX_THREAD_LOCAL=1)
endif()


#_Atomic
set(CMAKE_C_STANDARD 11)
if(HAVE_STDATOMIC_H)
check_source_compiles(C [[
	#include "stdatomic.h"
 	_Atomic int a=0;
	int main()
	{
		return 0;
	}
]] HAVE_C_ATOMIC)
else()
check_source_compiles(C [[
	#include "stdlib.h"
 	_Atomic int a=0;
	int main()
	{
		return 0;
	}
]] HAVE_C_ATOMIC)
endif()

if(HAVE_C_ATOMIC)
target_compile_definitions(HBox PUBLIC  HAVE_C_ATOMIC=1)
endif()

set(CMAKE_CXX_STANDARD  11)
check_source_compiles(CXX [[
	#include "atomic"
 	std::atomic<int> a;
	int main()
	{
		a=0;
		return 0;
	}
]] HAVE_CXX_ATOMIC)

if(HAVE_CXX_ATOMIC)
target_compile_definitions(HBox PUBLIC  HAVE_CXX_ATOMIC=1)
endif()





