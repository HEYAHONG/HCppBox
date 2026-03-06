
cmake_minimum_required(VERSION 3.20)

project(matiec)


if(WIN32)
if(MSVC)
add_compile_options(-utf-8)
endif()
endif()


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

file(GLOB MATIEC_ABSYNTAX_FILES
${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/matiec/absyntax/*.cc
${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/matiec/absyntax/*.hh)
add_library(matiec_absyntax STATIC ${MATIEC_ABSYNTAX_FILES})
target_include_directories(matiec_absyntax PUBLIC
${CMAKE_CURRENT_LIST_DIR}
${CMAKE_CURRENT_LIST_DIR}/config/
${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/matiec
)


file(GLOB MATIEC_ABSYNTAX_UTILS_FILES
${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/matiec/absyntax_utils/*.cc
${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/matiec/absyntax_utils/*.hh)
add_library(matiec_absyntax_utils STATIC ${MATIEC_ABSYNTAX_UTILS_FILES})
target_include_directories(matiec_absyntax_utils PUBLIC
${CMAKE_CURRENT_LIST_DIR}
${CMAKE_CURRENT_LIST_DIR}/config/
${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/matiec
)

file(GLOB MATIEC_STAGE1_2_FILES
${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/matiec/stage1_2/*.cc
${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/matiec/stage1_2/*.hh)
add_library(matiec_stage1_2 STATIC ${MATIEC_STAGE1_2_FILES})
target_include_directories(matiec_stage1_2 PUBLIC
${CMAKE_CURRENT_LIST_DIR}
${CMAKE_CURRENT_LIST_DIR}/config/
${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/matiec
)
target_compile_definitions(matiec_stage1_2 PRIVATE  -DDEFAULT_LIBDIR="lib" -DYY_BUF_SIZE=65536)
if(("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" ) OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" ) )
target_compile_options(matiec_stage1_2 PRIVATE -fpermissive)
endif()


file(GLOB MATIEC_STAGE3_FILES
${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/matiec/stage3/*.cc
${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/matiec/stage3/*.hh)
add_library(matiec_stage3 STATIC ${MATIEC_STAGE3_FILES})
target_include_directories(matiec_stage3 PUBLIC
${CMAKE_CURRENT_LIST_DIR}
${CMAKE_CURRENT_LIST_DIR}/config/
${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/matiec
${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/matiec/stage3
)



file(GLOB MATIEC_STAGE4_C_FILES
${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/matiec/stage4/*.cc
${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/matiec/stage4/*.hh
${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/matiec/stage4/generate_c/generate_c.cc
${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/matiec/stage4/generate_c/*.hh)
add_library(matiec_stage4_c STATIC ${MATIEC_STAGE4_C_FILES})
target_include_directories(matiec_stage4_c PUBLIC
${CMAKE_CURRENT_LIST_DIR}
${CMAKE_CURRENT_LIST_DIR}/config/
${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/matiec
${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/matiec/stage4
)
if(("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" ) OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" ) )
target_compile_options(matiec_stage4_c PRIVATE -fpermissive)
endif()


file(GLOB MATIEC_STAGE4_IEC_FILES
${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/matiec/stage4/*.cc
${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/matiec/stage4/*.hh
${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/matiec/stage4/generate_iec/generate_iec.cc
${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/matiec/stage4/generate_iec/*.hh)
add_library(matiec_stage4_iec STATIC ${MATIEC_STAGE4_IEC_FILES})
target_include_directories(matiec_stage4_iec PUBLIC
${CMAKE_CURRENT_LIST_DIR}
${CMAKE_CURRENT_LIST_DIR}/config/
${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/matiec
${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/matiec/stage4
)
if(("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" ) OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" ) )
target_compile_options(matiec_stage4_iec PRIVATE -fpermissive)
endif()


file(GLOB MATIEC_MAIN_FILES
${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/matiec/*.cc
${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/matiec/*.hh)

add_executable(iec2c ${MATIEC_MAIN_FILES})
target_link_libraries(iec2c matiec_stage4_c matiec_stage3 matiec_stage1_2 matiec_absyntax_utils matiec_absyntax )
add_executable(iec2iec ${MATIEC_MAIN_FILES})
target_link_libraries(iec2iec matiec_stage4_iec matiec_stage3 matiec_stage1_2 matiec_absyntax_utils matiec_absyntax )

include(GNUInstallDirs)
if(DEFINED HTOOLS_INSTALL_DIR)
install(TARGETS iec2c iec2iec
       RUNTIME DESTINATION ${HTOOLS_INSTALL_DIR}/bin
       LIBRARY DESTINATION ${HTOOLS_INSTALL_DIR}/lib
       ARCHIVE DESTINATION ${HTOOLS_INSTALL_DIR}/lib
       )
endif()

