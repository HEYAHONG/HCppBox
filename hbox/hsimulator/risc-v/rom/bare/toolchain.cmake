cmake_minimum_required(VERSION 3.20)
#riscv64-unknown-elf-gcc 工具链设置
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER riscv64-unknown-elf-gcc)
set(CMAKE_ASM_COMPILER riscv64-unknown-elf-gcc)
set(CMAKE_CXX_COMPILER riscv64-unknown-elf-g++)
set(CMAKE_ADDR2LINE riscv64-unknown-elf-addr2line)
set(CMAKE_AR riscv64-unknown-elf-ar)
set(CMAKE_RANLIB riscv64-unknown-elf-ranlib)
set(CMAKE_CXX_COMPILER_AR riscv64-unknown-elf-ar)
set(CMAKE_CXX_COMPILER_RANLIB riscv64-unknown-elf-ranlib)
set(CMAKE_C_COMPILER_AR riscv64-unknown-elf-ar)
set(CMAKE_C_COMPILER_RANLIB riscv64-unknown-elf-ranlib)
set(CMAKE_Fortran_COMPILER riscv64-unknown-elf-gfortan)
set(CMAKE_OBJCOPY riscv64-unknown-elf-objcopy)
set(CMAKE_OBJDUMP riscv64-unknown-elf-objdump)
set(CMAKE_RC_COMPILER riscv64-unknown-elf-windres)
set(CMAKE_READELF riscv64-unknown-elf-readelf)
set(CMAKE_SIZE  riscv64-unknown-elf-size)
set(CMAKE_STRIP riscv64-unknown-elf-strip)
set(CMAKE_LINKER riscv64-unknown-elf-ld)
set(CMAKE_EXECUTABLE_SUFFIX_ASM .elf)
set(CMAKE_EXECUTABLE_SUFFIX_C .elf)
set(CMAKE_EXECUTABLE_SUFFIX_CXX .elf)
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)
execute_process(	COMMAND  ${CMAKE_C_COMPILER} --version
					OUTPUT_QUIET
					COMMAND_ERROR_IS_FATAL ANY
				)
execute_process(    COMMAND  ${CMAKE_C_COMPILER} -specs=picolibcpp.specs --version
					RESULT_VARIABLE	PICOLIBCPP_SPECS_TEST_CODE
                    OUTPUT_QUIET
                    ERROR_QUIET
                )
if("${PICOLIBCPP_SPECS_TEST_CODE}" EQUAL "0")
	message(STATUS "libc using picolibc!")
	set(PICOLIBC TRUE)
else()
	execute_process(    COMMAND  ${CMAKE_C_COMPILER} -specs=nano.specs --version
                    RESULT_VARIABLE NANO_SPECS_TEST_CODE
                    OUTPUT_QUIET
                    ERROR_QUIET
                )
	if("${NANO_SPECS_TEST_CODE}" EQUAL "0")
		message(STATUS "libc using newlib!")
		set(NEWLIB TRUE)
	else()
		message(FATAL_ERROR "libc not support!")
	endif()
endif()
