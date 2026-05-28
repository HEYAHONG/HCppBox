
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
#include "runtime/libc.h"

/*
 * 基础组件(实现文件)
 */
#include "base/core.c"


/*
 * 运行时（实现文件）
 */
#include "runtime/criticalsection.c"
#include "runtime/tick.c"
#include "runtime/libc.c"

/*
 * 恢复之前的选项
 */
#pragma restore
