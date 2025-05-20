# 说明

此目录用于处理部分系统调用。

可外部配置的宏定义如下:

| 宏定义                                | 说明           | 备注                                                |
| ------------------------------------- | -------------- | --------------------------------------------------- |
| `HDEFAULTS_SYSCALL_NO_IMPLEMENTATION` | 不实现系统调用 | 可节约部分空间，但会造成某些功能在未改变API表时失效 |

# 目录说明

- [wrapper](wrapper)：将系统调用包装成函数
- [implementation](implementation)： 系统调用实现

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
