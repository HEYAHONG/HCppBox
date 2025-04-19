/***************************************************************
 * Name:      hshell_command.h
 * Purpose:   声明hshell_command接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-03-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSHELL_COMMAND_H__
#define __HSHELL_COMMAND_H__
#include "hcompiler.h"
#include "hdefaults.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 命令入口
 */
typedef int (*hshell_command_entry_t)(int argc,const char *argv[]);

struct hshell_command
{
    hshell_command_entry_t entry;
    const char *name;
    const char *help;
};


#if defined(HCOMPILER_ARMCC) || defined(HCOMPILER_ARMCLANG)
/*
 * armcc/armclang,使用名称为HShellCommand的section
 */

/*
 * 导出命令
 */
#define HSHELL_COMMAND_EXPORT(name,entry,help) \
    __USED\
    __SECTION("HShellCommand")\
    static const hshell_command_t hshell_commands_##name = \
    {\
        (hshell_command_entry_t)entry,\
        #name ,\
        #help \
    }

/*
 * 注册命令（注意:此宏定义会占用hshell上下文的命令数组）
 */
extern const  int HShellCommand$$Base;
extern const  int HShellCommand$$Limit;
#define HSHELL_COMMANDS_REGISTER(context) \
hshell_command_array_set(context,(hshell_command_t *)&HShellCommand$$Base,(((uintptr_t)(hshell_command_t *)&HShellCommand$$Limit)-((uintptr_t)(hshell_command_t *)&HShellCommand$$Base))/sizeof(hshell_command_t))

#elif defined(HCOMPILER_GCC) || defined(HCOMPILER_CLANG)
/*
 * gcc/clang,使用名称为.HShellCommand的section
 * gcc/clang必须在链接脚本中提供__hshell_command_start与__hshell_command_end,脚本示例如下:
 *          PROVIDE ( __hshell_command_start = . );
 *          KEEP (*(.HShellCommand))
 *          PROVIDE ( __hshell_command_end = . );
 *
 */

 /*
 * 导出命令
 */
#define HSHELL_COMMAND_EXPORT(name,entry,help) \
    __USED\
    __SECTION(".HShellCommand")\
    static const hshell_command_t hshell_commands_##name = \
    {\
        (hshell_command_entry_t)entry,\
        #name ,\
        #help \
    }

/*
 * 注册命令（注意:此宏定义会占用hshell上下文的命令数组）
 */
extern const  hshell_command_t __hshell_command_start[];
extern const  hshell_command_t __hshell_command_end[];
#define HSHELL_COMMANDS_REGISTER(context) \
hshell_command_array_set(context,__hshell_command_start,(((uintptr_t)__hshell_command_end)-((uintptr_t)__hshell_command_start))/sizeof(hshell_command_t))




#else
/*
 * 不支持的编译器（使用相关宏定义将无任何效果，也不会报错）
 */


/*
 * 导出命令
 */
#define HSHELL_COMMAND_EXPORT(name,entry,help)

/*
 * 注册命令（注意:此宏定义会占用hsell上下文的命令数组）
 */
#define HSHELL_COMMANDS_REGISTER(context)


#endif


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HSHELL_COMMAND_H__
