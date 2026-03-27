/***************************************************************
 * Name:      config.h
 * Purpose:   HCppBox 配置头文件
 * Author:    QClaw
 * Created:   2026-03-27
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

/** \brief HCppBox 配置头文件
 *
 * 本文件包含所有可外部配置的宏定义。
 * 由QClaw自动生成，请勿手动修改。
 *
 * \note 更新方法：
 * - 扫描源代码目录下hbox目录中的markdown文件
 * - 提取"可外部配置的宏定义"表格
 * - 更新config.h文件（保留原config.h风格）
 * - 文件格式：带BOM的UTF-8编码，Unix换行符(LF)
 * - 注释风格：Doxygen格式
 *
 * 使用方法：
 * - 在编译时定义 HAVE_CONFIG_H 宏以启用本配置文件
 * - 或定义 HBOX_CONFIG_HEADER 宏指定自定义配置文件路径
 */

#ifndef HBOX_CONFIG_H
#define HBOX_CONFIG_H

/* ========== hdefaults ========== */

/** \brief 默认获取节拍(毫秒)函数名称宏定义
 */
/* #define HDEFAULTS_TICK_GET */

/** \brief 默认内存分配函数名称宏定义
 */
/* #define HDEFAULTS_MALLOC */

/** \brief 默认内存释放函数名称宏定义
 */
/* #define HDEFAULTS_FREE */

/** \brief 默认互斥锁加锁函数(无参数，无返回值)名称宏定义,要求锁支持递归。
 *
 * 一般用于嵌入式编程,一般使用临界区实现。对于无操作系统的环境,可采用计数+开中断的方式实现(参考rt-thread)。
 */
/* #define HDEFAULTS_MUTEX_LOCK */

/** \brief 默认互斥锁解锁函数(无参数，无返回值)名称宏定义，要求锁支持递归。
 *
 * 一般用于嵌入式编程,一般使用临界区实现。对于无操作系统的环境,可采用计数+关中断的方式实现(参考rt-thread)。
 */
/* #define HDEFAULTS_MUTEX_UNLOCK */

/** \brief 当前环境有dlfcn.h头文件
 *
 * 使用CMake时自动判断，其它构建工具需自行根据实际情况添加。
 */
/* #define HAVE_DLFCN_H */

/** \brief 当前环境有sys/mman.h头文件
 *
 * 使用CMake时自动判断，其它构建工具需自行根据实际情况添加。
 */
/* #define HAVE_SYS_MMAN_H */

/** \brief 当前环境有unistd.h头文件
 *
 * 使用CMake时自动判断，其它构建工具需自行根据实际情况添加。
 */
/* #define HAVE_UNISTD_H */

/** \brief 当前环境有fcntl.h头文件
 *
 * 使用CMake时自动判断，其它构建工具需自行根据实际情况添加。
 */
/* #define HAVE_FCNTL_H */

/** \brief 当前环境有threads.h头文件
 *
 * 使用CMake时自动判断，其它构建工具需自行根据实际情况添加。
 */
/* #define HAVE_THREADS_H */

/** \brief 当前环境有stdatomic.h头文件
 *
 * 使用CMake时自动判断，其它构建工具需自行根据实际情况添加。
 */
/* #define HAVE_STDATOMIC_H */

/** \brief 当前环境支持FreeRTOS
 *
 * 通常用于外部支持的FreeRTOS,使用内置FreeRTOS内核(见h3rdparty)时无需定义。启用此定义后，需要在配置头文件中包含FreeRTOS头文件（由于某些环境的头文件与官方的不一致，因此需要用户手动实现）
 */
/* #define FREERTOS */

/* ========== hdefaults/libc ========== */

/** \brief 用户实现的errno变量
 *
 * 如需自行实现，一般需要定义为线程局部变量（若支持的话）。
 */
/* #define HERRNO */

/** \brief 用户实现的putchar
 *
 * 参数与返回值同hputchar
 */
/* #define HPUTCHAR */

/** \brief 用户实现的getchar
 *
 * 参数与返回值同hgetchar
 */
/* #define HGETCHAR */

/** \brief 用户实现的fputs
 *
 * 参数与返回值同hfputs
 */
/* #define HFPUTS */

/** \brief 用户实现的vfprintf
 *
 * 参数与返回值同hvfprintf
 */
/* #define HVFPRINTF */

/** \brief 用户实现的puts
 *
 * 参数与返回值同hputs
 */
/* #define HPUTS */

/** \brief 用户实现的ferror
 *
 * 参数与返回值同hferror
 */
/* #define HFERROR */

/** \brief 用户实现的fread
 *
 * 参数与返回值同hfread
 */
/* #define HFREAD */

/** \brief 用户实现的fwrite
 *
 * 参数与返回值同hfwrite
 */
/* #define HFWRITE */

/** \brief 用户实现的fclose
 *
 * 参数与返回值同hfclose
 */
/* #define HFCLOSE */

/** \brief 用户实现的fopen
 *
 * 参数与返回值同hfopen
 */
/* #define HFOPEN */

/** \brief 用户实现的fseek
 *
 * 参数与返回值同hfseek
 */
/* #define HFSEEK */

/** \brief 用户实现的getenv
 *
 * 参数与返回值同hgetenv
 */
/* #define HGETENV */

/** \brief 用户实现的abort
 *
 * 参数与返回值同habort
 */
/* #define HABORT */

/** \brief 用户实现的exit
 *
 * 参数与返回值同hexit
 */
/* #define HEXIT */

/** \brief 用户实现的time
 *
 * 参数与返回值同htime
 */
/* #define HTIME */

/** \brief 用户实现的clock
 *
 * 参数与返回值同hclock
 */
/* #define HCLOCK */

/** \brief 用户实现的strcmp
 *
 * 参数与返回值同hstrcmp
 */
/* #define HSTRCMP */

/** \brief 采用精简版strcmp
 *
 * 某些工具链的C库可能有问题，可启用此选项使用内置精简版strcmp
 */
/* #define HDEFAULTS_LIBC_TINY_STRCMP */

/** \brief 用户实现的strncmp
 *
 * 参数与返回值同hstrncmp
 */
/* #define HSTRNCMP */

/** \brief 采用精简版strncmp
 *
 * 某些工具链的C库可能有问题，可启用此选项使用内置精简版strncmp
 */
/* #define HDEFAULTS_LIBC_TINY_STRNCMP */

/** \brief 用户实现的strlen
 *
 * 参数与返回值同hstrlen
 */
/* #define HSTRLEN */

/** \brief 采用精简版strlen
 *
 * 某些工具链的C库可能有问题，可启用此选项使用内置精简版strlen
 */
/* #define HDEFAULTS_LIBC_TINY_STRLEN */

/** \brief 用户实现的memset
 *
 * 参数与返回值同hmemset
 */
/* #define HMEMSET */

/** \brief 采用精简版memset
 *
 * 某些工具链的C库可能有问题，可启用此选项使用内置精简版memset
 */
/* #define HDEFAULTS_LIBC_TINY_MEMSET */

/** \brief 支持C++的原子
 */
/* #define HAVE_CXX_ATOMIC */

/** \brief 支持C的原子
 */
/* #define HAVE_C_ATOMIC */

/** \brief 原子标志结构体
 *
 * 注意:如需使用则需要同时定义HATOMIC_FLAG_INIT用于初始化结构体。
 */
/* #define HATOMIC_FLAG */

/** \brief 用户实现的原子标志测试与设置
 *
 * 参数与返回值同hatomic_flag_test_and_set_explicit
 */
/* #define HATOMIC_FLAG_TEST_AND_SET_EXPLICIT */

/** \brief 用户实现的原子标志清除
 *
 * 参数与返回值同hatomic_flag_clear_explicit
 */
/* #define HATOMIC_FLAG_CLEAR_EXPLICIT */

/** \brief 用户可自行定义的thread_local
 *
 * 当用户已定义thread_local时使用用户定义。
 */
/* #define hthread_local */

/** \brief 支持C++的thread_local
 */
/* #define HAVE_CXX_THREAD_LOCAL */

/** \brief 支持C的thread_local
 */
/* #define HAVE_C_THREAD_LOCAL */

/** \brief 管理线程的结构体大小，单位为指针大小的倍数
 *
 * 不可小于1
 */
/* #define HTHRD_SIZE */

/** \brief 用户实现的线程创建
 *
 * 参数与返回值同hthrd_create
 */
/* #define HTHRD_CREATE */

/** \brief 用户实现的线程比较
 *
 * 参数与返回值同hthrd_equal
 */
/* #define HTHRD_EQUAL */

/** \brief 用户实现的当前线程获取
 *
 * 参数与返回值同hthrd_current
 */
/* #define HTHRD_CURRENT */

/** \brief 用户实现的线程睡眠
 *
 * 参数与返回值同hthrd_sleep
 */
/* #define HTHRD_SLEEP */

/** \brief 用户实现的让出事件片
 *
 * 参数与返回值同hthrd_yield
 */
/* #define HTHRD_YIELD */

/** \brief 用户实现的线程退出
 *
 * 参数与返回值同hthrd_exit
 */
/* #define HTHRD_EXIT */

/** \brief 用户实现的线程detach
 *
 * 参数与返回值同hthrd_detach
 */
/* #define HTHRD_DETACH */

/** \brief 用户实现的线程join
 *
 * 参数与返回值同hthrd_join
 */
/* #define HTHRD_JOIN */

/** \brief 允许在非unix(类unix)平台使用pthread
 *
 * 一般用于某些兼容pthread的环境
 */
/* #define HTHRD_USING_PTHREAD */

/** \brief 用户实现的nanosleep
 *
 * 用于pthread移植，参数与返回值同nanosleep
 */
/* #define NANOSLEEP */

/** \brief 允许使用FreeRTOS内核
 *
 * 用于嵌入式环境，用户需要在配置文件中包含FreeRTOS.h与task.h
 */
/* #define HTHRD_USING_FREERTOS */

/** \brief FreeRTOS默认栈大小
 */
/* #define HTHRD_FREERTOS_STACK_SIZE */

/** \brief FreeRTOS默认优先级
 */
/* #define HTHRD_FREERTOS_PRIORITY */

/** \brief FreeRTOS默认任务名称
 */
/* #define HTHRD_FREERTOS_NAME */

/** \brief once_flag的结构体大小，单位为指针大小的倍数
 *
 * 不可小于1.理论上只需要一个字节，但某些实现采用4字节，故仍然采用指针大小作为基本单位。
 */
/* #define HCALL_ONCE_SIZE */

/** \brief 用户实现的call_once.
 *
 * 参数与返回值同hcall_once
 */
/* #define HCALL_ONCE */

/** \brief 管理互斥锁的结构体大小，单位为指针大小的倍数
 *
 * 不可小于1
 */
/* #define HMTX_SIZE */

/** \brief 用户实现的互斥锁初始化
 *
 * 参数与返回值同hmtx_init
 */
/* #define HMTX_INIT */

/** \brief 用户实现的互斥锁加锁
 *
 * 参数与返回值同hmtx_lock
 */
/* #define HMTX_LOCK */

/** \brief 用户实现的带时间的互斥锁加锁
 *
 * 参数与返回值同hmtx_timedlock，时间采用实时时间而非单调时间
 */
/* #define HMTX_TIMEDLOCK */

/** \brief 用户实现的互斥锁尝试加锁
 *
 * 参数与返回值同hmtx_trylock
 */
/* #define HMTX_TRYLOCK */

/** \brief 用户实现的互斥锁解锁
 *
 * 参数与返回值同hmtx_unlock
 */
/* #define HMTX_UNLOCK */

/** \brief 用户实现的互斥锁销毁
 *
 * 参数与返回值同hmtx_destroy
 */
/* #define HMTX_DESTROY */

/** \brief 允许在非unix(类unix)平台使用pthread
 *
 * 一般用于某些兼容pthread的环境
 */
/* #define HMTX_USING_PTHREAD */

/** \brief 用户实现的setenv
 *
 * 参数与返回值同hsetenv
 */
/* #define HSETENV */

/** \brief 用户实现的unsetenv
 *
 * 参数与返回值同hunsetenv
 */
/* #define HUNSETENV */

/* ========== hdefaults/libc/hlibc/env ========== */

/** \brief 支持的环境变量个数
 */
/* #define HLIBC_ENV_ITEM_COUNT */

/** \brief 环境变量块大小
 *
 * 用于存储环境变量字符串
 */
/* #define HLIBC_ENV_BLOCK_SIZE */

/** \brief 不使用初始化
 *
 * 环境变量的初始化主要用于复制libc环境中的环境变量,如不进行初始化，将无法使用已有环境变量
 */
/* #define HLIBC_ENV_NO_INIT */

/* ========== hdefaults/libc/hlibc/time ========== */

/** \brief 宏函数，参数与返回值均为时区信息。
 *
 * 如不定义则使用全局时区信息
 */
/* #define HLIBC_TIME_TIMEZONE_INFO_HOOK */

/* ========== hdefaults/libdl ========== */

/** \brief 用户实现的dlopen
 *
 * 参数与返回值同hdlopen
 */
/* #define HDLOPEN */

/** \brief 用户实现的dlclose
 *
 * 参数与返回值同hdclose
 */
/* #define HDLCLOSE */

/** \brief 用户实现的dlsym
 *
 * 参数与返回值同hdsym
 */
/* #define HDLSYM */

/* ========== hdefaults/syscall ========== */

/** \brief 不实现系统调用
 *
 * 可节约部分空间，但会造成某些功能在未改变API表时失效
 */
/* #define HDEFAULTS_SYSCALL_NO_IMPLEMENTATION */

/** \brief 不使用hfiledescriptor组件实现文件描述符相关调用
 *
 * 可节约部分空间。但close、read、write、lseek、open等将在某些环境下不可用
 */
/* #define HDEFAULTS_SYSCALL_NO_HFILEDESCRIPTOR */

/** \brief 不实现hsyscall
 *
 * 可用于在编译器垃圾回收不完善时减少资源占用
 */
/* #define HSYSCALL_NO_IMPLEMENTATION */

/** \brief 不实现hsyscall的时间组件
 *
 * 可用于在编译器垃圾回收不完善时减少资源占用
 */
/* #define HSYSCALL_NO_TIME */

/** \brief 不实现hsyscall的随机数组件
 *
 * 可用于在编译器垃圾回收不完善时减少资源占用
 */
/* #define HSYSCALL_NO_RANDOM */

/** \brief 用户实现的gettimeofday
 *
 * 参数与返回值同hgettimeofday
 */
/* #define HGETTIMEOFDAY */

/** \brief 不实现gettimeofday
 */
/* #define HDEFAULTS_SYSCALL_NO_HGETTIMEOFDAY */

/** \brief 用户实现的settimeofday
 *
 * 参数与返回值同hsettimeofday
 */
/* #define HSETTIMEOFDAY */

/** \brief 不实现settimeofday
 */
/* #define HDEFAULTS_SYSCALL_NO_HSETTIMEOFDAY */

/** \brief 用户实现的getrandom
 *
 * 参数与返回值同hgetrandom
 */
/* #define HGETRANDOM */

/** \brief 不实现getrandom
 */
/* #define HDEFAULTS_SYSCALL_NO_HGETRANDOM */

/** \brief 用户实现的mmap
 *
 * 参数与返回值同hmmap,注意：文件偏移是64位无符号整数
 */
/* #define HMMAP */

/** \brief 不实现mmap
 */
/* #define HDEFAULTS_SYSCALL_NO_HMMAP */

/** \brief 用户实现的msync
 *
 * 参数与返回值同hmsync
 */
/* #define HMSYNC */

/** \brief 不实现msync
 */
/* #define HDEFAULTS_SYSCALL_NO_HMSYNC */

/** \brief 用户实现的munmap
 *
 * 参数与返回值同hmunmap
 */
/* #define HMUNMAP */

/** \brief 不实现munmap
 */
/* #define HDEFAULTS_SYSCALL_NO_HMUNMAP */

/** \brief 用户实现的mprotect
 *
 * 参数与返回值同hmprotect
 */
/* #define HMPROTECT */

/** \brief 不实现mprotect
 */
/* #define HDEFAULTS_SYSCALL_NO_HMPROTECT */

/** \brief 用户实现的close
 *
 * 参数与返回值同close
 */
/* #define HCLOSE */

/** \brief 不实现close
 */
/* #define HDEFAULTS_SYSCALL_NO_HCLOSE */

/** \brief 用户实现的read
 *
 * 参数与返回值同read
 */
/* #define HREAD */

/** \brief 不实现read
 */
/* #define HDEFAULTS_SYSCALL_NO_HREAD */

/** \brief 用户实现的write
 *
 * 参数与返回值同write
 */
/* #define HWRITE */

/** \brief 不实现write
 */
/* #define HDEFAULTS_SYSCALL_NO_HWRITE */

/** \brief 用户实现的lseek
 *
 * 参数与返回值同lseek
 */
/* #define HLSEEK */

/** \brief 不实现lseek
 */
/* #define HDEFAULTS_SYSCALL_NO_HLSEEK */

/** \brief 用户实现的open
 *
 * 参数与返回值同见具体实现
 */
/* #define HOPEN */

/** \brief 不实现open
 */
/* #define HDEFAULTS_SYSCALL_NO_HOPEN */

/** \brief 用户实现的fcntl
 *
 * 参数与返回值同见具体实现
 */
/* #define HFCNTL */

/** \brief 不实现fcntl
 */
/* #define HDEFAULTS_SYSCALL_NO_HFCNTL */

/** \brief 用户实现的openat
 *
 * 参数与返回值同见具体实现
 */
/* #define HOPENAT */

/** \brief 不实现openat
 */
/* #define HDEFAULTS_SYSCALL_NO_HOPENAT */

/** \brief 用户实现的ioctl
 *
 * 参数与返回值同见具体实现
 */
/* #define HIOCTL */

/** \brief 不实现ioctl
 */
/* #define HDEFAULTS_SYSCALL_NO_HIOCTL */

/** \brief 用户实现的clock_getres
 *
 * 参数与返回值同clock_getres
 */
/* #define HCLOCK_GETRES */

/** \brief 不实现clock_getres
 */
/* #define HDEFAULTS_SYSCALL_NO_HCLOCK_GETRES */

/** \brief 用户实现的clock_gettime
 *
 * 参数与返回值同clock_gettime
 */
/* #define HCLOCK_GETTIME */

/** \brief 不实现clock_gettime
 */
/* #define HDEFAULTS_SYSCALL_NO_HCLOCK_GETTIME */

/** \brief 用户实现的clock_settime
 *
 * 参数与返回值同clock_settime
 */
/* #define HCLOCK_SETTIME */

/** \brief 不实现clock_settime
 */
/* #define HDEFAULTS_SYSCALL_NO_HCLOCK_SETTIME */

/* ========== hdefaults/syscall/hsyscall/random ========== */

/** \brief 使用精简版getrandom
 *
 * 精简版getrandom不可用于加密类应用。
 */
/* #define HSYSCALL_GETRANDOM_TINY */

/** \brief 使用梅森旋转法生成不安全的随机数
 *
 * 梅森旋转法将消耗约2KB内存，在RAM容量较小的设备不推荐使用。
 */
/* #define HSYSCALL_GETRANDOM_USING_MT */

/* ========== hdefaults/syscall/hsyscall/time ========== */

/** \brief 时间更新宏函数，第一个参数为时间指针,第二个参数为时区指针
 *
 * gettimeofday调用时触发
 */
/* #define HSYSCALL_GETTIMEOFDAY_UPDATE */

/** \brief 时间更新宏函数，第一个参数为时间指针,第二个参数为时区指针
 *
 * settimeofday调用时触发
 */
/* #define HSYSCALL_SETTIMEOFDAY_UPDATE */

/** \brief 宏函数，参数与返回值同hsyscall_clock_getres
 */
/* #define HSYSCALL_CLOCK_GETRES_HOOK */

/** \brief 宏函数，参数与返回值同hsyscall_clock_gettime
 */
/* #define HSYSCALL_CLOCK_GETTIME_HOOK */

/** \brief 宏函数，参数与返回值同hsyscall_clock_settime
 */
/* #define HSYSCALL_CLOCK_SETTIME_HOOK */

/* ========== hevent ========== */

/** \brief 用户循环个数
 *
 * 此宏定义只影响内部的表
 */
/* #define HEVENTLOOP_USER_LOOP_COUNT */

/** \brief 不进行精简
 *
 * 默认不定义
 */
/* #define HEVENTLOOP_NO_SLIM */

/** \brief 用户槽个数
 *
 * 此宏定义只影响内部的表
 */
/* #define HEVENTSLOTS_USER_SLOTS_COUNT */

/** \brief 不进行精简
 *
 * 默认不定义
 */
/* #define HEVENTSLOTS_NO_SLIM */

/** \brief 用户链个数
 *
 * 此宏定义只影响内部的表
 */
/* #define HEVENTCHAIN_USER_CHAIN_COUNT */

/** \brief 不进行精简
 *
 * 默认不定义
 */
/* #define HEVENTCHAIN_NO_SLIM */

/* ========== hgui/hgui_gui_scene1 ========== */

/** \brief Gui驱动指针，NULL表示使用默认GUI驱动。
 */
/* #define HGUI_SCENE1_APP_DRIVER */

/** \brief 屏幕宽度，默认为128
 */
/* #define HGUI_SCENE1_APP_WIDTH */

/** \brief 屏幕 高度，默认为64
 */
/* #define HGUI_SCENE1_APP_HEIGHT */

/** \brief 屏幕栈，默认为4
 *
 * 当小于4时默认为4.
 */
/* #define HGUI_SCENE1_APP_SCREEN_STACK_DEPTH */

/** \brief 定义当应用初始化成功时执行的语句，默认为空语句块。
 *
 * 语句中可使用局部变量app访问应用数据，可使用局部变量usr访问用户自定义参数。
 */
/* #define HGUI_SCENE1_APP_ON_INIT_SUCCESS */

/** \brief 定义当应用初始化失败时执行的语句，默认为空语句块。
 *
 * 语句中可使用局部变量app访问应用数据，可使用局部变量usr访问用户自定义参数。
 */
/* #define HGUI_SCENE1_APP_ON_INIT_FAILURE */

/** \brief 定义当应用更新开始时执行的语句，默认为空语句块。
 *
 * 语句中可使用局部变量app访问应用数据，可使用局部变量usr访问用户自定义参数。
 */
/* #define HGUI_SCENE1_APP_ON_UPDATE_BEGIN */

/** \brief 定义当应用结束时执行的语句，默认为空语句块。
 *
 * 语句中可使用局部变量app访问应用数据，可使用局部变量usr访问用户自定义参数。
 */
/* #define HGUI_SCENE1_APP_ON_UPDATE_END */

/** \brief 定义当应用更新成功时执行的语句，默认为空语句块。
 *
 * 语句中可使用局部变量app访问应用数据，可使用局部变量usr访问用户自定义参数。
 */
/* #define HGUI_SCENE1_APP_ON_UPDATE_SUCCESS */

/** \brief 定义当更新成功且需要刷新时执行的语句，默认为空语句块。
 *
 * 语句中可使用局部变量app访问应用数据，可使用局部变量usr访问用户自定义参数。
 */
/* #define HGUI_SCENE1_APP_ON_UPDATE_SUCCESS_NEED_REFRESH */

/** \brief 定义当应用更新失败时执行的语句，默认为空语句块。
 *
 * 语句中可使用局部变量app访问应用数据，可使用局部变量usr访问用户自定义参数。
 */
/* #define HGUI_SCENE1_APP_ON_UPDATE_FAILURE */

/* ========== hmemory ========== */

/** \brief 对齐大小。默认为指针大小。
 *
 * 需根据对应架构的要求修改。
 */
/* #define HMEMORYHEAP_ALIGNED_SIZE */

/** \brief 魔数，用作标记是否初始化。默认为0xcccc55aa。
 */
/* #define HMEMORYHEAP_MAGIC_NUMBER */

/** \brief 指针大小。默认不定义(默认为4)。
 *
 * 对于64位系统一定要指定为8。
 */
/* #define HMEMORYHEAP_PTR_SIZE */

/** \brief 默认堆大小。至少256字节。默认不定义。
 */
/* #define HMEMORYHEAP_DEFAULT_POOL_SIZE */

/** \brief 默认堆属性，通常用于设置section属性
 */
/* #define HMEMORYHEAP_DEFAULT_POOL_ATTRIBUTE */

/** \brief 启用默认堆(默认256字节)并将其作为本工具箱默认内存分配函数。默认不定义
 */
/* #define USING_HMEMORYHEAP */

/* ========== hmemory/hfiledescriptor ========== */

/** \brief 文件描述符表中的项目个数
 */
/* #define HFILEDESCRIPTOR_COMMON_TABLE_SIZE */

/** \brief 文件描述符表的属性
 *
 * 通常用于指定section信息，可用于在启用MMU时将文件描述符表存放至指定区域。
 */
/* #define HFILEDESCRIPTOR_COMMON_TABLE_ATTRIBUTE */

/** \brief 文件描述符表标志的属性,用于存储文件描述符分配信息
 *
 * 通常用于指定section信息，可用于在启用MMU时将文件描述符表标志存放至指定区域。
 */
/* #define HFILEDESCRIPTOR_COMMON_TABLE_FLAG_ATTRIBUTE */

/** \brief 用户实现的hfiledescriptor_open
 *
 * 参数与返回值同hfiledescriptor_open
 */
/* #define HFILEDESCRIPTOR_OPEN */

/** \brief 用户实现的hfiledescriptor_openat
 *
 * 参数与返回值同hfiledescriptor_openat
 */
/* #define HFILEDESCRIPTOR_OPENAT */

/** \brief 用户实现的hfiledescriptor_socket
 *
 * 参数与返回值同hfiledescriptor_socket
 */
/* #define HFILEDESCRIPTOR_SOCKET */

/* ========== hruntime ========== */

/** \brief 启用初始化段
 *
 * 当hruntime_init被调用时将调用初始化段中的函数
 */
/* #define HRUNTIME_USING_INIT_SECTION */

/** \brief 启用循环段
 *
 * 当hruntime_loop被调用时将调用循环段中的函数
 */
/* #define HRUNTIME_USING_LOOP_SECTION */

/** \brief 循环缓存表
 *
 * 当内存充足时启用，可优化循环段调用，启用循环段时有效
 */
/* #define HRUNTIME_USING_LOOP_CACHE_TABLE */

/** \brief 循环缓存表大小
 *
 * 大小应大于循环端中函数的数量，否则在启用循环缓存表某些函数将不会执行
 */
/* #define HRUNTIME_USING_LOOP_CACHE_TABLE_ITEM_COUNT */

/** \brief 启用符号段
 *
 * 导出的符号可使用hruntime_symbol_find查找
 */
/* #define HRUNTIME_USING_SYMBOL_SECTION */

/** \brief 启用符号表
 *
 * 一般情况下，符号表与符号段任选其一即可实现符号查找
 */
/* #define HRUNTIME_USING_SYMBOL_TABLE */

/** \brief 不启用符号表
 *
 * 优先级较HRUNTIME_USING_SYMBOL_TABLE高。可有效减少生成的可执行文件大小
 */
/* #define HRUNTIME_NO_SYMBOL_TABLE */

/** \brief 启用动态符号表
 *
 * 启用动态符号表后，符号查找时优先查找动态符号表
 */
/* #define HRUNTIME_USING_SYMBOL_DYNAMIC_TABLE */

/** \brief 无h3rdparty组件
 *
 * 此选项通常用于自行调用h3rdparty组件，未定义此选项时默认由hruntime自动调用h3rdparty组件
 */
/* #define HRUNTIME_NO_H3RDPARTY */

/** \brief 无hdefaults组件
 *
 * 此选项通常用于自行调用hdefaults组件，未定义此选项时默认由hruntime自动调用hdefaults组件
 */
/* #define HRUNTIME_NO_HDEFAULTS */

/** \brief 无软件定时器
 *
 * 此选项通常用于减小代码，若启用则相关代码无效。未定义此选项时默认由hruntime自动运行软件定时器
 */
/* #define HRUNTIME_NO_SOFTWARETIMER */

/** \brief 无软件看门狗
 *
 * 此选项通常用于减小代码，若启用则相关代码无效。未定义此选项时默认由hruntime自动运行喂狗
 */
/* #define HRUNTIME_NO_SOFTWATCHDOG */

/** \brief 无软件PLC
 *
 * 此选项通常用于自行实现软件PLC的初始化及循环，未定义此选项时默认由hruntime自动运行软件PLC
 */
/* #define HRUNTIME_NO_SOFTPLC */

/** \brief 自定义软件定时器循环（用户自行调用hsoftwaretimer_mainloop）
 *
 * 通常用于多线程环境，为实现相对精确的定时，定时器线程优先级一般较高,此时用户需要自行调用hsoftwaretimer_mainloop。
 */
/* #define HSOFTWARETIMER_CUSTOM_LOOP */

#endif /* HBOX_CONFIG_H */