/***************************************************************
 * Name:      hfcntl.h
 * Purpose:   声明hfcntl接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-03-09
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HFCNTL_H__
#define __HFCNTL_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdarg.h"
#include "hopen.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#ifndef F_DUPFD
#define	F_DUPFD		0	/* Duplicate fildes */
#endif // F_DUPFD
#ifndef F_GETFD
#define	F_GETFD		1	/* Get fildes flags (close on exec) */
#endif // F_GETFD
#ifndef F_SETFD
#define	F_SETFD		2	/* Set fildes flags (close on exec) */
#endif // F_SETFD
#ifndef F_GETFL
#define	F_GETFL		3	/* Get file flags */
#endif // F_GETFL
#ifndef F_SETFL
#define	F_SETFL		4	/* Set file flags */
#endif

/** \brief 操作文件描述符
 *
 * \param fd int 文件描述符
 * \param op int 操作码
 * \param  ... arg 其它参数，根据操作码决定
 * \return int 返回值
 *
 */
int hfcntl(int fd, int op, ... /* arg */ );


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HFCNTL_H__
