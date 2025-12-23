/***************************************************************
 * Name:      hprintf.h
 * Purpose:   声明h3rdparty接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-12-23
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HPRINTF_H_INCLUDE__
#define __HPRINTF_H_INCLUDE__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


/*
 * 内嵌printf,为保证打印代码输出的一致性，可使用此系列函数进行打印操作
 */

/** \brief 设置hprintf输出的回调函数
 *
 * \param cb void (*)(char c) 回调函数
 *
 */
void hprintf_set_callback(void (*cb)(char c));

/** \brief 类似printf
 *
 *
 */
int hprintf(const char* format, ...);
/** \brief 类似sprintf
 *
 *
 */
int hsprintf(char* buffer, const char* format, ...);
/** \brief 类似snprintf
 *
 *
 */
int hsnprintf(char* buffer, size_t count, const char* format, ...);
/** \brief 类似vsnprintf
 *
 *
 */
int hvsnprintf(char* buffer, size_t count, const char* format, va_list va);
/** \brief 类似vprintf
 *
 *
 */
int hvprintf(const char* format, va_list va);
/** \brief 用于使用自定义输出函数(out)输出打印
 *
 *
 */
int hfctprintf(void (*out)(char character, void* arg), void* arg, const char* format, ...);

/** \brief 用于使用自定义输出函数(out)输出打印
 *
 *
 */
int hvfctprintf(void (*out)(char character, void* arg), void* arg, const char* format, va_list va);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HPRINTF_H_INCLUDE__
