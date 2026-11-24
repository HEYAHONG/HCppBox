add_custom_target(moxie_rom)
add_dependencies(hplatformbox moxie_rom)

find_program(MOXIE_UNKNOWN_ELF_GCC moxie-unknown-moxiebox-gcc moxie-unknown-elf-gcc )
if(MOXIE_UNKNOWN_ELF_GCC)
    message(STATUS "moxie-unknown-elf-gcc:" ${MOXIE_UNKNOWN_ELF_GCC})
    message(STATUS "Enable hplatformbox/moxie")
else()
    message(STATUS "moxie-unknown-elf-gcc is not found!")
endif()

if(MOXIE_UNKNOWN_ELF_GCC)
    file(GLOB MOXIE_ROM_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/moxiebox/*)
    foreach(MOXIE_ROM_DIR ${MOXIE_ROM_DIRS})
        if(IS_DIRECTORY ${MOXIE_ROM_DIR} AND EXISTS "${MOXIE_ROM_DIR}/CMakeLists.txt")
            file(RELATIVE_PATH MOXIE_ROM_NAME ${CMAKE_CURRENT_SOURCE_DIR}/moxiebox ${MOXIE_ROM_DIR})
            string(REPLACE "/" "_" MOXIE_ROM_TARGET ${MOXIE_ROM_NAME})
            message(STATUS "Add moxie ROM ${MOXIE_ROM_NAME}")
            add_custom_target(moxie_rom_${MOXIE_ROM_TARGET}
                ${CMAKE_COMMAND} -E make_directory moxie_rom_${MOXIE_ROM_TARGET}
                && ${CMAKE_COMMAND} -DHPLATFORMBOX_MOXIE_INSTALL_DIR="${HPLATFORMBOX_INSTALL_DIR}/moxie/" -S ${MOXIE_ROM_DIR} -B moxie_rom_${MOXIE_ROM_TARGET}
                && ${CMAKE_COMMAND} --build moxie_rom_${MOXIE_ROM_TARGET} -t install
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
                USES_TERMINAL)
            add_dependencies(moxie_rom moxie_rom_${MOXIE_ROM_TARGET})
        endif()
    endforeach()
endif()
