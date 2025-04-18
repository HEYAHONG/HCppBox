/***************************************************************
 * Name:      hruntime.h
 * Purpose:   声明hruntime接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-18
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HRUNTIME_H__
#define __HRUNTIME_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


/** \brief hruntime初始化(底层)。
 * 一般由hcpprt组件使用，当不能使用C++时，需要在hruntime_init前调用，此函数可不在任务中调用。
 *
 *
 */
void hruntime_init_lowlevel(void);


/** \brief hruntime初始化
 * 一般由hcpprt组件使用，当不能使用C++时，需要在空闲任务或者优先级较低的任务中调用。
 *
 */
void hruntime_init(void);

/** \brief hruntime循环
 * 一般由hcpprt组件使用，当不能使用C++时，需要在空闲任务或者优先级较低的任务中周期性调用。
 *
 */
void hruntime_loop(void);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HRUNTIME_H__
