/***************************************************************
 * Name:      hdlt645_common.h
 * Purpose:   声明hdlt645_common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-08-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDLT645_COMMON_H_INCLUDED__
#define __HDLT645_COMMON_H_INCLUDED__
#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"


#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 * 帧 : 帧起始符(1字节)+地址域（6字节）+ 帧起始符(1字节) + 控制码(1字节) + 数据域长度(1字节) + 数据域(数据域长度) + 校验码(1字节) + 帧结束符(1字节)
 */


/*
 * 帧前导，用于唤醒从机或者同步波特率(某些MCU的串口自动波特率可通过发送7个1实现)
 * 注意：本组件中前导不应当出现在接收的数据帧中或者发送的数据帧中，需要用户自行在数据接收中去除或者在数据发送时添加。
 */
#define HDLT645_FRAME_PREAMBLE              (0xFE)

/*
 * 帧起始符
 */
#define HDLT645_FRAME_SOF                   (0x68)

/*
 * 地址（低字节在前）
 */
typedef struct
{
    uint8_t A[6];
} hdlt645_bcd_addr_t;

/** \brief 地址是否匹配
 *
 * \param addr_src hdlt645_bcd_addr_t* 源地址
 * \param addr_dst hdlt645_bcd_addr_t* 目标地址
 * \return bool 是否成功匹配
 *
 */
bool hdlt645_bcd_addr_match(hdlt645_bcd_addr_t *addr_src,hdlt645_bcd_addr_t *addr_dst);


/** \brief 设置地址
 *
 * \param addr_src dlt645_bcd_addr_t* 地址
 * \param bcd_addr uint64_t BCD码表示的地址
 *
 */
void hdlt645_bcd_addr_set(hdlt645_bcd_addr_t *addr,uint64_t bcd_addr);

/** \brief 获取地址
 *
 * \param addr hdlt645_bcd_addr_t* 地址
 * \return uint64_t BCD码表示的地址
 *
 */
uint64_t hdlt645_bcd_addr_get(hdlt645_bcd_addr_t *addr);


/*
 * 地址通配符
 */
#define HDLT645_FRAME_ADDR_WILDCARD_BYTE    (0xAA)

/*
 * 广播地址（从机不回复）
 */
#define HDLT645_FRAME_BROADCAST_BCD_ADDR    (0x999999999999)

/*
 * 任意播地址(从机按照正常的地址匹配,从机使用自身地址正常回复)
 */
#define HDLT645_FRAME_ANYCAST_BCD_ADDR      (0xAAAAAAAAAAAA)

/*
 * 控制码（共1字节）
 */
typedef struct
{
    uint8_t fct:5;              /**< 功能码 */
    uint8_t ext:1;              /**< 后续数据 */
    uint8_t ack:1;              /**< 应答编码 */
    uint8_t dir:1;              /**< 方向 */
} hdlt645_control_t;

/** \brief 控制码解码
 *
 * \param control_code uint8_t 待解码的控制码
 * \return hdlt645_control_t 控制码
 *
 */
hdlt645_control_t hdlt645_control_decode(uint8_t control_code);

/** \brief 控制码编码
 *
 * \param control hdlt645_control_t 控制码
 * \return uint8_t 编码后的控制码
 *
 */
uint8_t hdlt645_control_encode(hdlt645_control_t control);

#define HDLT645_FRAME_CONTROL_DIR_MASK      (0x80)
#define HDLT645_FRAME_CONTROL_DIR_MASTER    (0x00)
#define HDLT645_FRAME_CONTROL_DIR_SLAVE     (0x80)
#define HDLT645_FRAME_CONTROL_ACK_MASK      (0x40)
#define HDLT645_FRAME_CONTROL_ACK_NORMAL    (0x00)
#define HDLT645_FRAME_CONTROL_ACK_ERROR     (0x40)
#define HDLT645_FRAME_CONTROL_EXT_MASK      (0x20)
#define HDLT645_FRAME_CONTROL_EXT_OFF       (0x00)
#define HDLT645_FRAME_CONTROL_EXT_ON        (0x20)
#define HDLT645_FRAME_CONTROL_FCT_MASK      (0x1F)
#define HDLT645_FRAME_CONTROL_FCT_RESERVED  (0b00000)
#define HDLT645_FRAME_CONTROL_FCT_TIME      (0b01000)
#define HDLT645_FRAME_CONTROL_FCT_READ      (0b10001)
#define HDLT645_FRAME_CONTROL_FCT_READEXT   (0b10010)
#define HDLT645_FRAME_CONTROL_FCT_READADDR  (0b10011)
#define HDLT645_FRAME_CONTROL_FCT_WRITE     (0b10100)
#define HDLT645_FRAME_CONTROL_FCT_WRITEADDR (0b10101)
#define HDLT645_FRAME_CONTROL_FCT_FREEZE    (0b10110)
#define HDLT645_FRAME_CONTROL_FCT_CHCOM     (0b10111)
#define HDLT645_FRAME_CONTROL_FCT_CHPASS    (0b11000)
#define HDLT645_FRAME_CONTROL_FCT_CLRMR     (0b11001)
#define HDLT645_FRAME_CONTROL_FCT_CLRALL    (0b11010)
#define HDLT645_FRAME_CONTROL_FCT_CLREVENT  (0b11011)


/*
 * 数据长度（1个字节），通过此宏定义可得:
 *     最大帧(不含前导)长度=数据长度+12
 *     最大帧(含4字节前导)长度=数据长度+16
 */
#define HDLT645_FRAME_DATALENGTH_MAX_READ       (200)
#define HDLT645_FRAME_DATALENGTH_MAX_WRITE      (50)

typedef struct
{
    uint8_t di[4];
} hdlt645_data_di_t;

#define HDLT645_DATA_DI_WILDCARD   (0xFF)          /**<数据标识通配符，通常用于标识数据块  */

/** \brief 数据标识是否匹配
 *
 * \param di_src hdlt645_data_di_t* 源数据标识
 * \param di_dst hdlt645_data_di_t* 目的数据标识
 * \return bool 是否匹配
 *
 */
bool hdlt645_data_di_match(hdlt645_data_di_t *di_src,hdlt645_data_di_t *di_dst);

/** \brief 设置数据标识
 *
 * \param di hdlt645_data_di_t* 数据标识
 * \param di_num uint32_t 数据标识
 *
 */
void hdlt645_data_di_set(hdlt645_data_di_t *di,uint32_t di_num);


/** \brief 读取数据标识
 *
 * \param di const hdlt645_data_di_t* 数据标识
 * \return uint32_t 数据标识
 *
 */
uint32_t hdlt645_data_di_get(const hdlt645_data_di_t *di);

typedef struct
{
    uint8_t pa;     /**< 操作权限 */
    uint8_t p0;     /**< 密码 */
    uint8_t p1;     /**< 密码 */
    uint8_t p2;     /**< 密码 */
} hdlt645_data_p_t;

/** \brief 设置密码
 *
 * \param p hdlt645_data_p_t* 密码
 * \param p_num uint32_t 密码
 *
 */
void hdlt645_data_p_set(hdlt645_data_p_t *p,uint32_t p_num);


/** \brief 读取密码
 *
 * \param p const hdlt645_data_p_t* 密码
 * \return uint32_t 密码
 *
 */
uint32_t hdlt645_data_p_get(const hdlt645_data_p_t *p);

typedef struct
{
    uint8_t c[4];
} hdlt645_data_c_t;

/** \brief 设置操作者
 *
 * \param c hdlt645_data_c_t* 操作者
 * \param c_num uint32_t 操作者
 *
 */
void hdlt645_data_c_set(hdlt645_data_c_t *c,uint32_t c_num);


/** \brief 读取操作者
 *
 * \param c const hdlt645_data_c_t* 操作者
 * \return uint32_t 操作者
 *
 */
uint32_t hdlt645_data_c_get(const hdlt645_data_c_t *c);


/** \brief 数据域数据打包(避免大量0对通信的影响)
 *
 * \param data uint8_t* 待打包的数据
 * \param datalen size_t 待打包的数据长度
 *
 */
void hdlt645_data_pack(uint8_t *data,size_t datalen);

/** \brief 数据域数据解包(避免大量0对通信的影响)
 *
 * \param data uint8_t* 待解包的数据
 * \param datalen size_t 待解包的数据长度
 *
 */
void hdlt645_data_unpack(uint8_t *data,size_t datalen);


/** \brief 校验和计算
 *
 * \param frame uint8_t* 帧指针,不含前导（计算校验时，数据域必须处于打包状态）
 * \param frame_check_len size_t 需要检查的长度（一般从帧起始到校验码前）
 * \return uint8_t 校验和
 *
 */
uint8_t hdlt645_checksum_calculate(const uint8_t *frame,size_t frame_check_len);


/*
 * 帧结束符
 */
#define HDLT645_FRAME_EOF               (0x16)


/** \brief 帧获取地址
 *
 * \param frame uint8_t* 帧缓冲
 * \param frame_len size_t 帧缓冲长度
 * \return hdlt645_bcd_addr_t* 地址
 *
 */
hdlt645_bcd_addr_t *hdlt645_frame_get_bcd_addr(uint8_t *frame,size_t frame_len);

/** \brief 帧获取控制码
 *
 * \param frame uint8_t* 帧缓冲
 * \param frame_len size_t 帧缓冲长度
 * \return hdlt645_bcd_addr_t* 控制码(数值)指针
 *
 */
uint8_t * hdlt645_frame_get_c(uint8_t *frame,size_t frame_len);

/** \brief 帧获取数据长度
 *
 * \param frame uint8_t* 帧缓冲
 * \param frame_len size_t 帧缓冲长度
 * \return hdlt645_bcd_addr_t* 数据长度指针
 *
 */
uint8_t * hdlt645_frame_get_datalen(uint8_t *frame,size_t frame_len);

/** \brief 帧获取数据
 *
 * \param frame uint8_t* 帧缓冲
 * \param frame_len size_t 帧缓冲长度
 * \return hdlt645_bcd_addr_t* 数据指针
 *
 */
uint8_t * hdlt645_frame_get_data(uint8_t *frame,size_t frame_len);



/** \brief 帧打包
 *
 * \param frame uint8_t* 数据帧，需要填写好地址、控制码、数据域长度及数据域
 * \param frame_len size_t 帧缓冲长度
 * \return size_t 实际帧长度
 *
 */
size_t hdlt645_frame_pack(uint8_t *frame,size_t frame_len);

/** \brief 帧解包
 *
 * \param frame uint8_t* 数据帧
 * \param frame_len size_t 数据帧长度
 * \return bool 是否解包成功
 *
 */
bool hdlt645_frame_unpack(uint8_t *frame,size_t frame_len);


/** \brief 帧检查
 *
 * \param frame const uint8_t* 数据帧
 * \param frame_len size_t 数据帧长度
 * \return bool 是否为合法数据帧
 *
 */
bool hdlt645_frame_check(const uint8_t *frame,size_t frame_len);


/*
 * 错误字（单个字节）
 */
#define HDLT645_ERR_RESERVED                (0x80)          /**< 保留 */
#define HDLT645_ERR_RATE                    (0x40)          /**< 费率数超 */
#define HDLT645_ERR_DAY                     (0x20)          /**< 日时区错误 */
#define HDLT645_ERR_YEAR                    (0x10)          /**< 年时区错误 */
#define HDLT645_ERR_COM                     (0x08)          /**< 通信设置错误，如无法更改速率 */
#define HDLT645_ERR_DENY                    (0x04)          /**< 密码错/未授权 */
#define HDLT645_ERR_NODATA                  (0x02)          /**< 无数据 */
#define HDLT645_ERR_OTHER                   (0x01)          /**< 其它错误，包括除开其它位已定义的错误的所有错误 */

/** \brief 帧获取错误字
 *
 * \param frame uint8_t* 帧缓冲
 * \param frame_len size_t 帧缓冲长度
 * \return hdlt645_bcd_addr_t* 错误字指针
 *
 */
uint8_t * hdlt645_frame_get_data_err(uint8_t *frame,size_t frame_len);

/** \brief 帧设置错误字
 *
 * \param frame uint8_t* 帧缓冲
 * \param frame_len size_t 帧缓冲长度
 * \param err uint8_t 错误字
 *
 */
void  hdlt645_frame_set_data_err(uint8_t *frame,size_t frame_len,uint8_t err);


/*
 * 通信速率特征字(单个字节)
 */
#define HDLT645_COM_Z_RESERVED0     (0x80)
#define HDLT645_COM_Z_19200         (0x40)
#define HDLT645_COM_Z_9600          (0x20)
#define HDLT645_COM_Z_4800          (0x10)
#define HDLT645_COM_Z_2400          (0x08)
#define HDLT645_COM_Z_1200          (0x04)
#define HDLT645_COM_Z_600           (0x02)
#define HDLT645_COM_Z_RESERVED1     (0x01)


/*
 * 电表运行状态字1
 */
#define HDLT645_STATUS1_REACTIVE_DIR                    (0x0020)  /**< 无功功率方向,0正向,1反向 */
#define HDLT645_STATUS1_ACTIVE_DIR                      (0x0010)  /**< 有功功率方向,0正向,1反向 */
#define HDLT645_STATUS1_OUTAGE_BATTERY                  (0x0008)  /**< 停电抄表电池,0正常,1欠压 */
#define HDLT645_STATUS1_CLOCK_BATTERY                   (0x0004)  /**< 时钟电池,0正常,1欠压 */
#define HDLT645_STATUS1_DEMAND_MODE                     (0x0002)  /**< 需量积算方式,0滑差,1区间 */

/*
 * 电表运行状态字2
 */
#define HDLT645_STATUS2_REACTIVE_C_DIR                  (0x0040)  /**< C相无功功率方向,0正向,1反向 */
#define HDLT645_STATUS2_REACTIVE_B_DIR                  (0x0020)  /**< B相无功功率方向,0正向,1反向 */
#define HDLT645_STATUS2_REACTIVE_A_DIR                  (0x0010)  /**< A相无功功率方向,0正向,1反向 */
#define HDLT645_STATUS2_ACTIVE_C_DIR                    (0x0004)  /**< C相有功功率方向,0正向,1反向 */
#define HDLT645_STATUS2_ACTIVE_B_DIR                    (0x0002)  /**< B相有功功率方向,0正向,1反向 */
#define HDLT645_STATUS2_ACTIVE_A_DIR                    (0x0001)  /**< A相有功功率方向,0正向,1反向 */

/*
 * 电表运行状态字3(操作类)
 */
#define HDLT645_STATUS3_RELAY                           (0x0010)  /**< 继电器状态,0通,1断 */
#define HDLT645_STATUS3_PROGRAM_ENABLE                  (0x0008)  /**< 编程允许,0禁止,1许可 */
#define HDLT645_STATUS3_POWER_SUPPLY_MASK               (0x0006)  /**< 供电方式掩码 */
#define HDLT645_STATUS3_POWER_SUPPLY_MAIN               (0x0000)  /**< 供电方式,主电源 */
#define HDLT645_STATUS3_POWER_SUPPLY_AUX                (0x0002)  /**< 供电方式,辅助电源 */
#define HDLT645_STATUS3_POWER_SUPPLY_BATTERY            (0x0004)  /**< 供电方式,电池供电 */
#define HDLT645_STATUS3_PERIOD                          (0x0001)  /**< 当前运行时段,0第一套,1第二套 */

/*
 * 电表运行状态字4(A相故障状态)
 */
#define HDLT645_STATUS4_PHASE_LOSS                      (0x0080)  /**< 断相 */
#define HDLT645_STATUS4_POWER_FLOW                      (0x0040)  /**< 潮流方向 */
#define HDLT645_STATUS4_OVERLOAD                        (0x0020)  /**< 过载 */
#define HDLT645_STATUS4_OVERCURRENT                     (0x0010)  /**< 过流 */
#define HDLT645_STATUS4_CURRENT_LOSS                    (0x0008)  /**< 失流 */
#define HDLT645_STATUS4_OVERVOLTAGE                     (0x0004)  /**< 过压 */
#define HDLT645_STATUS4_UNDERVOLTAGE                    (0x0002)  /**< 欠压 */
#define HDLT645_STATUS4_VOLTAGE_LOSS                    (0x0001)  /**< 失压 */
#define HDLT645_STATUS4_CURRENT_BREAK                   (0x0100)  /**< 断流 */

/*
 * 电表运行状态字5(B相故障状态)
 */
#define HDLT645_STATUS5_PHASE_LOSS                      (0x0080)  /**< 断相 */
#define HDLT645_STATUS5_POWER_FLOW                      (0x0040)  /**< 潮流方向 */
#define HDLT645_STATUS5_OVERLOAD                        (0x0020)  /**< 过载 */
#define HDLT645_STATUS5_OVERCURRENT                     (0x0010)  /**< 过流 */
#define HDLT645_STATUS5_CURRENT_LOSS                    (0x0008)  /**< 失流 */
#define HDLT645_STATUS5_OVERVOLTAGE                     (0x0004)  /**< 过压 */
#define HDLT645_STATUS5_UNDERVOLTAGE                    (0x0002)  /**< 欠压 */
#define HDLT645_STATUS5_VOLTAGE_LOSS                    (0x0001)  /**< 失压 */
#define HDLT645_STATUS5_CURRENT_BREAK                   (0x0100)  /**< 断流 */

/*
 * 电表运行状态字6(C相故障状态)
 */
#define HDLT645_STATUS6_PHASE_LOSS                      (0x0080)  /**< 断相 */
#define HDLT645_STATUS6_POWER_FLOW                      (0x0040)  /**< 潮流方向 */
#define HDLT645_STATUS6_OVERLOAD                        (0x0020)  /**< 过载 */
#define HDLT645_STATUS6_OVERCURRENT                     (0x0010)  /**< 过流 */
#define HDLT645_STATUS6_CURRENT_LOSS                    (0x0008)  /**< 失流 */
#define HDLT645_STATUS6_OVERVOLTAGE                     (0x0004)  /**< 过压 */
#define HDLT645_STATUS6_UNDERVOLTAGE                    (0x0002)  /**< 欠压 */
#define HDLT645_STATUS6_VOLTAGE_LOSS                    (0x0001)  /**< 失压 */
#define HDLT645_STATUS6_CURRENT_BREAK                   (0x0100)  /**< 断流 */

/*
 * 电表运行状态字7(合相故障状态)
 */
#define HDLT645_STATUS7_DEMAND_EXCEED                   (0x0040)  /**< 需量超限 */
#define HDLT645_STATUS7_POWER_FAIL                      (0x0020)  /**< 掉电 */
#define HDLT645_STATUS7_AUX_POWER_LOSS                  (0x0010)  /**< 辅助电源失电 */
#define HDLT645_STATUS7_CURRENT_IMBALANCE               (0x0008)  /**< 电流不平衡 */
#define HDLT645_STATUS7_VOLTAGE_IMBALANCE               (0x0004)  /**< 电压不平衡 */
#define HDLT645_STATUS7_CURRENT_REVERSE                 (0x0002)  /**< 电流逆相序 */
#define HDLT645_STATUS7_VOLTAGE_REVERSE                 (0x0001)  /**< 电压逆相序 */

/*
 * 有功组合方式特征字
 */
#define HDLT645_ACTIVE_COMB_REVERSE_SUB                 (0x08)    /**< 反向有功,0不减,1减 */
#define HDLT645_ACTIVE_COMB_REVERSE_ADD                 (0x04)    /**< 反向有功,0不加,1加 */
#define HDLT645_ACTIVE_COMB_FORWARD_SUB                 (0x02)    /**< 正向有功,0不减,1减 */
#define HDLT645_ACTIVE_COMB_FORWARD_ADD                 (0x01)    /**< 正向有功,0不加,1加 */

/*
 * 无功组合方式1、2特征字
 */
#define HDLT645_REACTIVE_COMB_IV_SUB                    (0x80)    /**< IV象限,0不减,1减 */
#define HDLT645_REACTIVE_COMB_IV_ADD                    (0x40)    /**< IV象限,0不加,1加 */
#define HDLT645_REACTIVE_COMB_III_SUB                   (0x20)    /**< III象限,0不减,1减 */
#define HDLT645_REACTIVE_COMB_III_ADD                   (0x10)    /**< III象限,0不加,1加 */
#define HDLT645_REACTIVE_COMB_II_SUB                    (0x08)    /**< II象限,0不减,1减 */
#define HDLT645_REACTIVE_COMB_II_ADD                    (0x04)    /**< II象限,0不加,1加 */
#define HDLT645_REACTIVE_COMB_I_SUB                     (0x02)    /**< I象限,0不减,1减 */
#define HDLT645_REACTIVE_COMB_I_ADD                     (0x01)    /**< I象限,0不加,1加 */

/*
 * 周休日特征字
 */
#define HDLT645_WEEKDAY_SATURDAY                        (0x40)    /**< 周六,0休息,1工作 */
#define HDLT645_WEEKDAY_FRIDAY                          (0x20)    /**< 周五,0休息,1工作 */
#define HDLT645_WEEKDAY_THURSDAY                        (0x10)    /**< 周四,0休息,1工作 */
#define HDLT645_WEEKDAY_WEDNESDAY                       (0x08)    /**< 周三,0休息,1工作 */
#define HDLT645_WEEKDAY_TUESDAY                         (0x04)    /**< 周二,0休息,1工作 */
#define HDLT645_WEEKDAY_MONDAY                          (0x02)    /**< 周一,0休息,1工作 */
#define HDLT645_WEEKDAY_SUNDAY                          (0x01)    /**< 周日,0休息,1工作 */

/*
 * 负荷记录模式字
 */
#define HDLT645_LOAD_RECORD_DEMAND                      (0x20)    /**< 当前需量 */
#define HDLT645_LOAD_RECORD_QUADRANT_ENERGY             (0x10)    /**< 四象限无功总电能 */
#define HDLT645_LOAD_RECORD_ENERGY                      (0x08)    /**< 有、无功总电能 */
#define HDLT645_LOAD_RECORD_POWER_FACTOR                (0x04)    /**< 功率因数 */
#define HDLT645_LOAD_RECORD_POWER                       (0x02)    /**< 有、无功功率 */
#define HDLT645_LOAD_RECORD_VOLTAGE_CURRENT_FREQUENCY   (0x01)    /**< 电压、电流、频率 */

/*
 * 冻结数据模式字
 */
#define HDLT645_FREEZE_VARIABLE                         (0x80)    /**< 变量 */
#define HDLT645_FREEZE_REVERSE_DEMAND                   (0x40)    /**< 反向有功最大需量及发生时间 */
#define HDLT645_FREEZE_FORWARD_DEMAND                   (0x20)    /**< 正向有功最大需量及发生时间 */
#define HDLT645_FREEZE_QUADRANT_ENERGY                  (0x10)    /**< 四象限无功总电能 */
#define HDLT645_FREEZE_REACTIVE2_ENERGY                 (0x08)    /**< 组合无功2电能 */
#define HDLT645_FREEZE_REACTIVE1_ENERGY                 (0x04)    /**< 组合无功1电能 */
#define HDLT645_FREEZE_REVERSE_ENERGY                   (0x02)    /**< 反向有功电能 */
#define HDLT645_FREEZE_FORWARD_ENERGY                   (0x01)    /**< 正向有功电能 */


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HDLT645_COMMON_H_INCLUDED
