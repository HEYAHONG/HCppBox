cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME hashtool)
project(${PROJECT_NAME}
	VERSION 1.0.0.0
	LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

if(WIN32)
    if(MSVC)
        add_compile_options(-utf-8)
    endif()
endif()


#添加线程库
find_package(Threads REQUIRED)

#设定HCppBox使用纯C++
set(HCPPBOX_USE_PURE_CPLUSPLUS ON)

#设定HRC文件系统目录为fs
set(HRC_FS_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/fs/)
#添加3rdparty/HCppBox
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../  HCppBox EXCLUDE_FROM_ALL)

#hmd4sum
add_executable(hmd4sum  hmd4sum.cpp)
hcppbox_enable(hmd4sum)
hrc_enable(hmd4sum)
target_link_libraries(hmd4sum  ${CMAKE_THREAD_LIBS_INIT})

#hmd5sum
add_executable(hmd5sum  hmd5sum.cpp)
hcppbox_enable(hmd5sum)
hrc_enable(hmd5sum)
target_link_libraries(hmd5sum  ${CMAKE_THREAD_LIBS_INIT})

#hsha1sum
add_executable(hsha1sum  hsha1sum.cpp)
hcppbox_enable(hsha1sum)
hrc_enable(hsha1sum)
target_link_libraries(hsha1sum  ${CMAKE_THREAD_LIBS_INIT})

#hsha224sum
add_executable(hsha224sum  hsha224sum.cpp)
hcppbox_enable(hsha224sum)
hrc_enable(hsha224sum)
target_link_libraries(hsha224sum  ${CMAKE_THREAD_LIBS_INIT})

#hsha256sum
add_executable(hsha256sum  hsha256sum.cpp)
hcppbox_enable(hsha256sum)
hrc_enable(hsha256sum)
target_link_libraries(hsha256sum  ${CMAKE_THREAD_LIBS_INIT})

#hsha384sum
add_executable(hsha384sum  hsha384sum.cpp)
hcppbox_enable(hsha384sum)
hrc_enable(hsha384sum)
target_link_libraries(hsha384sum  ${CMAKE_THREAD_LIBS_INIT})

#hsha512sum
add_executable(hsha512sum  hsha512sum.cpp)
hcppbox_enable(hsha512sum)
hrc_enable(hsha512sum)
target_link_libraries(hsha512sum  ${CMAKE_THREAD_LIBS_INIT})

#hsha3sum224
add_executable(hsha3sum224  hsha3sum224.cpp)
hcppbox_enable(hsha3sum224)
hrc_enable(hsha3sum224)
target_link_libraries(hsha3sum224  ${CMAKE_THREAD_LIBS_INIT})

#hsha3sum256
add_executable(hsha3sum256  hsha3sum256.cpp)
hcppbox_enable(hsha3sum256)
hrc_enable(hsha3sum256)
target_link_libraries(hsha3sum256  ${CMAKE_THREAD_LIBS_INIT})

#hsha3sum384
add_executable(hsha3sum384  hsha3sum384.cpp)
hcppbox_enable(hsha3sum384)
hrc_enable(hsha3sum384)
target_link_libraries(hsha3sum384  ${CMAKE_THREAD_LIBS_INIT})

#hsha3sum512
add_executable(hsha3sum512  hsha3sum512.cpp)
hcppbox_enable(hsha3sum512)
hrc_enable(hsha3sum512)
target_link_libraries(hsha3sum512  ${CMAKE_THREAD_LIBS_INIT})

#hripemd160sum
add_executable(hripemd160sum  hripemd160sum.cpp)
hcppbox_enable(hripemd160sum)
hrc_enable(hripemd160sum)
target_link_libraries(hripemd160sum  ${CMAKE_THREAD_LIBS_INIT})

#hsm3sum
add_executable(hsm3sum  hsm3sum.cpp)
hcppbox_enable(hsm3sum)
hrc_enable(hsm3sum)
target_link_libraries(hsm3sum  ${CMAKE_THREAD_LIBS_INIT})
