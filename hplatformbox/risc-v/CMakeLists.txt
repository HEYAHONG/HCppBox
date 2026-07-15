add_custom_target(risc-v_rom)
add_dependencies(hplatformbox risc-v_rom)


#sim固件(一般固件)
file(GLOB RISC_V_ROM_SIMS sim/*)
foreach(RISC_V_ROM_SIM ${RISC_V_ROM_SIMS})
    if(IS_DIRECTORY ${RISC_V_ROM_SIM})
        file(RELATIVE_PATH RISC_V_ROM_SIM_NAME ${CMAKE_CURRENT_SOURCE_DIR}/sim ${RISC_V_ROM_SIM})
        if(EXISTS "${RISC_V_ROM_SIM}/rv32i/CMakeLists.txt")
            message(STATUS "Add RISC-V 32 SIM ROM ${RISC_V_ROM_SIM_NAME}")
            add_custom_target(risc-v_rom_sim_${RISC_V_ROM_SIM_NAME}_rv32i  ${CMAKE_COMMAND} -E make_directory risc-v_rom_sim_${RISC_V_ROM_SIM_NAME}_rv32i && ${CMAKE_COMMAND} -DHPLATFORMBOX_RISC_V_INSTALL_DIR="${HPLATFORMBOX_INSTALL_DIR}/risc-v/sim/"  -S ${RISC_V_ROM_SIM}/rv32i -B risc-v_rom_sim_${RISC_V_ROM_SIM_NAME}_rv32i && ${CMAKE_COMMAND} --build risc-v_rom_sim_${RISC_V_ROM_SIM_NAME}_rv32i -t install
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
            USES_TERMINAL)
            add_dependencies(risc-v_rom risc-v_rom_sim_${RISC_V_ROM_SIM_NAME}_rv32i)
        endif()
        if(EXISTS "${RISC_V_ROM_SIM}/rv64i/CMakeLists.txt")
            message(STATUS "Add RISC-V 64 SIM ROM ${RISC_V_ROM_SIM_NAME}")
            add_custom_target(risc-v_rom_sim_${RISC_V_ROM_SIM_NAME}_rv64i  ${CMAKE_COMMAND} -E make_directory risc-v_rom_sim_${RISC_V_ROM_SIM_NAME}_rv64i && ${CMAKE_COMMAND} -DHPLATFORMBOX_RISC_V_INSTALL_DIR="${HPLATFORMBOX_INSTALL_DIR}/risc-v/sim/"  -S ${RISC_V_ROM_SIM}/rv64i -B risc-v_rom_sim_${RISC_V_ROM_SIM_NAME}_rv64i && ${CMAKE_COMMAND} --build risc-v_rom_sim_${RISC_V_ROM_SIM_NAME}_rv64i -t install
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
            USES_TERMINAL)
            add_dependencies(risc-v_rom risc-v_rom_sim_${RISC_V_ROM_SIM_NAME}_rv64i)
        endif()
    endif()
endforeach()

#sim固件(zephyr固件)
#注意：需要使用west配置好zephyr,并使用west zephyr-export导出cmake软件包
option(BUILD_HPLATFORMBOX_RISCV_SIM_ZEPHYR "build hplatformbox risc-v sim(zephyr)" OFF)
if(BUILD_HPLATFORMBOX_RISCV_SIM_ZEPHYR)
file(GLOB RISC_V_ROM_SIMS sim/zephyr/*)
    foreach(RISC_V_ROM_SIM ${RISC_V_ROM_SIMS})
        if(IS_DIRECTORY ${RISC_V_ROM_SIM} AND EXISTS "${RISC_V_ROM_SIM}/CMakeLists.txt")
            file(RELATIVE_PATH RISC_V_ROM_SIM_NAME ${CMAKE_CURRENT_SOURCE_DIR}/sim/zephyr/ ${RISC_V_ROM_SIM})
            message(STATUS "Add RISC-V Zephyr SIM ROM ${RISC_V_ROM_SIM_NAME}")
            add_custom_target(risc-v_rom_sim_zephyr_${RISC_V_ROM_SIM_NAME}_riscv32  ${CMAKE_COMMAND} -E make_directory risc-v_rom_sim_zephyr_${RISC_V_ROM_SIM_NAME}_riscv32 && ${CMAKE_COMMAND} -DBOARD=qemu_riscv32 -DHPLATFORMBOX_RISC_V_INSTALL_DIR="${HPLATFORMBOX_INSTALL_DIR}/risc-v/sim/zephyr/${RISC_V_ROM_SIM_NAME}/riscv32"  -S ${RISC_V_ROM_SIM}/ -B risc-v_rom_sim_zephyr_${RISC_V_ROM_SIM_NAME}_riscv32 && ${CMAKE_COMMAND} --build risc-v_rom_sim_zephyr_${RISC_V_ROM_SIM_NAME}_riscv32 -t install
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
                USES_TERMINAL)
            add_dependencies(risc-v_rom risc-v_rom_sim_zephyr_${RISC_V_ROM_SIM_NAME}_riscv32)
            add_custom_target(risc-v_rom_sim_zephyr_${RISC_V_ROM_SIM_NAME}_riscv64  ${CMAKE_COMMAND} -E make_directory risc-v_rom_sim_zephyr_${RISC_V_ROM_SIM_NAME}_riscv64 && ${CMAKE_COMMAND} -DBOARD=qemu_riscv64 -DHPLATFORMBOX_RISC_V_INSTALL_DIR="${HPLATFORMBOX_INSTALL_DIR}/risc-v/sim/zephyr/${RISC_V_ROM_SIM_NAME}/riscv64"  -S ${RISC_V_ROM_SIM}/ -B risc-v_rom_sim_zephyr_${RISC_V_ROM_SIM_NAME}_riscv64 && ${CMAKE_COMMAND} --build risc-v_rom_sim_zephyr_${RISC_V_ROM_SIM_NAME}_riscv64 -t install
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
                USES_TERMINAL)
            add_dependencies(risc-v_rom risc-v_rom_sim_zephyr_${RISC_V_ROM_SIM_NAME}_riscv64)
        endif()
    endforeach()
endif()
