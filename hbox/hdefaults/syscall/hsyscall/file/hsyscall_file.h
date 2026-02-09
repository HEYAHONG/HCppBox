/***************************************************************
 * Name:      hsyscall_file.h
 * Purpose:   声明hsyscall_file接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-02-09
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HHSYSCALL_FILE_H__
#define __HHSYSCALL_FILE_H__
#include "../../wrapper/hopen.h"
#include "../../wrapper/hlseek.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


#ifdef SEEK_SET
#define HSEEK_SET SEEK_SET
#endif // SEEK_SET
#ifdef SEEK_CUR
#define HSEEK_CUR SEEK_CUR
#endif // SEEK_CUR
#ifdef SEEK_END
#define HSEEK_END SEEK_END
#endif // SEEK_END
#ifndef HSEEK_SET
#define HSEEK_SET 0
#endif // HSEEK_SET
#ifndef HSEEK_CUR
#define HSEEK_CUR 1
#endif // HSEEK_CUR
#ifndef HSEEK_END
#define HSEEK_END 2
#endif // HSEEK_END


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HHSYSCALL_FILE_H__
