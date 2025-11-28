
if(NOT DEFINED HTOOLS_INSTALL_DIR)
    set(HTOOLS_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/htools)
endif()

add_subdirectory(hgui_tools)

add_subdirectory(hnet_tools)

add_subdirectory(hcrypto_tools)

add_subdirectory(hsimulator_tools)

add_subdirectory(hsoftplc_tools)

add_custom_target(htools
                                    DEPENDS  hgui_tools hnet_tools hcrypto_tools hsimulator_tools hsoftplc_tools)

