# 说明

本组件用于辅助编写简易无栈协程。无栈协程可用于实现简易的多任务系统。

无栈协程的特点如下：

- 协程没有自己的栈，直接使用调用协程的线程（进程）栈。
- 让出控制权时局部变量将失效，因此状态保存只能使用非局部变量（全局变量或者静态变量）。
- 可移植性强，几乎不依赖特定的操作系统特性或者硬件特性。
- 只能由用户控制是否让出控制权,没有抢占等特性。

# 定义协程块

定义协程块主要使用以下宏定义：

- `HSTACKLESSCOROUTINE_BLOCK_START(NAME)`
- `HSTACKLESSCOROUTINE_BLOCK_END(NAME)`

协程块的形式如下:

```c++
HSTACKLESSCOROUTINE_BLOCK_START(协程名称)
 .
 . //协程代码
 .
HSTACKLESSCOROUTINE_BLOCK_END(协程名称)
```

在协程代码中，可使用以下以下函数进行操作：

- `HSTACKLESSCOROUTINE_GET_CURRENT_CCB()`：获取当前协程控制块指针
- `HSTACKLESSCOROUTINE_GET_CURRENT_EVENT()`：获取当前事件指针
- `hstacklesscoroutine_yield()` ：让出控制权，下次将从此开始运行。
- `hstacklesscoroutine_yield_with_label(N)`：让出控制权并且使用标签（正整数），下次将从此开始运行。
- `hstacklesscoroutine_return()`：协程返回，常用于条件不满足时不继续运行，并从上一次让出控制权的地方重新运行。
- `hstacklesscoroutine_goto_label(N)`：跳转至特定标签，可配合`hstacklesscoroutine_yield_with_label(N)`使用。
- `hstacklesscoroutine_await(AWAITER)`:协程等待，与协程返回不同，协程等待条件满足前不会再运行任何用户代码

# 调用协程

所用的协程都需要一个或者多个线程运行。

对于MCU等嵌入式环境，无操作系统时可直接在`main()`函数中调用协程，有操作系统时可使用单独线程调用协程或者使用软件定时器调用协程。

当定义协程块与调用协程不在同一个文件时，需要使用`HSTACKLESSCOROUTINE_DECLARE_COROUTINE(NAME)`先声明协程。

调用协程可使用以下入口函数（任选其一）:

- `HSTACKLESSCOROUTINE_ENTRY(NAME)`
- `HSTACKLESSCOROUTINE_ENTRY_WITH_EVENT(NAME,EVENT)`
- `HSTACKLESSCOROUTINE_ENTRY_WITH_CCB_AND_EVENT(NAME,CCB,EVENT)`

以上函数的区别仅仅是是否使用自定义的协程控制块（CCB）与事件（EVENT）,入口函数应该多次调用，直到协程完成。

协程的全局协程控制块可使用`HSTACKLESSCOROUTINE_GET_GLOBAL_CCB(NAME)`获取，可使用协程控制块获取协程是否完成。

# 协程组

对于可以处理同一类型的事件的协程，可以放入同一个协程组中。

协程组的定义方式如下：

```c
HSTACKLESSCOROUTINE_GROUP_BLOCK_START(协程组名称)
HSTACKLESSCOROUTINE_GROUP_BLOCK_ITEM(协程名称1)
HSTACKLESSCOROUTINE_GROUP_BLOCK_ITEM(协程名称2)
	.
    .
    .
 HSTACKLESSCOROUTINE_GROUP_BLOCK_ITEM(协程名称N)
HSTACKLESSCOROUTINE_GROUP_BLOCK_END(协程组名称)
```

调用协程组前，如不在同一文件中，则需要使用 `HSTACKLESSCOROUTINE_DECLARE_COROUTINE_GROUP(NAME)`申明协程组，然后在合适位置调用`HSTACKLESSCOROUTINE_GROUP_ENTRY(NAME,EVENT)`即可实现对协程组的处理。

