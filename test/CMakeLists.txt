
cmake_minimum_required(VERSION 3.14)

project(test C CXX ASM)

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

