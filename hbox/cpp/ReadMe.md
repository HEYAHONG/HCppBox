# 说明

此C++组件主要用于嵌入式代码,一般不依赖C++运行库,依赖于hbox的C代码。

# 组件

## hcpprt

本组件用于常用的嵌入式（尤其是单片机）C++运行时常用移植操作。

### 函数

| 函数                 | 说明                   | 备注                                         |
| -------------------- | ---------------------- | -------------------------------------------- |
| `void hcpprt_init()` | C++运行时环境初始化    | 一般用于单片机环境，在主任务的初始化部分调用 |
| `void hcpprt_loop()` | 环境循环，更新内部状态 | 在较低优先级的任务或者空闲任务中周期调用     |

### 宏定义

| 宏定义                                | 说明                                  | 备注 |
| ------------------------------------- | ------------------------------------- | ---- |
| `HCPPRT_NO_NEW_AND_DELETE_OVERRIDE`   | 不重载new与delete,默认不定义          |      |
| `HCPPRT_NO_ATOMIC`                    | 不支持atomic头文件                    |      |
| `HCPPRT_SYSTEM_LOOP_NO_AUTOINIT`      | 不自动初始化系统循环槽                |      |
| `HCPPRT_SYSTEM_WORKQUEUE_NO_AUTOINIT` | 不自动初始化工作队列循环              |      |
| `HCPPRT_USE_CTORS`                    | 启用构造（调用`hcpprt_init()`时构造） |      |

### 类

| 类                   | 说明                                                         | 备注 |
| -------------------- | ------------------------------------------------------------ | ---- |
| `hcmemory`           | C语言的内存分配类，默认使用hdefaults的内存分配函数。         |      |
| `hlock`              | 锁,定义常用的锁的接口，默认使用hdefaults的锁函数，故而默认情况下是一个全局锁，与对象无关。 |      |
| `hspinlock`          | 简易自旋锁,利用原子操作实现，不依赖操作系统。注意:不支持递归,不能被hbox中的应用直接使用。 |      |
| `hreentrantspinlock` | 简易可重入自旋锁(不区分加锁顺序),利用原子操作实现,注意:此类不可直接使用，必须实现相应虚函数 |      |
| `hlockguard`         | 提供类似`std::lock_guard`的功能，配合hlock进行锁定操作。     |      |

## hchain

此组件主要用于包装heventchain组件

### 类

| 类       | 说明            | 备注 |
| -------- | --------------- | ---- |
| `hchain` | 包装heventchain |      |

## hloop

此组件主要用于包装heventloop组件

### 类

| 类      | 说明           | 备注 |
| ------- | -------------- | ---- |
| `hloop` | 包装heventloop |      |

## hslots

此组件主要用于包装heventslots组件

### 类

| 类       | 说明            | 备注 |
| -------- | --------------- | ---- |
| `hslots` | 包装heventslots |      |

## hfifo

此组件主要用于包装hringbuf组件

### 类

| 类      | 说明         | 备注 |
| ------- | ------------ | ---- |
| `hfifo` | 包装hringbuf |      |

## hsoftdog

此组件主要用于包装hwatchdog组件

### 类

| 类         | 说明          | 备注 |
| ---------- | ------------- | ---- |
| `hsoftdog` | 包装hwatchdog |      |

### 无栈协程

无栈协程主要指使用`hstacklesscoroutine`组件编写的协程。

| 协程名称   | 说明               | 备注                 |
| ---------- | ------------------ | -------------------- |
| `hsoftdog` | 执行喂看门狗的操作 | 一般由hcpprt组件调用 |

## hobj

此组件主要用于包装hobject组件

### 类

| 类                 | 说明                              | 备注                    |
| ------------------ | --------------------------------- | ----------------------- |
| `hstaticobjectext` | 使静态数据可用hobject包装。       |                         |
| `hstaticobject`    | 使静态数据可用hobject包装。       |                         |
| `hdymaicobj`       | 使得堆上的数据可使用hobject包装。 | 需要`new`与`delete`可用 |

## hsim

此组件主要用于包装hsimulator组件

### 类

| 类                | 说明              | 备注 |
| ----------------- | ----------------- | ---- |
| `hsimmcs51tiny`   | MCS-51 tiny模型   |      |
| `hsimmcs51small`  | MCS-51 small模型  |      |
| `hsimmcs51medium` | MCS-51 medium模型 |      |
| `hsimmcs51large`  | MCS-51 large模型  |      |
| `hsimmcs51huge`   | MCS-51 huge模型   |      |