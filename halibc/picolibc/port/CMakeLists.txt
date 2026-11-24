
add_library(picolibc_c_port STATIC)

file(GLOB PICOLIBC_C_PORT_COMMON_FILES *.c *.h)
target_sources(picolibc_c_port PRIVATE ${PICOLIBC_C_PORT_COMMON_FILES})

if(HALIBC_PICOLIBC_MOXIEBOX)
    add_subdirectory(moxiebox)
endif()

target_link_libraries(c PRIVATE picolibc_c_port)
