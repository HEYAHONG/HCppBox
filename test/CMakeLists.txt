
cmake_minimum_required(VERSION 3.21)

find_package(Git)

if(GIT_FOUND)
	message(STATUS "Git found:${GIT_EXECUTABLE}")
	execute_process(COMMAND ${GIT_EXECUTABLE} pull
							 WORKING_DIRECTORY	${CMAKE_SOURCE_DIR})
	execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
		  					WORKING_DIRECTORY 	${CMAKE_SOURCE_DIR}
		 					OUTPUT_VARIABLE PROJECT_VERSION)
	message(STATUS "Project Version:${PROJECT_VERSION}")
endif()

project(test C CXX ASM)

#设定HRC文件系统目录为fs
set(HRC_FS_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/fs/)

#包含库文件
add_subdirectory(../ HCppBox)

if(MSVC)
    add_compile_options(-utf-8 )
endif()

file(GLOB TESTS *)
foreach(TEST ${TESTS})
	if(IS_DIRECTORY ${TEST})
        if(EXISTS "${TEST}/CMakeLists.txt")
            file(RELATIVE_PATH TESTNAME ${CMAKE_CURRENT_SOURCE_DIR}/ ${TEST})
            message(STATUS "Add Test ${TESTNAME}")
            add_subdirectory(${TEST})
        endif()
	endif()
endforeach()

