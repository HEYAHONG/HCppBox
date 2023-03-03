cmake_minimum_required(VERSION 3.20)
if(POLICY CMP0079 )
  cmake_policy(SET CMP0079 NEW)
endif()


add_subdirectory(src EXCLUDE_FROM_ALL)

set_target_properties(CPPObject
			PROPERTIES CPPOBJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}
			)

#启用cppobject
function(cppobject_enable targetname)
target_link_libraries(${targetname} CPPObject)
endfunction()

