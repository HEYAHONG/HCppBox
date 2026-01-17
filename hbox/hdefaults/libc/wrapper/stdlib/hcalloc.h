/***************************************************************
 * Name:      hcalloc.h
 * Purpose:   声明hcalloc接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCALLOC_H__
#define __HCALLOC_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


/** \brief 内存分配
 *
 * \param num size_t 分配的对象的数量
 * \param nBytes size_t 单个对象大小
 * \return void* 分配的内存的指针
 *
 */
void* hcalloc( size_t num, size_t nBytes);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HCALLOC_H__
