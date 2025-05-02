# 说明

setjmp与longjmp是C语言标准库函数，可用于跨函数跳转,其可移植性较强，通常用于异常处理与流程控制。

使用setjmp与longjmp可升级无栈协程，使其可在多层函数中让出控制权，需要注意以下事项：

- 使用C++需要注意异常的使用与局部变量生命周期可能会受到影响。
- 使用C时需要注意VLA的使用可能会受到影响。

本组件实现的模式如下：

- 非裸机模式：实现无栈协程,兼容性强。
- 裸机模式下：实现栈拷贝协程,兼容性不强。

本组件中主要有以下层次：

- 调度器：一个调度器将包含一个或多个协程的协程控制块，当所有协程完成时或者出错时调度器将退出。
- 协程控制块：一个协程控制块代表一个协程，包含一个任务。协程处于创建状态或者停止状态时任务可更换。
- 任务：包含任务入口与执行任务的其它信息。

# 宏定义

以下宏定义可外部配置：

|                 宏定义                 |      说明      |                     备注                      |
| :------------------------------------: | :------------: | :-------------------------------------------: |
|  `HSTACKLESSCOROUTINE2_BARE_MACHINE`   |  使用裸机模式  |                                               |
| `HSTACKLESSCOROUTINE2_NO_BARE_MACHINE` | 使用非裸机模式 | 优先级高于`HSTACKLESSCOROUTINE2_BARE_MACHINE` |

# 非裸机模式

非裸机模式下实现简易无栈协程。

无栈协程可用于实现简易的多任务系统。

无栈协程的特点如下：

- 协程没有自己的栈，直接使用调用协程的线程（进程）栈。
- 让出控制权时局部变量将失效，因此状态保存只能使用非局部变量（全局变量或者静态变量）。
- 可移植性强，几乎不依赖特定的操作系统特性或者硬件特性。
- 只能由用户控制是否让出控制权,没有抢占等特性。

任务函数中可定义协程块，其形式如下：

协程任务函数中可定义一个或者多个协程块（不可嵌套），以管理执行流程,其形式如下：

```cpp
/*
 * 协程任务入口
 */
void co_task_entry(hstacklesscoroutine2_scheduler_t *scheduler,hstacklesscoroutine2_ccb_t *ccb,void *usr)
{
    //协程块1
    HSTACKLESSCOROUTINE2_BLOCK_START(ccb)
    .
    .
    协程代码
    .
    .
    HSTACKLESSCOROUTINE2_BLOCK_END(ccb)

    //协程块2
    HSTACKLESSCOROUTINE2_BLOCK_START(ccb)
    .
    .
    协程代码
    .
    .
    HSTACKLESSCOROUTINE2_BLOCK_END(ccb)
}
```

在协程块中可使用以下操作：

- `HSTACKLESSCOROUTINE2_BLOCK_POINT(CCB_NAME)`:协程块点，标记一个协程块点，此点之后下一个点之前的代码让出控制权时从此点继续执行。
- `HSTACKLESSCOROUTINE2_BLOCK_LABEL(CCB_NAME,N)`：协程标签，配合GOTO实现跳转。
- `HSTACKLESSCOROUTINE2_BLOCK_GOTO(CCB_NAME,N)`：协程GOTO，配合标签实现跳转。

# 裸机模式

**注意：此功能处于实验阶段。**

裸机模式下实现简易栈拷贝协程，此时协程任务的写法没有太大限制，不过仍然遵循setjmp与longjmp的注意事项。

栈拷贝协程的特点如下：

- 协程没有独立的栈，共享调用协程的线程（进程）栈。
- 让出控制权时将保存栈，恢复控制权时将还原栈。
- 只能由用户控制是否让出控制权,没有抢占等特性。
