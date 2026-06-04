
add_custom_target(mcs-51_rom)
add_dependencies(hplatformbox mcs-51_rom)

#sim固件
file(GLOB MCS_51_ROM_SIMS sim/*)
foreach(MCS_51_ROM_SIM ${MCS_51_ROM_SIMS})
    if(IS_DIRECTORY ${MCS_51_ROM_SIM})
        if(EXISTS "${MCS_51_ROM_SIM}/CMakeLists.txt")
            file(RELATIVE_PATH MCS_51_ROM_SIM_NAME ${CMAKE_CURRENT_SOURCE_DIR}/sim ${MCS_51_ROM_SIM})
            message(STATUS "Add MCS-51 SIM ROM ${MCS_51_ROM_SIM_NAME}")
            add_custom_target(mcs-51_rom_sim_${MCS_51_ROM_SIM_NAME}  ${CMAKE_COMMAND} -E make_directory mcs-51_rom_sim_${MCS_51_ROM_SIM_NAME} && ${CMAKE_COMMAND} -DHPLATFORMBOX_MCS_51_INSTALL_DIR="${HPLATFORMBOX_INSTALL_DIR}/mcs-51/sim/"  -S ${MCS_51_ROM_SIM}/ -B mcs-51_rom_sim_${MCS_51_ROM_SIM_NAME} && ${CMAKE_COMMAND} --build mcs-51_rom_sim_${MCS_51_ROM_SIM_NAME} -t install
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
            USES_TERMINAL)
            add_dependencies(mcs-51_rom mcs-51_rom_sim_${MCS_51_ROM_SIM_NAME})
        endif()
    endif()
endforeach()
