#ifndef __MODBUS_MODBUS_H_INCLUDED__
#define __MODBUS_MODBUS_H_INCLUDED__

#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 *广播地址,从机不予回答，一般只用于写
 */
#ifndef MODBUS_BROADCAST_ADDRESS
#define MODBUS_BROADCAST_ADDRESS 0
#endif

/*
 *节点地址(最小值)
 */
#ifndef MODBUS_NODE_ADDRESS_MIN
#define MODBUS_NODE_ADDRESS_MIN 1
#endif

/*
 *节点地址(最大值)
 */
#ifndef MODBUS_NODE_ADDRESS_MAX
#define MODBUS_NODE_ADDRESS_MAX 247
#endif

/*
 *节点地址(默认值)
 */
#ifndef MODBUS_NODE_ADDRESS_DEFAULT
#define MODBUS_NODE_ADDRESS_DEFAULT MODBUS_NODE_ADDRESS_MIN
#endif

/* Modbus_Application_Protocol_V1_1b.pdf (chapter 6 section 1 page 12)
 * Quantity of Coils to read (2 bytes): 1 to 2000 (0x7D0)
 * (chapter 6 section 11 page 29)
 * Quantity of Coils to write (2 bytes): 1 to 1968 (0x7B0)
 */
#ifndef MODBUS_MAX_READ_BITS
#define MODBUS_MAX_READ_BITS  2000
#endif
#ifndef MODBUS_MAX_WRITE_BITS
#define MODBUS_MAX_WRITE_BITS 1968
#endif

/* Modbus_Application_Protocol_V1_1b.pdf (chapter 6 section 3 page 15)
 * Quantity of Registers to read (2 bytes): 1 to 125 (0x7D)
 * (chapter 6 section 12 page 31)
 * Quantity of Registers to write (2 bytes) 1 to 123 (0x7B)
 * (chapter 6 section 17 page 38)
 * Quantity of Registers to write in R/W registers (2 bytes) 1 to 121 (0x79)
 */
#ifndef MODBUS_MAX_READ_REGISTERS
#define MODBUS_MAX_READ_REGISTERS     125
#endif
#ifndef MODBUS_MAX_WRITE_REGISTERS
#define MODBUS_MAX_WRITE_REGISTERS    123
#endif
#ifndef MODBUS_MAX_WR_WRITE_REGISTERS
#define MODBUS_MAX_WR_WRITE_REGISTERS 121
#endif
#ifndef MODBUS_MAX_WR_READ_REGISTERS
#define MODBUS_MAX_WR_READ_REGISTERS  125
#endif

/* The size of the MODBUS PDU is limited by the size constraint inherited from
 * the first MODBUS implementation on Serial Line network (max. RS485 ADU = 256
 * bytes). Therefore, MODBUS PDU for serial line communication = 256 - Server
 * address (1 byte) - CRC (2 bytes) = 253 bytes.
 */
#ifndef MODBUS_MAX_PDU_LENGTH
#define MODBUS_MAX_PDU_LENGTH 253
#endif


/* Modbus_Application_Protocol_V1_1b.pdf Chapter 4 Section 1 Page 5
 * RS232 / RS485 ADU = 253 bytes + slave (1 byte) + CRC (2 bytes) = 256 bytes
 */
#ifndef MODBUS_RTU_MAX_ADU_LENGTH
#define MODBUS_RTU_MAX_ADU_LENGTH 256
#endif

/*
 * Modbus_Application_Protocol_V1_1b.pdf Chapter 4 Section 1 Page 5
 * TCP MODBUS ADU = 253 bytes +7 bytes(MBAP) = 260 bytes
 */
#ifndef MODBUS_TCP_MAX_ADU_LENGTH
#define MODBUS_TCP_MAX_ADU_LENGTH 260
#endif


/** \brief 检查一帧数据的crc
 *
 * \param adu 整帧数据(包含CRC)的指针
 * \param adu_length 长度(包含CRC)
 * \return CRC是否通过
 *
 */
bool modbus_rtu_adu_check_crc(const uint8_t *adu,size_t adu_length);



/** \brief 在数据帧末尾的添加crc校验
 *
 * \param adu 整帧数据的指针，需要在末尾留两个字节以填写CRC
 * \param adu_length 长度(包含CRC)
 * \return 是否调用成功
 *
 */
bool modbus_rtu_adu_append_crc(uint8_t *adu,size_t adu_length);

/** \brief 从数据帧中获取uint16_t数据
 *
 * \param data uint8_t* 数据帧指针
 * \param offset size_t 数据在数据帧中的偏移
 * \param data_length size_t 数据帧长度
 * \return uint16_t 读取的数据
 *
 */
uint16_t modbus_data_get_uint16_t(uint8_t *data,size_t offset,size_t data_length);

/** \brief 向数据帧中获取设置uint16_t数据
 *
 * \param data uint8_t* 数据帧指针
 * \param offset size_t 数据在数据帧中的偏移
 * \param data_length size_t 数据帧长度
 * \param val uint16_t 设置的数据
 *
 */
void modbus_data_set_uint16_t(uint8_t *data,size_t offset,size_t data_length,uint16_t val);

/** \brief RTU的PDU数据帧回调
 */
typedef bool (*modbus_rtu_pdu_callback_t)(uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr);

/** \brief 从RTU的ADU数据中提取PDU数据
 *
 * \param adu uint8_t* ADU数据帧
 * \param adu_length size_t ADU数据帧长度
 * \param cb modbus_rtu_pdu_callback_t PDU数据帧回调
 * \param usr void* 用户参数,用于PDU数据帧回调
 * \return bool 是否成功提取
 *
 */
bool modbus_rtu_get_pdu_from_adu(const uint8_t *adu,size_t adu_length,modbus_rtu_pdu_callback_t cb,void *usr);

/** \brief 向RTU的ADU数据中设置PDU数据
 *
 * \param adu uint8_t* ADU数据指针
 * \param adu_length size_t ADU长度
 * \param node_address uint8_t 节点地址
 * \param pdu const uint8_t* PDU数据指针
 * \param pdu_length size_t PDU长度
 * \return size_t 实际ADU长度
 *
 */
size_t modbus_rtu_set_pdu_to_adu(uint8_t *adu,size_t adu_length,uint8_t node_address,const uint8_t *pdu,size_t pdu_length);

/** \brief TCP的PDU数据帧回调
 */
typedef bool (*modbus_tcp_pdu_callback_t)(uint16_t TId,uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr);

/** \brief 从TCP的ADU数据中提取PDU数据
 *
 * \param adu uint8_t* ADU数据帧
 * \param adu_length size_t ADU数据帧长度
 * \param cb modbus_tcp_pdu_callback_t PDU数据帧回调
 * \param usr void* 用户参数,用于PDU数据帧回调
 * \return bool 是否成功提取
 *
 */
bool modbus_tcp_get_pdu_from_adu(const uint8_t *adu,size_t adu_length,modbus_tcp_pdu_callback_t cb,void *usr);

/** \brief 向TCP的ADU数据中设置PDU数据
 *
 * \param adu uint8_t* ADU数据指针
 * \param adu_length size_t ADU长度
 * \param TId uint16_t 传输ID，用于区分请求与响应是否为一对
 * \param node_address uint8_t 节点地址
 * \param pdu const uint8_t* PDU数据指针
 * \param pdu_length size_t PDU长度
 * \return size_t 实际ADU长度
 *
 */
size_t modbus_tcp_set_pdu_to_adu(uint8_t *adu,size_t adu_length,uint16_t TId,uint8_t node_address,const uint8_t *pdu,size_t pdu_length);




#ifdef __cplusplus
}
#endif

#endif // MODBUS_H_INCLUDED
