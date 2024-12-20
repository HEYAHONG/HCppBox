# 说明

本组件主要用于提供辅助进行嵌入式GUI开发的组件。

主要适用于具有以下特点的GUI开发:

- 显示简易的图片文字。
- 提供简易的交互(如菜单等)。

更加复杂的交互(如音视频、动画、字体效果等)应当使用更加成熟的库,如[Qt](https://www.qt.io/)、[minigui](https://github.com/VincentWei/MiniGUI.git)、[lvgl](https://github.com/lvgl/lvgl.git)、[u8g2](https://github.com/olikraus/u8g2.git)等

主要目标硬件为以下类型的硬件:

-  单色屏，典型为分辨率为128x64的点阵屏。
- 低分辨率彩屏，典型为分辨率为320x240 的彩屏。

# GUI设计

对于嵌入式GUI设计而言，主要包含以下内容：

- 界面设计
- 事件处理

## 界面设计

界面对象一般可分为:

- 窗口(Window):可分为Frame（窗口框）与Dialog(对话框)。
- 控件(Control/Widget):如按钮、标签、文本框等。

界面对象之间一般采用树状结构，即每个界面对象都应该有一个父对象，通常当父对象为空(NULL)时 通常默认为顶层的界面对象。

对于桌面平台而言，顶层的界面对象一般是窗口(Window)，而对于廉价的嵌入式显示方案而言，顶层的界面对象一般是整个屏幕(Screen)。

对于某些资源极其受限的平台的GUI言，其屏幕上没有子对象（直接由用户绘制整个屏幕），只是区分不同的界面,此时，每一个界面就是界面对象。

## 事件处理

对于一个GUI应用而言，一般有一个事件队列(通常运行在单一线程（主线程）)。

对于大多数GUI应用（如基于Qt的GUI程序、基于wxWidgets的GUI程序）而言，有且只有一个应用对象（全局唯一），由GUI线程（桌面平台中一般是主线程）执行对该对象中的事件队列的处理，同时，应用对象也包含了其它GUI应用所需的全局唯一信息（如在桌面平台上时，应用对象通常包含主窗口信息、程序运行参数等）。

# 场景

嵌入式GUI的应用场景与桌面GUI不同，不同的场景有不同的需求。

不同场景之间的代码一般不可混用（特殊声明的情况除外）。

## 场景1

适用于处理器性能不高，屏幕分辨率较低的场景。特点如下：

- 主要目标硬件为单色屏,典型为分辨率为128x64的点阵屏。
- 仅支持一块屏幕。
- 无动态内存分配，资源均采用静态分配。
- 无缓冲(低分辨率屏幕一般数据量较小),也可自行实现缓冲。
- 用户直接绘制整个屏幕，可有若干个不同的界面。

具体说明见[hgui_gui_scene1](hgui_gui_scene1)。

# 注意事项

- 本组件不包括实际的硬件驱动,在driver子组件中提供了一系列接口，用户需要根据实际硬件实现相应接口。
- 在大多数GUI库中，GUI都是单线程管理的（由单个线程完成绘图及相关事件处理）。因此，本组件中的大多数函数均不是线程安全的，如在多线程环境下运行，需要自行加锁。

# 子组件说明

## common

配置、数据类型定义等。

文件:

- [hgui_gui_common.c](hgui_gui_common.c)
- [hgui_gui_common.h](hgui_gui_common.h)

## pixel

像素处理等。

提供一些像素点的类型转换功能。

此子组件的对象大体可分为存储像素的数据与回调函数两类，当像素对象是回调函数时，需要调用`hgui_pixel_bits_get`转换为存储像素的数据(此时的坐标若未特殊说明，传入的坐标为驱动接口操作对象的坐标)。

回调函数配合C++的Lambda表达式可轻松实现带缓冲的GUI显示。

文件:

- [hgui_gui_pixel.c](hgui_gui_pixel.c)

- [hgui_gui_pixel.h](hgui_gui_pixel.h)

## driver

驱动接口等。

**用户需要根据实际硬件实现相应的接口，否则整个GUI组件可能不工作。**

此子组件提供一个默认的驱动对象,当驱动指针为NULL时默认使用此对象。

驱动实现时需要注意像素对象的类型，若是回调函数则调用相应函数(`hgui_pixel_bits_get`)需要转换为实际存储像素的数据。

驱动操作的操作对象是显示器（Display）/屏幕(Screen)，模拟时其表现可能时一个窗口（Window,Windows原生应用/Linux原生应用）或者画布(Canvas，wasm应用),实际在嵌入式平台工作时，每一个显示器（Display）/屏幕(Screen)都需要一个驱动对象（只有一个显示器（Display）/屏幕(Screen)时可使用默认的驱动对象）。

文件:

- [hgui_gui_driver.c](hgui_gui_driver.c)

- [hgui_gui_driver.h]( hgui_gui_driver.h ) 

## dotfont

点阵字体处理。

此子组件提供ASCII码点阵字体，如需汉字显示，需要自行实现汉字点阵字体。

内置字体如下：

- `extern const hgui_gui_dotfont_t hgui_gui_dotfont_ascii_0806;`:ASCII 8X6 字体。
- `extern const hgui_gui_dotfont_t hgui_gui_dotfont_ascii_1206;`:ASCII 12X6 字体。
- `extern const hgui_gui_dotfont_t hgui_gui_dotfont_ascii_1608;`:ASCII 16X8 字体。
- `extern const hgui_gui_dotfont_t hgui_gui_dotfont_ascii_2416;`:ASCII 24X16 字体。
- `extern const hgui_gui_dotfont_t hgui_gui_dotfont_unicode_dummy_1616;`:假 16X16 字体。对于dummy字体而言，可构造一个临时字体并将字体参数(font)可设置为其它字体(`hgui_gui_dotfont_t *` 指针),这样就可以实现当其它字体失败时显示dummy字体。
- `extern const hgui_gui_dotfont_t hgui_gui_dotfont_unicode_dummy_2424;`:假 24X24 字体。对于dummy字体而言，可构造一个临时字体并将字体参数(font)可设置为其它字体(`hgui_gui_dotfont_t *` 指针),这样就可以实现当其它字体失败时显示dummy字体。
- `extern const hgui_gui_dotfont_t hgui_gui_dotfont_unicode_dummy_3232;`:假 32X32 字体。对于dummy字体而言，可构造一个临时字体并将字体参数(font)可设置为其它字体(`hgui_gui_dotfont_t *` 指针),这样就可以实现当其它字体失败时显示dummy字体。

此子组件的字体为结构体常量，若在链接时开启垃圾回收功能（对于ld而言，其标志为`--gc-sections`，配合gcc的`-ffunction-sections -fdata-sections`可实现垃圾回收），未使用的字体(主要针对汉字点阵字体)将不会出现在最终的可执行文件中。

用户自行实现字体时，推荐将字体与字体内部的数据都定义为常量，这样可以省下大量RAM空间。

文件:

- [hgui_gui_dotfont.c](hgui_gui_dotfont.c)  
- [hgui_gui_dotfont.h](hgui_gui_dotfont.h)

额外的构建目标：

- `dotfontscan_tool`:若采用CMake编译，则可尝试使用`dotfontscan_tool`目标构建[dotfontscan](tools/dotfontscan)工具来生成点阵字体。

## xpm

XPM图像显示。

XPM是一种可直接嵌入C代码的图像格式，非常适合嵌入式GUI显示（容易制作，无需文件系统支持）。

XPM适合小图标，当图像较大时可能加载速度较慢，此时推荐先将图像加载到缓冲中，再进行显示。

XPM可使用[GIMP](https://www.gimp.org/)从其它图像格式导出。

文件:

- [hgui_gui_xpm.c](hgui_gui_xpm.c) 
- [hgui_gui_xpm.h](hgui_gui_xpm.h) 

## scene1

场景1相关代码。详细说明见目录[hgui_gui_scene1](hgui_gui_scene1)。

文件:

- [hgui_gui_scene1.h](hgui_gui_scene1.h)
- [hgui_gui_scene1.c](hgui_gui_scene1.c)
- 目录[hgui_gui_scene1](hgui_gui_scene1)中的源代码文件(注意：该目录中的文件默认不会添加至构建树，而是由前两个文件引入)。

# 工具

工具主要指在主机上运行的工具,通常需要单独编译。

见[tools](tools)目录。
