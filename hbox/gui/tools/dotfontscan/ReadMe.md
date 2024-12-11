#  尚未完成

# 说明

本工具主要用于生成GUI所用点阵字体，可扫描文件(UTF-8)的内容并根据其包含的字符生成字体。

其经典应用为直接将GUI的C/C++源代码(多个文件可使用cat命令输入到一个文件中)输入本工具，然后就可以生成要使用的字体。

本工具采用CMake作为构建工具。

# 系统字体文件

在windows下默认使用simsun.ttc。

在非windows下默认使用`/usr/share/fonts/truetype/wqy/wqy-zenhei.ttc`,debian系统(最新版)使用`sudo apt-get install fonts-wqy-zenhei`安装后即可使用。

# 依赖库

- [freetype](http://www.freetype.org)