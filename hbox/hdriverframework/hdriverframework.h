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
#include "hdefaults.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define HDRIVERFRAMEWORK_EOK                    HEOK
#define HDRIVERFRAMEWORK_EPERM                  HEPERM
#define HDRIVERFRAMEWORK_ENOENT                 HENOENT
#define HDRIVERFRAMEWORK_ESRCH                  HESRCH
#define HDRIVERFRAMEWORK_EINTR                  HEINTR
#define HDRIVERFRAMEWORK_EIO                    HEIO
#define HDRIVERFRAMEWORK_ENXIO                  HENXIO
#define HDRIVERFRAMEWORK_E2BIG                  HE2BIG
#define HDRIVERFRAMEWORK_ENOEXEC                HENOEXEC
#define HDRIVERFRAMEWORK_EBADF                  HEBADF
#define HDRIVERFRAMEWORK_ECHILD                 HECHILD
#define HDRIVERFRAMEWORK_EAGAIN                 HEAGAIN
#define HDRIVERFRAMEWORK_ENOMEM                 HENOMEM
#define HDRIVERFRAMEWORK_EACCES                 HEACCES
#define HDRIVERFRAMEWORK_EFAULT                 HEFAULT
#define HDRIVERFRAMEWORK_ENOTBLK                HENOTBLK
#define HDRIVERFRAMEWORK_EBUSY                  HEBUSY
#define HDRIVERFRAMEWORK_EEXIST                 HEEXIST
#define HDRIVERFRAMEWORK_EXDEV                  HEXDEV
#define HDRIVERFRAMEWORK_ENODEV                 HENODEV
#define HDRIVERFRAMEWORK_ENOTDIR                HENOTDIR
#define HDRIVERFRAMEWORK_EISDIR                 HEISDIR
#define HDRIVERFRAMEWORK_EINVAL                 HEINVAL
#define HDRIVERFRAMEWORK_ENFILE                 HENFILE
#define HDRIVERFRAMEWORK_EMFILE                 HEMFILE
#define HDRIVERFRAMEWORK_ENOTTY                 HENOTTY
#define HDRIVERFRAMEWORK_ETXTBSY                HETXTBSY
#define HDRIVERFRAMEWORK_EFBIG                  HEFBIG
#define HDRIVERFRAMEWORK_ENOSPC                 HENOSPC
#define HDRIVERFRAMEWORK_ESPIPE                 HESPIPE
#define HDRIVERFRAMEWORK_EROFS                  HEROFS
#define HDRIVERFRAMEWORK_EMLINK                 HEMLINK
#define HDRIVERFRAMEWORK_EPIPE                  HEPIPE
#define HDRIVERFRAMEWORK_EDOM                   HEDOM
#define HDRIVERFRAMEWORK_ERANGE                 HERANGE
#define HDRIVERFRAMEWORK_ENOMSG                 HENOMSG
#define HDRIVERFRAMEWORK_EIDRM                  HEIDRM
#define HDRIVERFRAMEWORK_ECHRNG                 HECHRNG
#define HDRIVERFRAMEWORK_EL2NSYNC               HEL2NSYNC
#define HDRIVERFRAMEWORK_EL3HLT                 HEL3HLT
#define HDRIVERFRAMEWORK_EL3RST                 HEL3RST
#define HDRIVERFRAMEWORK_ELNRNG                 HELNRNG
#define HDRIVERFRAMEWORK_EUNATCH                HEUNATCH
#define HDRIVERFRAMEWORK_ENOCSI                 HENOCSI
#define HDRIVERFRAMEWORK_EL2HLT                 HEL2HLT
#define HDRIVERFRAMEWORK_EDEADLK                HEDEADLK
#define HDRIVERFRAMEWORK_ENOLCK                 HENOLCK
#define HDRIVERFRAMEWORK_EBADE                  HEBADE
#define HDRIVERFRAMEWORK_EBADR                  HEBADR
#define HDRIVERFRAMEWORK_EXFULL                 HEXFULL
#define HDRIVERFRAMEWORK_ENOANO                 HENOANO
#define HDRIVERFRAMEWORK_EBADRQC                HEBADRQC
#define HDRIVERFRAMEWORK_EBADSLT                HEBADSLT
#define HDRIVERFRAMEWORK_EDEADLOCK              HEDEADLOCK
#define HDRIVERFRAMEWORK_EBFONT                 HEBFONT
#define HDRIVERFRAMEWORK_ENOSTR                 HENOSTR
#define HDRIVERFRAMEWORK_ENODATA                HENODATA
#define HDRIVERFRAMEWORK_ETIME                  HETIME
#define HDRIVERFRAMEWORK_ENOSR                  HENOSR
#define HDRIVERFRAMEWORK_ENONET                 HENONET
#define HDRIVERFRAMEWORK_ENOPKG                 HENOPKG
#define HDRIVERFRAMEWORK_EREMOTE                HEREMOTE
#define HDRIVERFRAMEWORK_ENOLINK                HENOLINK
#define HDRIVERFRAMEWORK_EADV                   HEADV
#define HDRIVERFRAMEWORK_ESRMNT                 HESRMNT
#define HDRIVERFRAMEWORK_ECOMM                  HECOMM
#define HDRIVERFRAMEWORK_EPROTO                 HEPROTO
#define HDRIVERFRAMEWORK_EMULTIHOP              HEMULTIHOP
#define HDRIVERFRAMEWORK_ELBIN                  HELBIN
#define HDRIVERFRAMEWORK_EDOTDOT                HEDOTDOT
#define HDRIVERFRAMEWORK_EBADMSG                HEBADMSG
#define HDRIVERFRAMEWORK_EFTYPE                 HEFTYPE
#define HDRIVERFRAMEWORK_ENOTUNIQ               HENOTUNIQ
#define HDRIVERFRAMEWORK_EBADFD                 HEBADFD
#define HDRIVERFRAMEWORK_EREMCHG                HEREMCHG
#define HDRIVERFRAMEWORK_ELIBACC                HELIBACC
#define HDRIVERFRAMEWORK_ELIBBAD                HELIBBAD
#define HDRIVERFRAMEWORK_ELIBSCN                HELIBSCN
#define HDRIVERFRAMEWORK_ELIBMAX                HELIBMAX
#define HDRIVERFRAMEWORK_ELIBEXEC               HELIBEXEC
#define HDRIVERFRAMEWORK_ENOSYS                 HENOSYS
#define HDRIVERFRAMEWORK_ENOTEMPTY              HENOTEMPTY
#define HDRIVERFRAMEWORK_ENAMETOOLONG           HENAMETOOLONG
#define HDRIVERFRAMEWORK_ELOOP                  HELOOP
#define HDRIVERFRAMEWORK_EOPNOTSUPP             HEOPNOTSUPP
#define HDRIVERFRAMEWORK_EPFNOSUPPORT           HEPFNOSUPPORT
#define HDRIVERFRAMEWORK_ECONNRESET             HECONNRESET
#define HDRIVERFRAMEWORK_ENOBUFS                HENOBUFS
#define HDRIVERFRAMEWORK_EAFNOSUPPORT           HEAFNOSUPPORT
#define HDRIVERFRAMEWORK_EPROTOTYPE             HEPROTOTYPE
#define HDRIVERFRAMEWORK_ENOTSOCK               HENOTSOCK
#define HDRIVERFRAMEWORK_ENOPROTOOPT            HENOPROTOOPT
#define HDRIVERFRAMEWORK_ESHUTDOWN              HESHUTDOWN
#define HDRIVERFRAMEWORK_ECONNREFUSED           HECONNREFUSED
#define HDRIVERFRAMEWORK_EADDRINUSE             HEADDRINUSE
#define HDRIVERFRAMEWORK_ECONNABORTED           HECONNABORTED
#define HDRIVERFRAMEWORK_ENETUNREACH            HENETUNREACH
#define HDRIVERFRAMEWORK_ENETDOWN               HENETDOWN
#define HDRIVERFRAMEWORK_ETIMEDOUT              HETIMEDOUT
#define HDRIVERFRAMEWORK_EHOSTDOWN              HEHOSTDOWN
#define HDRIVERFRAMEWORK_EHOSTUNREACH           HEHOSTUNREACH
#define HDRIVERFRAMEWORK_EINPROGRESS            HEINPROGRESS
#define HDRIVERFRAMEWORK_EALREADY               HEALREADY
#define HDRIVERFRAMEWORK_EDESTADDRREQ           HEDESTADDRREQ
#define HDRIVERFRAMEWORK_EMSGSIZE               HEMSGSIZE
#define HDRIVERFRAMEWORK_EPROTONOSUPPORT        HEPROTONOSUPPORT
#define HDRIVERFRAMEWORK_ESOCKTNOSUPPORT        HESOCKTNOSUPPORT
#define HDRIVERFRAMEWORK_EADDRNOTAVAIL          HEADDRNOTAVAIL
#define HDRIVERFRAMEWORK_ENETRESET              HENETRESET
#define HDRIVERFRAMEWORK_EISCONN                HEISCONN
#define HDRIVERFRAMEWORK_ENOTCONN               HENOTCONN
#define HDRIVERFRAMEWORK_ETOOMANYREFS           HETOOMANYREFS
#define HDRIVERFRAMEWORK_EPROCLIM               HEPROCLIM
#define HDRIVERFRAMEWORK_EUSERS                 HEUSERS
#define HDRIVERFRAMEWORK_EDQUOT                 HEDQUOT
#define HDRIVERFRAMEWORK_ESTALE                 HESTALE
#define HDRIVERFRAMEWORK_ENOTSUP                HENOTSUP
#define HDRIVERFRAMEWORK_ENOMEDIUM              HENOMEDIUM
#define HDRIVERFRAMEWORK_EILSEQ                 HEILSEQ
#define HDRIVERFRAMEWORK_EOVERFLOW              HEOVERFLOW
#define HDRIVERFRAMEWORK_ECANCELED              HECANCELED
#define HDRIVERFRAMEWORK_ENOTRECOVERABLE        HENOTRECOVERABLE
#define HDRIVERFRAMEWORK_EOWNERDEAD             HEOWNERDEAD
#define HDRIVERFRAMEWORK_ESTRPIPE               HESTRPIPE
#define HDRIVERFRAMEWORK_EHWPOISON              HEHWPOISON
#define HDRIVERFRAMEWORK_EISNAM                 HEISNAM
#define HDRIVERFRAMEWORK_EKEYEXPIRED            HEKEYEXPIRED
#define HDRIVERFRAMEWORK_EKEYREJECTED           HEKEYREJECTED
#define HDRIVERFRAMEWORK_EKEYREVOKED            HEKEYREVOKED


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

/*
 * 基础驱动寄存器数量(最少8个)
 */
#ifndef HDRIVERFRAMEWORK_DRIVER_BASE_REGISTER_NUMBER
#if defined(HDEFAULTS_OS_UNIX) || defined(HDEFAULTS_OS_WINDOWS)
#define  HDRIVERFRAMEWORK_DRIVER_BASE_REGISTER_NUMBER    (1024)
#else
#define  HDRIVERFRAMEWORK_DRIVER_BASE_REGISTER_NUMBER    (8)
#endif
#endif

#if     HDRIVERFRAMEWORK_DRIVER_BASE_REGISTER_NUMBER  >= (8)
typedef uintptr_t hdriverframework_driver_base_register_t;
#endif


struct hdriverframework_driver_base
{
    hdriverframework_driver_base_process_t process;                                                      /**< 驱动处理函数，所有的请求均通过此函数完成请求 */
    void * usr;                                                                                          /**< 用户参数，一般情况下，由最底层的移植使用，供用户自行使用 */
#if     HDRIVERFRAMEWORK_DRIVER_BASE_REGISTER_NUMBER  >= (8)
    hdriverframework_driver_base_register_t registers[HDRIVERFRAMEWORK_DRIVER_BASE_REGISTER_NUMBER];     /**< 驱动寄存器,每个寄存器均可用于存储指针或者其它参数，与用户参数不同，驱动寄存器可由不同的抽象级别（由具体的驱动确定）使用，用于防止多个抽象级别同时使用用户参数而冲突 */
#endif
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
    HDRIVERFRAMEWORK_OP_GET_MAX_VM_OP_SIZE=6,                                   /**< 获取最大的虚拟内存操作的字节数大小（一般为2的N次方，一般使用对齐访问），其可变参数为size_t * */
    HDRIVERFRAMEWORK_OP_GET_MIN_VM_OP_SIZE=7,                                   /**< 获取最小的虚拟内存操作的字节数大小（一般为2的N次方，一般使用对齐访问），其可变参数为size_t * */
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


/** \brief 获取最大的虚拟内存操作的字节数大小
 *
 * \param drv hdriverframework_driver_base_t* 驱动指针
 * \return size_t 最大的虚拟内存操作的字节数大小,返回0表示选项不可用
 *
 */
size_t hdriverframework_driver_base_getmaxvmopsize(hdriverframework_driver_base_t *drv);


/** \brief 获取最小的虚拟内存操作的字节数大小
 *
 * \param drv hdriverframework_driver_base_t* 驱动指针
 * \return size_t 最大的虚拟内存操作的字节数大小,返回0表示选项不可用
 *
 */
size_t hdriverframework_driver_base_getminvmopsize(hdriverframework_driver_base_t *drv);


/** \brief 获取用户参数
 *
 * \param drv hdriverframework_driver_base_t* 驱动指针
 * \return void  * 用户参数，失败返回NULL
 *
 */
void * hdriverframework_driver_base_getusr(hdriverframework_driver_base_t *drv);


/** \brief 设置用户参数
 *
 * \param drv hdriverframework_driver_base_t* 驱动指针
 * \param usr void* 用户参数
 *
 */
void hdriverframework_driver_base_setusr(hdriverframework_driver_base_t *drv,void * usr);


/** \brief 获取驱动寄存器值
 *
 * \param drv hdriverframework_driver_base_t* 驱动指针
 * \param index size_t 引索
 * \param regval hdriverframework_driver_base_register_t* 寄存器值指针
 * \return bool  是否成功
 *
 */
bool hdriverframework_driver_base_getregister(hdriverframework_driver_base_t *drv,size_t index,hdriverframework_driver_base_register_t *regval);


/** \brief  设置驱动寄存器值
 *
 * \param drv hdriverframework_driver_base_t* 驱动指针
 * \param index size_t 引索
 * \param regval hdriverframework_driver_base_register_t 寄存器值指针
 * \return bool  是否成功
 *
 */
bool hdriverframework_driver_base_setregister(hdriverframework_driver_base_t *drv,size_t index,hdriverframework_driver_base_register_t regval);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HDRIVERFRAMEWORK_H__
