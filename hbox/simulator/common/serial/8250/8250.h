/***************************************************************
 * Name:      8250.h
 * Purpose:   声明8250接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-02-06
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HS_COMMON_SERIAL_8250_H__
#define __HS_COMMON_SERIAL_8250_H__
#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus



/*
 *  8250时钟,对于模拟器而言，8250显然一般不会用真实时钟，为方便除数寄存器的计算，需要定义一个时钟频率。
 *  除数寄存器值=时钟频率/(16*波特率)
 *  同理，波特率=时钟频率/(16*除数寄存器值)
 */
#ifndef HS_COMMON_SERIAL_8250_CLK_FREQ
#define HS_COMMON_SERIAL_8250_CLK_FREQ                   (1843200)
#endif // HS_COMMON_SERIAL_8250_CLK_FREQ

typedef enum
{
    HS_COMMON_SERIAL_8250_REGISTER_RBR=0,           /**< 接收缓冲寄存器，只可读取，DLAB=0访问此寄存器*/
    HS_COMMON_SERIAL_8250_REGISTER_THR,             /**< 发送保持寄存器，只可写入 ，DLAB=0访问此寄存器*/
    HS_COMMON_SERIAL_8250_REGISTER_IER,             /**< 中断使能寄存器， 可读写，DLAB=0访问此寄存器*/
    HS_COMMON_SERIAL_8250_REGISTER_IIR,             /**< 中断指示寄存器，只可读取*/
    HS_COMMON_SERIAL_8250_REGISTER_FCR,             /**< FIFO控制寄存器，只可写入*/
    HS_COMMON_SERIAL_8250_REGISTER_LCR,             /**< 线路控制寄存器，可读写*/
    HS_COMMON_SERIAL_8250_REGISTER_MCR,             /**< Modem控制寄存器，可读写*/
    HS_COMMON_SERIAL_8250_REGISTER_LSR,             /**< 线路状态寄存器，只可读取*/
    HS_COMMON_SERIAL_8250_REGISTER_MSR,             /**< Modem状态寄存器，只可读取*/
    HS_COMMON_SERIAL_8250_REGISTER_SCR,             /**< Scratch寄存器，只可读取*/
    HS_COMMON_SERIAL_8250_REGISTER_DLL,             /**< 除数低字节寄存器，DLAB=1访问此寄存器 */
    HS_COMMON_SERIAL_8250_REGISTER_DLM,             /**< 除数高字节寄存器，DLAB=1访问此寄存器 */
    HS_COMMON_SERIAL_8250_REGISTER_MAX              //用于确定寄存器的数量
} hs_common_serial_8250_register_t;             /**< 寄存器 */


typedef enum
{
    HS_COMMON_SERIAL_8250_IO_OPERATE_TRANSMIT_BYTE=0,       /**< 发送数据,写THR时触发*/
    HS_COMMON_SERIAL_8250_IO_OPERATE_RECEIVE_BYTE,          /**< 接收数据,读RBR时触发 */
    HS_COMMON_SERIAL_8250_IO_OPERATE_IRQ,                   /**< 中断请求，当中断使能且发生相应中断时触发，数据为IIR寄存器 */
    HS_COMMON_SERIAL_8250_IO_OPERATE_TICK                   /**< 时钟节拍，一般用于8250内部寄存器更新 */
} hs_common_serial_8250_io_operate_t;                       /**< IO操作 */


/** \brief 8250设备前向声明
 */
struct hs_common_serial_8250;

/** \brief  8250 IO操作(用户需要实现相应IO操作)
     *
     * \param dev struct hs_common_serial_8250* 8250设备指针
     * \param io_operate hs_common_serial_8250_io_operate_t 操作类型
     * \param data uint8_t* 数据指针，用于传递操作
     * \return bool 是否成功
     *
     */
typedef bool (*hs_common_serial_8250_io_callback_t)(struct hs_common_serial_8250 *dev,hs_common_serial_8250_io_operate_t io_operate,uint8_t *data);

typedef struct hs_common_serial_8250
{

    hs_common_serial_8250_io_callback_t io;                         /**< IO操作 */
    void *      usr;                                                /**< 用户参数 */
    size_t      clk_freq;                                           /**< 时钟频率，用于计算波特率 */
    uint8_t     registers[HS_COMMON_SERIAL_8250_REGISTER_MAX];      /**< 内部寄存器，可使用hs_common_serial_8250_register_t作为下标访问相应寄存器 */
}   hs_common_serial_8250_t;


/** \brief hs_common_serial_8250_t初始化参数
 *
 *
 */
#define HS_COMMON_SERIAL_8250_INITIALIZER       \
{\
    NULL,\
    NULL,\
    HS_COMMON_SERIAL_8250_CLK_FREQ,\
    {\
        0,\
        0,\
        0,\
        1,\
        0,\
        3,\
        0,\
        60,\
        0,\
        0,\
        1,\
        0\
    }\
}

/** \brief  8250 初始化
     *
     * \param dev struct hs_common_serial_8250* 8250设备指针
     * \param io hs_common_serial_8250_io_callback_t IO操作
     * \param usr void * 用户参数
     * \param clk_freq size_t 时钟频率
     *
     */
void  hs_common_serial_8250_init(hs_common_serial_8250_t *dev,hs_common_serial_8250_io_callback_t io,void *usr,size_t clk_freq);

#define HS_COMMON_SERIAL_8250_REGISTER_ADDRESS_RBR      0   /**< 接收缓冲寄存器，只可读取，DLAB=0访问此寄存器*/
#define HS_COMMON_SERIAL_8250_REGISTER_ADDRESS_THR      0   /**< 发送保持寄存器，只可写入 ，DLAB=0访问此寄存器*/
#define HS_COMMON_SERIAL_8250_REGISTER_ADDRESS_IER      1   /**< 中断使能寄存器， 可读写，DLAB=0访问此寄存器*/
#define HS_COMMON_SERIAL_8250_REGISTER_ADDRESS_IIR      2   /**< 中断指示寄存器，只可读取*/
#define HS_COMMON_SERIAL_8250_REGISTER_ADDRESS_FCR      2   /**< FIFO控制寄存器，只可写入*/
#define HS_COMMON_SERIAL_8250_REGISTER_ADDRESS_LCR      3   /**< 线路控制寄存器，可读写*/
#define HS_COMMON_SERIAL_8250_REGISTER_ADDRESS_MCR      4   /**< Modem控制寄存器，可读写*/
#define HS_COMMON_SERIAL_8250_REGISTER_ADDRESS_LSR      5   /**< 线路状态寄存器，只可读取*/
#define HS_COMMON_SERIAL_8250_REGISTER_ADDRESS_MSR      6   /**< Modem状态寄存器，只可读取*/
#define HS_COMMON_SERIAL_8250_REGISTER_ADDRESS_SCR      7   /**< Scratch寄存器，只可读取*/
#define HS_COMMON_SERIAL_8250_REGISTER_ADDRESS_DLL      0   /**< 除数低字节寄存器，DLAB=1访问此寄存器 */
#define HS_COMMON_SERIAL_8250_REGISTER_ADDRESS_DLM      1   /**< 除数高字节寄存器，DLAB=1访问此寄存器 */


/** \brief 8250 总线写(一般由总线上的主设备(如CPU)调用)
 *
 * \param dev hs_common_serial_8250_t* 8250设备指针
 * \param address uint8_t   总线地址，范围0到7,见HS_COMMON_SERIAL_8250_REGISTER_ADDRESS_*。
 * \param reg_data uint8_t  寄存器数据
 *
 */
void hs_common_serial_8250_bus_write(hs_common_serial_8250_t *dev,uint8_t address,uint8_t reg_data);

/** \brief 8250 总线读(一般由总线上的主设备(如CPU)调用)
 *
 * \param dev hs_common_serial_8250_t* 8250设备指针
 * \param address uint8_t   总线地址，范围0到7,见HS_COMMON_SERIAL_8250_REGISTER_ADDRESS_*。
 * \param reg_data uint8_t*  寄存器数据指针
 *
 */
void hs_common_serial_8250_bus_read(hs_common_serial_8250_t *dev,uint8_t address,uint8_t *reg_data);


/** \brief 8250 总线32位对齐写(一般由总线上的主设备(如CPU)调用)
 *
 * \param dev hs_common_serial_8250_t* 8250设备指针
 * \param address uint8_t   总线地址，范围0到0x1C,此地址为字节地址，一般使用场景为需要32位对齐的系统，位[2 7]见HS_COMMON_SERIAL_8250_REGISTER_ADDRESS_*。
 * \param reg_data uint32_t  寄存器数据
 *
 */
void hs_common_serial_8250_bus_write32(hs_common_serial_8250_t *dev,uint8_t address,uint32_t reg_data);

/** \brief 8250 总线32位对齐读(一般由总线上的主设备(如CPU)调用)
 *
 * \param dev hs_common_serial_8250_t* 8250设备指针
 * \param address uint8_t   总线地址，范围0到0x1C，此地址为字节地址，一般使用场景为需要32位对齐的系统位[2 7]，见HS_COMMON_SERIAL_8250_REGISTER_ADDRESS_*。
 * \param reg_data uint32_t*  寄存器数据指针
 *
 */
void hs_common_serial_8250_bus_read32(hs_common_serial_8250_t *dev,uint8_t address,uint32_t *reg_data);

/** \brief  8250总线节拍(一般由总线上的主设备(如CPU)调用)，在进行读写前调用，可用于更新寄存器状态或者调用中断
 *
 * \param dev hs_common_serial_8250_t*
 *
 */
void hs_common_serial_8250_bus_tick(hs_common_serial_8250_t *dev);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HS_COMMON_SERIAL_8250_H__
