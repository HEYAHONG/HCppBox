cmake_minimum_required(VERSION 3.20)
if(POLICY CMP0079 )
  cmake_policy(SET CMP0079 NEW)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../hbox hbox)


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../hcppbox hcppbox)


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../hrc hrc)





