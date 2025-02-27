/***************************************************************
 * Name:      mcs_51_timer.h
 * Purpose:   声明mcs_51_timer接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-02-27
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HS_MCS_51_TIMER_H__
#define __HS_MCS_51_TIMER_H__
#include "mcs_51_common.h"
#include "mcs_51_core.h"
#ifdef __cplusplus
extern "C"
{
#endif


struct hs_mcs_51_timer;
typedef struct hs_mcs_51_timer hs_mcs_51_timer_t;


struct hs_mcs_51_timer
{
    size_t clk_freq;                    /**< 时钟频率，一般用于外部实现定时器时参考 */
    struct
    {
        uint32_t    external_clk:1;     /**< 是否使用外部时钟Tick，默认情况下使用Core的时钟Tick，当为1时用户必须单独调用定时器的Tick */
        uint32_t    enable_t2:1;        /**< 定时器2使能 */
        uint32_t    t2ex:1;             /**< 定时器2 T2EX值 */
        uint32_t    tf0:1;              /**< 定时器0标志，当进入中断服务程序时置1 */
        uint32_t    tf1:1;              /**< 定时器1标志，当进入中断服务程序时置1 */
        uint32_t    tf2:1;              /**< 定时器2标志，当进入中断服务程序时置1 */
    } flag;                             /**< 标志位 */
};

/** \brief MCS-51 定时器初始化
 *
 * \param timer hs_mcs_51_timer_t*      MCS-51 定时器指针
 *
 */
void hs_mcs_51_timer_init(hs_mcs_51_timer_t *timer);

/** \brief  MCS-51 定时器总线IO操作(一般由总线上的主设备(如CPU)调用)
 *
  * \param core hs_mcs_51_core_t*       MCS-51内核指针，见hs_mcs_51_io_t。
 * \param address uint16_t              地址，见hs_mcs_51_io_t。
 * \param opt hs_mcs_51_io_opt_t        IO操作选项，见hs_mcs_51_io_t。
 * \param data uint8_t*                 数据长度，见hs_mcs_51_io_t。
 * \param length uint16_t               数据长度，见hs_mcs_51_io_t。
 * \param usr void*                     用户指针，见hs_mcs_51_io_t。
 * \param timer hs_mcs_51_timer_t*      MCS-51 定时器指针，当为NULL时此函数内部不操作。
 *
 */
void hs_mcs_51_timer_bus_io(hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr,hs_mcs_51_timer_t *timer);

/** \brief MCS-51 定时器0（作为计数器）自增
 *
 * \param core hs_mcs_51_core_t*    MCS-51内核指针
 * \param timer hs_mcs_51_timer_t*  MCS-51 定时器指针
 *
 */
void hs_mcs_51_timer_counter0_inc(hs_mcs_51_core_t *core,hs_mcs_51_timer_t *timer);

/** \brief MCS-51 定时器1（作为计数器）自增
 *
 * \param core hs_mcs_51_core_t*    MCS-51内核指针
 * \param timer hs_mcs_51_timer_t*  MCS-51 定时器指针
 *
 */
void hs_mcs_51_timer_counter1_inc(hs_mcs_51_core_t *core,hs_mcs_51_timer_t *timer);

/** \brief MCS-51 定时器2（作为计数器）自增
 *
 * \param core hs_mcs_51_core_t*    MCS-51内核指针
 * \param timer hs_mcs_51_timer_t*  MCS-51 定时器指针
 *
 */
void hs_mcs_51_timer_counter2_inc(hs_mcs_51_core_t *core,hs_mcs_51_timer_t *timer);

/** \brief MCS-51 定时器外部时钟节拍
 *
 * \param core hs_mcs_51_core_t*  MCS-51内核指针
 * \param timer hs_mcs_51_timer_t* MCS-51 定时器指针
 * \param enable_clk bool 是否启用外部时钟，true=使用此Tick,false=使用Core的Tick
 *
 */
void hs_mcs_51_timer_external_clk_tick(hs_mcs_51_core_t *core,hs_mcs_51_timer_t *timer,bool enable_clk);

/** \brief MCS-51 定时器获取时钟频率
 *
 * \param timer hs_mcs_51_timer_t* MCS-51 定时器指针
 * \return size_t   定时器时钟频率
 *
 */
size_t hs_mcs_51_timer_clk_freq_get(hs_mcs_51_timer_t *timer);

/** \brief MCS-51 定时器设置时钟频率。注意：此操作只能改变要求的频率，能否实际改变取决于用户的实现
 *
 * \param timer hs_mcs_51_timer_t* MCS-51 定时器指针
 * \param clk_freq size_t   定时器时钟频率。0=关闭定时器
 *
 */
void hs_mcs_51_timer_clk_freq_set(hs_mcs_51_timer_t *timer,size_t clk_freq);

/** \brief MCS-51 定时器2使能设置
 *
 * \param timer hs_mcs_51_timer_t* MCS-51 定时器指针
 * \param enable bool 是否使能
 *
 */
void hs_mcs_51_timer_timer2_enable(hs_mcs_51_timer_t *timer,bool enable);

/** \brief MCS-51 定时器2设置T2EX
 *
 * \param core hs_mcs_51_core_t*  MCS-51内核指针
 * \param timer hs_mcs_51_timer_t* MCS-51 定时器指针
 * \param value bool 值
 *
 */
void hs_mcs_51_timer_timer2_t2ex_set(hs_mcs_51_core_t *core,hs_mcs_51_timer_t *timer,bool value);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HS_MCS_51_TIMER_H__
