# 说明

本组件主要用于提供辅助进行嵌入式GUI开发的组件。

主要适用于具有以下特点的GUI开发:

- 显示简易的图片文字。
- 提供简易的交互(如菜单等)。

更加复杂的交互(如音视频、动画、字体效果等)应当使用更加成熟的库,如[Qt](https://www.qt.io/)、[minigui](https://github.com/VincentWei/MiniGUI.git)、[lvgl](https://github.com/lvgl/lvgl.git)、[u8g2](https://github.com/olikraus/u8g2.git)等

# 子组件说明

## common

配置、数据类型定义等。

## pixel

像素处理等。

提供一些像素点的类型转换功能。

## driver

驱动接口等。

**用户需要根据实际硬件实现相应的接口，否则整个GUI组件可能不工作。**

此子组件提供一个默认的驱动对象,当驱动指针为NULL时默认使用此对象。

## dotfont

点阵字体处理。

此子组件提供ASCII码点阵字体，如需汉字显示，需要自行实现汉字点阵字体。

