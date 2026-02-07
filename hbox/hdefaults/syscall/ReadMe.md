# 说明

此目录用于处理部分系统调用。

可外部配置的宏定义如下:

| 宏定义                                 | 说明                                              | 备注                                                         |
| -------------------------------------- | ------------------------------------------------- | ------------------------------------------------------------ |
| `HDEFAULTS_SYSCALL_NO_IMPLEMENTATION`  | 不实现系统调用                                    | 可节约部分空间，但会造成某些功能在未改变API表时失效          |
| `HDEFAULTS_SYSCALL_NO_HFILEDESCRIPTOR` | 不使用`hfiledescriptor`组件实现文件描述符相关调用 | 可节约部分空间。但`close`、`read`、`write`、`lseek`、`open`等将在某些环境下不可用 |
| `HSYSCALL_NO_IMPLEMENTATION`           | 不实现[hsyscall](hsyscall)                        | 可用于在编译器垃圾回收不完善时减少资源占用                   |
| `HSYSCALL_NO_TIME`                     | 不实现[hsyscall](hsyscall)的时间组件              | 可用于在编译器垃圾回收不完善时减少资源占用                   |
| `HSYSCALL_NO_RANDOM`                   | 不实现[hsyscall](hsyscall)的随机数组件            | 可用于在编译器垃圾回收不完善时减少资源占用                   |

# 目录说明

- [wrapper](wrapper)：将系统调用包装成函数
- [implementation](implementation)： 系统调用实现
- [hsyscall](hsyscall):用于扩展某些环境下（如无操作系统环境）的系统调用

# 系统调用

## `gettimeofday`

获取当前时间。

当系统支持`gettimeofday`时，采用系统的实现。

当系统有类似的API时采用相应API实现。

当找不到其它实现时，默认采用`hdefaults_tick_t hdefaults_tick_get(void)`实现。

可外部配置的宏定义如下:

| 宏定义                               | 说明                     | 备注                          |
| ------------------------------------ | ------------------------ | ----------------------------- |
| `HGETTIMEOFDAY`                      | 用户实现的`gettimeofday` | 参数与返回值同`hgettimeofday` |
| `HDEFAULTS_SYSCALL_NO_HGETTIMEOFDAY` | 不实现`gettimeofday`     |                               |

## `settimeofday`

设置当前时间。

当系统支持`settimeofday`时，采用系统的实现。

当系统有类似的API时采用相应API实现。

当找不到其它实现时，处理方法同`gettimeofday`。

可外部配置的宏定义如下:

| 宏定义                               | 说明                     | 备注                          |
| ------------------------------------ | ------------------------ | ----------------------------- |
| `HSETTIMEOFDAY`                      | 用户实现的`settimeofday` | 参数与返回值同`hsettimeofday` |
| `HDEFAULTS_SYSCALL_NO_HSETTIMEOFDAY` | 不实现`settimeofday`     |                               |

## `getrandom`

获取随机数。

当系统支持`getrandom`时，采用系统的实现。

当系统有类似的API时采用相应API实现。

当找不到其它实现时，采用C库。

可外部配置的宏定义如下:

| 宏定义                            | 说明                  | 备注                       |
| --------------------------------- | --------------------- | -------------------------- |
| `HGETRANDOM`                      | 用户实现的`getrandom` | 参数与返回值同`hgetrandom` |
| `HDEFAULTS_SYSCALL_NO_HGETRANDOM` | 不实现`getrandom`     |                            |

## `mmap`

将文件或者其它对象映射到内存中。

此API可方便文件的操作,如用于加载文件（如可重定位对象文件、动态共享库文件、可执行文件）。

当系统支持`mmap`时，采用系统的实现。

当系统不支持`mmap`时,采用`malloc`实现部分映射功能（匿名映射）,此时内存保护标志及大部分标志无效，默认情况下`malloc`分配的内存应当具有读、写及执行权限，若分配的内存不具有某些权限，应当自行实现mmap。注意：使用mmap申请内存时，应尽量申请大内存，否则将造成极大的内存浪费。

可外部配置的宏定义如下:

| 宏定义                       | 说明             | 备注                                                 |
| ---------------------------- | ---------------- | ---------------------------------------------------- |
| `HMMAP`                      | 用户实现的`mmap` | 参数与返回值同`hmmap`,注意：文件偏移是64位无符号整数 |
| `HDEFAULTS_SYSCALL_NO_HMMAP` | 不实现`mmap`     |                                                      |

## `msync`

同步mmap映射的文件

当系统支持`msync`时，采用系统的实现。

可外部配置的宏定义如下:

| 宏定义                        | 说明              | 备注                   |
| ----------------------------- | ----------------- | ---------------------- |
| `HMSYNC`                      | 用户实现的`msync` | 参数与返回值同`hmsync` |
| `HDEFAULTS_SYSCALL_NO_HMSYNC` | 不实现`msync`     |                        |

## `munmap`

取消mmap映射。

当系统支持`munmap`时，采用系统的实现。

当系统不支持`mmap`时，`munmap`采用`free`释放`malloc`申请的内存。

可外部配置的宏定义如下:

| 宏定义                         | 说明               | 备注                    |
| ------------------------------ | ------------------ | ----------------------- |
| `HMUNMAP`                      | 用户实现的`munmap` | 参数与返回值同`hmunmap` |
| `HDEFAULTS_SYSCALL_NO_HMUNMAP` | 不实现`munmap`     |                         |

## `mprotect`

修改内存保护标志。

当系统支持`mprotect`时，采用系统的实现。

可外部配置的宏定义如下:

| 宏定义                           | 说明                 | 备注                      |
| -------------------------------- | -------------------- | ------------------------- |
| `HMPROTECT`                      | 用户实现的`mprotect` | 参数与返回值同`hmprotect` |
| `HDEFAULTS_SYSCALL_NO_HMPROTECT` | 不实现`mprotect`     |                           |

## `close`

关闭文件描述符

当系统支持`close`时，采用系统的实现。

可外部配置的宏定义如下:

| 宏定义                        | 说明              | 备注                  |
| ----------------------------- | ----------------- | --------------------- |
| `HCLOSE`                      | 用户实现的`close` | 参数与返回值同`close` |
| `HDEFAULTS_SYSCALL_NO_HCLOSE` | 不实现`close`     |                       |

## `read`

从文件描述符读取数据

当系统支持`read`时，采用系统的实现。

可外部配置的宏定义如下:

| 宏定义                       | 说明             | 备注                 |
| ---------------------------- | ---------------- | -------------------- |
| `HREAD`                      | 用户实现的`read` | 参数与返回值同`read` |
| `HDEFAULTS_SYSCALL_NO_HREAD` | 不实现`read`     |                      |

## `write`

向文件描述符写入数据

当系统支持`write`时，采用系统的实现。

可外部配置的宏定义如下:

| 宏定义                        | 说明              | 备注                  |
| ----------------------------- | ----------------- | --------------------- |
| `HWRITE`                      | 用户实现的`write` | 参数与返回值同`write` |
| `HDEFAULTS_SYSCALL_NO_HWRITE` | 不实现`write`     |                       |

## `lseek`

修改文件描述符关联的文件的偏移

当系统支持`lseek`时，采用系统的实现。

可外部配置的宏定义如下:

| 宏定义                        | 说明              | 备注                  |
| ----------------------------- | ----------------- | --------------------- |
| `HLSEEK`                      | 用户实现的`lseek` | 参数与返回值同`lseek` |
| `HDEFAULTS_SYSCALL_NO_HLSEEK` | 不实现`lseek`     |                       |

## `open`

打开文件(返回文件描述符)

当系统支持`open`时，采用系统的实现。

可外部配置的宏定义如下:

| 宏定义                       | 说明             | 备注                 |
| ---------------------------- | ---------------- | -------------------- |
| `HOPEN`                      | 用户实现的`open` | 参数与返回值同`open` |
| `HDEFAULTS_SYSCALL_NO_HOPEN` | 不实现`open`     |                      |

## `clock_getres`

获取时钟精度

当系统支持`clock_getres`时，采用系统的实现。

可外部配置的宏定义如下:

| 宏定义                               | 说明                     | 备注                         |
| ------------------------------------ | ------------------------ | ---------------------------- |
| `HCLOCK_GETRES`                      | 用户实现的`clock_getres` | 参数与返回值同`clock_getres` |
| `HDEFAULTS_SYSCALL_NO_HCLOCK_GETRES` | 不实现`clock_getres`     |                              |

## `clock_gettime`

获取时钟

当系统支持`clock_gettime`时，采用系统的实现。

可外部配置的宏定义如下:

| 宏定义                                | 说明                      | 备注                          |
| ------------------------------------- | ------------------------- | ----------------------------- |
| `HCLOCK_GETTIME`                      | 用户实现的`clock_gettime` | 参数与返回值同`clock_gettime` |
| `HDEFAULTS_SYSCALL_NO_HCLOCK_GETTIME` | 不实现`clock_gettime`     |                               |

## `clock_settime`

设置时钟

当系统支持`clock_settime`时，采用系统的实现。

可外部配置的宏定义如下:

| 宏定义                                | 说明                      | 备注                          |
| ------------------------------------- | ------------------------- | ----------------------------- |
| `HCLOCK_SETTIME`                      | 用户实现的`clock_settime` | 参数与返回值同`clock_settime` |
| `HDEFAULTS_SYSCALL_NO_HCLOCK_SETTIME` | 不实现`clock_settime`     |                               |
