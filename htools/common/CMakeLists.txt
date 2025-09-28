cmake_minimum_required(VERSION 3.20)
if(POLICY CMP0079 )
  cmake_policy(SET CMP0079 NEW)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../hbox hbox)


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../hcppbox hcppbox)


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../hrc hrc)

add_library(FreeRTOSPort)

if(WIN32 OR CYGWIN)
target_include_directories(FreeRTOSPort PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/)
target_include_directories(FreeRTOSPort PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/win32/)
target_include_directories(FreeRTOSPort PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../hbox/)
target_include_directories(FreeRTOSPort PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../hbox/cpp/)
file(GLOB  FREERTOS_PORT_SOURCE FreeRTOS/win32/*.c FreeRTOS/win32/*.s)
target_sources(FreeRTOSKernelSim PUBLIC ${FREERTOS_PORT_SOURCE})
target_link_libraries(FreeRTOSPort winmm)
else()
target_include_directories(FreeRTOSPort PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/)
target_include_directories(FreeRTOSPort PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/posix/)
target_include_directories(FreeRTOSPort PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../hbox/)
target_include_directories(FreeRTOSPort PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../hbox/cpp/)
file(GLOB  FREERTOS_PORT_SOURCE FreeRTOS/posix/*.c FreeRTOS/posix/*.s FreeRTOS/posix/utils/*.c)
target_sources(FreeRTOSPort PUBLIC ${FREERTOS_PORT_SOURCE})
endif()
target_compile_definitions(FreeRTOSPort PUBLIC -DFREERTOS_KERNEL=1 )

#HBox设置
if(WIN32 OR CYGWIN)
target_include_directories(HBox PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/win32/)
else()
target_include_directories(HBox PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/posix/)
endif()
target_include_directories(HBox PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/)
target_compile_definitions(HBox PUBLIC -DFREERTOS_KERNEL=1 )
target_link_libraries(HBox FreeRTOSPort)


