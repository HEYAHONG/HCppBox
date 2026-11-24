
if(HALIBC_ENABLE_PICOLIBC)
    message(STATUS "halibc: picolibc enable!")
    add_subdirectory(master)
else()
    message(STATUS "halibc: picolibc disabled!")
endif()
