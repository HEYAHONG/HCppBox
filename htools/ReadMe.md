#  说明

本目录主要用于开发过程中用到的工具，其中大多数需要依赖于第三方库。
一般运行于非嵌入式环境。

# 编译

确保安装好编译环境（编译器+CMake+Ninja/Make/NMake)。

默认情况下，采用CMake作为构建系统。

本目录的工具默认不会自动构建，需要手动指定目标为`htools`才会进行构建。

# 目录说明

- [hgui_tools](hgui_tools):hgui组件所使用的工具。
- [hnet_tools](hnet_tools):hnet组件所使用的工具。
- [hcrypto_tools](hcrypto_tools):hcrypto组件所使用的工具。
- [hsimulator_tools](hsimulator_tools)：hsimulator组件所使用的工具
- [hsoftplc_tools](hsoftplc_tools)：hsoftplc组件所使用的工具
- [common](common):公共代码
