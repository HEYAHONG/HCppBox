/***************************************************************
 * Name:      hioctl.h
 * Purpose:   声明hioctl接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-02-09
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HIOCTL_H__
#define __HIOCTL_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdarg.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#if defined(HDEFAULTS_OS_UNIX) || defined(HAVE_UNISTD_H)
#include "unistd.h"
#endif
#if defined(HDEFAULTS_OS_UNIX) || defined(HAVE_SYS_IOCTL_H)
#include "sys/ioctl.h"
#endif

/* ioctl command encoding: 32 bits total, command in lower 16 bits,
 * size of the parameter structure in the lower 14 bits of the
 * upper 16 bits.
 * Encoding the size of the parameter structure in the ioctl request
 * is useful for catching programs compiled with old versions
 * and to avoid overwriting user space outside the user buffer area.
 * The highest 2 bits are reserved for indicating the ``access mode''.
 * NOTE: This limits the max parameter size to 16kB -1 !
 */

/*
 * The following is for compatibility across the various Linux
 * platforms.  The generic ioctl numbering scheme doesn't really enforce
 * a type field.  De facto, however, the top 8 bits of the lower 16
 * bits are indeed used as a type field, so we might just as well make
 * this explicit here.  Please be sure to use the decoding macros
 * below from now on.
 */
#if !defined(_IOC_NRBITS)
#define __HIOC_NRBITS     8
#else
#define __HIOC_NRBITS     _IOC_NRBITS
#endif
#if !defined(_IOC_TYPEBITS)
#define __HIOC_TYPEBITS   8
#else
#define __HIOC_TYPEBITS   _IOC_TYPEBITS
#endif

/*
 * Let any architecture override either of the following before
 * including this file.
 */

#if !defined(_IOC_SIZEBITS)
#define __HIOC_SIZEBITS  14
#else
#define __HIOC_SIZEBITS  _IOC_SIZEBITS
#endif

#if !defined(_IOC_DIRBITS)
#define __HIOC_DIRBITS   2
#else
#define __HIOC_DIRBITS   _IOC_DIRBITS
#endif

#define __HIOC_NRMASK     ((1 << __HIOC_NRBITS)-1)
#define __HIOC_TYPEMASK   ((1 << __HIOC_TYPEBITS)-1)
#define __HIOC_SIZEMASK   ((1 << __HIOC_SIZEBITS)-1)
#define __HIOC_DIRMASK    ((1 << __HIOC_DIRBITS)-1)

#define __HIOC_NRSHIFT    0
#define __HIOC_TYPESHIFT  (__HIOC_NRSHIFT+__HIOC_NRBITS)
#define __HIOC_SIZESHIFT  (__HIOC_TYPESHIFT+__HIOC_TYPEBITS)
#define __HIOC_DIRSHIFT   (__HIOC_SIZESHIFT+__HIOC_SIZEBITS)

/*
 * Direction bits, which any architecture can choose to override
 * before including this file.
 *
 * NOTE: __HIOC_WRITE means userland is writing and kernel is
 * reading. __HIOC_READ means userland is reading and kernel is writing.
 */

#ifndef __HIOC_NONE
# define __HIOC_NONE      0U
#endif

#ifndef __HIOC_WRITE
# define __HIOC_WRITE     1U
#endif

#ifndef __HIOC_READ
# define __HIOC_READ      2U
#endif

#define __HIOC(dir,type,nr,size) \
        (((dir)  << __HIOC_DIRSHIFT) | \
         ((type) << __HIOC_TYPESHIFT) | \
         ((nr)   << __HIOC_NRSHIFT) | \
         ((size) << __HIOC_SIZESHIFT))

#define __HIOC_TYPECHECK(t) (sizeof(t))

/*
 * Used to create numbers.
 *
 * NOTE: _IOW means userland is writing and kernel is reading. _IOR
 * means userland is reading and kernel is writing.
 */
#define __HIO(type,nr)            __HIOC(__HIOC_NONE,(type),(nr),0)
#define __HIOR(type,nr,size)      __HIOC(__HIOC_READ,(type),(nr),(__HIOC_TYPECHECK(size)))
#define __HIOW(type,nr,size)      __HIOC(__HIOC_WRITE,(type),(nr),(__HIOC_TYPECHECK(size)))
#define __HIOWR(type,nr,size)     __HIOC(__HIOC_READ|__HIOC_WRITE,(type),(nr),(__HIOC_TYPECHECK(size)))
#define __HIOR_BAD(type,nr,size)  __HIOC(__HIOC_READ,(type),(nr),sizeof(size))
#define __HIOW_BAD(type,nr,size)  __HIOC(__HIOC_WRITE,(type),(nr),sizeof(size))
#define __HIOWR_BAD(type,nr,size) __HIOC(__HIOC_READ|__HIOC_WRITE,(type),(nr),sizeof(size))

/* used to decode ioctl numbers.. */
#define __HIOC_DIR(nr)            (((nr) >> __HIOC_DIRSHIFT) & __HIOC_DIRMASK)
#define __HIOC_TYPE(nr)           (((nr) >> __HIOC_TYPESHIFT) & __HIOC_TYPEMASK)
#define __HIOC_NR(nr)             (((nr) >> __HIOC_NRSHIFT) & __HIOC_NRMASK)
#define __HIOC_SIZE(nr)           (((nr) >> __HIOC_SIZESHIFT) & __HIOC_SIZEMASK)

/* ...and for the drivers/sound files... */

#define HIOC_IN          (__HIOC_WRITE << __HIOC_DIRSHIFT)
#define HIOC_OUT         (__HIOC_READ << __HIOC_DIRSHIFT)
#define HIOC_INOUT       ((__HIOC_WRITE|__HIOC_READ) << __HIOC_DIRSHIFT)
#define HIOCSIZE_MASK    (__HIOC_SIZEMASK << __HIOC_SIZESHIFT)
#define HIOCSIZE_SHIFT   (__HIOC_SIZESHIFT)



/** \brief IO控制.
 * 注意：在有操作系统的情况下，仍然推荐使用原操作系统的ioctl
 * \param fd int 文件描述符
 * \param op unsigned long 操作
 * \param ... 其它参数.一般有三种情况，无参数，无符号整型，指针
 * \return int 成功返回0
 *
 */
int hioctl(int fd, unsigned long op, ...);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HIOCTL_H__
