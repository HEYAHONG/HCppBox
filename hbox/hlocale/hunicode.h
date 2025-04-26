/***************************************************************
 * Name:      hunicode.h
 * Purpose:   声明hunicode接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-08-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HUNICODE_H_INCLUDED__
#define __HUNICODE_H_INCLUDED__
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"
#include "limits.h"
#include "wchar.h"
/*
本文件用于处理Unicode的相关辅助函数。
*/

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#if WCHAR_MAX > USHRT_MAX
#ifndef HUNICODE_USE_WCHAR_T
#define HUNICODE_USE_WCHAR_T 1
#endif // HUNICODE_USE_WCHAR_T
#endif

#ifdef HUNICODE_USE_WCHAR_T
#define hunicode_char_t wchar_t
#else
#define hunicode_char_t int
#endif

/** \brief 判断是否为ascii字符串
 *
 * \param str const char* 字符串指针
 * \return bool 是否是ascii字符串
 *
 */
bool hunicode_cchar_string_is_ascii(const char *str);

/** \brief 判断是否为utf8字符串
 *
 * \param str const char* 字符串指针
 * \return bool 是否是utf8字符串
 *
 */
bool hunicode_cchar_string_is_utf8(const char *str);

/** \brief 获取char字符串长度
 *
 * \param str const char* 字符串指针
 * \return size_t 字符串长度
 *
 */
size_t hunicode_cchar_string_length(const char *str);

/** \brief 获取char(UTF-8)字符串长度
 *
 * \param utf8_str const char* (UTF-8)字符串指针
 * \return size_t (UTF-8)字符串长度,可获得转换为宽字符字符串需要多少个字符。
 *
 */
size_t hunicode_cchar_utf8_string_length(const char *utf8_str);

/** \brief 获取wchar_t字符串长度
 *
 * \param str const wchar_t* 字符串指针
 * \return size_t 字符串长度
 *
 */
size_t hunicode_wchar_t_string_length(const wchar_t *str);

/** \brief 获取hunicode_char_t字符串长度
 *
 * \param str const hunicode_char_t* 字符串指针
 * \return size_t 字符串长度
 *
 */
size_t hunicode_char_string_length(const hunicode_char_t *str);

/** \brief 从wchar_t转换到hunicode_char_t
 *
 * \param dest hunicode_char_t* hunicode_char_t指针
 * \param dest_length size_t hunicode_char_t指针所指的缓冲区长度
 * \param src const wchar_t* wchar_t指针
 * \param src_length size_t wchar_t指针所指的缓冲区长度
 *
 */
void hunicode_char_from_wchar(hunicode_char_t *dest,size_t dest_length,const wchar_t *src,size_t src_length);

/** \brief 从wchar_t字符串转换到hunicode_char_t
 *
 * \param dest hunicode_char_t* hunicode_char_t指针
 * \param dest_length size_t hunicode_char_t指针所指的缓冲区长度
 * \param src const wchar_t* wchar_t字符串指针
 *
 */
void hunicode_char_from_wchar_string(hunicode_char_t *dest,size_t dest_length,const wchar_t *src);

/** \brief 从char(UTF-8)字符串转换到hunicode_char_t
 *
 * \param dest hunicode_char_t* hunicode_char_t指针
 * \param dest_length size_t hunicode_char_t指针所指的缓冲区长度
 * \param src const char* char(UTF-8)字符串指针
 *
 */
void hunicode_char_from_utf8_string(hunicode_char_t *dest,size_t dest_length,const char *src);

/** \brief 从hunicode_char_t转换到wchar_t
 *
 * \param dest wchar_t* wchar_t指针
 * \param dest_length size_t wchar_t指针所指的缓冲区长度
 * \param src const hunicode_char_t* hunicode_char_t指针
 * \param src_length size_t hunicode_char_t指针所指的缓冲区长度
 *
 */
void hunicode_char_to_wchar(wchar_t *dest,size_t dest_length,const hunicode_char_t *src,size_t src_length);

/** \brief 从hunicode_char_t字符串转换到wchar_t
 *
 * \param dest wchar_t* wchar_t指针
 * \param dest_length size_t wchar_t指针所指的缓冲区长度
 * \param src const hunicode_char_t* hunicode_char_t字符串指针
 *
 */
void hunicode_char_string_to_wchar(wchar_t *dest,size_t dest_length,const hunicode_char_t *src);

/** \brief 从hunicode_char_t转换到char(UTF-8)字符串
 *
 * \param dest char* char(UTF-8)字符串缓冲区指针
 * \param dest_length size_t char(UTF-8)字符串所指的缓冲区长度
 * \param src const hunicode_char_t* hunicode_char_t指针
 * \param src_length size_t hunicode_char_t指针所指的缓冲区长度
 *
 */
void hunicode_char_to_utf8(char*dest,size_t dest_length,const hunicode_char_t *src,size_t src_length);

/** \brief 从hunicode_char_t字符串转换到char(UTF-8)字符串
 *
 * \param dest char* char(UTF-8)字符串缓冲区指针
 * \param dest_length size_t char(UTF-8)字符串所指的缓冲区长度
 * \param src const hunicode_char_t* hunicode_char_t字符串指针
 *
 */
void hunicode_char_string_to_utf8(char *dest,size_t dest_length,const hunicode_char_t *src);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HUNICODE_H_INCLUDED__
