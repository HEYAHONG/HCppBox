# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.

cmake_minimum_required(VERSION 3.22.1)

project("monochromescreen_simplegui")

#HCppBox不链接外部库
set(HCPPBOX_USE_PURE_CPLUSPLUS TRUE)

#设定HRC文件系统目录为fs
set(HRC_FS_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/fs/)

add_library(${CMAKE_PROJECT_NAME} SHARED
    native-lib.cpp
    android_hgui_driver.cpp
    app_main.cpp)

# 添加HCppBox
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../../ HCppBox EXCLUDE_FROM_ALL)

#启用HCppBox
hcppbox_enable(${PROJECT_NAME})

#启用Hrc
hrc_enable(${PROJECT_NAME})

#链接hsimplegui
target_link_libraries(${PROJECT_NAME} hsimplegui)

# Specifies libraries CMake should link to your target library.
target_link_libraries(${CMAKE_PROJECT_NAME}
    android
    log
    dl)
