# 说明

本目录字体文件一般分为三类文件:

- 字体列表：以txt为后缀的文本文件，记录那些字需要导出字体,可使用[SimplePixelTools](https://gitee.com/Polarix/SimplePixelTools.git)打开。
- 字体数据：以txt为后缀的文件文件，一般可嵌入C语言代码，记录点阵数据，可使用[SimplePixelTools](https://gitee.com/Polarix/SimplePixelTools.git)导出。
- 字体信息：以c为后缀的文本文件，一般可嵌入C语言代码，记录字的信息，一般在导出字体数据时同时自动导出。

# ASCII字符集字体

- 字体列表:[ASCII.txt](ASCII.txt)

## 8

- 字体数据： [ASCII_font_8.txt](ASCII_font_8.txt)
- 字体信息：[ASCII_font_8.c](ASCII_font_8.c)

## 12

- 字体数据：[ASCII_font_12.txt](ASCII_font_12.txt)
- 字体信息：[ASCII_font_12.c](ASCII_font_12.c)

## 16

- 字体数据：[ASCII_font_16.txt](ASCII_font_16.txt)
- 字体信息：[ASCII_font_16.c](ASCII_font_16.c)

# Chinese

中文字体。仅包含极少数用于测试的字，实际使用中需要另外生成字体。

- 字体列表:[Chinese.txt](Chinese.txt)

## 12

- 字体数据： [Chinese_font_12.txt](Chinese_font_12.txt)
- 字体信息： [Chinese_font_12.c](Chinese_font_12.c)

## 16

- 字体数据： [Chinese_font_16.txt](Chinese_font_16.txt)
- 字体信息： [Chinese_font_16.c](Chinese_font_16.c)