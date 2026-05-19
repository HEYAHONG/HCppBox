/***************************************************************
 * Name:      hz80cpu.c
 * Purpose:   实现hz80cpu接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-05-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hz80cpu.h"
#include H3RDPARTY_Z80_HEADER

struct hz80cpu_core_callback
{
    hz80cpu_callback_function_t cb;
    void *usr;
};

typedef struct hz80cpu_core_callback hz80cpu_core_callback_t;

struct  hz80cpu
{
    Z80 core;
    hz80cpu_core_callback_t callback[8];
};

static bool hz80cpu_core_callback_dispatch(hz80cpu_t *cpu,enum hz80cpu_callback cb_type,uint16_t address,uint8_t *data)
{
    bool ret=false;
    if(cpu==NULL)
    {
        return ret;
    }
    for(size_t i=0; i<sizeof(cpu->callback)/sizeof(cpu->callback[0]); i++)
    {
        if(cpu->callback[i].cb!=NULL)
        {
            ret=cpu->callback[i].cb(cpu->callback[i].usr,cpu,cb_type,address,data);
        }
        if(ret)
        {
            break;
        }
    }
    return ret;
}

size_t hz80cpu_size(void)
{
    return sizeof(hz80cpu_t);
}

static zuint8 hz80cpu_callback_fetch_opcode(void *ctx,zuint16 address)
{
    uint8_t ret=0xFF;
    hz80cpu_t *cpu=(hz80cpu_t *)ctx;
    hz80cpu_core_callback_dispatch(cpu,HZ80CPU_CALLBACK_FETCH_OPCODE,address,&ret);
    return ret;
}

static zuint8 hz80cpu_callback_fetch(void *ctx,zuint16 address)
{
    uint8_t ret=0xFF;
    hz80cpu_t *cpu=(hz80cpu_t *)ctx;
    hz80cpu_core_callback_dispatch(cpu,HZ80CPU_CALLBACK_FETCH,address,&ret);
    return ret;
}

static zuint8 hz80cpu_callback_read(void *ctx,zuint16 address)
{
    uint8_t ret=0xFF;
    hz80cpu_t *cpu=(hz80cpu_t *)ctx;
    hz80cpu_core_callback_dispatch(cpu,HZ80CPU_CALLBACK_READ,address,&ret);
    return ret;
}

static void hz80cpu_callback_write(void *ctx,zuint16 address,zuint8 data)
{
    uint8_t ret=data;
    hz80cpu_t *cpu=(hz80cpu_t *)ctx;
    hz80cpu_core_callback_dispatch(cpu,HZ80CPU_CALLBACK_WRITE,address,&ret);
}

static zuint8 hz80cpu_callback_in(void *ctx,zuint16 address)
{
    uint8_t ret=0xFF;
    hz80cpu_t *cpu=(hz80cpu_t *)ctx;
    hz80cpu_core_callback_dispatch(cpu,HZ80CPU_CALLBACK_IN,address,&ret);
    return ret;
}

static void hz80cpu_callback_out(void *ctx,zuint16 address,zuint8 data)
{
    uint8_t ret=data;
    hz80cpu_t *cpu=(hz80cpu_t *)ctx;
    hz80cpu_core_callback_dispatch(cpu,HZ80CPU_CALLBACK_OUT,address,&ret);
}

static void hz80cpu_callback_halt(void *ctx,zuint8 signal_data)
{
    uint8_t ret=signal_data;
    hz80cpu_t *cpu=(hz80cpu_t *)ctx;
    hz80cpu_core_callback_dispatch(cpu,HZ80CPU_CALLBACK_HALT,0,&ret);
}

static zuint8 hz80cpu_callback_nop(void *ctx,zuint16 address)
{
    uint8_t ret=0xFF;
    hz80cpu_t *cpu=(hz80cpu_t *)ctx;
    hz80cpu_core_callback_dispatch(cpu,HZ80CPU_CALLBACK_NOP,address,&ret);
    return ret;
}

static zuint8 hz80cpu_callback_nmia(void *ctx,zuint16 address)
{
    uint8_t ret=0xFF;
    hz80cpu_t *cpu=(hz80cpu_t *)ctx;
    hz80cpu_core_callback_dispatch(cpu,HZ80CPU_CALLBACK_NMIA,address,&ret);
    return ret;
}

static zuint8 hz80cpu_callback_inta(void *ctx,zuint16 address)
{
    uint8_t ret=0xFF;
    hz80cpu_t *cpu=(hz80cpu_t *)ctx;
    hz80cpu_core_callback_dispatch(cpu,HZ80CPU_CALLBACK_INTA,address,&ret);
    return ret;
}

static zuint8 hz80cpu_callback_int_fetch(void *ctx,zuint16 address)
{
    uint8_t ret=0xFF;
    hz80cpu_t *cpu=(hz80cpu_t *)ctx;
    hz80cpu_core_callback_dispatch(cpu,HZ80CPU_CALLBACK_INT_FETCH,address,&ret);
    return ret;
}

static void hz80cpu_callback_ld_i_a(void *ctx)
{
    uint8_t ret=0xFF;
    hz80cpu_t *cpu=(hz80cpu_t *)ctx;
    hz80cpu_core_callback_dispatch(cpu,HZ80CPU_CALLBACK_LD_I_A,0,&ret);
}

static void hz80cpu_callback_ld_r_a(void *ctx)
{
    uint8_t ret=0xFF;
    hz80cpu_t *cpu=(hz80cpu_t *)ctx;
    hz80cpu_core_callback_dispatch(cpu,HZ80CPU_CALLBACK_LD_R_A,0,&ret);
}

static void hz80cpu_callback_reti(void *ctx)
{
    uint8_t ret=0xFF;
    hz80cpu_t *cpu=(hz80cpu_t *)ctx;
    hz80cpu_core_callback_dispatch(cpu,HZ80CPU_CALLBACK_RETI,0,&ret);
}

static void hz80cpu_callback_retn(void *ctx)
{
    uint8_t ret=0xFF;
    hz80cpu_t *cpu=(hz80cpu_t *)ctx;
    hz80cpu_core_callback_dispatch(cpu,HZ80CPU_CALLBACK_RETN,0,&ret);
}

static zuint8 hz80cpu_callback_hook(void *ctx,zuint16 address)
{
    uint8_t ret=0xFF;
    hz80cpu_t *cpu=(hz80cpu_t *)ctx;
    hz80cpu_core_callback_dispatch(cpu,HZ80CPU_CALLBACK_HOOK,address,&ret);
    return ret;
}

static zuint8 hz80cpu_callback_illegal(Z80 *cpu_core,zuint8 opcode)
{
    uint8_t ret=opcode;
    hz80cpu_t *cpu=(hz80cpu_t *)cpu_core;
    hz80cpu_core_callback_dispatch(cpu,HZ80CPU_CALLBACK_ILLEGAL,0,&ret);
    return ret;
}

hz80cpu_t *hz80cpu_init(void *mem,size_t length)
{
    if(mem == NULL || length < hz80cpu_size())
    {
        return NULL;
    }
    hz80cpu_t *ret=(hz80cpu_t *)mem;
    memset(ret,0,hz80cpu_size());

    ret->core.context=&ret;
    ret->core.fetch_opcode=hz80cpu_callback_fetch_opcode;
    ret->core.fetch=hz80cpu_callback_fetch;
    ret->core.read=hz80cpu_callback_read;
    ret->core.write=hz80cpu_callback_write;
    ret->core.in=hz80cpu_callback_in;
    ret->core.out=hz80cpu_callback_out;
    ret->core.halt=hz80cpu_callback_halt;
    ret->core.nop=hz80cpu_callback_nop;
    ret->core.nmia=hz80cpu_callback_nmia;
    ret->core.inta=hz80cpu_callback_inta;
    ret->core.int_fetch=hz80cpu_callback_int_fetch;
    ret->core.ld_i_a=hz80cpu_callback_ld_i_a;
    ret->core.ld_r_a=hz80cpu_callback_ld_r_a;
    ret->core.reti=hz80cpu_callback_reti;
    ret->core.retn=hz80cpu_callback_retn;
    ret->core.hook=NULL;
    ret->core.illegal=hz80cpu_callback_illegal;

    return ret;
}

bool hz80cpu_callback_enable_get(hz80cpu_t *cpu,enum hz80cpu_callback cb_type)
{
    bool ret=false;
    if(cpu == NULL)
    {
        return ret;
    }
    switch(cb_type)
    {
    case HZ80CPU_CALLBACK_FETCH_OPCODE:
    {
        ret=cpu->core.fetch_opcode!=NULL;
    }
    break;
    case HZ80CPU_CALLBACK_FETCH:
    {
        ret=cpu->core.fetch!=NULL;
    }
    break;
    case HZ80CPU_CALLBACK_READ:
    {
        ret=cpu->core.read!=NULL;
    }
    break;
    case HZ80CPU_CALLBACK_WRITE:
    {
        ret=cpu->core.write!=NULL;
    }
    break;
    case HZ80CPU_CALLBACK_IN:
    {
        ret=cpu->core.in!=NULL;
    }
    break;
    case HZ80CPU_CALLBACK_OUT:
    {
        ret=cpu->core.out!=NULL;
    }
    break;
    case HZ80CPU_CALLBACK_HALT:
    {
        ret=cpu->core.halt!=NULL;
    }
    break;
    case HZ80CPU_CALLBACK_NOP:
    {
        ret=cpu->core.nop!=NULL;
    }
    break;
    case HZ80CPU_CALLBACK_NMIA:
    {
        ret=cpu->core.nmia!=NULL;
    }
    break;
    case HZ80CPU_CALLBACK_INTA:
    {
        ret=cpu->core.inta!=NULL;
    }
    break;
    case HZ80CPU_CALLBACK_INT_FETCH:
    {
        ret=cpu->core.int_fetch!=NULL;
    }
    break;
    case HZ80CPU_CALLBACK_LD_I_A:
    {
        ret=cpu->core.ld_i_a!=NULL;
    }
    break;
    case HZ80CPU_CALLBACK_LD_R_A:
    {
        ret=cpu->core.ld_r_a!=NULL;
    }
    break;
    case HZ80CPU_CALLBACK_RETI:
    {
        ret=cpu->core.reti!=NULL;
    }
    break;
    case HZ80CPU_CALLBACK_RETN:
    {
        ret=cpu->core.retn!=NULL;
    }
    break;
    case HZ80CPU_CALLBACK_HOOK:
    {
        ret=cpu->core.hook!=NULL;
    }
    break;
    case HZ80CPU_CALLBACK_ILLEGAL:
    {
        ret=cpu->core.illegal!=NULL;
    }
    break;
    default:
    {

    }
    break;
    }
    return ret;
}

bool hz80cpu_callback_enable_set(hz80cpu_t *cpu,enum hz80cpu_callback cb_type,bool enable)
{
    if(cpu == NULL)
    {
        return false;
    }
    switch(cb_type)
    {
    case HZ80CPU_CALLBACK_HALT:
    {
        cpu->core.halt=(enable?(hz80cpu_callback_halt):NULL);
    }
    break;
    case HZ80CPU_CALLBACK_NOP:
    {
        cpu->core.nop=(enable?(hz80cpu_callback_nop):NULL);
    }
    break;
    case HZ80CPU_CALLBACK_NMIA:
    {
        cpu->core.nmia=(enable?(hz80cpu_callback_nmia):NULL);
    }
    break;
    case HZ80CPU_CALLBACK_INTA:
    {
        cpu->core.inta=(enable?(hz80cpu_callback_inta):NULL);
    }
    break;
    case HZ80CPU_CALLBACK_INT_FETCH:
    {
        cpu->core.int_fetch=(enable?(hz80cpu_callback_int_fetch):NULL);
    }
    break;
    case HZ80CPU_CALLBACK_LD_I_A:
    {
        cpu->core.ld_i_a=(enable?(hz80cpu_callback_ld_i_a):NULL);
    }
    break;
    case HZ80CPU_CALLBACK_LD_R_A:
    {
        cpu->core.ld_r_a=(enable?(hz80cpu_callback_ld_r_a):NULL);
    }
    break;
    case HZ80CPU_CALLBACK_RETI:
    {
        cpu->core.reti=(enable?(hz80cpu_callback_reti):NULL);
    }
    break;
    case HZ80CPU_CALLBACK_RETN:
    {
        cpu->core.retn=(enable?(hz80cpu_callback_retn):NULL);
    }
    break;
    case HZ80CPU_CALLBACK_HOOK:
    {
        cpu->core.hook=(enable?(hz80cpu_callback_hook):NULL);
    }
    break;
    case HZ80CPU_CALLBACK_ILLEGAL:
    {
        cpu->core.illegal=(enable?(hz80cpu_callback_illegal):NULL);
    }
    break;
    default:
    {

    }
    break;
    }
    return hz80cpu_callback_enable_get(cpu,cb_type);
}

bool hz80cpu_callback_register(hz80cpu_t *cpu,hz80cpu_callback_function_t cb,void *usr)
{
    bool ret=false;
    if(cpu == NULL || cb ==NULL)
    {
        return ret;
    }

    for(size_t i=0; i<sizeof(cpu->callback)/sizeof(cpu->callback[0]); i++)
    {
        if(cpu->callback[i].cb==NULL)
        {
            cpu->callback[i].cb=cb;
            cpu->callback[i].usr=usr;
        }
    }

    return ret;
}

void hz80cpu_callback_deregister(hz80cpu_t *cpu,hz80cpu_callback_function_t cb)
{
    if(cpu==NULL || cb == NULL)
    {
        return;
    }
    for(size_t i=0; i<sizeof(cpu->callback)/sizeof(cpu->callback[0]); i++)
    {
        if(cpu->callback[i].cb==cb)
        {
            cpu->callback[i].cb=NULL;
        }
    }
}

void hz80cpu_op_power(hz80cpu_t *cpu, bool state)
{
    if(cpu==NULL)
    {
        return;
    }
    z80_power(&cpu->core,state?Z_TRUE:Z_FALSE);
}
void hz80cpu_op_instant_reset(hz80cpu_t *cpu)
{
    if(cpu==NULL)
    {
        return;
    }
    z80_instant_reset(&cpu->core);

}
void hz80cpu_op_special_reset(hz80cpu_t *cpu)
{
    if(cpu==NULL)
    {
        return;
    }
    z80_special_reset(&cpu->core);

}
void hz80cpu_op_int(hz80cpu_t *cpu, bool state)
{
    if(cpu==NULL)
    {
        return;
    }
    z80_int(&cpu->core,state?Z_TRUE:Z_FALSE);
}
void hz80cpu_op_nmi(hz80cpu_t *cpu)
{
    if(cpu==NULL)
    {
        return;
    }
    z80_nmi(&cpu->core);
}
size_t hz80cpu_op_execute(hz80cpu_t *cpu, size_t cycles)
{
    if(cpu==NULL)
    {
        return 0;
    }
    return z80_execute(&cpu->core,cycles);
}
size_t hz80cpu_op_run(hz80cpu_t *cpu, size_t cycles)
{
    if(cpu==NULL)
    {
        return 0;
    }
    return z80_run(&cpu->core,cycles);
}
void hz80cpu_op_break(hz80cpu_t *cpu)
{
    if(cpu==NULL)
    {
        return;
    }
    z80_break(&cpu->core);
}
uint8_t hz80cpu_op_r(const hz80cpu_t * cpu)
{
    if(cpu==NULL)
    {
        return 0;
    }
    return z80_r(&cpu->core);
}
uint16_t hz80cpu_op_refresh_address(const hz80cpu_t * cpu)
{
    if(cpu==NULL)
    {
        return 0;
    }
    return z80_refresh_address(&cpu->core);
}
uint8_t hz80cpu_op_in_cycle(const hz80cpu_t * cpu)
{
    if(cpu==NULL)
    {
        return 0;
    }
    return z80_in_cycle(&cpu->core);
}
uint8_t hz80cpu_op_out_cycle(const hz80cpu_t * cpu)
{
    if(cpu==NULL)
    {
        return 0;
    }
    return z80_out_cycle(&cpu->core);
}
void hz80cpu_op_wait(hz80cpu_t *cpu, size_t cycles)
{
    if(cpu==NULL)
    {
        return;
    }
    z80_wait(&cpu->core,cycles);
}


