# 说明

posix兼容层，用于兼容部分posix代码。一般用于增强[picolibc](../picolibc)。

# CMake变量

本章节的变量主要指CMake中的相关变量，一般由用户设定而非CMake内部变量。

变量仅当使用CMake进行构建时有效。

| 变量名                                 | 说明                     | 备注                                  |
| -------------------------------------- | ------------------------ | ------------------------------------- |
| `HALIBC_ENABLE_POSIX`                  | 启用posix                | 未启用posix兼容层时本目录所有代码无效 |
| `HALIBC_POSIX_SOCKET`                  | 启用socket兼容层         | 需要先启用`HALIBC_ENABLE_POSIX`才有效 |
| `HALIBC_POSIX_SOCKET_DISABLE_FALLBACK` | 关闭socket的fallback实现 | 用户需要手动实现fallback中的内容      |

