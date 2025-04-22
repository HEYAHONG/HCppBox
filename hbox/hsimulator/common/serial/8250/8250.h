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
    HS_COMMON_SERIAL_8250_REGISTER_LSR,             /**< 线路状态寄存器，可读写**/
    HS_COMMON_SERIAL_8250_REGISTER_MSR,             /**< Modem状态寄存器，可读写**/
    HS_COMMON_SERIAL_8250_REGISTER_SCR,             /**< Scratch寄存器，可读写**/
    HS_COMMON_SERIAL_8250_REGISTER_DLL,             /**< 除数低字节寄存器，DLAB=1访问此寄存器 */
    HS_COMMON_SERIAL_8250_REGISTER_DLM,             /**< 除数高字节寄存器，DLAB=1访问此寄存器 */
    HS_COMMON_SERIAL_8250_REGISTER_MAX              //用于确定寄存器的数量
} hs_common_serial_8250_register_t;             /**< 寄存器 */


typedef enum
{
    HS_COMMON_SERIAL_8250_IO_OPERATE_TRANSMIT_BYTE=0,       /**< 发送数据,写THR时触发*/
    HS_COMMON_SERIAL_8250_IO_OPERATE_RECEIVE_BYTE,          /**< 接收数据,读RBR时触发 */
    HS_COMMON_SERIAL_8250_IO_OPERATE_IRQ,                   /**< 中断请求，当中断使能且发生相应中断时触发，数据为IIR寄存器 */
    HS_COMMON_SERIAL_8250_IO_OPERATE_TICK,                  /**< 时钟节拍，一般用于8250内部寄存器更新 */
    HS_COMMON_SERIAL_8250_IO_OPERATE_DCD,                   /**< DCD信号，数据返回1表示有效（并非指高电平） */
    HS_COMMON_SERIAL_8250_IO_OPERATE_RI,                    /**< RI信号， 数据返回1表示有效（并非指高电平）*/
    HS_COMMON_SERIAL_8250_IO_OPERATE_DSR,                   /**< DSR信号， 数据返回1表示有效（并非指高电平）*/
    HS_COMMON_SERIAL_8250_IO_OPERATE_CTS,                   /**< CTS信号， 数据返回1表示有效（并非指高电平）*/
    HS_COMMON_SERIAL_8250_IO_OPERATE_OUT2,                  /**< 输出OUT2信号， 数据为1表示有效（并非指高电平，通常低电平有效）*/
    HS_COMMON_SERIAL_8250_IO_OPERATE_OUT1,                  /**< 输出OUT1信号， 数据为1表示有效（并非指高电平，通常低电平有效）*/
    HS_COMMON_SERIAL_8250_IO_OPERATE_RTS,                   /**< 输出RTS信号， 数据为1表示有效（并非指高电平，通常低电平有效）*/
    HS_COMMON_SERIAL_8250_IO_OPERATE_DTR,                   /**< 输出DTR信号， 数据为1表示有效（并非指高电平，通常低电平有效）*/
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
        0x00,\
        0xff,\
        0x00,\
        0x01,\
        0x00,\
        0x03,\
        0x00,\
        0x60,\
        0x00,\
        0x00,\
        0x01,\
        0x00\
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
#define HS_COMMON_SERIAL_8250_REGISTER_ADDRESS_LSR      5   /**< 线路状态寄存器，可读写*/
#define HS_COMMON_SERIAL_8250_REGISTER_ADDRESS_MSR      6   /**< Modem状态寄存器，可读写*/
#define HS_COMMON_SERIAL_8250_REGISTER_ADDRESS_SCR      7   /**< Scratch寄存器，可读写*/
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
 * \param dev hs_common_serial_8250_t* 8250设备指针
 *
 */
void hs_common_serial_8250_bus_tick(hs_common_serial_8250_t *dev);


/** \brief  8250获取波特率。
 *          注意：多线程操作时需要加锁
 *
 * \param dev hs_common_serial_8250_t* 8250设备指针
 * \return size_t 波特率
 *
 */
size_t hs_common_serial_8250_config_baud_get(hs_common_serial_8250_t *dev);

/** \brief  8250设置波特率
 *          注意：多线程操作时需要加锁
 *
 * \param dev hs_common_serial_8250_t*  8250设备指针
 * \param baud size_t 波特率
 *
 */
void hs_common_serial_8250_config_baud_set(hs_common_serial_8250_t *dev,size_t baud);

typedef enum
{
    HS_COMMON_SERIAL_8250_CONFIG_PARITY_NONE=0,     /**< 无校验 */
    HS_COMMON_SERIAL_8250_CONFIG_PARITY_EVEN,       /**< 偶校验*/
    HS_COMMON_SERIAL_8250_CONFIG_PARITY_ODD,        /**< 奇校验 */
    HS_COMMON_SERIAL_8250_CONFIG_PARITY_MARK,       /**< 校验值固定为1 */
    HS_COMMON_SERIAL_8250_CONFIG_PARITY_SPACE       /**< 校验值固定为0 */
} hs_common_serial_8250_config_parity_t;            /**< 校验类型定义 */

/** \brief  8250获取校验位
 *          注意：多线程操作时需要加锁
 *
 * \param dev hs_common_serial_8250_t*  8250设备指针
 * \return hs_common_serial_8250_config_parity_t    校验类型
 *
 */
hs_common_serial_8250_config_parity_t hs_common_serial_8250_config_parity_get(hs_common_serial_8250_t *dev);

/** \brief  8250设置校验位
 *          注意：多线程操作时需要加锁
 *
 * \param dev hs_common_serial_8250_t*  8250设备指针
 * \param parity hs_common_serial_8250_config_parity_t    校验类型
 *
 */
void hs_common_serial_8250_config_parity_set(hs_common_serial_8250_t *dev,hs_common_serial_8250_config_parity_t parity);

/** \brief  8250获取停止位
 *          注意：多线程操作时需要加锁
 *
 * \param dev hs_common_serial_8250_t* 8250设备指针
 * \return size_t 停止位 1=停止位为1，2=停止位为2（字长为5时为1.5停止位）
 *
 */
size_t hs_common_serial_8250_config_stopbits_get(hs_common_serial_8250_t *dev);

/** \brief  8250设置停止位
 *          注意：多线程操作时需要加锁
 *
 * \param dev hs_common_serial_8250_t* 8250设备指针
 * \param  stopbits size_t 停止位 1=停止位为1，2=停止位为2（字长为5时为1.5停止位）
 *
 */
void hs_common_serial_8250_config_stopbits_set(hs_common_serial_8250_t *dev,size_t stopbits);


/** \brief  8250获取数据位
 *          注意：多线程操作时需要加锁
 *
 * \param dev hs_common_serial_8250_t*  8250设备指针
 * \return size_t 数据位
 *
 */
size_t hs_common_serial_8250_config_databits_get(hs_common_serial_8250_t *dev);

/** \brief  8250设置数据位
 *          注意：多线程操作时需要加锁
 *
 * \param dev hs_common_serial_8250_t*  8250设备指针
 * \param databits size_t 数据位
 *
 */
void hs_common_serial_8250_config_databits_set(hs_common_serial_8250_t *dev,size_t databits);


/** \brief  8250获取是否有数据
 *          注意：多线程操作时需要加锁
 *
 * \param dev hs_common_serial_8250_t* 8250设备指针
 * \return bool 数据是否准备好，当为true时，表示8250中有数据待读取,此时不可写入数据。
 *
 */
bool hs_common_serial_8250_status_dataready_get(hs_common_serial_8250_t *dev);

/** \brief  8250设置数据
 *          注意：多线程操作时需要加锁
 *
 * \param dev hs_common_serial_8250_t* 8250设备指针
 * \param data uint8_t  串口数据。注意：即使将数据已写入RBR，总线读取时仍然会调用io操作回调，如果用户选择在io操作回调中接收数据，此处可使用假数据。
 * \return bool 是否成功写入。
 *
 */
bool hs_common_serial_8250_status_dataready_set(hs_common_serial_8250_t *dev,uint8_t data);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HS_COMMON_SERIAL_8250_H__
