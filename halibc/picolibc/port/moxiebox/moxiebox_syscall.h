/***************************************************************
 * Name:      moxiebox_syscall.h
 * Purpose:   定义moxiebox系统调用封装(供picolibc使用)
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#ifndef __MOXIEBOX_SYSCALL_H__
#define __MOXIEBOX_SYSCALL_H__

#include <stddef.h>
#include <stdint.h>

/*
 * moxiebox system call numbers (same as the 32-bit Linux ABI).
 */
#define MOXIEBOX_SYS_EXIT           1
#define MOXIEBOX_SYS_READ           3
#define MOXIEBOX_SYS_WRITE          4
#define MOXIEBOX_SYS_OPEN           5
#define MOXIEBOX_SYS_CLOSE          6
#define MOXIEBOX_SYS_LSEEK          19
#define MOXIEBOX_SYS_IOCTL          54
#define MOXIEBOX_SYS_FCNTL          55
#define MOXIEBOX_SYS_GETTIMEOFDAY   78
#define MOXIEBOX_SYS_MMAP           90
#define MOXIEBOX_SYS_MUNMAP         91
#define MOXIEBOX_SYS_MPROTECT       125
#define MOXIEBOX_SYS_CLOCK_SETTIME  264
#define MOXIEBOX_SYS_CLOCK_GETTIME  265
#define MOXIEBOX_SYS_CLOCK_GETRES   266
#define MOXIEBOX_SYS_OPENAT         295
#define MOXIEBOX_SYS_GETRANDOM      355

/*
 * moxiebox open flags.
 */
#define MOXIEBOX_O_RDONLY       0
#define MOXIEBOX_O_WRONLY       1
#define MOXIEBOX_O_RDWR         2
#define MOXIEBOX_O_ACCMODE      3
#define MOXIEBOX_O_CREAT        0100
#define MOXIEBOX_O_EXCL         0200
#define MOXIEBOX_O_NOCTTY       0400
#define MOXIEBOX_O_TRUNC        01000
#define MOXIEBOX_O_APPEND       02000
#define MOXIEBOX_O_NONBLOCK     04000
#define MOXIEBOX_O_DSYNC        010000
#define MOXIEBOX_O_DIRECT       040000
#define MOXIEBOX_O_LARGEFILE    0100000
#define MOXIEBOX_O_DIRECTORY    0200000
#define MOXIEBOX_O_NOFOLLOW     0400000
#define MOXIEBOX_O_NOATIME      01000000
#define MOXIEBOX_O_CLOEXEC      02000000
#define MOXIEBOX_O_SYNC         04010000
#define MOXIEBOX_O_PATH         010000000
#define MOXIEBOX_O_TMPFILE      020200000

/*
 * moxiebox mmap protection and flag values.
 */
#define MOXIEBOX_PROT_EXEC      (1UL << 0)
#define MOXIEBOX_PROT_READ      (1UL << 1)
#define MOXIEBOX_PROT_WRITE     (1UL << 2)

#define MOXIEBOX_MAP_SHARED     (1UL << 0)
#define MOXIEBOX_MAP_PRIVATE    (1UL << 1)
#define MOXIEBOX_MAP_ANONYMOUS  (1UL << 2)

/*
 * moxiebox fcntl commands and fd flags.
 */
#define MOXIEBOX_F_DUPFD            0
#define MOXIEBOX_F_GETFD            1
#define MOXIEBOX_F_SETFD            2
#define MOXIEBOX_F_GETFL            3
#define MOXIEBOX_F_SETFL            4
#define MOXIEBOX_F_DUPFD_CLOEXEC    1030
#define MOXIEBOX_FD_CLOEXEC         1

/*
 * moxiebox special directory fd.
 */
#define MOXIEBOX_AT_FDCWD           (-100)

/*
 * moxiebox clock ids.
 */
#define MOXIEBOX_CLOCK_REALTIME             0
#define MOXIEBOX_CLOCK_MONOTONIC            1
#define MOXIEBOX_CLOCK_PROCESS_CPUTIME_ID   2
#define MOXIEBOX_CLOCK_THREAD_CPUTIME_ID    3
#define MOXIEBOX_CLOCK_MONOTONIC_RAW        4
#define MOXIEBOX_CLOCK_REALTIME_COARSE      5
#define MOXIEBOX_CLOCK_MONOTONIC_COARSE     6
#define MOXIEBOX_CLOCK_BOOTTIME             7
#define MOXIEBOX_CLOCK_REALTIME_ALARM       8
#define MOXIEBOX_CLOCK_BOOTTIME_ALARM       9

static inline void
__moxiebox_exit(int status)
{
    register long r0 __asm__("$r0") = status;

    __asm__ volatile("swi 1" : : "r"(r0) : "memory");

    for (;;) {
    }
}

static inline long
__moxiebox_read(int fd, void *buf, size_t count)
{
    register long r0 __asm__("$r0") = fd;
    register void *r1 __asm__("$r1") = buf;
    register size_t r2 __asm__("$r2") = count;

    __asm__ volatile("swi 3" : "+r"(r0) : "r"(r1), "r"(r2) : "memory");
    return r0;
}

static inline long
__moxiebox_write(int fd, const void *buf, size_t count)
{
    register long r0 __asm__("$r0") = fd;
    register const void *r1 __asm__("$r1") = buf;
    register size_t r2 __asm__("$r2") = count;

    __asm__ volatile("swi 4" : "+r"(r0) : "r"(r1), "r"(r2) : "memory");
    return r0;
}

static inline long
__moxiebox_open(const char *path, int flags, int mode)
{
    register long r0 __asm__("$r0") = (long)(uintptr_t)path;
    register long r1 __asm__("$r1") = flags;
    register long r2 __asm__("$r2") = mode;

    __asm__ volatile("swi 5" : "+r"(r0) : "r"(r1), "r"(r2) : "memory");
    return r0;
}

static inline long
__moxiebox_openat(int dirfd, const char *path, int flags, int mode)
{
    register long r0 __asm__("$r0") = dirfd;
    register const char *r1 __asm__("$r1") = path;
    register long r2 __asm__("$r2") = flags;
    register long r3 __asm__("$r3") = mode;

    __asm__ volatile("swi 295"
                     : "+r"(r0)
                     : "r"(r1), "r"(r2), "r"(r3)
                     : "memory");
    return r0;
}

static inline long
__moxiebox_close(int fd)
{
    register long r0 __asm__("$r0") = fd;

    __asm__ volatile("swi 6" : "+r"(r0) : : "memory");
    return r0;
}

static inline long
__moxiebox_ioctl(int fd, unsigned long request, void *param)
{
    register long r0 __asm__("$r0") = fd;
    register unsigned long r1 __asm__("$r1") = request;
    register void *r2 __asm__("$r2") = param;

    __asm__ volatile("swi 54" : "+r"(r0) : "r"(r1), "r"(r2) : "memory");
    return r0;
}

static inline long
__moxiebox_fcntl(int fd, int cmd, long arg)
{
    register long r0 __asm__("$r0") = fd;
    register long r1 __asm__("$r1") = cmd;
    register long r2 __asm__("$r2") = arg;

    __asm__ volatile("swi 55" : "+r"(r0) : "r"(r1), "r"(r2) : "memory");
    return r0;
}

static inline long
__moxiebox_lseek(int fd, long offset, int whence)
{
    register long r0 __asm__("$r0") = fd;
    register long r1 __asm__("$r1") = offset;
    register long r2 __asm__("$r2") = whence;

    __asm__ volatile("swi 19" : "+r"(r0) : "r"(r1), "r"(r2) : "memory");
    return r0;
}

static inline long
__moxiebox_gettimeofday(void *tv, void *tz)
{
    register long r0 __asm__("$r0") = (long)(uintptr_t)tv;
    register long r1 __asm__("$r1") = (long)(uintptr_t)tz;

    __asm__ volatile("swi 78" : "+r"(r0) : "r"(r1) : "memory");
    return r0;
}

static inline long
__moxiebox_clock_settime(long clockid, const void *tp)
{
    register long r0 __asm__("$r0") = clockid;
    register const void *r1 __asm__("$r1") = tp;

    __asm__ volatile("swi 264" : "+r"(r0) : "r"(r1) : "memory");
    return r0;
}

static inline long
__moxiebox_clock_gettime(long clockid, void *tp)
{
    register long r0 __asm__("$r0") = clockid;
    register void *r1 __asm__("$r1") = tp;

    __asm__ volatile("swi 265" : "+r"(r0) : "r"(r1) : "memory");
    return r0;
}

static inline long
__moxiebox_clock_getres(long clockid, void *res)
{
    register long r0 __asm__("$r0") = clockid;
    register void *r1 __asm__("$r1") = res;

    __asm__ volatile("swi 266" : "+r"(r0) : "r"(r1) : "memory");
    return r0;
}

static inline long
__moxiebox_mprotect(void *addr, size_t length, int prot)
{
    register long r0 __asm__("$r0") = (long)(uintptr_t)addr;
    register size_t r1 __asm__("$r1") = length;
    register long r2 __asm__("$r2") = prot;

    __asm__ volatile("swi 125" : "+r"(r0) : "r"(r1), "r"(r2) : "memory");
    return r0;
}

static inline long
__moxiebox_mmap(void *addr, size_t length, int prot, int flags, int fd, long offset)
{
    register long r0 __asm__("$r0") = (long)(uintptr_t)addr;
    register size_t r1 __asm__("$r1") = length;
    register long r2 __asm__("$r2") = prot;
    register long r3 __asm__("$r3") = flags;
    register long r4 __asm__("$r4") = fd;
    register long r5 __asm__("$r5") = offset;

    __asm__ volatile("swi 90"
                     : "+r"(r0)
                     : "r"(r1), "r"(r2), "r"(r3), "r"(r4), "r"(r5)
                     : "memory");
    return r0;
}

static inline long
__moxiebox_munmap(void *addr, size_t length)
{
    register long r0 __asm__("$r0") = (long)(uintptr_t)addr;
    register size_t r1 __asm__("$r1") = length;

    __asm__ volatile("swi 91" : "+r"(r0) : "r"(r1) : "memory");
    return r0;
}

static inline long
__moxiebox_getrandom(void *buf, size_t length, unsigned int flags)
{
    register long r0 __asm__("$r0") = (long)(uintptr_t)buf;
    register size_t r1 __asm__("$r1") = length;
    register long r2 __asm__("$r2") = flags;

    __asm__ volatile("swi 355" : "+r"(r0) : "r"(r1), "r"(r2) : "memory");
    return r0;
}

/*
 * moxie special registers used by moxiebox.
 */
#define MOXIEBOX_SREG_MEMMAP    6
#define MOXIEBOX_SREG_STACK     7

static inline uint32_t
__moxiebox_sreg_read(unsigned int sreg)
{
    uint32_t value;

    __asm__ volatile("gsr %0, %1" : "=r"(value) : "i"(sreg));
    return value;
}

static inline void
__moxiebox_sreg_write(unsigned int sreg, uint32_t value)
{
    __asm__ volatile("ssr %0, %1" : : "i"(sreg), "r"(value));
}

#endif /* __MOXIEBOX_SYSCALL_H__ */
