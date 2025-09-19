cmake_minimum_required(VERSION 3.20)
include("${CMAKE_CURRENT_SOURCE_DIR}/../toolchain.cmake")
set(PROJECT_NAME rom)
project(${PROJECT_NAME} C CXX ASM)

file(GLOB C_CXX_FILES ../*.h ../*.c ../*.cpp ../*.s *.h *.c *.cpp)

if(${PICOLIBC})
set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} \
	-march=rv64i_zicsr\
    -mabi=lp64\
    -mcmodel=medany \
    -std=gnu11\
    -fPIC\
    -ffunction-sections\
    -fdata-sections\
    --specs=picolibcpp.specs\
")
set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} \
    -march=rv64i_zicsr\
    -mabi=lp64\
    -mcmodel=medany \
    -std=gnu++11\
    -fPIC\
    -ffunction-sections\
    -fdata-sections\
    --specs=picolibcpp.specs\
")
set(CMAKE_EXE_LINKER_FLAGS " ${CMAKE_EXE_LINKER_FLAGS}\
    -march=rv64i_zicsr\
    -mabi=lp64\
    -mcmodel=medany \
    -nostdlib++\
    -T${CMAKE_BINARY_DIR}/rom.ld\
    -Wl,--gc-sections \
    -Wl,--no-relax   \
    -Wl,-Map,${PROJECT_NAME}.map\
")
endif()

if(${NEWLIB})
set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} \
    -march=rv64i_zicsr\
    -mabi=lp64\
    -mcmodel=medany \
	-std=gnu11\
    -fPIC\
    -ffunction-sections\
    -fdata-sections\
    --specs=nano.specs\
")
set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} \
    -march=rv64i_zicsr\
    -mabi=lp64\
    -mcmodel=medany \
    -std=gnu++11\
    -fPIC\
    -ffunction-sections\
    -fdata-sections\
    --specs=nano.specs\
")
set(CMAKE_EXE_LINKER_FLAGS " ${CMAKE_EXE_LINKER_FLAGS}\
    -march=rv64i_zicsr\
    -mabi=lp64\
    -mcmodel=medany \
    -nostdlib++\
    -T${CMAKE_BINARY_DIR}/rom.ld\
    -Wl,--gc-sections \
    -Wl,--no-relax   \
    -Wl,-Map,${PROJECT_NAME}.map\
")
endif()

set(CMAKE_ASM_FLAGS ${CMAKE_C_FLAGS})

add_custom_target(link_script
COMMAND ${CMAKE_CXX_COMPILER} -E -march=rv64i_zicsr -mabi=lp64  -o ${CMAKE_BINARY_DIR}/rom.ld ${CMAKE_CURRENT_SOURCE_DIR}/../rom.ldS.h
)


add_definitions(
-DHBOX_CONFIG_HEADER=hbox_config.h
)

add_executable(${PROJECT_NAME} ${C_CXX_FILES})

include_directories(
${CMAKE_CURRENT_SOURCE_DIR}/
${CMAKE_CURRENT_SOURCE_DIR}/../
)

#设定HRC文件系统目录为fs
set(HRC_FS_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../fs/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../hbox HBox EXCLUDE_FROM_ALL)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../hrc HRC EXCLUDE_FROM_ALL)
#启用HBox、HRC
hbox_enable(${PROJECT_NAME})
hrc_enable(${PROJECT_NAME})

add_dependencies(${PROJECT_NAME} link_script)

add_custom_command(TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND  ${CMAKE_SIZE} ${PROJECT_NAME}.elf
	COMMAND  ${CMAKE_OBJCOPY} --verbose -O binary ${PROJECT_NAME}.elf  ${PROJECT_NAME}.bin
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
