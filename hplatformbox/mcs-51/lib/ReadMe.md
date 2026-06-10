# 说明

MCS-51 公共代码库。

本库中除[libmono.c](libmono.c)的其他文件均由[libmono.c](libmono.c)包含，无需用户手动包含。

# 注意

- 本库使用了64位的整型，因此需要手工使用`-l liblonglong.lib`链接64位整型库
- 本库占用的内存较多，一般推荐外部内存不少于4K
- 本库要求SDCC版本不得低于4.0.0

# 使用

虽然SDCC支持多个文件链接，但本库使用单文件方式，可通过以下方式在主程序C源代码中使用：

```c
/*
 * 头文件
 */

/*
 * 变量,函数声明
 */

/*
 * libmono.c 的配置宏定义
 */

/*
 * 包含libmono.c,具体路径应根据实际路径与包含路径设置确定
 */

#include "libmono.c"

/*
 * 实际业务代码(包括main函数)
 */

```

# 组件

## 基础

基础组件，见[base](base)。

### 可外部配置的宏定义

| 宏定义                                 | 说明           | 备注                                                         |
| -------------------------------------- | -------------- | ------------------------------------------------------------ |
| `LIBMONO_DATA_ATTRIBUTE`               | (普通)数据属性 | 根据模型不同有不同的默认值                                   |
| `LIBMONO_CONST_DATA_ATTRIBUTE`         | 常量数据属性   |                                                              |
| `LIBMONO_FUNCTION_ATTRIBUTE`           | (普通)函数属性 |                                                              |
| `LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE` | 可重入函数属性 |                                                              |
| `LIBMONO_BASE_CORE_DISABLE_INTERRUPT`  | 关中断         | 通常用于实现临界区，用户可重新定义此宏定义用于控制libmono库行为 |
| `LIBMONO_BASE_CORE_ENABLE_INTERRUPT`   | 开中断         | 通常用于实现临界区，用户可重新定义此宏定义用于控制libmono库行为 |

## 运行时

运行时组件，见[runtime](runtime)。

### 可外部配置的宏定义

| 宏定义                                     | 说明                     | 备注                                                         |
| ------------------------------------------ | ------------------------ | ------------------------------------------------------------ |
| `LIBMONO_RUNTIME_LIBC_NO_GETCHAR`          | 不移植`getchar`          | 若用户不实现`getchar`可能会在某些情况下遇到链接错误          |
| `GETCHAR`                                  | 用户实现的`getchar`      |                                                              |
| `LIBMONO_RUNTIME_LIBC_NO_PUTCHAR`          | 不移植`putchar`          | 若用户不实现`putchar`可能会在某些情况下遇到链接错误          |
| `PUTCHAR`                                  | 用户实现的`putchar`      |                                                              |
| `LIBMONO_RUNTIME_LIBC_STDIO_SERIAL`        | 使用串口作为标准输入输出 |                                                              |
| `LIBMONO_RUNTIME_LIBC_NO_TIME`             | 不移植`time`             | 若用户不实现`time`将使用C库的版本                            |
| `TIME`                                     | 用户实现的`time`         |                                                              |
| `LIBMONO_RUNTIME_TIMEZONE_OFFSET`          | 时区偏移                 | `UTC时间-当前时区时间`，如北京时间的时区偏移为`-8*3600`      |
| `LIBMONO_RUNTIME_MAINLOOP_TASK_ARRAY_NAME` | 任务数组名称             | 定义任务数组时类型采用`libmono_runtime_mainloop_task_context_t * const`且需要以NULL结尾。 |

### 主循环

 libmono中绝大部分运行时代码均运行在主循环中，对于用户而言，需要实际业务代码中调用主循环才能保证libmono库运行正常。

```c

/*
 * 主函数入口，若使用了RTOS,也可替换为RTOS任务入口。
 * 注意：在编写实际业务代码时需要提前包含libmono.c
 */
void main(void)
{
    /*
     * 硬件初始化代码
     */
    
    /*
     * libmono初始化代码
     */
    
    while(1)
    {
        /*
         * libmono主循环
         */
        libmono_runtime_mainloop_process();
        
        /*
         * 其它业务代码
         */
       
    }
}

```

## shell

shell可通过`putchar`的移植实现用户交互。

### 可外部配置的宏定义

| 宏定义                        | 说明          | 备注                                |
| ----------------------------- | ------------- | ----------------------------------- |
| `LIBMONO_SHELL_BUFFER_LENGTH` | shell缓冲大小 | 一般不宜过大                        |
| `LIBMONO_SHELL_COMMAND_LIST`  | shell命令列表 | 具体格式见`libmono_shell_command_t` |

### 内置命令

| 名称 | 说明         | 备注                                    |
| ---- | ------------ | --------------------------------------- |
| help | 查看帮助     | 可通过`help 命令名称`查看单个命令的帮助 |
| date | 查看当前时间 |                                         |

