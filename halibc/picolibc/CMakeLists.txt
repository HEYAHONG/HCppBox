
cmake_minimum_required(VERSION 3.25)

if(POLICY CMP0079)
    cmake_policy(SET CMP0079 NEW)
endif()

if(HALIBC_ENABLE_PICOLIBC)
    message(STATUS "halibc: picolibc enable!")

    if(HALIBC_PICOLIBC_MOXIEBOX)
        include(${CMAKE_CURRENT_SOURCE_DIR}/port/moxiebox/moxiebox.cmake)
    endif()

    add_subdirectory(master)
    add_subdirectory(port)
else()
    message(STATUS "halibc: picolibc disabled!")
endif()
