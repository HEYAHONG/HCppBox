cmake_minimum_required(VERSION 3.20)
if(POLICY CMP0079 )
  cmake_policy(SET CMP0079 NEW)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/hbox")
add_subdirectory(hbox)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/hcppbox")
add_subdirectory(hcppbox)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/hrc")
add_subdirectory(hrc)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/htools")
add_subdirectory(htools EXCLUDE_FROM_ALL)
endif()


if(TARGET HCppBox)
set_target_properties(HCppBox
			PROPERTIES HCPPBOX_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}
			)
endif()

