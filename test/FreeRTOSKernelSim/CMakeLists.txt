
cmake_minimum_required(VERSION 3.14)


#添加可执行文件
add_executable(FreeRTOSKernelSim)

target_compile_definitions(FreeRTOSKernelSim PUBLIC -DFREERTOS_KERNEL=1)

if(WIN32)
	if(MSVC)
		add_compile_options(-utf-8)
	endif()
endif()


#设置C++标准
set_property(TARGET FreeRTOSKernelSim PROPERTY CXX_STANDARD 23)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)

#启用HCppBox
hcppbox_enable(FreeRTOSKernelSim)


#启用HRC
hrc_enable(FreeRTOSKernelSim)


#添加源代码
file(GLOB  FREERTOS_KERNEL_SIM_CPP_C_FILES *.cpp *.CPP *.c *.C *.h ../../hbox/h3rdparty/h3rdparty_freertos_kernel*.c)
target_sources(FreeRTOSKernelSim PUBLIC ${FREERTOS_KERNEL_SIM_CPP_C_FILES})

if(WIN32)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/win32)
file(GLOB  FREERTOS_PORT_SOURCE win32/*.c win32/*.s)
target_sources(FreeRTOSKernelSim PUBLIC ${FREERTOS_PORT_SOURCE})
target_link_libraries(FreeRTOSKernelSim winmm)
else()
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/posix)
file(GLOB  FREERTOS_PORT_SOURCE posix/*.c posix/*.s posix/utils/*.c)
target_sources(FreeRTOSKernelSim PUBLIC ${FREERTOS_PORT_SOURCE})
endif()
