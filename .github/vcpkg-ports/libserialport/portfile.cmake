vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO sigrokproject/libserialport
    REF "libserialport-0.1.2"
    SHA512 584b25ddd2b79cb8177c2a7996dce5c2de90bdc04e7b131e5d0c8ebf98e38eda7f7ab8f3c3e415e8d57128cb3887b1a81e6487a465989554473392ab82d3b0a7
    HEAD_REF master
)

# Upstream has no CMake build, so use the small wrapper next to this portfile.
file(COPY "${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt" DESTINATION "${SOURCE_PATH}")

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
)
vcpkg_cmake_install()

file(REMOVE_RECURSE
    "${CURRENT_PACKAGES_DIR}/debug/include"
    "${CURRENT_PACKAGES_DIR}/debug/share"
)

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/COPYING")
