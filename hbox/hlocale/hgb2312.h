/***************************************************************
 * Name:      hgb2312.h
 * Purpose:   声明hgb2312接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-06
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HGB2312_H__
#define __HGB2312_H__
#include "hunicode.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 获取gb2312字符串长度
 *
 * \param str const char* 字符串指针
 * \return size_t 字符串长度,注意：并非字节数
 *
 */
size_t hgb2312_gb2312_string_length(const char *str);


/** \brief 从gb2312转换到hunicode_char_t
 *
 * \param dest hunicode_char_t* hunicode_char_t指针
 * \param dest_length size_t hunicode_char_t指针所指的缓冲区长度
 * \param src const char* gb2312指针
 * \param src_length size_t gb2312指针所指的缓冲区长度
 *
 */
void hgb2312_string_to_unicode(hunicode_char_t *dest,size_t dest_length,const char *src,size_t src_length);

/** \brief 从hunicode_char_t转换到gb2312
 *
 * \param dest char* gb2312_t指针
 * \param dest_length size_t gb2312指针所指的缓冲区长度
 * \param src const hunicode_char_t* hunicode_char_t指针
 * \param src_length size_t hunicode_char_t指针所指的缓冲区长度
 *
 */
void hgb2312_string_from_unicode(char *dest,size_t dest_length,const hunicode_char_t *src,size_t src_length);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HGB2312_H__
