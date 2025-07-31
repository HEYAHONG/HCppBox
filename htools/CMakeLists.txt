
add_subdirectory(hgui_tools)

add_subdirectory(hnet_tools)

add_custom_target(htools
                                    DEPENDS  hgui_tools hnet_tools)

