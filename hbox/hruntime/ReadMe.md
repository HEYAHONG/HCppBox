# 说明

辅助处理运行时相关工作，如：

- 处理各个C语言组件的初始化及任务循环。
- 简易多任务系统。

#  组件

## hruntime

处理各个C语言组件的初始化及任务循环。


可外部配置的宏定义如下:

|             宏定义              |     说明     |                             备注                             |
| :-----------------------------: | :----------: | :----------------------------------------------------------: |
|  `HRUNTIME_USING_INIT_SECTION`  | 启用初始化段 |       当`hruntime_init`被调用时将调用初始化段中的函数        |
|  `HRUNTIME_USING_LOOP_SECTION`  |  启用循环段  |        当`hruntime_loop`被调用时将调用循环段中的函数         |
| `HRUNTIME_USING_SYMBOL_SECTION` |  启用符号段  |          导出的符号可使用`hruntime_symbol_find`查找          |
|  `HRUNTIME_USING_SYMBOL_TABLE`  |  启用符号表  |      一般情况下，符号表与符号段任选其一即可实现符号查找      |
|   `HRUNTIME_NO_SYMBOL_TABLE`    | 不启用符号表 | 优先级较`HRUNTIME_USING_SYMBOL_TABLE`高。可有效减少生成的可执行文件大小 |

## hstacklesscoroutine

本组件用于辅助编写简易无栈协程。无栈协程可用于实现简易的多任务系统。

具体实现见[hstacklesscoroutine](hstacklesscoroutine)。