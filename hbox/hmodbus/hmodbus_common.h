/***************************************************************
 * Name:      hmodbus_common.h
 * Purpose:   hmodbus_common
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-03-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __MODBUS_HMODBUS_COMMON_H__
#define __MODBUS_HMODBUS_COMMON_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
 *广播地址,从机不予回答，一般只用于写,见Modbus_over_serial_line_V1_02.pdf 2.2节
 */
#ifndef MODBUS_BROADCAST_ADDRESS
#define MODBUS_BROADCAST_ADDRESS 0
#endif

/*
 *节点地址(最小值),见Modbus_over_serial_line_V1_02.pdf 2.2节
 */
#ifndef MODBUS_NODE_ADDRESS_MIN
#define MODBUS_NODE_ADDRESS_MIN 1
#endif

/*
 *节点地址(最大值),见Modbus_over_serial_line_V1_02.pdf 2.2节
 */
#ifndef MODBUS_NODE_ADDRESS_MAX
#define MODBUS_NODE_ADDRESS_MAX 247
#endif


/*
 *节点保留地址(最小值),见Modbus_over_serial_line_V1_02.pdf 2.2节
 */
#ifndef MODBUS_NODE_ADDRESS_RESERVED_MIN
#define MODBUS_NODE_ADDRESS_RESERVED_MIN    248
#endif // MODBUS_NODE_ADDRESS_RESERVED_MIN

/*
 *节点保留地址(最大值),见Modbus_over_serial_line_V1_02.pdf 2.2节
 */
#ifndef MODBUS_NODE_ADDRESS_RESERVED_MAX
#define MODBUS_NODE_ADDRESS_RESERVED_MAX    255
#endif // MODBUS_NODE_ADDRESS_RESERVED_MAX


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

/*
 *  功能码
 */
#ifndef MODBUS_FC_READ_COILS
#define MODBUS_FC_READ_COILS                        0x01
#endif
#ifndef MODBUS_FC_READ_DISCRETE_INPUTS
#define MODBUS_FC_READ_DISCRETE_INPUTS              0x02
#endif
#ifndef MODBUS_FC_READ_HOLDING_REGISTERS
#define MODBUS_FC_READ_HOLDING_REGISTERS            0x03
#endif
#ifndef MODBUS_FC_READ_INPUT_REGISTERS
#define MODBUS_FC_READ_INPUT_REGISTERS              0x04
#endif
#ifndef MODBUS_FC_WRITE_SINGLE_COIL
#define MODBUS_FC_WRITE_SINGLE_COIL                 0x05
#endif
#ifndef MODBUS_FC_WRITE_SINGLE_REGISTER
#define MODBUS_FC_WRITE_SINGLE_REGISTER             0x06
#endif
#ifndef MODBUS_FC_SERIAL_READ_EXCEPTION_STATUS
#define MODBUS_FC_SERIAL_READ_EXCEPTION_STATUS      0x07
#endif
#ifndef MODBUS_FC_SERIAL_DIAGNOSTICS
#define MODBUS_FC_SERIAL_DIAGNOSTICS                0x08
#endif
#ifndef MODBUS_FC_SERIAL_GET_COMM_EVENT_COUNTER
#define MODBUS_FC_SERIAL_GET_COMM_EVENT_COUNTER     0x0B
#endif
#ifndef MODBUS_FC_SERIAL_GET_COMM_EVENT_LOG
#define MODBUS_FC_SERIAL_GET_COMM_EVENT_LOG         0x0C
#endif
#ifndef MODBUS_FC_WRITE_MULTIPLE_COILS
#define MODBUS_FC_WRITE_MULTIPLE_COILS              0x0F
#endif
#ifndef MODBUS_FC_WRITE_MULTIPLE_REGISTERS
#define MODBUS_FC_WRITE_MULTIPLE_REGISTERS          0x10
#endif
#ifndef MODBUS_FC_SERIAL_REPORT_SERVER_ID
#define MODBUS_FC_SERIAL_REPORT_SERVER_ID           0x11
#endif
#ifndef MODBUS_FC_READ_FILE_RECORD
#define MODBUS_FC_READ_FILE_RECORD                  0x14
#endif
#ifndef MODBUS_FC_WRITE_FILE_RECORD
#define MODBUS_FC_WRITE_FILE_RECORD                 0x15
#endif
#ifndef MODBUS_FC_MASK_WRITE_REGISTER
#define MODBUS_FC_MASK_WRITE_REGISTER               0x16
#endif
#ifndef MODBUS_FC_WRITE_AND_READ_REGISTERS
#define MODBUS_FC_WRITE_AND_READ_REGISTERS          0x17
#endif
#ifndef MODBUS_FC_READ_FIFO_QUEUE
#define MODBUS_FC_READ_FIFO_QUEUE                   0x18
#endif
#ifndef MODBUS_FC_ENCAPSULATED_INTERFACE_TRANSPORT
#define MODBUS_FC_ENCAPSULATED_INTERFACE_TRANSPORT  0x2B
#endif

/*
 * 异常功能码
 */
#ifndef MODBUS_FC_EXCEPTION_BASE
/*
 *  对于异常的功能码，其功能码为 原功能码+MODBUS_FC_EXCEPTION_BASE,正常的功能码均小于 MODBUS_FC_EXCEPTION_BASE。
 */
#define MODBUS_FC_EXCEPTION_BASE 0x80
#endif

/*
 *  异常代码
 */
#ifndef MODBUS_EXCEPTION_ILLEGAL_FUNCITON
#define MODBUS_EXCEPTION_ILLEGAL_FUNCITON                           0x01
#endif
#ifndef MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS
#define MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS                       0x02
#endif
#ifndef MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE
#define MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE                         0x03
#endif
#ifndef MODBUS_EXCEPTION_SERVER_DEVICE_FAILURE
#define MODBUS_EXCEPTION_SERVER_DEVICE_FAILURE                      0x04
#endif
#ifndef MODBUS_EXCEPTION_ACKNOWLEDGE
#define MODBUS_EXCEPTION_ACKNOWLEDGE                                0x05
#endif
#ifndef MODBUS_EXCEPTION_SERVER_DEVICE_BUSY
#define MODBUS_EXCEPTION_SERVER_DEVICE_BUSY                         0x06
#endif
#ifndef MODBUS_EXCEPTION_NON_ACKNOWLEDGE
#define MODBUS_EXCEPTION_NON_ACKNOWLEDGE                            0x07
#endif
#ifndef MODBUS_EXCEPTION_MEMORY_PARITY_ERROR
#define MODBUS_EXCEPTION_MEMORY_PARITY_ERROR                        0x08
#endif
#ifndef MODBUS_EXCEPTION_GATEWAY_PATH_UNAVAILABLE
#define MODBUS_EXCEPTION_GATEWAY_PATH_UNAVAILABLE                   0x0A
#endif
#ifndef MODBUS_EXCEPTION_GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND
#define MODBUS_EXCEPTION_GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND    0x0B
#endif


/*
 *  modbus数据地址，地址范围0～0xFFFF，起始值为0
 */
typedef uint16_t modbus_data_address_t;

/*
 *  寄存器数据类型，16位
 */
typedef uint16_t modbus_data_register_t;

/*
 *  对于标准的modbus而言，数据的ID是从1开始的，其与数据地址一一对应，其使用场景为工业组态，本库编程时为了方便一般不使用从1开始的数据ID。
 */
#define MODBUS_DATA_ID_GET_ID_FROM_ADDRESS(addr)    ((addr)+1)
#define MODBUS_DATA_ID_GET_ADDRESS_FROM_ID(id)      ((id)-1)

#ifdef __cplusplus
}
#endif

#endif // __MODBUS_HMODBUS_COMMON_H__



