/***************************************************************
 * Name:      hlseek.h
 * Purpose:   声明hlseek接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HLSEEK_H__
#define __HLSEEK_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#if defined(HDEFAULTS_OS_UNIX) || defined(HAVE_UNISTD_H)
#include "unistd.h"
#endif

typedef int64_t hlseek_off_t;


#ifndef SEEK_SET
#define SEEK_SET 0
#endif // SEEK_SET
#ifndef SEEK_CUR
#define SEEK_CUR 1
#endif // SEEK_CUR
#ifndef SEEK_END
#define SEEK_END 2
#endif // SEEK_END

/** \brief 修改文件描述符关联的文件的偏移
 *
 * \param fd int 文件描述符
 * \param offset hlseek_off_t 偏移
 * \param whence int 何处
 * \return hlseek_off_t 返回当前从文件起始位置开始的偏移,-1表示出错
 *
 */
hlseek_off_t hlseek(int fd,hlseek_off_t offset,int whence);

/*
 * 64位版本同lseek
 */
#define hlseek64 hlseek

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HLSEEK_H__
