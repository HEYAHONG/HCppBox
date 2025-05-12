/***************************************************************
 * Name:      hmalloc.h
 * Purpose:   声明hmalloc接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HMALLOC_H__
#define __HMALLOC_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


/** \brief 内存分配
 *
 * \param nBytes size_t 待分配的大小
 * \return void* 分配的内存的指针
 *
 */
void * hmalloc(size_t nBytes);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HMALLOC_H__
