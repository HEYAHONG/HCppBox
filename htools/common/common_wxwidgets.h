#ifndef __COMMON_WXWIDGETS_H__
#define __COMMON_WXWIDGETS_H__
#include "HCPPBox.h"
#include "hrc.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief hcppbox初始化（必须由主线程调用）
 *
 *
 */
void hcppbox_init(void);


/** \brief hcppbox反初始化（必须由主线程调用）
 *
 *
 */
void hcppbox_deinit(void);

/** \brief 软件定时器回调，需要GUI框架中的定时器周期性调用
 *
 *
 */
void hcppbox_softwaretimer_isr(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __COMMON_WXWIDGETS_H__

