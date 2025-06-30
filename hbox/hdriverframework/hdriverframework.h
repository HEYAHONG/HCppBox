/***************************************************************
 * Name:      hdriverframework.h
 * Purpose:   声明hdriverframework接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-27
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDRIVERFRAMEWORK_H__
#define __HDRIVERFRAMEWORK_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"
#include "hmemory.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define HDRIVERFRAMEWORK_EOK                    0
#define HDRIVERFRAMEWORK_EPERM                  1
#define HDRIVERFRAMEWORK_ENOENT                 2
#define HDRIVERFRAMEWORK_ESRCH                  3
#define HDRIVERFRAMEWORK_EINTR                  4
#define HDRIVERFRAMEWORK_EIO                    5
#define HDRIVERFRAMEWORK_ENXIO                  6
#define HDRIVERFRAMEWORK_E2BIG                  7
#define HDRIVERFRAMEWORK_ENOEXEC                8
#define HDRIVERFRAMEWORK_EBADF                  9
#define HDRIVERFRAMEWORK_ECHILD                 10
#define HDRIVERFRAMEWORK_EAGAIN                 11
#define HDRIVERFRAMEWORK_ENOMEM                 12
#define HDRIVERFRAMEWORK_EACCES                 13
#define HDRIVERFRAMEWORK_EFAULT                 14
#define HDRIVERFRAMEWORK_ENOTBLK                15
#define HDRIVERFRAMEWORK_EBUSY                  16
#define HDRIVERFRAMEWORK_EEXIST                 17
#define HDRIVERFRAMEWORK_EXDEV                  18
#define HDRIVERFRAMEWORK_ENODEV                 19
#define HDRIVERFRAMEWORK_ENOTDIR                20
#define HDRIVERFRAMEWORK_EISDIR                 21
#define HDRIVERFRAMEWORK_EINVAL                 22
#define HDRIVERFRAMEWORK_ENFILE                 23
#define HDRIVERFRAMEWORK_EMFILE                 24
#define HDRIVERFRAMEWORK_ENOTTY                 25
#define HDRIVERFRAMEWORK_ETXTBSY                26
#define HDRIVERFRAMEWORK_EFBIG                  27
#define HDRIVERFRAMEWORK_ENOSPC                 28
#define HDRIVERFRAMEWORK_ESPIPE                 29
#define HDRIVERFRAMEWORK_EROFS                  30
#define HDRIVERFRAMEWORK_EMLINK                 31
#define HDRIVERFRAMEWORK_EPIPE                  32
#define HDRIVERFRAMEWORK_EDOM                   33
#define HDRIVERFRAMEWORK_ERANGE                 34
#define HDRIVERFRAMEWORK_ENOMSG                 35
#define HDRIVERFRAMEWORK_EIDRM                  36
#define HDRIVERFRAMEWORK_ECHRNG                 37
#define HDRIVERFRAMEWORK_EL2NSYNC               38
#define HDRIVERFRAMEWORK_EL3HLT                 39
#define HDRIVERFRAMEWORK_EL3RST                 40
#define HDRIVERFRAMEWORK_ELNRNG                 41
#define HDRIVERFRAMEWORK_EUNATCH                42
#define HDRIVERFRAMEWORK_ENOCSI                 43
#define HDRIVERFRAMEWORK_EL2HLT                 44
#define HDRIVERFRAMEWORK_EDEADLK                45
#define HDRIVERFRAMEWORK_ENOLCK                 46
#define HDRIVERFRAMEWORK_EBADE                  50
#define HDRIVERFRAMEWORK_EBADR                  51
#define HDRIVERFRAMEWORK_EXFULL                 52
#define HDRIVERFRAMEWORK_ENOANO                 53
#define HDRIVERFRAMEWORK_EBADRQC                54
#define HDRIVERFRAMEWORK_EBADSLT                55
#define HDRIVERFRAMEWORK_EDEADLOCK              56
#define HDRIVERFRAMEWORK_EBFONT                 57
#define HDRIVERFRAMEWORK_ENOSTR                 60
#define HDRIVERFRAMEWORK_ENODATA                61
#define HDRIVERFRAMEWORK_ETIME                  62
#define HDRIVERFRAMEWORK_ENOSR                  63
#define HDRIVERFRAMEWORK_ENONET                 64
#define HDRIVERFRAMEWORK_ENOPKG                 65
#define HDRIVERFRAMEWORK_EREMOTE                66
#define HDRIVERFRAMEWORK_ENOLINK                67
#define HDRIVERFRAMEWORK_EADV                   68
#define HDRIVERFRAMEWORK_ESRMNT                 69
#define HDRIVERFRAMEWORK_ECOMM                  70
#define HDRIVERFRAMEWORK_EPROTO                 71
#define HDRIVERFRAMEWORK_EMULTIHOP              74
#define HDRIVERFRAMEWORK_ELBIN                  75
#define HDRIVERFRAMEWORK_EDOTDOT                76
#define HDRIVERFRAMEWORK_EBADMSG                77
#define HDRIVERFRAMEWORK_EFTYPE                 79
#define HDRIVERFRAMEWORK_ENOTUNIQ               80
#define HDRIVERFRAMEWORK_EBADFD                 81
#define HDRIVERFRAMEWORK_EREMCHG                82
#define HDRIVERFRAMEWORK_ELIBACC                83
#define HDRIVERFRAMEWORK_ELIBBAD                84
#define HDRIVERFRAMEWORK_ELIBSCN                85
#define HDRIVERFRAMEWORK_ELIBMAX                86
#define HDRIVERFRAMEWORK_ELIBEXEC               87
#define HDRIVERFRAMEWORK_ENOSYS                 88
#define HDRIVERFRAMEWORK_ENOTEMPTY              90
#define HDRIVERFRAMEWORK_ENAMETOOLONG           91
#define HDRIVERFRAMEWORK_ELOOP                  92
#define HDRIVERFRAMEWORK_EOPNOTSUPP             95
#define HDRIVERFRAMEWORK_EPFNOSUPPORT           96
#define HDRIVERFRAMEWORK_ECONNRESET             104
#define HDRIVERFRAMEWORK_ENOBUFS                105
#define HDRIVERFRAMEWORK_EAFNOSUPPORT           106
#define HDRIVERFRAMEWORK_EPROTOTYPE             107
#define HDRIVERFRAMEWORK_ENOTSOCK               108
#define HDRIVERFRAMEWORK_ENOPROTOOPT            109
#define HDRIVERFRAMEWORK_ESHUTDOWN              110
#define HDRIVERFRAMEWORK_ECONNREFUSED           111
#define HDRIVERFRAMEWORK_EADDRINUSE             112
#define HDRIVERFRAMEWORK_ECONNABORTED           113
#define HDRIVERFRAMEWORK_ENETUNREACH            114
#define HDRIVERFRAMEWORK_ENETDOWN               115
#define HDRIVERFRAMEWORK_ETIMEDOUT              116
#define HDRIVERFRAMEWORK_EHOSTDOWN              117
#define HDRIVERFRAMEWORK_EHOSTUNREACH           118
#define HDRIVERFRAMEWORK_EINPROGRESS            119
#define HDRIVERFRAMEWORK_EALREADY               120
#define HDRIVERFRAMEWORK_EDESTADDRREQ           121
#define HDRIVERFRAMEWORK_EMSGSIZE               122
#define HDRIVERFRAMEWORK_EPROTONOSUPPORT        123
#define HDRIVERFRAMEWORK_ESOCKTNOSUPPORT        124
#define HDRIVERFRAMEWORK_EADDRNOTAVAIL          125
#define HDRIVERFRAMEWORK_ENETRESET              126
#define HDRIVERFRAMEWORK_EISCONN                127
#define HDRIVERFRAMEWORK_ENOTCONN               128
#define HDRIVERFRAMEWORK_ETOOMANYREFS           129
#define HDRIVERFRAMEWORK_EPROCLIM               130
#define HDRIVERFRAMEWORK_EUSERS                 131
#define HDRIVERFRAMEWORK_EDQUOT                 132
#define HDRIVERFRAMEWORK_ESTALE                 133
#define HDRIVERFRAMEWORK_ENOTSUP                134
#define HDRIVERFRAMEWORK_ENOMEDIUM              135
#define HDRIVERFRAMEWORK_EILSEQ                 138
#define HDRIVERFRAMEWORK_EOVERFLOW              139
#define HDRIVERFRAMEWORK_ECANCELED              140
#define HDRIVERFRAMEWORK_ENOTRECOVERABLE        141
#define HDRIVERFRAMEWORK_EOWNERDEAD             142
#define HDRIVERFRAMEWORK_ESTRPIPE               143
#define HDRIVERFRAMEWORK_EHWPOISON              144
#define HDRIVERFRAMEWORK_EISNAM                 145
#define HDRIVERFRAMEWORK_EKEYEXPIRED            146
#define HDRIVERFRAMEWORK_EKEYREJECTED           147
#define HDRIVERFRAMEWORK_EKEYREVOKED            148

/*
 * 一般情况下，编写驱动就是注册一系列信息（如回调函数等），并在回调函数中完成相应硬件功能（如Windows的派遣函数、Linux的驱动结构体等）。
 * 本框架中，只有一个回调函数，具体操作通过枚举值区分。
 */

typedef struct hdriverframework_driver_base hdriverframework_driver_base_t;

/** \brief 驱动处理函数
 *
 * \param drv hdriverframework_driver_base_t* 驱动指针
 * \param op int 操作枚举值
 * \return int 返回值，见HDRIVERFRAMEWORK_E*
 *
 */
typedef int (*hdriverframework_driver_base_process_t)(hdriverframework_driver_base_t *drv,int op,...);


struct hdriverframework_driver_base
{
    hdriverframework_driver_base_process_t process;     /**< 驱动处理函数，所有的请求均通过此函数完成请求 */
    void * usr;                                         /**< 用户参数供用户自行使用 */
};

enum
{
    HDRIVERFRAMEWORK_OP_GETNAME=0,                                              /**< 获取驱动名称，其可变参数为const char **,某些情况下，可通过名称加载对应驱动*/
    HDRIVERFRAMEWORK_OP_INIT=1,                                                 /**< 初始化,无可变参数 */
    HDRIVERFRAMEWORK_OP_PROBE=HDRIVERFRAMEWORK_OP_INIT,                         /**< 驱动探测，同HDRIVERFRAMEWORK_OP_INIT */
    HDRIVERFRAMEWORK_OP_DEINIT=2,                                               /**< 反初始化，无可变参数,通常用于可卸载的驱动*/
    HDRIVERFRAMEWORK_OP_REMOVE=HDRIVERFRAMEWORK_OP_DEINIT,                      /**< 驱动移除,同HDRIVERFRAMEWORK_OP_DEINIT */
    HDRIVERFRAMEWORK_OP_GET_OF_COMPATIBLE=3,                                    /**< 获取设备树(Open Firmware Device Tree)的兼容字符串数据(必须以NULL结尾)，其可变参数为const char ***,在使用设备树的情况下,用于加载驱动 */
    HDRIVERFRAMEWORK_OP_VM_READ=4,                                              /**< 虚拟内存读,相关参数见hsoftwarevirtualmemory,当驱动有可映射至虚拟内存的可编址资源时，可使用此操作 */
    HDRIVERFRAMEWORK_OP_VM_WRITE=5,                                             /**< 虚拟内存写,相关参数见hsoftwarevirtualmemory,当驱动有可映射至虚拟内存的可编址资源时，可使用此操作 */
    HDRIVERFRAMEWORK_OP_DRIVER_BASE=0x10000,                                    /**< 驱动操作起始,由具体的驱动使用 */
};

/** \brief 驱动设置回调函数
 *
 * \param drv hdriverframework_driver_base_t* 驱动指针
 * \param process hdriverframework_driver_base_process_t 驱动处理函数
 * \param usr void* 用户参数
 * \return hdriverframework_driver_base_t* 驱动指针
 *
 */
hdriverframework_driver_base_t *hdriverframework_driver_base_set(hdriverframework_driver_base_t *drv,hdriverframework_driver_base_process_t process,void * usr);

/** \brief 获取驱动名称
 *
 * \param drv hdriverframework_driver_base_t* 驱动指针
 * \return const char* 名称,失败将返回NULL
 *
 */
const char * hdriverframework_driver_base_getname(hdriverframework_driver_base_t *drv);

/** \brief 驱动初始化
 *
 * \param drv hdriverframework_driver_base_t* 驱动指针
 * \return int 错误代码
 *
 */
int hdriverframework_driver_base_init(hdriverframework_driver_base_t *drv);
#define hdriverframework_driver_base_probe hdriverframework_driver_base_init

/** \brief 驱动反初始化
 *
 * \param drv hdriverframework_driver_base_t* 驱动指针
 * \return int 错误代码
 *
 */
int hdriverframework_driver_base_deinit(hdriverframework_driver_base_t *drv);
#define hdriverframework_driver_base_remove hdriverframework_driver_base_deinit


/** \brief 获取设备树兼容字符串数组(以NULL结尾)
 *
 * \param drv hdriverframework_driver_base_t* 驱动指针
 * \return const char** 设备树兼容字符串数组(以NULL结尾)，失败返回NULL
 *
 */
const char ** hdriverframework_driver_base_get_of_compatible(hdriverframework_driver_base_t *drv);

/** \brief 匹配设备树兼容字符串
 *
 * \param drv hdriverframework_driver_base_t* 驱动指针
 * \param of_compatible const char* 待匹配的设备树兼容字符串
 * \return bool 是否匹配
 *
 */
bool hdriverframework_driver_base_match_of_compatible(hdriverframework_driver_base_t *drv,const char *of_compatible);


/** \brief 虚拟内存读
 *
 * \param drv hdriverframework_driver_base_t* 驱动指针
 * \param map_item const hsoftwarevirtualmemory_map_item_t* 虚拟内存的映射表项
 * \param address uintptr_t 虚拟内存地址
 * \param data uint8_t* 读取缓冲地址
 * \param length size_t 读取缓冲长度
 * \return size_t 已读取的大小
 *
 */
size_t hdriverframework_driver_base_vm_read(hdriverframework_driver_base_t *drv,const hsoftwarevirtualmemory_map_item_t *map_item,uintptr_t address,uint8_t *data,size_t length);

/** \brief 虚拟内存写
 *
 * \param drv hdriverframework_driver_base_t* 驱动指针
 * \param map_item const hsoftwarevirtualmemory_map_item_t* 虚拟内存的映射表项
 * \param address uintptr_t 虚拟内存地址
 * \param data const uint8_t* 写入缓冲地址
 * \param length size_t 写入缓冲长度
 * \return size_t 已写入的大小
 *
 */
size_t hdriverframework_driver_base_vm_write(hdriverframework_driver_base_t *drv,const hsoftwarevirtualmemory_map_item_t *map_item,uintptr_t address,const uint8_t *data,size_t length);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HDRIVERFRAMEWORK_H__
