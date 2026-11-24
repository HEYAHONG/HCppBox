# 说明

picolibc源代码，见[master](master)目录。

# CMake变量

本章节的变量主要指CMake中的相关变量，一般由用户设定而非CMake内部变量。

变量仅当使用CMake进行构建时有效。

除了可设定picolibc本身的选项外，用户还需要设定一些选项以控制功能：

| 变量名                     | 说明                   | 备注                                                         |
| -------------------------- | ---------------------- | ------------------------------------------------------------ |
| `HALIBC_ENABLE_PICOLIBC`   | 启用picolibc           | 未启用picolibc时不会引入picolibc源代码，仍然使用编译器自带的libc |
| `HALIBC_PICOLIBC_MOXIEBOX` | 启用picolibc的moxiebox | 需要先启用`HALIBC_ENABLE_PICOLIBC`才有效                     |