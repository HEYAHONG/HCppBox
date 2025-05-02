/***************************************************************
 * Name:      hmodbus_io_interface.h
 * Purpose:   hmodbus_io_interface
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-03-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __MODBUS_HMODBUS_IO_INTERFACE_H__
#define __MODBUS_HMODBUS_IO_INTERFACE_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
 *  用于modbus rtu主机/tcp客户端的IO接口定义
 */
struct modbus_io_interface;
typedef struct modbus_io_interface modbus_io_interface_t;
struct modbus_io_interface
{
    void *usr; /**< 用户指针，用户可使用此指针传递用户数据 */
    size_t (*send)(modbus_io_interface_t *io,const uint8_t *adu,size_t adu_length); /**< 发送数据,注意:用户必须在此函数中将一帧数据发送完并且返回已发送的数据长度 */
    size_t (*recv)(modbus_io_interface_t *io,uint8_t *buffer,size_t buffer_length); /**< 接收数据,注意:用户必须在此函数中将一帧数据接收完并且返回已接收的数据长度 */
};


/** \brief 默认io接口,用于初始化接口
 *
 * \return modbus_io_interface_t io接口
 *
 */
modbus_io_interface_t modbus_io_interface_default(void);

typedef enum
{
    MODBUS_IO_INTERFACE_TCP_CLIENT=0,       //TCP Client请求(modbus tcp，不支持串口功能码)
    MODBUS_IO_INTERFACE_RTU_MASTER,         //RTU Master请求((modbus tcp)
    MODBUS_IO_INTERFACE_TCP_GATEWAY_CLIENT  //TCP Gateway Client请求(modbus tcp,支持串口功能码)
} modbus_io_interface_request_t;

/*
 * IO接口上下文（基类）
 */
struct modbus_io_interface_context_base;
typedef struct modbus_io_interface_context_base modbus_io_interface_context_base_t;
struct modbus_io_interface_context_base
{
    uint8_t slave_addr;//从机地址，RTU与TCP均有效
    uint16_t Tid;//发送标识，TCP有效，可由用户设置单次请求的发送标识，每请求一次便自增一。对于RTU而言，仅有计数作用
    void *usr;//用户指针，由用户确定使用场景
    void (*on_exception)(modbus_io_interface_context_base_t *ctx,uint8_t function_code,uint8_t exception_code);//异常处理，注意功能码为异常功能码（大于MODBUS_FC_EXCEPTION_BASE）
};


/** \brief 获取接口上下文（基类）
 *
 * \return modbus_io_interface_context_base_t modbus_io_interface_context_base_t 接口上下文（基类）
 *
 */
modbus_io_interface_context_base_t modbus_io_interface_context_base_default(void);

/*
 *  读取线圈上下文
 */
struct modbus_io_interface_context_read_coils;
typedef struct modbus_io_interface_context_read_coils modbus_io_interface_context_read_coils_t;
struct modbus_io_interface_context_read_coils
{
    modbus_io_interface_context_base_t base;
    void *usr;//用户指针，由用户确定使用场景
    void (*on_read_coils)(modbus_io_interface_context_read_coils_t *ctx,modbus_data_address_t addr,bool value);
    modbus_data_address_t   starting_address;//起始地址
    modbus_data_register_t  quantity_of_coils;//不超过MODBUS_MAX_READ_BITS且至少为1
};

/** \brief  读取线圈上下文
 *
 * \return modbus_io_interface_context_read_coils_t 读取线圈上下文
 *
 */
modbus_io_interface_context_read_coils_t modbus_io_interface_context_read_coils_default(void);

/*
 *  读取离散输入上下文
 */
struct modbus_io_interface_context_read_discrete_inputs;
typedef struct modbus_io_interface_context_read_discrete_inputs modbus_io_interface_context_read_discrete_inputs_t;
struct modbus_io_interface_context_read_discrete_inputs
{
    modbus_io_interface_context_base_t base;
    void *usr;//用户指针，由用户确定使用场景
    void (*on_read_discrete_inputs)(modbus_io_interface_context_read_discrete_inputs_t *ctx,modbus_data_address_t addr,bool value);
    modbus_data_address_t   starting_address;//起始地址
    modbus_data_register_t  quantity_of_inputs;//不超过MODBUS_MAX_READ_BITS且至少为1
};

/** \brief  读取离散输入上下文
 *
 * \return modbus_io_interface_context_read_discrete_inputs_t 读取离散输入上下文
 *
 */
modbus_io_interface_context_read_discrete_inputs_t modbus_io_interface_context_read_discrete_inputs_default(void);

/*
 *  读取保持寄存器上下文
 */
struct modbus_io_interface_context_read_holding_registers;
typedef struct modbus_io_interface_context_read_holding_registers modbus_io_interface_context_read_holding_registers_t;
struct modbus_io_interface_context_read_holding_registers
{
    modbus_io_interface_context_base_t base;
    void *usr;//用户指针，由用户确定使用场景
    void (*on_read_holding_registers)(modbus_io_interface_context_read_holding_registers_t *ctx,modbus_data_address_t addr,modbus_data_register_t value);
    modbus_data_address_t   starting_address;//起始地址
    modbus_data_register_t  quantity_of_registers;//不超过MODBUS_MAX_READ_REGISTERS且至少为1
};

/** \brief  读取保持寄存器上下文
 *
 * \return modbus_io_interface_context_read_holding_registers_t 读取保持寄存器上下文
 *
 */
modbus_io_interface_context_read_holding_registers_t modbus_io_interface_context_read_holding_registers_default(void);

/*
 *  读取输入寄存器上下文
 */
struct modbus_io_interface_context_read_input_registers;
typedef struct modbus_io_interface_context_read_input_registers modbus_io_interface_context_read_input_registers_t;
struct modbus_io_interface_context_read_input_registers
{
    modbus_io_interface_context_base_t base;
    void *usr;//用户指针，由用户确定使用场景
    void (*on_read_input_registers)(modbus_io_interface_context_read_input_registers_t *ctx,modbus_data_address_t addr,modbus_data_register_t value);
    modbus_data_address_t   starting_address;//起始地址
    modbus_data_register_t  quantity_of_registers;//不超过MODBUS_MAX_READ_REGISTERS且至少为1
};

/** \brief  读取输入寄存器上下文
 *
 * \return modbus_io_interface_context_read_input_registers_t 读取输入寄存器上下文
 *
 */
modbus_io_interface_context_read_input_registers_t modbus_io_interface_context_read_input_registers_default(void);

/*
 *  写单个线圈上下文
 */
struct modbus_io_interface_context_write_single_coil;
typedef struct modbus_io_interface_context_write_single_coil modbus_io_interface_context_write_single_coil_t;
struct modbus_io_interface_context_write_single_coil
{
    modbus_io_interface_context_base_t base;
    void *usr;//用户指针，由用户确定使用场景
    void (*on_write_single_coil)(modbus_io_interface_context_write_single_coil_t *ctx,modbus_data_address_t addr,modbus_data_register_t value);
    modbus_data_address_t   output_address;//地址
    bool output_value;//值，注意：回调函数仅用于用户检查输出值，在调用回调函数前此值将被更新。
};

/** \brief  写单个线圈上下文
 *
 * \return modbus_io_interface_context_write_single_coil_t 写单个线圈上下文
 *
 */
modbus_io_interface_context_write_single_coil_t modbus_io_interface_context_write_single_coil_default(void);

/*
 *  写单个寄存器上下文
 */
struct modbus_io_interface_context_write_single_register;
typedef struct modbus_io_interface_context_write_single_register modbus_io_interface_context_write_single_register_t;
struct modbus_io_interface_context_write_single_register
{
    modbus_io_interface_context_base_t base;
    void *usr;//用户指针，由用户确定使用场景
    void (*on_write_single_register)(modbus_io_interface_context_write_single_register_t *ctx,modbus_data_address_t addr,modbus_data_register_t value);
    modbus_data_address_t   output_address;//地址
    modbus_data_register_t output_value;//值，注意：回调函数仅用于用户检查输出值，在调用回调函数前此值将被更新。
};

/** \brief  写单个寄存器上下文
 *
 * \return modbus_io_interface_context_write_single_register_t 写单个寄存器上下文
 *
 */
modbus_io_interface_context_write_single_register_t modbus_io_interface_context_write_single_register_default(void);

/*
 *  读异常代码上下文(注意：此处的异常更类似设备的状态码，按位定义，而非modbus通信异常代码。)
 */
struct modbus_io_interface_context_read_exception_status;
typedef struct modbus_io_interface_context_read_exception_status modbus_io_interface_context_read_exception_status_t;
struct modbus_io_interface_context_read_exception_status
{
    modbus_io_interface_context_base_t base;
    void *usr;//用户指针，由用户确定使用场景
    void (*on_read_exception_status)(modbus_io_interface_context_read_exception_status_t *ctx,uint8_t exception_code);
};

/** \brief  读异常代码上下文
 *
 * \return modbus_io_interface_context_read_exception_status_t 读异常代码上下文
 *
 */
modbus_io_interface_context_read_exception_status_t modbus_io_interface_context_read_exception_status_default(void);

/*
 *  诊断上下文（用于检查通信系统）
 */
struct modbus_io_interface_context_diagnostics;
typedef struct modbus_io_interface_context_diagnostics modbus_io_interface_context_diagnostics_t;
struct modbus_io_interface_context_diagnostics
{
    modbus_io_interface_context_base_t base;
    void *usr;//用户指针，由用户确定使用场景
    uint16_t sub_function;//子功能码，见Modbus_Application_Protocol_V1_1b3.pdf 6.8
    size_t  (*request_data)(modbus_io_interface_context_diagnostics_t *ctx,uint8_t *data,size_t max_data_length);//设置请求数据
    bool    (*response_data)(modbus_io_interface_context_diagnostics_t *ctx,const uint8_t *data,size_t data_length);//处理响应数据
};

/** \brief  诊断上下文
 *
 * \return modbus_io_interface_context_diagnostics_t 诊断上下文
 *
 */
modbus_io_interface_context_diagnostics_t modbus_io_interface_context_diagnostics_default(void);


/*
 *  获取通信事件计数器上下文
 */
struct modbus_io_interface_context_get_comm_event_counter;
typedef struct modbus_io_interface_context_get_comm_event_counter modbus_io_interface_context_get_comm_event_counter_t;
struct modbus_io_interface_context_get_comm_event_counter
{
    modbus_io_interface_context_base_t base;
    void *usr;//用户指针，由用户确定使用场景
    void (*on_get_comm_event_counter)(modbus_io_interface_context_get_comm_event_counter_t *ctx,uint16_t status,uint16_t event_count);
};

/** \brief  获取通信事件计数器上下文
 *
 * \return modbus_io_interface_context_get_comm_event_counter_t 获取通信事件计数器上下文
 *
 */
modbus_io_interface_context_get_comm_event_counter_t modbus_io_interface_context_get_comm_event_counter_default(void);

/*
 *  获取通信事件记录上下文
 */
struct modbus_io_interface_context_get_comm_event_log;
typedef struct modbus_io_interface_context_get_comm_event_log modbus_io_interface_context_get_comm_event_log_t;
struct modbus_io_interface_context_get_comm_event_log
{
    modbus_io_interface_context_base_t base;
    void *usr;//用户指针，由用户确定使用场景
    void (*on_get_comm_event_log)(modbus_io_interface_context_get_comm_event_log_t *ctx,uint16_t status,uint16_t event_count,uint16_t message_count,const uint8_t *events,size_t events_length);
};

/** \brief  获取通信事件记录上下文
 *
 * \return modbus_io_interface_context_get_comm_event_log_t 获取通信事件记录上下文
 *
 */
modbus_io_interface_context_get_comm_event_log_t modbus_io_interface_context_get_comm_event_log_default(void);


/*
 *  写多个线圈上下文
 */
struct modbus_io_interface_context_write_multiple_coils;
typedef struct modbus_io_interface_context_write_multiple_coils modbus_io_interface_context_write_multiple_coils_t;
struct modbus_io_interface_context_write_multiple_coils
{
    modbus_io_interface_context_base_t base;
    void *usr;//用户指针，由用户确定使用场景
    void (*coil_value)(modbus_io_interface_context_write_multiple_coils_t *ctx,modbus_data_address_t addr,bool *value);//线圈值,此回调函数应当提供具体待写入的值
    void (*on_write_multiple_coils)(modbus_io_interface_context_write_multiple_coils_t *ctx,modbus_data_address_t addr,modbus_data_register_t length);
    modbus_data_address_t starting_address;//待写入的起始地址
    modbus_data_register_t quantity_of_output;//输出的数量，需要不超过MODBUS_MAX_WRITE_BITS且至少为1
};

/** \brief  写多个线圈上下文
 *
 * \return modbus_io_interface_context_write_multiple_coils_t 写多个线圈上下文
 *
 */
modbus_io_interface_context_write_multiple_coils_t modbus_io_interface_context_write_multiple_coils_default(void);

/*
 *  写多个寄存器上下文
 */
struct modbus_io_interface_context_write_multiple_registers;
typedef struct modbus_io_interface_context_write_multiple_registers modbus_io_interface_context_write_multiple_registers_t;
struct modbus_io_interface_context_write_multiple_registers
{
    modbus_io_interface_context_base_t base;
    void *usr;//用户指针，由用户确定使用场景
    void (*register_value)(modbus_io_interface_context_write_multiple_registers_t *ctx,modbus_data_address_t addr,modbus_data_register_t *value);//寄存器值,此回调函数应当提供具体待写入的值
    void (*on_write_multiple_registers)(modbus_io_interface_context_write_multiple_registers_t *ctx,modbus_data_address_t addr,modbus_data_register_t length);
    modbus_data_address_t starting_address;//待写入的起始地址
    modbus_data_register_t quantity_of_output;//输出的数量，需要不超过MODBUS_MAX_WRITE_REGISTERS且至少为1
};

/** \brief  写多个寄存器上下文
 *
 * \return modbus_io_interface_context_write_multiple_registers_t 写多个寄存器上下文
 *
 */
modbus_io_interface_context_write_multiple_registers_t modbus_io_interface_context_write_multiple_registers_default(void);

/*
 *  报告服务器ID上下文(注意：本上下文中server_id由设备自定义,包含所有modbus字段（包含服务器id、运行状态指示、额外数据）)
 */
struct modbus_io_interface_context_report_server_id;
typedef struct modbus_io_interface_context_report_server_id modbus_io_interface_context_report_server_id_t;
struct modbus_io_interface_context_report_server_id
{
    modbus_io_interface_context_base_t base;
    void *usr;//用户指针，由用户确定使用场景
    void (*on_report_server_id)(modbus_io_interface_context_report_server_id_t *ctx,const uint8_t *server_id,size_t server_id_length);
};

/** \brief  报告服务器ID上下文
 *
 * \return modbus_io_interface_context_report_server_id_t 报告服务器ID上下文
 *
 */
modbus_io_interface_context_report_server_id_t modbus_io_interface_context_report_server_id_default(void);

/*
 *  读取文件记录上下文
 */
struct modbus_io_interface_context_read_file_record;
typedef struct modbus_io_interface_context_read_file_record modbus_io_interface_context_read_file_record_t;
struct modbus_io_interface_context_read_file_record
{
    modbus_io_interface_context_base_t base;
    void *usr;//用户指针，由用户确定使用场景
    void    (*file_record)(modbus_io_interface_context_read_file_record_t *ctx,size_t x,uint8_t *reference_type,uint16_t *file_number,uint16_t *record_number,uint16_t *record_length);//此回调提供文件记录信息
    void    (*on_read_file_record)(modbus_io_interface_context_read_file_record_t *ctx,size_t x,uint8_t reference_type,const uint8_t *data,size_t data_length);//读取回调，注意此处的数据（data）指（16位,大端模式）寄存器的数据缓冲，长度为record_length的两倍。
    size_t  x_max;//子请求长度,不超过35且至少为1
};

/** \brief  读取文件记录上下文
 *
 * \return modbus_io_interface_context_read_file_record_t 读取文件记录上下文
 *
 */
modbus_io_interface_context_read_file_record_t modbus_io_interface_context_read_file_record_default(void);

/*
 *  写入文件记录上下文
 */
struct modbus_io_interface_context_write_file_record;
typedef struct modbus_io_interface_context_write_file_record modbus_io_interface_context_write_file_record_t;
struct modbus_io_interface_context_write_file_record
{
    modbus_io_interface_context_base_t base;
    void *usr;//用户指针，由用户确定使用场景
    size_t  (*file_record)(modbus_io_interface_context_write_file_record_t *ctx,size_t x,uint8_t *reference_type,uint16_t *file_number,uint16_t *record_number,uint8_t *data,size_t data_max_length);//此回调提供文件记录信息,注意此处的数据（data）指（16位,大端模式）寄存器的数据缓冲，返回的长度为record_length的两倍。
    void    (*on_write_file_record)(modbus_io_interface_context_write_file_record_t *ctx,size_t x,uint8_t reference_type,uint16_t file_number,uint16_t record_number,const uint8_t *data,size_t data_length);//此回调提供文件记录信息,注意此处的数据（data）指（16位,大端模式）寄存器的数据缓冲，长度为record_length的两倍。
    size_t  x_max;//子请求长度,至少为1,所有子请求加起来的pdu长度不可大于MODBUS_MAX_PDU_LENGTH
};

/** \brief  写入文件记录上下文
 *
 * \return modbus_io_interface_context_write_file_record_t 写入文件记录上下文
 *
 */
modbus_io_interface_context_write_file_record_t modbus_io_interface_context_write_file_record_default(void);

/*
 *  掩码写寄存器上下文
 */
struct modbus_io_interface_context_mask_write_register;
typedef struct modbus_io_interface_context_mask_write_register modbus_io_interface_context_mask_write_register_t;
struct modbus_io_interface_context_mask_write_register
{
    modbus_io_interface_context_base_t base;
    void *usr;//用户指针，由用户确定使用场景
    void (*on_mask_write_register)(modbus_io_interface_context_mask_write_register_t *ctx,modbus_data_address_t addr,modbus_data_register_t and_mask,modbus_data_register_t or_mask);
    modbus_data_address_t reference_address;
    modbus_data_register_t and_mask;
    modbus_data_register_t or_mask;
};

/** \brief  掩码写寄存器上下文
 *
 * \return modbus_io_interface_context_mask_write_register_t 掩码写寄存器上下文
 *
 */
modbus_io_interface_context_mask_write_register_t modbus_io_interface_context_mask_write_register_default(void);

/*
 *  读写多个寄存器上下文
 */
struct modbus_io_interface_context_read_write_multiple_registers;
typedef struct modbus_io_interface_context_read_write_multiple_registers modbus_io_interface_context_read_write_multiple_registers_t;
struct modbus_io_interface_context_read_write_multiple_registers
{
    modbus_io_interface_context_base_t base;
    void *usr;//用户指针，由用户确定使用场景
    void (*write_register_value)(modbus_io_interface_context_read_write_multiple_registers_t *ctx,modbus_data_address_t addr,modbus_data_register_t *value);//此回调函数用于提供待写入的寄存器值
    void (*on_read_multiple_registers)(modbus_io_interface_context_read_write_multiple_registers_t *ctx,modbus_data_address_t addr,modbus_data_register_t value);
    modbus_data_address_t read_starting_address;
    modbus_data_register_t quantity_to_read;//待读取的寄存器，不超过MODBUS_MAX_WR_READ_REGISTERS且至少为1
    modbus_data_address_t write_starting_address;
    modbus_data_register_t quantity_to_write;//待写入的寄存器，不超过MODBUS_MAX_WR_WRITE_REGISTERS且至少为1
};

/** \brief  读写多个寄存器上下文
 *
 * \return modbus_io_interface_context_read_write_multiple_registers_t 读写多个寄存器上下文
 *
 */
modbus_io_interface_context_read_write_multiple_registers_t modbus_io_interface_context_read_write_multiple_registers_default(void);

/*
 *  读取FIFO队列上下文
 */
struct modbus_io_interface_context_read_fifo_queue;
typedef struct modbus_io_interface_context_read_fifo_queue modbus_io_interface_context_read_fifo_queue_t;
struct modbus_io_interface_context_read_fifo_queue
{
    modbus_io_interface_context_base_t base;
    void *usr;//用户指针，由用户确定使用场景
    void  (*on_read_fifo_queue)(modbus_io_interface_context_read_fifo_queue_t *ctx,const uint8_t *data,size_t data_length);//注意此处的数据（data）指（16位,大端模式）寄存器的数据缓冲，长度为fifo_count的两倍。
    modbus_data_address_t fifo_pointer_address;
};

/** \brief  读取FIFO队列上下文
 *
 * \return modbus_io_interface_context_read_fifo_queue_t 读取FIFO队列上下文
 *
 */
modbus_io_interface_context_read_fifo_queue_t modbus_io_interface_context_read_fifo_queue_default(void);

/*
 *  封装传输上下文（注意：此上下文使用时需要根据MEI（modbus封装接口）不同而处理不同数据）
 */
struct modbus_io_interface_context_encapsulated_interface_transport;
typedef struct modbus_io_interface_context_encapsulated_interface_transport modbus_io_interface_context_encapsulated_interface_transport_t;
struct modbus_io_interface_context_encapsulated_interface_transport
{
    modbus_io_interface_context_base_t base;
    void *usr;//用户指针，由用户确定使用场景
    size_t  (*mei_request)(modbus_io_interface_context_encapsulated_interface_transport_t *ctx,uint8_t *mei_data,size_t mei_data_max_length);//设置请求数据
    void    (*mei_response)(modbus_io_interface_context_encapsulated_interface_transport_t *ctx,const uint8_t *mei_data,size_t mei_data_length);//处理响应数据
    uint8_t mei_type;
};

/** \brief  封装传输上下文
 *
 * \return modbus_io_interface_context_encapsulated_interface_transport_t 封装传输上下文
 *
 */
modbus_io_interface_context_encapsulated_interface_transport_t modbus_io_interface_context_encapsulated_interface_transport_default(void);

/** \brief IO请求(rtu主机或者tcp客户端)
 *          注意：此函数对栈的要求较高，需要保证栈足够大
 *
 * \param type modbus_io_interface_request_t 请求类型
 * \param io modbus_io_interface_t* IO接口
 * \param function_code uint8_t 功能码，见MODBUS_FC_*
 * \param context void* 上下文，根据功能码的不同而不同
 * \param context_length size_t 上下文长度
 * \return bool 是否成功请求
 *
 */
bool modbus_io_interface_request(modbus_io_interface_request_t type,modbus_io_interface_t *io,uint8_t function_code,void *context,size_t context_length);


#ifdef __cplusplus
}
#endif

#endif // __MODBUS_HMODBUS_IO_INTERFACE_H__



