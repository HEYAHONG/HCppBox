/***************************************************************
 * Name:      mcs_51_pin.h
 * Purpose:   声明mcs_51_pin接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-02-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HS_MCS_51_PIN_H__
#define __HS_MCS_51_PIN_H__
#include "mcs_51_core.h"
#ifdef __cplusplus
extern "C"
{
#endif

struct hs_mcs_51_pin;
typedef struct hs_mcs_51_pin hs_mcs_51_pin_t;


typedef enum
{
    HS_MCS_51_PIN_PORT_0=0,
    HS_MCS_51_PIN_PORT_1=1,
    HS_MCS_51_PIN_PORT_2=2,
    HS_MCS_51_PIN_PORT_3=3,
} hs_mcs_51_pin_port_t;         /**< 端口 */

typedef enum
{
    HS_MCS_51_PIN_IO_PIN_CHANGE=0,          /**< pin被（内核）改变,回调时PIN已改变（此时若读取，为改变后的值） */
    HS_MCS_51_PIN_IO_USR_PIN_CHANGE,        /**< pin被（用户）改变,回调时PIN未改变（此时若读取，为改变前的值） */
    HS_MCS_51_PIN_IO_INT0_PIN_CHANGE,       /**< int0引脚被（用户）改变,回调时PIN未改变（此时若读取，为改变前的值），通常用于实现定时器相关操作 */
    HS_MCS_51_PIN_IO_INT1_PIN_CHANGE,       /**< int1引脚被（用户）改变,回调时PIN未改变（此时若读取，为改变前的值），通常用于实现定时器相关操作 */
    HS_MCS_51_PIN_IO_T0_PIN_CHANGE,         /**< t0引脚被（用户）改变,回调时PIN未改变（此时若读取，为改变前的值），通常用于实现定时器相关操作 */
    HS_MCS_51_PIN_IO_T1_PIN_CHANGE,         /**< t1引脚被（用户）改变,回调时PIN未改变（此时若读取，为改变前的值），通常用于实现定时器相关操作 */
} hs_mcs_51_pin_io_t;

/** \brief MCS-51 PIN IO回调
 *
 * \param pin hs_mcs_51_pin_t*              MCS-51 PIN指针
 * \param port hs_mcs_51_pin_port_t         端口
 * \param pinnum uint8_t                    引脚编号,范围为0～7
 *
 */
typedef void  (*hs_mcs_51_pin_io_callback_t)(hs_mcs_51_pin_t *pin,hs_mcs_51_pin_io_t io_type,hs_mcs_51_pin_port_t port,uint8_t pinnum);

struct hs_mcs_51_pin
{
    hs_mcs_51_pin_io_callback_t io;         /**< IO回调 */
    void*                       usr;        /**< 用户指针 */
    uint8_t     port[4];                    /**< 4个IO端口 */
    struct
    {
        uint8_t ie0:1;
        uint8_t ie1:1;
    } flag;
};

/** \brief MCS-51 PIN初始化
 *
 * \param pin hs_mcs_51_pin_t*              MCS-51 PIN指针
 * \param io hs_mcs_51_pin_io_callback_t    IO回调
 * \param usr void*                         用户指针
 *
 */
void hs_mcs_51_pin_init(hs_mcs_51_pin_t *pin,hs_mcs_51_pin_io_callback_t io,void *usr);

/** \brief  MCS-51 PIN总线IO操作(一般由总线上的主设备(如CPU)调用)
 *
  * \param core hs_mcs_51_core_t*       MCS-51内核指针，见hs_mcs_51_io_t。
 * \param address uint16_t              地址，见hs_mcs_51_io_t。
 * \param opt hs_mcs_51_io_opt_t        IO操作选项，见hs_mcs_51_io_t。
 * \param data uint8_t*                 数据长度，见hs_mcs_51_io_t。
 * \param length uint16_t               数据长度，见hs_mcs_51_io_t。
 * \param usr void*                     用户指针，见hs_mcs_51_io_t。
 * \param pin hs_mcs_51_pin_t*          MCS-51 PIN指针，当为NULL时此函数内部不操作。
 *
 */
void hs_mcs_51_pin_bus_io(hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr,hs_mcs_51_pin_t *pin);



/** \brief  MCS-51 PIN获取端口值
 *
 * \param core hs_mcs_51_core_t*        MCS-51内核指针
 * \param pin hs_mcs_51_pin_t*          MCS-51 PIN指针
 * \param port hs_mcs_51_pin_port_t     端口
 * \return uint8_t 当前端口值
 *
 */
uint8_t hs_mcs_51_pin_port_get(hs_mcs_51_core_t *core,hs_mcs_51_pin_t *pin,hs_mcs_51_pin_port_t port);

/** \brief  MCS-51 PIN设置端口值
 *
 * \param core hs_mcs_51_core_t*        MCS-51内核指针
 * \param pin hs_mcs_51_pin_t*          MCS-51 PIN指针
 * \param port hs_mcs_51_pin_port_t     端口
 * \param value uint8_t                 端口值
 *
 */
void hs_mcs_51_pin_port_set(hs_mcs_51_core_t *core,hs_mcs_51_pin_t *pin,hs_mcs_51_pin_port_t port,uint8_t value);

/** \brief  MCS-51 PIN获取PIN
 *
 * \param core hs_mcs_51_core_t*        MCS-51内核指针
 * \param pin hs_mcs_51_pin_t*          MCS-51 PIN指针
 * \param port hs_mcs_51_pin_port_t     端口
 * \param pinnum uint8_t                引脚编号,范围为0～7
 * \return bool 当前端口值
 *
 */
bool hs_mcs_51_pin_port_pin_get(hs_mcs_51_core_t *core,hs_mcs_51_pin_t *pin,hs_mcs_51_pin_port_t port,uint8_t pinnum);

/** \brief  MCS-51 PIN设置PIN
 *
 * \param core hs_mcs_51_core_t*        MCS-51内核指针
 * \param pin hs_mcs_51_pin_t*          MCS-51 PIN指针
 * \param port hs_mcs_51_pin_port_t     端口
 * \param pinnum uint8_t                引脚编号,范围为0～7
 * \param value bool                    当前端口值
 *
 */
void hs_mcs_51_pin_port_pin_set(hs_mcs_51_core_t *core,hs_mcs_51_pin_t *pin,hs_mcs_51_pin_port_t port,uint8_t pinnum,bool value);


/** \brief  MCS-51 PIN获取PIN
 *
 * \param core hs_mcs_51_core_t*        MCS-51内核指针
 * \param pin hs_mcs_51_pin_t*          MCS-51 PIN指针
 * \param bit_addr uint8_t              引脚在SFR的位地址
 * \return bool 当前端口值
 *
 */
bool hs_mcs_51_pin_port_pin_get_by_bitaddr(hs_mcs_51_core_t *core,hs_mcs_51_pin_t *pin,uint8_t bit_addr);

/** \brief  MCS-51 PIN设置PIN
 *
 * \param core hs_mcs_51_core_t*        MCS-51内核指针
 * \param pin hs_mcs_51_pin_t*          MCS-51 PIN指针
 * \param bit_addr uint8_t              引脚在SFR的位地址
 * \param value bool                    当前端口值
 *
 */
void hs_mcs_51_pin_port_pin_set_by_bitaddr(hs_mcs_51_core_t *core,hs_mcs_51_pin_t *pin,uint8_t bit_addr,bool value);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HS_MCS_51_PIN_H__
