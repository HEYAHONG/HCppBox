add_custom_target(risc-v_rom)
add_dependencies(hplatformbox risc-v_rom)


#sim固件
file(GLOB RISC_V_ROM_SIMS sim/*)
foreach(RISC_V_ROM_SIM ${RISC_V_ROM_SIMS})
    if(IS_DIRECTORY ${RISC_V_ROM_SIM})
        file(RELATIVE_PATH RISC_V_ROM_SIM_NAME ${CMAKE_CURRENT_SOURCE_DIR}/sim ${RISC_V_ROM_SIM})
        message(STATUS "Add RISC-V SIM ROM ${RISC_V_ROM_SIM_NAME}")
        if(EXISTS "${RISC_V_ROM_SIM}/rv32i/CMakeLists.txt")
            add_custom_target(risc-v_rom_sim_${RISC_V_ROM_SIM_NAME}_rv32i  ${CMAKE_COMMAND} -E make_directory risc-v_rom_sim_${RISC_V_ROM_SIM_NAME}_rv32i && ${CMAKE_COMMAND} -DHPLATFORMBOX_RISC_V_INSTALL_DIR="${HPLATFORMBOX_INSTALL_DIR}/risc-v/sim/"  -S ${RISC_V_ROM_SIM}/rv32i -B risc-v_rom_sim_${RISC_V_ROM_SIM_NAME}_rv32i && ${CMAKE_COMMAND} --build risc-v_rom_sim_${RISC_V_ROM_SIM_NAME}_rv32i -t install
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
            USES_TERMINAL)
            add_dependencies(risc-v_rom risc-v_rom_sim_${RISC_V_ROM_SIM_NAME}_rv32i)
        endif()
        if(EXISTS "${RISC_V_ROM_SIM}/rv64i/CMakeLists.txt")
            add_custom_target(risc-v_rom_sim_${RISC_V_ROM_SIM_NAME}_rv64i  ${CMAKE_COMMAND} -E make_directory risc-v_rom_sim_${RISC_V_ROM_SIM_NAME}_rv64i && ${CMAKE_COMMAND} -DHPLATFORMBOX_RISC_V_INSTALL_DIR="${HPLATFORMBOX_INSTALL_DIR}/risc-v/sim/"  -S ${RISC_V_ROM_SIM}/rv64i -B risc-v_rom_sim_${RISC_V_ROM_SIM_NAME}_rv64i && ${CMAKE_COMMAND} --build risc-v_rom_sim_${RISC_V_ROM_SIM_NAME}_rv64i -t install
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
            USES_TERMINAL)
            add_dependencies(risc-v_rom risc-v_rom_sim_${RISC_V_ROM_SIM_NAME}_rv64i)
        endif()
    endif()
endforeach()
