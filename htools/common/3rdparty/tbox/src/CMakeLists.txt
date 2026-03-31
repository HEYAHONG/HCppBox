# src/CMakeLists.txt
# Equivalent of src/xmake.lua

add_subdirectory(tbox)

if(TBOX_BUILD_DEMO)
    add_subdirectory(demo)
endif()
