# 说明

本目录为Android测试。

每个项目均采用Android Studio作为IDE工具。

由于本项目为C++项目，故而需要在Android Studio的SDK管理器中安装以下工具：

- NDK
- CMake

对于非Windows操作系统，需要安装gcc工具以编译相关工具。

# Android Studio

Android Studio是用于开发 Android 应用的官方集成开发环境 (IDE)。

可在以下地址下载:

- https://developer.android.google.cn/studio

安装好Android Studio后，可通过SDK管理器安装最新版NDK与CMake。

# 调试

对于C/C++代码而言，除了IDE提供的方式（打断点），另外一种较为常用的方式是采用日志进行调试。

可通过以下方式查看日志：

- 在Android Studio的LogCat窗口查看日志。一般适用于开发过程中。
- 在adb的shell中运行logcat命令查看日志。一般适用于只有安装包的情况。

# 项目列表

## HelloWorld

helloworld，一般用于测试编译。

本项目可直接使用Android Studio打开工程目录[HelloWorld](HelloWorld)。

## monochromescreen

128x64单色点阵屏示例（与[../monochromescreen](../monochromescreen)功能一致）。

由于HCPPGui不支持Android，本项目在native层自行注册hgui驱动，并使用Android自定义View作为点阵屏画布。驱动采用双缓冲，C++层一次完整的界面操作完成后才提交到显示缓冲区，避免绘图过程中出现闪烁。界面下半部分提供屏幕按键：

- 第一排：F1、F2、F3
- 第二排：确定（Enter）、取消（Esc）、切换（Tab）
- 方向键：上、下、左、右（十字排列）

本项目可直接使用Android Studio打开工程目录[monochromescreen](monochromescreen)。

## monochromescreen_simplegui

128x64单色点阵屏SimpleGUI示例（与[../monochromescreen_simplegui](../monochromescreen_simplegui)功能一致）。

与monochromescreen相同，本项目使用自定义View作为点阵屏画布（同样采用双缓冲），并提供F1、F2、F3、确定、取消、切换及十字方向键。

本项目可直接使用Android Studio打开工程目录[monochromescreen_simplegui](monochromescreen_simplegui)。
