/***************************************************************
 * Name:      hfree.h
 * Purpose:   声明hfree接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HFREE_H__
#define __HFREE_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


/** \brief 内存释放
 *
 * \param ptr void* 待释放的内存
 *
 */
void  hfree(void *ptr);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HFREE_H__
