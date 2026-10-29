
/*
 * 保存之前的选项
 */
#pragma save

/*
 * 使用SDCC99
 */
#pragma std_sdcc99

/*
 * C99头文件
 */
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"


/*
 * 基础组件(头文件)
 */
#include "base/registers.h"
#include "base/model.h"
#include "base/globalconfig.h"
#include "base/core.h"

/*
 * 运行时（头文件）
 */
#include "runtime/criticalsection.h"
#include "runtime/tick.h"
#include "runtime/mainloop.h"
#include "runtime/libc.h"

/*
 * shell(头文件)
 */
#include "shell/shell.h"

/*
 * 基础组件(实现文件)
 */
#ifndef LIBMONO_BASE_CORE_NO_IMPLEMENT
#include "base/core.c"
#endif // LIBMONO_BASE_CORE_NO_IMPLEMENT


/*
 * 运行时（实现文件）
 */
#ifndef LIBMONO_RUNTIME_CRITICALSECTION_NO_IMPLEMENT
#include "runtime/criticalsection.c"
#endif // LIBMONO_RUNTIME_CRITICALSECTION_NO_IMPLEMENT

#ifndef LIBMONO_RUNTIME_TICK_NO_IMPLEMENT
#include "runtime/tick.c"
#endif // LIBMONO_RUNTIME_TICK_NO_IMPLEMENT

#ifndef LIBMONO_RUNTIME_MAINLOOP_NO_IMPLEMENT
#include "runtime/mainloop.c"
#endif // LIBMONO_RUNTIME_MAINLOOP_NO_IMPLEMENT

#ifndef LIBMONO_RUNTIME_LIBC_NO_IMPLEMENT
#include "runtime/libc.c"
#endif // LIBMONO_RUNTIME_LIBC_NO_IMPLEMENT

/*
 * shell(实现文件)
 */
#ifndef LIBMONO_SHELL_SHELL_NO_IMPLEMENT
#include "shell/shell.c"
#endif // LIBMONO_SHELL_SHELL_NO_IMPLEMENT

/*
 * 恢复之前的选项
 */
#pragma restore
