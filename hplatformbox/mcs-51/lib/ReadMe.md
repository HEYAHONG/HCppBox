# 说明

MCS-51 公共代码库。

本库中除[libmono.c](libmono.c)的其他文件均由[libmono.c](libmono.c)包含，无需用户手动包含。

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

基础头文件，见[base](base)。

### 可外部配置的宏定义

| 宏定义                                 | 说明           | 备注                                                         |
| -------------------------------------- | -------------- | ------------------------------------------------------------ |
| `LIBMONO_DATA_ATTRIBUTE`               | (普通)数据属性 | 根据模型不同有不同的默认值                                   |
| `LIBMONO_CONST_DATA_ATTRIBUTE`         | 常量数据属性   |                                                              |
| `LIBMONO_FUNCTION_ATTRIBUTE`           | (普通)函数属性 |                                                              |
| `LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE` | 可重入函数属性 |                                                              |
| `LIBMONO_BASE_CORE_DISABLE_INTERRUPT`  | 关中断         | 通常用于实现临界区，用户可重新定义此宏定义用于控制libmono库行为 |
| `LIBMONO_BASE_CORE_ENABLE_INTERRUPT`   | 开中断         | 通常用于实现临界区，用户可重新定义此宏定义用于控制libmono库行为 |

