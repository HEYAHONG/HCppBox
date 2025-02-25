/***************************************************************
 * Name:      mcs_51_serial.h
 * Purpose:   声明mcs_51_serial接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-02-13
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HS_MCS_51_SERIAL_H__
#define __HS_MCS_51_SERIAL_H__
#include "mcs_51_common.h"
#include "mcs_51_core.h"
#ifdef __cplusplus
extern "C"
{
#endif


struct hs_mcs_51_serial;
typedef struct hs_mcs_51_serial hs_mcs_51_serial_t;

typedef enum
{
    HS_MCS_51_SERIAL_IO_TRANSMIT=0,       /**< 发送数据,写SBUF时触发*/
    HS_MCS_51_SERIAL_IO_RECEIVE,          /**< 接收数据,读SBUF时触发 */
    HS_MCS_51_SERIAL_IO_TICK,             /**< 节拍，总线节拍时触发，数据为内核PC值*/
} hs_mcs_51_serial_io_t;

/** \brief MCS-51串口IO回调
 *
 * \param serial hs_mcs_51_serial_t*     MCS-51串口指针
 * \param io_type hs_mcs_51_serial_io_t io类型
 * \param data uint16_t* 待发送/接收的数据
 * \return bool 是否成功操作
 *
 */
typedef bool  (*hs_mcs_51_serial_io_callback_t)(hs_mcs_51_serial_t *serial,hs_mcs_51_serial_io_t io_type,uint16_t *data);

struct hs_mcs_51_serial
{
    hs_mcs_51_serial_io_callback_t io;      /**< IO回调 */
    void*       usr;                        /**< 用户指针，用于串口的用户指针 */
    uint16_t    RB;                         /**< 接收缓冲 */
    uint16_t    TH;                         /**< 发送保持 */
};

/** \brief MCS-51串口初始化
 *
 * \param serial hs_mcs_51_serial_t* MCS-51串口指针
 * \param io hs_mcs_51_serial_io_callback_t IO回调
 * \param usr void* 用户参数
 *
 */
void hs_mcs_51_serial_init(hs_mcs_51_serial_t *serial,hs_mcs_51_serial_io_callback_t io,void *usr);

/** \brief  MCS-51串口总线IO操作(一般由总线上的主设备(如CPU)调用)
 *
  * \param core hs_mcs_51_core_t*       MCS-51内核指针，见hs_mcs_51_io_t。
 * \param address uint16_t              地址，见hs_mcs_51_io_t。
 * \param opt hs_mcs_51_io_opt_t        IO操作选项，见hs_mcs_51_io_t。
 * \param data uint8_t*                 数据长度，见hs_mcs_51_io_t。
 * \param length uint16_t               数据长度，见hs_mcs_51_io_t。
 * \param usr void*                     用户指针，见hs_mcs_51_io_t。
 * \param serial hs_mcs_51_serial_t*    MCS-51串口指针，当为NULL时此函数内部不操作。
 *
 */
void hs_mcs_51_serial_bus_io(hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr,hs_mcs_51_serial_t *serial);


/** \brief MCS-51串口获取串口模式
 *
 * \param core hs_mcs_51_core_t*        MCS-51内核指针
 * \param serial hs_mcs_51_serial_t*    MCS-51串口指针
 * \return size_t 模式
 *
 */
size_t hs_mcs_51_serial_config_mode_get(hs_mcs_51_core_t *core,hs_mcs_51_serial_t *serial);

/** \brief MCS-51串口获取串口波特率
 *
 * \param core hs_mcs_51_core_t*        MCS-51内核指针
 * \param serial hs_mcs_51_serial_t*    MCS-51串口指针
 * \return size_t 波特率
 *
 */
size_t hs_mcs_51_serial_config_baud_get(hs_mcs_51_core_t *core,hs_mcs_51_serial_t *serial);

/** \brief MCS-51串口设置TI
 *
 * \param core hs_mcs_51_core_t*        MCS-51内核指针
 * \param serial hs_mcs_51_serial_t*    MCS-51串口指针
 *
 */
void hs_mcs_51_serial_config_ti_set(hs_mcs_51_core_t *core,hs_mcs_51_serial_t *serial);

/** \brief MCS-51串口获取RI
 *
 * \param core hs_mcs_51_core_t*        MCS-51内核指针
 * \param serial hs_mcs_51_serial_t*    MCS-51串口指针
 * \return bool RI标志
 *
 */
bool hs_mcs_51_serial_config_ri_get(hs_mcs_51_core_t *core,hs_mcs_51_serial_t *serial);


/** \brief MCS-51串口设置数据（用于设置接收到的数据）
 *
 * \param core hs_mcs_51_core_t*        MCS-51内核指针
 * \param serial hs_mcs_51_serial_t*    MCS-51串口指针
 * \param data uint16_t                 接收到的数据。若未启用SM2，则可使用假数据(真正的数据在io回调中设置)
 * \return bool 是否成功设置，未成功设置时需要内核执行一段时间后重设
 *
 */
bool hs_mcs_51_serial_status_dataready_set(hs_mcs_51_core_t *core,hs_mcs_51_serial_t *serial,uint16_t data);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HS_MCS_51_SERIAL_H__


