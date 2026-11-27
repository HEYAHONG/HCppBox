
cmake_minimum_required(VERSION 3.25)

if(POLICY CMP0079)
    cmake_policy(SET CMP0079 NEW)
endif()

if(HALIBC_ENABLE_POSIX)
    message(STATUS "halibc: posix enable!")

    if(HALIBC_POSIX_SOCKET)
        add_subdirectory(socket)
    endif()

else()
    message(STATUS "halibc: posix disabled!")
endif()
