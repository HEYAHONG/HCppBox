/***************************************************************
 * Name:      habort.h
 * Purpose:   声明habort接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HABORT_H__
#define __HABORT_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


/** \brief 通过Abort信号退出程序
 *
 *
 */
void habort(void);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HABORT_H__
