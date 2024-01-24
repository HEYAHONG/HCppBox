cmake_minimum_required(VERSION 3.20)
if(POLICY CMP0079 )
  cmake_policy(SET CMP0079 NEW)
endif()

add_subdirectory(hbox EXCLUDE_FROM_ALL)

add_subdirectory(hcppbox EXCLUDE_FROM_ALL)

add_subdirectory(hrc EXCLUDE_FROM_ALL)

set_target_properties(HCppBox
			PROPERTIES HCPPBOX_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}
			)


