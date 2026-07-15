
if(NOT DEFINED HPLATFORMBOX_INSTALL_DIR)
    set(HPLATFORMBOX_INSTALL_DIR ${CMAKE_BINARY_DIR}/hplatformbox)
endif()

add_custom_target(hplatformbox)


find_program(SDCC sdcc)
if(SDCC)
    message(STATUS "sdcc:" ${SDCC})
    message(STATUS "Enable hplatformbox/mcs-51")
    add_subdirectory(mcs-51)
else()
    message(STATUS "sdcc is not found!")
endif()


add_subdirectory(risc-v)


