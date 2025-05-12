/***************************************************************
 * Name:      hrealloc.h
 * Purpose:   声明hrealloc接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HREALLOC_H__
#define __HREALLOC_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 重新分配内存
 *
 * \param ptr void* 旧内存指针
 * \param new_size size_t 新分配内存大小
 * \return void* 分配的内存的指针
 *
 */
void *hrealloc( void *ptr, size_t new_size );

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HREALLOC_H__
