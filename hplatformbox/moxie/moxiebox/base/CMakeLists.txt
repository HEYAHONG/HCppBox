cmake_minimum_required(VERSION 3.25)

include("${CMAKE_CURRENT_SOURCE_DIR}/../../../../halibc/picolibc/port/moxiebox/hello-world/toolchain.cmake")

project(base LANGUAGES C CXX ASM)

set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../..")
set(PICOLIBC_DIR "${ROOT_DIR}/halibc/picolibc")
set(HBOX_DIR "${ROOT_DIR}/hbox")

set(HALIBC_ENABLE_PICOLIBC ON CACHE BOOL "" FORCE)
set(HALIBC_PICOLIBC_MOXIEBOX ON CACHE BOOL "" FORCE)
set(HALIBC_ENABLE_POSIX ON CACHE BOOL "" FORCE)
set(HALIBC_POSIX_SOCKET ON CACHE BOOL "" FORCE)
set(HALIBC_POSIX_SOCKET_DISABLE_FALLBACK OFF CACHE BOOL "" FORCE)

add_subdirectory(${PICOLIBC_DIR} picolibc EXCLUDE_FROM_ALL)
add_subdirectory(${ROOT_DIR}/halibc/posix posix EXCLUDE_FROM_ALL)

get_target_property(PICOLIBC_INCLUDE_DIRECTORIES c INTERFACE_INCLUDE_DIRECTORIES)

set(CMAKE_REQUIRED_INCLUDES ${PICOLIBC_INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${PICOLIBC_INCLUDE_DIRECTORIES})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_compile_options(
    -ffreestanding
    -fno-stack-protector
    )

add_compile_definitions(
    __STDC_NO_ATOMICS__=1
    __STDC_NO_THREADS__=1
    HDEFAULTS_LIBC_OPTIMIZE_LEVEL=0
    )

add_definitions(
    -DHBOX_CONFIG_HEADER=hbox_config.h
    )

set(HAVE_DLFCN_H 0 CACHE INTERNAL "" FORCE)
set(HAVE_C_ATOMIC 0 CACHE INTERNAL "" FORCE)
set(HAVE_CXX_ATOMIC 0 CACHE INTERNAL "" FORCE)

add_subdirectory(${HBOX_DIR} HBox EXCLUDE_FROM_ALL)

set(HRC_FS_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/fs)
add_subdirectory(${ROOT_DIR}/hrc HRC EXCLUDE_FROM_ALL)

add_executable(${PROJECT_NAME}
    main.cpp
    shell.cpp
    hbox_config.c
    port.c
    )

hbox_enable(${PROJECT_NAME})
hrc_enable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${HBOX_DIR}/h3rdparty/3rdparty/FreeRTOS/Kernel)

target_include_directories(${PROJECT_NAME} BEFORE PRIVATE
    ${ROOT_DIR}/halibc/posix/socket/include)

target_link_libraries(${PROJECT_NAME}
    "-Wl,--start-group"
    HBox
    h3rdparty_freertos_kernel
    h3rdparty_printf
    h3rdparty_common
    picolibc_c_port
    halibc_posix_socket
    c
    stdc++
    supc++
    "-Wl,--end-group"
    gcc)

target_link_options(${PROJECT_NAME} PRIVATE
    -nostdlib
    -nostartfiles
    -T${PICOLIBC_DIR}/port/moxiebox/moxiebox.ld
    -Wl,-e,_start
    )

set_target_properties(${PROJECT_NAME} PROPERTIES
    SUFFIX ".elf")

if(DEFINED HPLATFORMBOX_MOXIE_INSTALL_DIR)
    file(RELATIVE_PATH MOXIE_INSTALL_SUBDIR ${CMAKE_CURRENT_SOURCE_DIR}/../.. ${CMAKE_CURRENT_SOURCE_DIR})
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.elf
        DESTINATION ${HPLATFORMBOX_MOXIE_INSTALL_DIR}/${MOXIE_INSTALL_SUBDIR})
endif()
