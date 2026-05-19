/***************************************************************
 * Name:      hz80cpu.h
 * Purpose:   声明hz80cpu接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-05-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HZ80CPU_H_INCLUDE__
#define __HZ80CPU_H_INCLUDE__
#include "h3rdparty_port_Z80.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

struct hz80cpu;
typedef struct hz80cpu hz80cpu_t;

/** \brief 获取Z80 CPU结构体大小
 *
 * \return size_t 大小
 *
 */
size_t hz80cpu_size(void);


/** \brief 初始化Z80 CPU结构体
 *
 * \param mem void*  内存指针，注意需要对齐
 * \param length size_t 内存大小
 * \return hz80cpu_t* Z80 CPU结构体指针
 *
 */
hz80cpu_t *hz80cpu_init(void *mem,size_t length);

/*
 * 回调,具体见Z80库
 */
enum hz80cpu_callback
{
    HZ80CPU_CALLBACK_FETCH_OPCODE=0,
    HZ80CPU_CALLBACK_FETCH,
    HZ80CPU_CALLBACK_READ,
    HZ80CPU_CALLBACK_WRITE,
    HZ80CPU_CALLBACK_IN,
    HZ80CPU_CALLBACK_OUT,
    HZ80CPU_CALLBACK_HALT,
    HZ80CPU_CALLBACK_NOP,
    HZ80CPU_CALLBACK_NMIA,
    HZ80CPU_CALLBACK_INTA,
    HZ80CPU_CALLBACK_INT_FETCH,
    HZ80CPU_CALLBACK_LD_I_A,
    HZ80CPU_CALLBACK_LD_R_A,
    HZ80CPU_CALLBACK_RETI,
    HZ80CPU_CALLBACK_RETN,
    HZ80CPU_CALLBACK_HOOK,
    HZ80CPU_CALLBACK_ILLEGAL,
};

/** \brief Z80 CPU回调
 *
 * \param usr void * 用户参数
 * \param cpu hz80cpu_t* Z80 CPU指针
 * \param cb_type enum hz80cpu_callback 回调类型
 * \param address uint16_t 地址
 * \param data uint8_t* 数据指针
 * \return bool 是否处理，处理成功返回true，返回false会给下一个回调
 *
 */
typedef bool (*hz80cpu_callback_function_t)(void *usr,hz80cpu_t *cpu,enum hz80cpu_callback cb_type,uint16_t address,uint8_t *data);

/** \brief Z80 CPU获取回调是否启用
 *
 * \param cpu hz80cpu_t* Z80 CPU指针
 * \param cb_type enum hz80cpu_callback 回调类型
 * \return bool 是否启用
 *
 */
bool hz80cpu_callback_enable_get(hz80cpu_t *cpu,enum hz80cpu_callback cb_type);

/** \brief Z80 CPU获取回调是否启用
 *
 * \param cpu hz80cpu_t* Z80 CPU指针
 * \param cb_type enum hz80cpu_callback 回调类型
 * \param enable bool 是否启用
 * \return bool 是否启用
 *
 */
bool hz80cpu_callback_enable_set(hz80cpu_t *cpu,enum hz80cpu_callback cb_type,bool enable);


/** \brief Z80 CPU回调函数注册
 *
 * \param cpu hz80cpu_t* Z80 CPU指针
 * \param cb hz80cpu_callback_function_t 回调函数
 * \param usr void* 用户参数
 * \return bool 成功注册
 *
 */
bool hz80cpu_callback_register(hz80cpu_t *cpu,hz80cpu_callback_function_t cb,void *usr);

/** \brief Z80 CPU回调函数反注册
 *
 * \param cpu hz80cpu_t* Z80 CPU指针
 * \param cb hz80cpu_callback_function_t 回调函数
 *
 */
void hz80cpu_callback_deregister(hz80cpu_t *cpu,hz80cpu_callback_function_t cb);


/*
 * Z80 CPU操作，具体含义见Z80库
 */
void hz80cpu_op_power(hz80cpu_t *cpu, bool state);
void hz80cpu_op_instant_reset(hz80cpu_t *cpu);
void hz80cpu_op_special_reset(hz80cpu_t *cpu);
void hz80cpu_op_int(hz80cpu_t *cpu, bool state);
void hz80cpu_op_nmi(hz80cpu_t *cpu);
size_t hz80cpu_op_execute(hz80cpu_t *cpu, size_t cycles);
size_t hz80cpu_op_run(hz80cpu_t *cpu, size_t cycles);
void hz80cpu_op_break(hz80cpu_t *cpu);
uint8_t hz80cpu_op_r(const hz80cpu_t * cpu);
uint16_t hz80cpu_op_refresh_address(const hz80cpu_t * cpu);
uint8_t hz80cpu_op_in_cycle(const hz80cpu_t * cpu);
uint8_t hz80cpu_op_out_cycle(const hz80cpu_t * cpu);
void hz80cpu_op_wait(hz80cpu_t *cpu, size_t cycles);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HZ80CPU_H_INCLUDE__
