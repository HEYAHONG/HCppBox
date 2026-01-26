/***************************************************************
 * Name:      hthreads_common.h
 * Purpose:   声明hthreads_common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-26
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HTHREADS_COMMON_H__
#define __HTHREADS_COMMON_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "hdefaults/hdefaults_libc.h"

/* Exit and error codes.  */
enum
{
    hthrd_success  = 0,
    hthrd_busy     = 1,
    hthrd_error    = 2,
    hthrd_nomem    = 3,
    hthrd_timedout = 4
};

/** \brief 获取C标准对应的代码值
 *
 * \param wrapper_code int 包装的代码值
 * \return int C标准对应的代码值
 *
 */
int hthrd_code_cstd(int wrapper_code);

/** \brief 获取包装对应的代码值
 *
 * \param cstd_code int C标准对应的代码值
 * \return int 包装的代码值
 *
 */
int hthrd_code_wrapper(int cstd_code);

/* Mutex types.  */
enum
{
    hmtx_plain     = 0,
    hmtx_recursive = 1,
    hmtx_timed     = 2
};

/** \brief 获取C标准对应的互斥类型值
 *
 * \param wrapper_type int 包装类型（可组合）
 * \return int C标准对应的互斥锁类型
 *
 */
int hmtx_type_cstd(int wrapper_type);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HTHREADS_COMMON_H__
