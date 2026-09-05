
cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME hmini-rv32imav2)
project(${PROJECT_NAME}
    VERSION 1.0.0.0
    LANGUAGES C CXX)


if(WIN32)
if(MSVC)
add_compile_options(-utf-8)
endif()
endif()


file(GLOB C_CXX_FILES *.h *.cpp *.c *.hpp *.cxx
    )
add_executable(${PROJECT_NAME}  ${C_CXX_FILES})
#添加线程库
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME}  ${CMAKE_THREAD_LIBS_INIT})


#设定HCppBox使用纯C++
set(HCPPBOX_USE_PURE_CPLUSPLUS ON)

#设定HRC文件系统目录为fs
set(HRC_FS_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/fs/)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../common/  common)

#启用HCppBox、HRC
hcppbox_enable(${PROJECT_NAME})
hrc_enable(${PROJECT_NAME})

#启用hopenblt
target_link_libraries(${PROJECT_NAME} hopenblt)
target_compile_definitions(hopenblt PUBLIC 
-DBOOT_COM_NET_ENABLE=1
-DBOOT_NVM_SIZE_KB=65536
-DBOOT_COM_NET_DEFERRED_INIT_ENABLE=0
-DHOPENBLT_NVM_NO_IMPLEMENTATION=1
-DHOPENBLT_NET_PATCH_HEADER=1
-DHOPENBLT_NET_NO_IMPLEMENTATION=1
-DBOOT_COM_NET_PORT=51001
-DHOPENBLT_CPU_FUNCTION_ATTRIBUTE=
-DHOPENBLT_TIMER_FUNCTION_ATTRIBUTE=
)


include(GNUInstallDirs)
if(NOT DEFINED HTOOLS_INSTALL_DIR)
    set(HTOOLS_INSTALL_DIR ${CMAKE_INSTALL_PREFIX})
endif()
if(DEFINED HTOOLS_INSTALL_DIR)
install(TARGETS ${PROJECT_NAME}
       RUNTIME DESTINATION ${HTOOLS_INSTALL_DIR}/bin
       LIBRARY DESTINATION ${HTOOLS_INSTALL_DIR}/lib
       ARCHIVE DESTINATION ${HTOOLS_INSTALL_DIR}/lib
       )
endif()

