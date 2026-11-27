/***************************************************************
 * Name:      halibc_posix_socket_fallback.h
 * Purpose:   声明socket兼容层fallback实现辅助宏
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HALIBC_POSIX_SOCKET_FALLBACK_H__
#define __HALIBC_POSIX_SOCKET_FALLBACK_H__

/*
 * fallback实现均使用弱符号，用户提供同名强符号实现时自动覆盖。
 */
#if defined(__GNUC__) || defined(__clang__)
#define HALIBC_POSIX_SOCKET_FALLBACK_WEAK __attribute__((weak))
#else
#define HALIBC_POSIX_SOCKET_FALLBACK_WEAK
#endif

#define HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(x) ((void)(x))

#endif /* __HALIBC_POSIX_SOCKET_FALLBACK_H__ */
