/***************************************************************
 * Name:      risc-v_machine_rv64.c
 * Purpose:   实现risc-v_machine_rv64接口(虚拟CPU模型-hsimulator)
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "risc-v_machine_rv64.h"
#ifndef RISCV_ENCODING_H
#include "encoding.out.h"
#endif

#define HS_MACHINE_CSR_W_MSTATUS (1UL<<0)
#define HS_MACHINE_CSR_W_MEDELEG (1UL<<1)
#define HS_MACHINE_CSR_W_MIDELEG (1UL<<2)
#define HS_MACHINE_CSR_W_MIE (1UL<<3)
#define HS_MACHINE_CSR_W_MTVEC (1UL<<4)
#define HS_MACHINE_CSR_W_MCOUNTEREN (1UL<<5)
#define HS_MACHINE_CSR_W_MSCRATCH (1UL<<6)
#define HS_MACHINE_CSR_W_MEPC (1UL<<7)
#define HS_MACHINE_CSR_W_MCAUSE (1UL<<8)
#define HS_MACHINE_CSR_W_MTVAL (1UL<<9)
#define HS_MACHINE_CSR_W_MIP (1UL<<10)
#define HS_MACHINE_CSR_W_MSTATUSH (1UL<<11)
#define HS_MACHINE_CSR_W_SSTATUS (1UL<<12)
#define HS_MACHINE_CSR_W_SIE (1UL<<13)
#define HS_MACHINE_CSR_W_STVEC (1UL<<14)
#define HS_MACHINE_CSR_W_SCOUNTEREN (1UL<<15)
#define HS_MACHINE_CSR_W_SSCRATCH (1UL<<16)
#define HS_MACHINE_CSR_W_SEPC (1UL<<17)
#define HS_MACHINE_CSR_W_SCAUSE (1UL<<18)
#define HS_MACHINE_CSR_W_STVAL (1UL<<19)
#define HS_MACHINE_CSR_W_SIP (1UL<<20)
#define HS_MACHINE_CSR_W_SATP (1UL<<21)


/*
 * 地址空间划分(高4位)
 */
#define HS_RISC_V_MACHINE_RV64_FLASH_BASE       0x10000000UL
#define HS_RISC_V_MACHINE_RV64_RAM_BASE         0x20000000UL
#define HS_RISC_V_MACHINE_RV64_ROM_BASE         0xD0000000UL
#define HS_RISC_V_MACHINE_RV64_IO_BASE          0xE0000000UL

#define HS_RISC_V_MACHINE_RV64_X_BASE           0xF0000000UL
#define HS_RISC_V_MACHINE_RV64_X_END            0xF0000200UL
#define HS_RISC_V_MACHINE_RV64_PC_BASE          0xF0000200UL
#define HS_RISC_V_MACHINE_RV64_PC_END           0xF0000210UL
#define HS_RISC_V_MACHINE_RV64_XLEN_BASE        0xF0000210UL
#define HS_RISC_V_MACHINE_RV64_XLEN_END         0xF0000220UL
#define HS_RISC_V_MACHINE_RV64_F_BASE           0xF0001000UL
#define HS_RISC_V_MACHINE_RV64_F_END            0xF0001200UL
#define HS_RISC_V_MACHINE_RV64_FCSR_BASE        0xF0001200UL
#define HS_RISC_V_MACHINE_RV64_FCSR_END         0xF0001210UL
#define HS_RISC_V_MACHINE_RV64_FLEN_BASE        0xF0001210UL
#define HS_RISC_V_MACHINE_RV64_FLEN_END         0xF0001220UL
#define HS_RISC_V_MACHINE_RV64_CSR_BASE         0xF1000000UL
#define HS_RISC_V_MACHINE_RV64_MTIME_BASE       0xF0000300UL
#define HS_RISC_V_MACHINE_RV64_MTIME_END        0xF0000310UL
#define HS_RISC_V_MACHINE_RV64_MTIMECMP_BASE    0xF0000310UL

/*
 * 虚拟CPU模型规定: 向量基地址固定为0，向量模式，不可修改。
 * 复位向量为中断号16，即地址0x40。
 */
#define HS_RISC_V_MACHINE_RV64_VECTOR_BASE      0x00000000UL
#define HS_RISC_V_MACHINE_RV64_VECTOR_MODE      0x00000001UL
#define HS_RISC_V_MACHINE_RV64_RESET_VECTOR     (HS_RISC_V_MACHINE_RV64_VECTOR_BASE+4UL*16UL)
#define HS_RISC_V_MACHINE_RV64_MTIMECMP_END     0xF0000320UL
#define HS_RISC_V_MACHINE_RV64_DTB_BASE         0xF2000000UL

static hs_risc_v_core_rv64_t *hs_risc_v_machine_rv64_core_get(hs_risc_v_machine_rv64_t *machine)
{
    if(machine==NULL)
    {
        return NULL;
    }
    return (hs_risc_v_core_rv64_t *)machine->core.bytes;
}

static uint32_t hs_risc_v_machine_rv64_read_u32_le(const uint8_t *data,size_t len)
{
    uint32_t value=0;
    for(size_t i=0; i<len && i<sizeof(value); i++)
    {
        value|=((uint32_t)data[i])<<(8*i);
    }
    return value;
}

static uint64_t hs_risc_v_machine_rv64_read_u64_le(const uint8_t *data,size_t len)
{
    uint64_t value=0;
    for(size_t i=0; i<len && i<sizeof(value); i++)
    {
        value|=((uint64_t)data[i])<<(8*i);
    }
    return value;
}

static void hs_risc_v_machine_rv64_write_u64_le(uint8_t *data,size_t len,uint64_t value)
{
    for(size_t i=0; i<len && i<sizeof(value); i++)
    {
        data[i]=(uint8_t)((value>>(8*i))&0xFF);
    }
}

static size_t hs_risc_v_machine_rv64_region_read(const hs_risc_v_machine_rv64_memory_t *mem,uint32_t address,void *data,size_t len)
{
    if(mem!=NULL && mem->read!=NULL)
    {
        return mem->read(mem,address,data,len);
    }
    return 0;
}

static size_t hs_risc_v_machine_rv64_region_write(const hs_risc_v_machine_rv64_memory_t *mem,uint32_t address,const void *data,size_t len)
{
    if(mem!=NULL && mem->write!=NULL)
    {
        return mem->write(mem,address,data,len);
    }
    return 0;
}

#define HS_MACHINE_CSR_READ_CASE(ADDR,FLAG,FIELD) \
    case ADDR: \
        if((machine->csr_written&FLAG)==0) \
        { \
            return false; \
        } \
        value=machine->FIELD; \
        break;

#define HS_MACHINE_CSR_WRITE_CASE(ADDR,FLAG,FIELD) \
    case ADDR: \
        machine->FIELD=hs_risc_v_machine_rv64_read_u64_le(data,len); \
        machine->csr_written|=FLAG; \
        return true;

/*
 * 部分CSR由机器实现(浮点CSR/计数器/时间)
 */
static bool hs_risc_v_machine_rv64_csr_read_internal(hs_risc_v_machine_rv64_t *machine,uint32_t csr,uint8_t *data,size_t len)
{
    uint64_t value=0;
    switch(csr)
    {
    case CSR_FFLAGS:
    {
        value=machine->fcsr&0x1F;
    }
    break;
    case CSR_FRM:
    {
        value=(machine->fcsr>>5)&0x7;
    }
    break;
    case CSR_FCSR:
    {
        value=machine->fcsr&0xFF;
    }
    break;
    case CSR_TIME:
    {
        value=machine->mtime;
    }
    break;
    case CSR_TIMEH:
    {
        value=machine->mtime>>32;
    }
    break;
    case CSR_CYCLE:
    case CSR_MCYCLE:
    {
        value=machine->cycle;
    }
    break;
    case CSR_CYCLEH:
    case CSR_MCYCLEH:
    {
        value=machine->cycle>>32;
    }
    break;
    case CSR_INSTRET:
    case CSR_MINSTRET:
    {
        value=machine->instret;
    }
    break;
    case CSR_INSTRETH:
    case CSR_MINSTRETH:
    {
        value=machine->instret>>32;
    }
    break;
    HS_MACHINE_CSR_READ_CASE(CSR_MSTATUS,HS_MACHINE_CSR_W_MSTATUS,mstatus)
    HS_MACHINE_CSR_READ_CASE(CSR_MEDELEG,HS_MACHINE_CSR_W_MEDELEG,medeleg)
    HS_MACHINE_CSR_READ_CASE(CSR_MIDELEG,HS_MACHINE_CSR_W_MIDELEG,mideleg)
    HS_MACHINE_CSR_READ_CASE(CSR_MIE,HS_MACHINE_CSR_W_MIE,mie)
    case CSR_MTVEC:
    {
        value=HS_RISC_V_MACHINE_RV64_VECTOR_MODE;
    }
    break;
    HS_MACHINE_CSR_READ_CASE(CSR_MCOUNTEREN,HS_MACHINE_CSR_W_MCOUNTEREN,mcounteren)
    HS_MACHINE_CSR_READ_CASE(CSR_MSCRATCH,HS_MACHINE_CSR_W_MSCRATCH,mscratch)
    HS_MACHINE_CSR_READ_CASE(CSR_MEPC,HS_MACHINE_CSR_W_MEPC,mepc)
    HS_MACHINE_CSR_READ_CASE(CSR_MCAUSE,HS_MACHINE_CSR_W_MCAUSE,mcause)
    HS_MACHINE_CSR_READ_CASE(CSR_MTVAL,HS_MACHINE_CSR_W_MTVAL,mtval)
    HS_MACHINE_CSR_READ_CASE(CSR_MIP,HS_MACHINE_CSR_W_MIP,mip)
    HS_MACHINE_CSR_READ_CASE(CSR_MSTATUSH,HS_MACHINE_CSR_W_MSTATUSH,mstatush)
    HS_MACHINE_CSR_READ_CASE(CSR_SSTATUS,HS_MACHINE_CSR_W_SSTATUS,sstatus)
    HS_MACHINE_CSR_READ_CASE(CSR_SIE,HS_MACHINE_CSR_W_SIE,sie)
    HS_MACHINE_CSR_READ_CASE(CSR_STVEC,HS_MACHINE_CSR_W_STVEC,stvec)
    HS_MACHINE_CSR_READ_CASE(CSR_SCOUNTEREN,HS_MACHINE_CSR_W_SCOUNTEREN,scounteren)
    HS_MACHINE_CSR_READ_CASE(CSR_SSCRATCH,HS_MACHINE_CSR_W_SSCRATCH,sscratch)
    HS_MACHINE_CSR_READ_CASE(CSR_SEPC,HS_MACHINE_CSR_W_SEPC,sepc)
    HS_MACHINE_CSR_READ_CASE(CSR_SCAUSE,HS_MACHINE_CSR_W_SCAUSE,scause)
    HS_MACHINE_CSR_READ_CASE(CSR_STVAL,HS_MACHINE_CSR_W_STVAL,stval)
    HS_MACHINE_CSR_READ_CASE(CSR_SIP,HS_MACHINE_CSR_W_SIP,sip)
    HS_MACHINE_CSR_READ_CASE(CSR_SATP,HS_MACHINE_CSR_W_SATP,satp)
    case CSR_MHARTID:
    {
        value=machine->hart_id;
    }
    break;
    case HS_RISC_V_MACHINE_RV64_CSR_CONSOLE_OUT:
    {
        value=0;
    }
    break;
    case HS_RISC_V_MACHINE_RV64_CSR_CONSOLE_IN:
    {
        uint8_t data_byte=0;
        if(machine->console_get!=NULL &&
           machine->console_get(machine,machine->console_usr,&data_byte))
        {
            value=data_byte;
        }
        else
        {
            value=0;
        }
    }
    break;
    case HS_RISC_V_MACHINE_RV64_CSR_CONSOLE_STATUS:
    {
        value=2;
        if(machine->console_has_data!=NULL &&
           machine->console_has_data(machine,machine->console_usr))
        {
            value|=1;
        }
    }
    break;
    default:
    {
        return false;
    }
    }
    hs_risc_v_machine_rv64_write_u64_le(data,len,value);
    return true;
}

static bool hs_risc_v_machine_rv64_csr_write_internal(hs_risc_v_machine_rv64_t *machine,uint32_t csr,const uint8_t *data,size_t len)
{
    uint64_t value=hs_risc_v_machine_rv64_read_u64_le(data,len);
    switch(csr)
    {
    case CSR_FFLAGS:
    {
        machine->fcsr=(machine->fcsr&(~0x1FUL))|(value&0x1F);
    }
    break;
    case CSR_FRM:
    {
        machine->fcsr=(machine->fcsr&(~(0x7UL<<5)))|((value&0x7)<<5);
    }
    break;
    case CSR_FCSR:
    {
        machine->fcsr=value&0xFF;
    }
    break;
    case CSR_CYCLE:
    case CSR_MCYCLE:
    {
        machine->cycle=value;
    }
    break;
    case CSR_CYCLEH:
    case CSR_MCYCLEH:
    {
        machine->cycle=(machine->cycle&0xFFFFFFFFULL)|(((uint64_t)value)<<32);
    }
    break;
    case CSR_INSTRET:
    case CSR_MINSTRET:
    {
        machine->instret=value;
    }
    break;
    case CSR_INSTRETH:
    case CSR_MINSTRETH:
    {
        machine->instret=(machine->instret&0xFFFFFFFFULL)|(((uint64_t)value)<<32);
    }
    break;
    HS_MACHINE_CSR_WRITE_CASE(CSR_MSTATUS,HS_MACHINE_CSR_W_MSTATUS,mstatus)
    HS_MACHINE_CSR_WRITE_CASE(CSR_MEDELEG,HS_MACHINE_CSR_W_MEDELEG,medeleg)
    HS_MACHINE_CSR_WRITE_CASE(CSR_MIDELEG,HS_MACHINE_CSR_W_MIDELEG,mideleg)
    HS_MACHINE_CSR_WRITE_CASE(CSR_MIE,HS_MACHINE_CSR_W_MIE,mie)
    case CSR_MTVEC:
    {
        /*
         * 向量基地址固定为0，向量模式，不可修改
         */
        return true;
    }
    HS_MACHINE_CSR_WRITE_CASE(CSR_MCOUNTEREN,HS_MACHINE_CSR_W_MCOUNTEREN,mcounteren)
    HS_MACHINE_CSR_WRITE_CASE(CSR_MSCRATCH,HS_MACHINE_CSR_W_MSCRATCH,mscratch)
    HS_MACHINE_CSR_WRITE_CASE(CSR_MEPC,HS_MACHINE_CSR_W_MEPC,mepc)
    HS_MACHINE_CSR_WRITE_CASE(CSR_MCAUSE,HS_MACHINE_CSR_W_MCAUSE,mcause)
    HS_MACHINE_CSR_WRITE_CASE(CSR_MTVAL,HS_MACHINE_CSR_W_MTVAL,mtval)
    HS_MACHINE_CSR_WRITE_CASE(CSR_MIP,HS_MACHINE_CSR_W_MIP,mip)
    HS_MACHINE_CSR_WRITE_CASE(CSR_MSTATUSH,HS_MACHINE_CSR_W_MSTATUSH,mstatush)
    HS_MACHINE_CSR_WRITE_CASE(CSR_SSTATUS,HS_MACHINE_CSR_W_SSTATUS,sstatus)
    HS_MACHINE_CSR_WRITE_CASE(CSR_SIE,HS_MACHINE_CSR_W_SIE,sie)
    HS_MACHINE_CSR_WRITE_CASE(CSR_STVEC,HS_MACHINE_CSR_W_STVEC,stvec)
    HS_MACHINE_CSR_WRITE_CASE(CSR_SCOUNTEREN,HS_MACHINE_CSR_W_SCOUNTEREN,scounteren)
    HS_MACHINE_CSR_WRITE_CASE(CSR_SSCRATCH,HS_MACHINE_CSR_W_SSCRATCH,sscratch)
    HS_MACHINE_CSR_WRITE_CASE(CSR_SEPC,HS_MACHINE_CSR_W_SEPC,sepc)
    HS_MACHINE_CSR_WRITE_CASE(CSR_SCAUSE,HS_MACHINE_CSR_W_SCAUSE,scause)
    HS_MACHINE_CSR_WRITE_CASE(CSR_STVAL,HS_MACHINE_CSR_W_STVAL,stval)
    HS_MACHINE_CSR_WRITE_CASE(CSR_SIP,HS_MACHINE_CSR_W_SIP,sip)
    HS_MACHINE_CSR_WRITE_CASE(CSR_SATP,HS_MACHINE_CSR_W_SATP,satp)
    case HS_RISC_V_MACHINE_RV64_CSR_CONSOLE_OUT:
    {
        if(machine->console_put!=NULL)
        {
            machine->console_put(machine,machine->console_usr,(uint8_t)(value&0xFF));
        }
        return true;
    }
    case HS_RISC_V_MACHINE_RV64_CSR_CONSOLE_IN:
    case HS_RISC_V_MACHINE_RV64_CSR_CONSOLE_STATUS:
    {
        return true;
    }
    default:
    {
        return false;
    }
    }
    return true;
}

static bool hs_risc_v_machine_rv64_csr_read_value(hs_risc_v_machine_rv64_t *machine,uint32_t csr,uint64_t *value)
{
    uint8_t data[sizeof(uint64_t)]= {0};
    if(hs_risc_v_machine_rv64_csr_read_internal(machine,csr,data,sizeof(data)))
    {
        *value=hs_risc_v_machine_rv64_read_u64_le(data,sizeof(data));
        return true;
    }
    return false;
}

static bool hs_risc_v_machine_rv64_csr_write_value(hs_risc_v_machine_rv64_t *machine,uint32_t csr,uint64_t value)
{
    uint8_t data[sizeof(uint64_t)];
    hs_risc_v_machine_rv64_write_u64_le(data,sizeof(data),value);
    return hs_risc_v_machine_rv64_csr_write_internal(machine,csr,data,sizeof(data));
}

/*
 * 0xF0000000-0xF2FFFFFF内部寄存器读取
 */
static size_t hs_risc_v_machine_rv64_info_read(hs_risc_v_machine_rv64_t *machine,uint32_t address,void *data,size_t len)
{
    if(machine==NULL || data==NULL)
    {
        return 0;
    }
    uint8_t *out=(uint8_t *)data;
    size_t done=0;
    while(done<len)
    {
        uint32_t addr=address+(uint32_t)done;
        size_t remain=len-done;
        if(addr>=HS_RISC_V_MACHINE_RV64_X_BASE && addr<HS_RISC_V_MACHINE_RV64_X_END)
        {
            uint32_t index=(addr-HS_RISC_V_MACHINE_RV64_X_BASE)>>4;
            uint32_t offset=(addr-HS_RISC_V_MACHINE_RV64_X_BASE)&0xF;
            size_t chunk=16-offset;
            if(chunk>remain)
            {
                chunk=remain;
            }
            for(size_t i=0; i<chunk; i++)
            {
                uint32_t byte_offset=offset+(uint32_t)i;
                out[done+i]=(byte_offset<sizeof(machine->x[0]))?(uint8_t)((machine->x[index&0x1F]>>(8*byte_offset))&0xFF):0;
            }
            done+=chunk;
        }
        else if(addr>=HS_RISC_V_MACHINE_RV64_PC_BASE && addr<HS_RISC_V_MACHINE_RV64_PC_END)
        {
            uint32_t offset=addr-HS_RISC_V_MACHINE_RV64_PC_BASE;
            size_t chunk=16-offset;
            if(chunk>remain)
            {
                chunk=remain;
            }
            for(size_t i=0; i<chunk; i++)
            {
                uint32_t byte_offset=offset+(uint32_t)i;
                out[done+i]=(byte_offset<sizeof(machine->pc))?(uint8_t)((machine->pc>>(8*byte_offset))&0xFF):0;
            }
            done+=chunk;
        }
        else if(addr>=HS_RISC_V_MACHINE_RV64_XLEN_BASE && addr<HS_RISC_V_MACHINE_RV64_XLEN_END)
        {
            uint32_t offset=addr-HS_RISC_V_MACHINE_RV64_XLEN_BASE;
            size_t chunk=16-offset;
            if(chunk>remain)
            {
                chunk=remain;
            }
            for(size_t i=0; i<chunk; i++)
            {
                uint32_t byte_offset=offset+(uint32_t)i;
                out[done+i]=(byte_offset<sizeof(machine->xlen))?(uint8_t)((machine->xlen>>(8*byte_offset))&0xFF):0;
            }
            done+=chunk;
        }
        else if(addr>=HS_RISC_V_MACHINE_RV64_F_BASE && addr<HS_RISC_V_MACHINE_RV64_F_END)
        {
            uint32_t index=(addr-HS_RISC_V_MACHINE_RV64_F_BASE)>>4;
            uint32_t offset=(addr-HS_RISC_V_MACHINE_RV64_F_BASE)&0xF;
            size_t chunk=16-offset;
            if(chunk>remain)
            {
                chunk=remain;
            }
            uint64_t value=machine->f[index&0x1F];
            for(size_t i=0; i<chunk; i++)
            {
                uint32_t byte_offset=offset+(uint32_t)i;
                out[done+i]=(byte_offset<sizeof(value))?(uint8_t)((value>>(8*byte_offset))&0xFF):0;
            }
            done+=chunk;
        }
        else if(addr>=HS_RISC_V_MACHINE_RV64_FCSR_BASE && addr<HS_RISC_V_MACHINE_RV64_FCSR_END)
        {
            uint32_t offset=addr-HS_RISC_V_MACHINE_RV64_FCSR_BASE;
            size_t chunk=16-offset;
            if(chunk>remain)
            {
                chunk=remain;
            }
            for(size_t i=0; i<chunk; i++)
            {
                uint32_t byte_offset=offset+(uint32_t)i;
                out[done+i]=(byte_offset<sizeof(machine->fcsr))?(uint8_t)((machine->fcsr>>(8*byte_offset))&0xFF):0;
            }
            done+=chunk;
        }
        else if(addr>=HS_RISC_V_MACHINE_RV64_FLEN_BASE && addr<HS_RISC_V_MACHINE_RV64_FLEN_END)
        {
            uint32_t offset=addr-HS_RISC_V_MACHINE_RV64_FLEN_BASE;
            size_t chunk=16-offset;
            if(chunk>remain)
            {
                chunk=remain;
            }
            for(size_t i=0; i<chunk; i++)
            {
                uint32_t byte_offset=offset+(uint32_t)i;
                out[done+i]=(byte_offset<sizeof(machine->flen))?(uint8_t)((machine->flen>>(8*byte_offset))&0xFF):0;
            }
            done+=chunk;
        }
        else if(addr>=HS_RISC_V_MACHINE_RV64_MTIME_BASE && addr<HS_RISC_V_MACHINE_RV64_MTIME_END)
        {
            uint32_t offset=addr-HS_RISC_V_MACHINE_RV64_MTIME_BASE;
            size_t chunk=16-offset;
            if(chunk>remain)
            {
                chunk=remain;
            }
            for(size_t i=0; i<chunk; i++)
            {
                uint32_t byte_offset=offset+(uint32_t)i;
                out[done+i]=(byte_offset<sizeof(machine->mtime))?(uint8_t)((machine->mtime>>(8*byte_offset))&0xFF):0;
            }
            done+=chunk;
        }
        else if(addr>=HS_RISC_V_MACHINE_RV64_MTIMECMP_BASE && addr<HS_RISC_V_MACHINE_RV64_MTIMECMP_END)
        {
            uint32_t offset=addr-HS_RISC_V_MACHINE_RV64_MTIMECMP_BASE;
            size_t chunk=16-offset;
            if(chunk>remain)
            {
                chunk=remain;
            }
            for(size_t i=0; i<chunk; i++)
            {
                uint32_t byte_offset=offset+(uint32_t)i;
                out[done+i]=(byte_offset<sizeof(machine->mtimecmp))?(uint8_t)((machine->mtimecmp>>(8*byte_offset))&0xFF):0;
            }
            done+=chunk;
        }
        else if(addr>=HS_RISC_V_MACHINE_RV64_CSR_BASE && addr<(HS_RISC_V_MACHINE_RV64_CSR_BASE+0x00010000UL))
        {
            uint32_t csr=(addr-HS_RISC_V_MACHINE_RV64_CSR_BASE)>>4;
            uint32_t offset=(addr-HS_RISC_V_MACHINE_RV64_CSR_BASE)&0xF;
            size_t chunk=16-offset;
            if(chunk>remain)
            {
                chunk=remain;
            }
            uint64_t value=0;
            bool handled=hs_risc_v_machine_rv64_csr_read_value(machine,csr&0xFFF,&value);
            if(!handled && machine->csr_read!=NULL)
            {
                value=0;
                handled=machine->csr_read(machine,machine->csr_usr,csr&0xFFF,&value);
            }
            if(!handled)
            {
                value=0;
            }
            for(size_t i=0; i<chunk; i++)
            {
                uint32_t byte_offset=offset+(uint32_t)i;
                out[done+i]=(byte_offset<sizeof(value))?(uint8_t)((value>>(8*byte_offset))&0xFF):0;
            }
            done+=chunk;
        }
        else if(addr>=HS_RISC_V_MACHINE_RV64_DTB_BASE && addr<(HS_RISC_V_MACHINE_RV64_DTB_BASE+0x01000000UL))
        {
            if(machine->device_tree_read==NULL ||
               machine->device_tree_read(machine,machine->device_tree_usr,addr-HS_RISC_V_MACHINE_RV64_DTB_BASE,out+done,remain)!=remain)
            {
                memset(out+done,0,remain);
            }
            done+=remain;
        }
        else
        {
            memset(out+done,0,remain);
            done+=remain;
        }
    }
    return len;
}

/*
 * 0xF0000000-0xF2FFFFFF内部寄存器写入
 */
static size_t hs_risc_v_machine_rv64_info_write(hs_risc_v_machine_rv64_t *machine,uint32_t address,const void *data,size_t len)
{
    if(machine==NULL || data==NULL)
    {
        return 0;
    }
    const uint8_t *in=(const uint8_t *)data;
    size_t done=0;
    while(done<len)
    {
        uint32_t addr=address+(uint32_t)done;
        size_t remain=len-done;
        if(addr>=HS_RISC_V_MACHINE_RV64_X_BASE && addr<HS_RISC_V_MACHINE_RV64_X_END)
        {
            uint32_t index=(addr-HS_RISC_V_MACHINE_RV64_X_BASE)>>4;
            uint32_t offset=(addr-HS_RISC_V_MACHINE_RV64_X_BASE)&0xF;
            size_t chunk=16-offset;
            if(chunk>remain)
            {
                chunk=remain;
            }
            if(index!=0)
            {
                for(size_t i=0; i<chunk; i++)
                {
                    uint32_t byte_offset=offset+(uint32_t)i;
                    if(byte_offset<sizeof(machine->x[0]))
                    {
                        uint64_t mask=0xFFULL<<(8*byte_offset);
                        machine->x[index&0x1F]&=~mask;
                        machine->x[index&0x1F]|=((uint64_t)in[done+i])<<(8*byte_offset);
                    }
                }
            }
            done+=chunk;
        }
        else if(addr>=HS_RISC_V_MACHINE_RV64_PC_BASE && addr<HS_RISC_V_MACHINE_RV64_PC_END)
        {
            uint32_t offset=addr-HS_RISC_V_MACHINE_RV64_PC_BASE;
            size_t chunk=16-offset;
            if(chunk>remain)
            {
                chunk=remain;
            }
            for(size_t i=0; i<chunk; i++)
            {
                uint32_t byte_offset=offset+(uint32_t)i;
                if(byte_offset<sizeof(machine->pc))
                {
                    uint64_t mask=0xFFULL<<(8*byte_offset);
                    machine->pc&=~mask;
                    machine->pc|=((uint64_t)in[done+i])<<(8*byte_offset);
                }
            }
            done+=chunk;
        }
        else if(addr>=HS_RISC_V_MACHINE_RV64_XLEN_BASE && addr<HS_RISC_V_MACHINE_RV64_XLEN_END)
        {
            done+=remain;
        }
        else if(addr>=HS_RISC_V_MACHINE_RV64_F_BASE && addr<HS_RISC_V_MACHINE_RV64_F_END)
        {
            uint32_t index=(addr-HS_RISC_V_MACHINE_RV64_F_BASE)>>4;
            uint32_t offset=(addr-HS_RISC_V_MACHINE_RV64_F_BASE)&0xF;
            size_t chunk=16-offset;
            if(chunk>remain)
            {
                chunk=remain;
            }
            for(size_t i=0; i<chunk; i++)
            {
                uint32_t byte_offset=offset+(uint32_t)i;
                if(byte_offset<sizeof(machine->f[0]))
                {
                    uint64_t mask=0xFFULL<<(8*byte_offset);
                    machine->f[index&0x1F]&=~mask;
                    machine->f[index&0x1F]|=((uint64_t)in[done+i])<<(8*byte_offset);
                }
            }
            done+=chunk;
        }
        else if(addr>=HS_RISC_V_MACHINE_RV64_FCSR_BASE && addr<HS_RISC_V_MACHINE_RV64_FCSR_END)
        {
            uint32_t offset=addr-HS_RISC_V_MACHINE_RV64_FCSR_BASE;
            size_t chunk=16-offset;
            if(chunk>remain)
            {
                chunk=remain;
            }
            for(size_t i=0; i<chunk; i++)
            {
                uint32_t byte_offset=offset+(uint32_t)i;
                if(byte_offset<sizeof(machine->fcsr))
                {
                    uint64_t mask=0xFFULL<<(8*byte_offset);
                    machine->fcsr&=~mask;
                    machine->fcsr|=((uint32_t)in[done+i])<<(8*byte_offset);
                }
            }
            done+=chunk;
        }
        else if(addr>=HS_RISC_V_MACHINE_RV64_FLEN_BASE && addr<HS_RISC_V_MACHINE_RV64_FLEN_END)
        {
            done+=remain;
        }
        else if(addr>=HS_RISC_V_MACHINE_RV64_MTIME_BASE && addr<HS_RISC_V_MACHINE_RV64_MTIME_END)
        {
            done+=remain;
        }
        else if(addr>=HS_RISC_V_MACHINE_RV64_MTIMECMP_BASE && addr<HS_RISC_V_MACHINE_RV64_MTIMECMP_END)
        {
            uint32_t offset=addr-HS_RISC_V_MACHINE_RV64_MTIMECMP_BASE;
            size_t chunk=16-offset;
            if(chunk>remain)
            {
                chunk=remain;
            }
            for(size_t i=0; i<chunk; i++)
            {
                uint32_t byte_offset=offset+(uint32_t)i;
                if(byte_offset<sizeof(machine->mtimecmp))
                {
                    uint64_t mask=0xFFULL<<(8*byte_offset);
                    machine->mtimecmp&=~mask;
                    machine->mtimecmp|=((uint64_t)in[done+i])<<(8*byte_offset);
                }
            }
            done+=chunk;
        }
        else if(addr>=HS_RISC_V_MACHINE_RV64_CSR_BASE && addr<(HS_RISC_V_MACHINE_RV64_CSR_BASE+0x00010000UL))
        {
            uint32_t csr=(addr-HS_RISC_V_MACHINE_RV64_CSR_BASE)>>4;
            uint32_t offset=(addr-HS_RISC_V_MACHINE_RV64_CSR_BASE)&0xF;
            size_t chunk=16-offset;
            if(chunk>remain)
            {
                chunk=remain;
            }
            uint64_t value=0;
            (void)hs_risc_v_machine_rv64_csr_read_value(machine,csr&0xFFF,&value);
            for(size_t i=0; i<chunk; i++)
            {
                uint32_t byte_offset=offset+(uint32_t)i;
                if(byte_offset<sizeof(value))
                {
                    value&=(~(0xFFULL<<(8*byte_offset)));
                    value|=((uint64_t)in[done+i])<<(8*byte_offset);
                }
            }
            if(!hs_risc_v_machine_rv64_csr_write_value(machine,csr&0xFFF,value))
            {
                if(machine->csr_write!=NULL)
                {
                    machine->csr_write(machine,machine->csr_usr,csr&0xFFF,&value);
                }
            }
            done+=chunk;
        }
        else if(addr>=HS_RISC_V_MACHINE_RV64_DTB_BASE && addr<(HS_RISC_V_MACHINE_RV64_DTB_BASE+0x01000000UL))
        {
            done+=remain;
        }
        else
        {
            done+=remain;
        }
    }
    return len;
}

static size_t hs_risc_v_machine_rv64_memory_read_internal(hs_risc_v_machine_rv64_t *machine,uint32_t address,void *data,size_t len)
{
    if(machine==NULL)
    {
        return 0;
    }
    switch(address>>28)
    {
    case 0x0:
    {
        if(machine->flash.read!=NULL)
        {
            return hs_risc_v_machine_rv64_region_read(&machine->flash,address,data,len);
        }
        return hs_risc_v_machine_rv64_region_read(&machine->rom,address,data,len);
    }
    case 0x1:
    {
        return hs_risc_v_machine_rv64_region_read(&machine->flash,address,data,len);
    }
    case 0x2:
    {
        return hs_risc_v_machine_rv64_region_read(&machine->ram,address,data,len);
    }
    case 0xD:
    {
        return hs_risc_v_machine_rv64_region_read(&machine->rom,address,data,len);
    }
    case 0xE:
    {
        if(machine->io_read!=NULL)
        {
            return machine->io_read(machine,machine->io_usr,address,data,len);
        }
        return 0;
    }
    case 0xF:
    {
        return hs_risc_v_machine_rv64_info_read(machine,address,data,len);
    }
    default:
    {
    }
    break;
    }
    return 0;
}

static size_t hs_risc_v_machine_rv64_memory_write_internal(hs_risc_v_machine_rv64_t *machine,uint32_t address,const void *data,size_t len)
{
    if(machine==NULL)
    {
        return 0;
    }
    switch(address>>28)
    {
    case 0x0:
    {
        if(machine->flash.write!=NULL)
        {
            return hs_risc_v_machine_rv64_region_write(&machine->flash,address,data,len);
        }
        return hs_risc_v_machine_rv64_region_write(&machine->rom,address,data,len);
    }
    case 0x1:
    {
        return hs_risc_v_machine_rv64_region_write(&machine->flash,address,data,len);
    }
    case 0x2:
    {
        return hs_risc_v_machine_rv64_region_write(&machine->ram,address,data,len);
    }
    case 0xD:
    {
        return hs_risc_v_machine_rv64_region_write(&machine->rom,address,data,len);
    }
    case 0xE:
    {
        if(machine->io_write!=NULL)
        {
            return machine->io_write(machine,machine->io_usr,address,data,len);
        }
        return 0;
    }
    case 0xF:
    {
        return hs_risc_v_machine_rv64_info_write(machine,address,data,len);
    }
    default:
    {
    }
    break;
    }
    return 0;
}

static bool hs_risc_v_machine_rv64_io(hs_risc_v_core_rv64_t *core,uint32_t opt,uint64_t address,uint8_t *data,size_t len,void *usr)
{
    hs_risc_v_machine_rv64_t *machine=(hs_risc_v_machine_rv64_t *)usr;
    if(machine==NULL || data==NULL)
    {
        return false;
    }
    switch(opt&0x00FFFFFFUL)
    {
    case HS_RISC_V_CORE_RV64_IO_RESET:
    {
        memset(machine->x,0,sizeof(machine->x));
        memset(machine->f,0,sizeof(machine->f));
        machine->pc=0;
        machine->fcsr=0;
        machine->flags.reservation_valid=false;
        machine->reservation_address=0;
        machine->mstatus=0;
        machine->medeleg=0;
        machine->mideleg=0;
        machine->mie=0;
        machine->mtvec=HS_RISC_V_MACHINE_RV64_VECTOR_MODE;
        machine->mcounteren=0;
        machine->mscratch=0;
        machine->mepc=0;
        machine->mcause=0;
        machine->mtval=0;
        machine->mip=0;
        machine->mstatush=0;
        machine->sstatus=0;
        machine->sie=0;
        machine->stvec=0;
        machine->scounteren=0;
        machine->sscratch=0;
        machine->sepc=0;
        machine->scause=0;
        machine->stval=0;
        machine->sip=0;
        machine->satp=0;
        machine->csr_written=0;
    }
    break;
    case HS_RISC_V_CORE_RV64_IO_ENTRY_PC_READ:
    {
        uint32_t entry=(machine->entry!=0)?machine->entry:HS_RISC_V_MACHINE_RV64_RESET_VECTOR;
        hs_risc_v_machine_rv64_write_u64_le(data,len,(uint64_t)entry);
    }
    break;
    case HS_RISC_V_CORE_RV64_IO_INITIAL_SP_READ:
    {
        uint32_t sp=machine->ram.base+(uint32_t)machine->ram.size;
        hs_risc_v_machine_rv64_write_u64_le(data,len,(uint64_t)sp);
    }
    break;
    case HS_RISC_V_CORE_RV64_IO_MEMORY_READ:
    case HS_RISC_V_CORE_RV64_IO_MEMORY_AMO_READ:
    {
        return hs_risc_v_machine_rv64_memory_read_internal(machine,(uint32_t)address,data,len)==len;
    }
    case HS_RISC_V_CORE_RV64_IO_MEMORY_WRITE:
    case HS_RISC_V_CORE_RV64_IO_MEMORY_AMO_WRITE:
    {
        return hs_risc_v_machine_rv64_memory_write_internal(machine,(uint32_t)address,data,len)==len;
    }
    case HS_RISC_V_CORE_RV64_IO_MEMORY_AMO_LR:
    {
        if(hs_risc_v_machine_rv64_memory_read_internal(machine,(uint32_t)address,data,len)==len)
        {
            machine->flags.reservation_valid=true;
            machine->reservation_address=(uint32_t)address;
            return true;
        }
        return false;
    }
    case HS_RISC_V_CORE_RV64_IO_MEMORY_AMO_SC:
    {
        bool have_reservation=machine->flags.reservation_valid && (machine->reservation_address==(uint32_t)address);
        machine->flags.reservation_valid=false;
        if(have_reservation)
        {
            return hs_risc_v_machine_rv64_memory_write_internal(machine,(uint32_t)address,data,len)==len;
        }
        return false;
    }
    case HS_RISC_V_CORE_RV64_IO_CSR_READ:
    {
        uint64_t value=0;
        if(hs_risc_v_machine_rv64_csr_read_value(machine,(uint32_t)address,&value))
        {
            hs_risc_v_machine_rv64_write_u64_le(data,len,value);
            return true;
        }
        if(machine->csr_read!=NULL)
        {
            value=0;
            bool ret=machine->csr_read(machine,machine->csr_usr,(uint32_t)address,&value);
            if(ret)
            {
                hs_risc_v_machine_rv64_write_u64_le(data,len,value);
            }
            return ret;
        }
        return false;
    }
    case HS_RISC_V_CORE_RV64_IO_CSR_WRITE:
    {
        uint64_t value=hs_risc_v_machine_rv64_read_u64_le(data,len);
        if(hs_risc_v_machine_rv64_csr_write_value(machine,(uint32_t)address,value))
        {
            return true;
        }
        if(machine->csr_write!=NULL)
        {
            return machine->csr_write(machine,machine->csr_usr,(uint32_t)address,&value);
        }
        return false;
    }
    case HS_RISC_V_CORE_RV64_IO_X_REGISTER_READ:
    {
        hs_risc_v_machine_rv64_write_u64_le(data,len,(address<32)?machine->x[(size_t)address]:0);
    }
    break;
    case HS_RISC_V_CORE_RV64_IO_X_REGISTER_WRITE:
    {
        if(address<32)
        {
            machine->x[(size_t)address]=hs_risc_v_machine_rv64_read_u64_le(data,len);
        }
    }
    break;
    case HS_RISC_V_CORE_RV64_IO_PC_REGISTER_READ:
    {
        hs_risc_v_machine_rv64_write_u64_le(data,len,machine->pc);
    }
    break;
    case HS_RISC_V_CORE_RV64_IO_PC_REGISTER_WRITE:
    {
        machine->pc=hs_risc_v_machine_rv64_read_u64_le(data,len);
    }
    break;
    case HS_RISC_V_CORE_RV64_IO_F_REGISTER_READ:
    {
        hs_risc_v_machine_rv64_write_u64_le(data,len,(address<32)?machine->f[(size_t)address]:0);
    }
    break;
    case HS_RISC_V_CORE_RV64_IO_F_REGISTER_WRITE:
    {
        if(address<32)
        {
            machine->f[(size_t)address]=hs_risc_v_machine_rv64_read_u64_le(data,len);
        }
    }
    break;
    case HS_RISC_V_CORE_RV64_IO_INSTRUCTION_ENTER:
    {
        uint32_t instruction=hs_risc_v_machine_rv64_read_u32_le(data,len);
        if(machine->instruction_enter!=NULL)
        {
            machine->instruction_enter(machine,machine->instruction_usr,(uint32_t)address,instruction);
        }
    }
    break;
    case HS_RISC_V_CORE_RV64_IO_INSTRUCTION_EXIT:
    {
        uint32_t instruction=hs_risc_v_machine_rv64_read_u32_le(data,len);
        if(machine->instruction_exit!=NULL)
        {
            machine->instruction_exit(machine,machine->instruction_usr,(uint32_t)address,instruction);
        }
    }
    break;
    default:
    {
    }
    break;
    }
    (void)core;
    return true;
}

static void hs_risc_v_machine_rv64_reset_internal(hs_risc_v_machine_rv64_t *machine)
{
    machine->mtime=0;
    machine->cycle=0;
    machine->instret=0;
    hs_risc_v_core_rv64_reset(hs_risc_v_machine_rv64_core_get(machine));
    machine->x[10]=machine->hart_id;
    machine->x[11]=machine->dtb_address;
}

size_t hs_risc_v_machine_rv64_size(void)
{
    return sizeof(hs_risc_v_machine_rv64_t);
}

hs_risc_v_machine_rv64_t *hs_risc_v_machine_rv64_init(void *mem)
{
    if(mem==NULL)
    {
        return NULL;
    }
    hs_risc_v_machine_rv64_t *machine=(hs_risc_v_machine_rv64_t *)mem;
    memset(machine,0,sizeof(hs_risc_v_machine_rv64_t));
    machine->xlen=64;
    machine->flen=64;
    machine->hart_id=0;
    machine->dtb_address=HS_RISC_V_MACHINE_RV64_DTB_BASE;
    machine->mtimecmp=UINT64_MAX;
    machine->flags.started=false;
    if(hs_risc_v_core_rv64_init(machine->core.bytes,hs_risc_v_machine_rv64_io,machine)==NULL)
    {
        return NULL;
    }
    return machine;
}

bool hs_risc_v_machine_rv64_check(hs_risc_v_machine_rv64_t *machine)
{
    if(machine==NULL)
    {
        return false;
    }
    /*
     * RAM读/写回调必须设置
     */
    if(machine->ram.read==NULL || machine->ram.write==NULL)
    {
        return false;
    }
    if(machine->ram.size==0)
    {
        return false;
    }
    return true;
}

int hs_risc_v_machine_rv64_step(hs_risc_v_machine_rv64_t *machine,uint32_t elapsed_us,size_t count)
{
    if(machine==NULL)
    {
        return -1;
    }
    if(!hs_risc_v_machine_rv64_check(machine))
    {
        return -1;
    }
    if(!machine->flags.started)
    {
        hs_risc_v_machine_rv64_reset_internal(machine);
        machine->flags.started=true;
    }
    machine->mtime+=elapsed_us;
    {
        uint64_t *clint_mtime=hs_risc_v_core_rv64_clint_mtime(hs_risc_v_machine_rv64_core_get(machine));
        uint64_t *clint_mtimecmp=hs_risc_v_core_rv64_clint_mtimecmp(hs_risc_v_machine_rv64_core_get(machine));
        if(clint_mtime!=NULL)
        {
            *clint_mtime=machine->mtime;
        }
        if(clint_mtimecmp!=NULL)
        {
            *clint_mtimecmp=machine->mtimecmp;
        }
    }
    if(hs_risc_v_core_rv64_wfi(hs_risc_v_machine_rv64_core_get(machine)))
    {
        if(machine->mtimecmp!=0 && machine->mtime>=machine->mtimecmp)
        {
            /*
             * 定时器到期，清除WFI以便处理中断
             */
            hs_risc_v_core_rv64_wfi_clear(hs_risc_v_machine_rv64_core_get(machine));
        }
        else
        {
            return 1;
        }
    }
    hs_risc_v_core_rv64_tick(hs_risc_v_machine_rv64_core_get(machine),count);
    if(hs_risc_v_core_rv64_wfi(hs_risc_v_machine_rv64_core_get(machine)))
    {
        return 1;
    }
    return 0;
}

hs_risc_v_core_rv64_t *hs_risc_v_machine_rv64_core(hs_risc_v_machine_rv64_t *machine)
{
    return hs_risc_v_machine_rv64_core_get(machine);
}

/*
 * 清理内部宏定义，便于与其它机器模型在同一编译单元中包含
 */
#undef HS_MACHINE_CSR_READ_CASE
#undef HS_MACHINE_CSR_WRITE_CASE
#undef HS_MACHINE_CSR_W_MSTATUS
#undef HS_MACHINE_CSR_W_MEDELEG
#undef HS_MACHINE_CSR_W_MIDELEG
#undef HS_MACHINE_CSR_W_MIE
#undef HS_MACHINE_CSR_W_MTVEC
#undef HS_MACHINE_CSR_W_MCOUNTEREN
#undef HS_MACHINE_CSR_W_MSCRATCH
#undef HS_MACHINE_CSR_W_MEPC
#undef HS_MACHINE_CSR_W_MCAUSE
#undef HS_MACHINE_CSR_W_MTVAL
#undef HS_MACHINE_CSR_W_MIP
#undef HS_MACHINE_CSR_W_MSTATUSH
#undef HS_MACHINE_CSR_W_SSTATUS
#undef HS_MACHINE_CSR_W_SIE
#undef HS_MACHINE_CSR_W_STVEC
#undef HS_MACHINE_CSR_W_SCOUNTEREN
#undef HS_MACHINE_CSR_W_SSCRATCH
#undef HS_MACHINE_CSR_W_SEPC
#undef HS_MACHINE_CSR_W_SCAUSE
#undef HS_MACHINE_CSR_W_STVAL
#undef HS_MACHINE_CSR_W_SIP
#undef HS_MACHINE_CSR_W_SATP
