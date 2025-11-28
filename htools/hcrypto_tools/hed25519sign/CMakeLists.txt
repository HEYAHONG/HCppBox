cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME hed25519sign)
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
#添加HCppBox
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../common/  common EXCLUDE_FROM_ALL)

#hed25519sign
add_executable(hed25519sign  hed25519sign.cpp)
hcppbox_enable(hed25519sign)
hrc_enable(hed25519sign)
target_link_libraries(hed25519sign  ${CMAKE_THREAD_LIBS_INIT})

include(GNUInstallDirs)
if(DEFINED HTOOLS_INSTALL_DIR)
install(TARGETS hed25519sign
       RUNTIME DESTINATION ${HTOOLS_INSTALL_DIR}/bin
       LIBRARY DESTINATION ${HTOOLS_INSTALL_DIR}/lib
       ARCHIVE DESTINATION ${HTOOLS_INSTALL_DIR}/lib
       )
endif()
