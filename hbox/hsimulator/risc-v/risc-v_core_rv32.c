/***************************************************************
 * Name:      risc-v_core_rv32.c
 * Purpose:   实现risc-v_core_rv32接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-02-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "risc-v_core_rv32.h"
#ifndef  RISCV_ENCODING_H
#include "encoding.out.h"
#endif
#include "risc-v_crypto.h"
#include "h3rdparty.h"
#include "hdefaults.h"

struct hs_risc_v_core_rv32
{
    hs_risc_v_core_rv32_io_t    io;
    void*                       usr;
    uint32_t                    instruction_sets;       /**< 支持的扩展指令集 */
    uint32_t                    exception_pending;      /**< 等待执行的异常（狭义的异常） */
    uint32_t                    interrupt_pending;      /**< 等待执行的中断 */
    uint32_t                    exception_value;        /**< 异常附带的值（如非法指令、出错地址） */
    struct
    {
        uint64_t                mtime;                  /**< 全局计数寄存器 */
        uint64_t                mtimecmp;               /**< 全局比较寄存器 */
    }                           clint;
    struct
    {
        uint32_t                wfi_enable:1;           /**< 当前已启用WFI */
        uint32_t                privilege:2;            /**< 当前特权等级(0=U,1=S,3=M) */
        uint32_t                skip_counter_inc:1;     /**< 跳过本次计数器自增（写计数器时使用） */
        uint32_t                abort:1;                /**< 当前指令因异常终止，抑制后续寄存器/内存写 */
    }                           flags;
};

#define HS_RISC_V_CORE_RV32_PRIVILEGE_U 0
#define HS_RISC_V_CORE_RV32_PRIVILEGE_S 1
#define HS_RISC_V_CORE_RV32_PRIVILEGE_M 3

size_t hs_risc_v_core_rv32_size(void)
{
    return sizeof(hs_risc_v_core_rv32_t);
}

static inline uint32_t hs_risc_v_core_rv32_privilege_get(hs_risc_v_core_rv32_t *core)
{
    return (core!=NULL)?core->flags.privilege:HS_RISC_V_CORE_RV32_PRIVILEGE_M;
}

static inline void hs_risc_v_core_rv32_privilege_set(hs_risc_v_core_rv32_t *core,uint32_t privilege)
{
    if(core!=NULL)
    {
        core->flags.privilege=privilege&0x03;
    }
}


hs_risc_v_core_rv32_t *hs_risc_v_core_rv32_init(void *mem,hs_risc_v_core_rv32_io_t io,void *usr)
{
    if(mem==NULL)
    {
        return NULL;
    }
    hs_risc_v_core_rv32_t *ret=(hs_risc_v_core_rv32_t *)mem;
    ret->io=io;
    ret->usr=usr;
    hs_risc_v_core_rv32_reset(ret);
    return ret;
}

static inline  uint32_t hs_risc_v_core_rv32_pc_read(hs_risc_v_core_rv32_t *core)
{
    if(core!=NULL && core->io!=NULL)
    {
        hs_risc_v_common_memory_word_t pc;
        pc.value=0;
        core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_PC_REGISTER_READ | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_CORE,0,pc.bytes,sizeof(pc.bytes),core->usr);
        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(pc);
        return pc.value;
    }
    return 0;
}

static inline  void hs_risc_v_core_rv32_pc_write(hs_risc_v_core_rv32_t *core,uint32_t pc_value)
{
    if(core!=NULL && core->io!=NULL)
    {
        hs_risc_v_common_memory_word_t pc;
        pc.value=pc_value;
        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(pc);
        core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_PC_REGISTER_WRITE | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_CORE,0,pc.bytes,sizeof(pc.bytes),core->usr);
    }
}


static inline  uint32_t hs_risc_v_core_rv32_x_register_read(hs_risc_v_core_rv32_t *core,size_t address)
{
    if(address==0)
    {
        return 0;
    }
    if(core!=NULL && core->io!=NULL)
    {
        hs_risc_v_common_memory_word_t value;
        value.value=0;
        core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_X_REGISTER_READ | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_CORE,address,value.bytes,sizeof(value.bytes),core->usr);
        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
        return value.value;
    }
    return 0;
}

static inline  void hs_risc_v_core_rv32_x_register_write(hs_risc_v_core_rv32_t *core,size_t address,uint32_t reg_value)
{
    if(address==0)
    {
        return;
    }
    if(core!=NULL && core->flags.abort!=0)
    {
        /*
         * 当前指令已因页错误等异常终止，不再写回寄存器
         */
        return;
    }
    if(core!=NULL && core->io!=NULL)
    {
        hs_risc_v_common_memory_word_t value;
        value.value=reg_value;
        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
        core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_X_REGISTER_WRITE | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_CORE,address,value.bytes,sizeof(value.bytes),core->usr);
    }
}

static inline  uint64_t hs_risc_v_core_rv32_f_register_read64(hs_risc_v_core_rv32_t *core,size_t address)
{
    if(core!=NULL && core->io!=NULL)
    {
        hs_risc_v_common_memory_doubleword_t value;
        value.value=0;
        core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_F_REGISTER_READ | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_CORE,address,value.bytes,sizeof(value.bytes),core->usr);
        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
        return value.value;
    }
    return 0;
}

static inline  void hs_risc_v_core_rv32_f_register_write64(hs_risc_v_core_rv32_t *core,size_t address,uint64_t reg_value)
{
    if(core!=NULL && core->flags.abort!=0)
    {
        return;
    }
    if(core!=NULL && core->io!=NULL)
    {
        hs_risc_v_common_memory_doubleword_t value;
        value.value=reg_value;
        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
        core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_F_REGISTER_WRITE | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_CORE,address,value.bytes,sizeof(value.bytes),core->usr);
    }
}

static inline bool hs_risc_v_core_rv32_has_d(hs_risc_v_core_rv32_t *core)
{
    return (core!=NULL) && hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32D);
}

/*
 * 单精度浮点寄存器在存在"D"扩展时(FLEN=64)需要按照RISC-V规范进行NaN-boxing。
 */
static inline  uint32_t hs_risc_v_core_rv32_f_register_read32(hs_risc_v_core_rv32_t *core,size_t address)
{
    if(core!=NULL && core->io!=NULL)
    {
        if(hs_risc_v_core_rv32_has_d(core))
        {
            uint64_t value=hs_risc_v_core_rv32_f_register_read64(core,address);
            if((value >> 32) != 0xFFFFFFFFULL)
            {
                /*
                 * 未正确NaN-boxing，返回规范NaN
                 */
                return 0x7FC00000;
            }
            return (uint32_t)value;
        }
        else
        {
            hs_risc_v_common_memory_word_t value;
            value.value=0;
            core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_F_REGISTER_READ | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_CORE,address,value.bytes,sizeof(value.bytes),core->usr);
            HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
            return value.value;
        }
    }
    return 0;
}

static inline  void hs_risc_v_core_rv32_f_register_write32(hs_risc_v_core_rv32_t *core,size_t address,uint32_t reg_value)
{
    if(core!=NULL && core->flags.abort!=0)
    {
        return;
    }
    if(core!=NULL && core->io!=NULL)
    {
        if(hs_risc_v_core_rv32_has_d(core))
        {
            hs_risc_v_core_rv32_f_register_write64(core,address,0xFFFFFFFF00000000ULL | (uint64_t)reg_value);
        }
        else
        {
            hs_risc_v_common_memory_word_t value;
            value.value=reg_value;
            HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
            core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_F_REGISTER_WRITE | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_CORE,address,value.bytes,sizeof(value.bytes),core->usr);
        }
    }
}

static inline  uint32_t hs_risc_v_core_rv32_csr_read_default_value(hs_risc_v_core_rv32_t *core,size_t address)
{
    uint32_t reg_value=0;
    switch(address)
    {
    case CSR_MISA:
    {
        reg_value |= (1ULL<<(30));         //XLEN=32
        reg_value |= (1ULL<<(8));          //RV32I
        reg_value |= (1ULL<<(18));         //S
        reg_value |= (1ULL<<(20));         //U
        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32A))
        {
            reg_value |= (1ULL<<(0));          //RV32A
        }
        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32C))
        {
            reg_value |= (1ULL<<(2));          //RV32C
        }
        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32D))
        {
            reg_value |= (1ULL<<(3));          //RV32D
        }
        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32F))
        {
            reg_value |= (1ULL<<(5));          //RV32F
        }
        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32M))
        {
            reg_value |= (1ULL<<(12));          //RV32M
        }
    }
    break;
    case CSR_MVENDORID:
    {
        reg_value=0;                        //0=未实现
    }
    break;
    case CSR_MARCHID:
    {
        reg_value=0;                        //0=未实现
    }
    break;
    case CSR_MIMPID:
    {
        reg_value=0;                        //0=未实现
    }
    break;
    case CSR_MHARTID:
    {
        reg_value=0;                        //0=hart0,必须存在hartid为0的内核
    }
    break;
    case CSR_MSTATUS:
    {
        /*
         * 默认特权模式为3
         */
        reg_value=MSTATUS_MPP;
    }
    break;
    case CSR_MSTATUSH:
    {

    }
    break;
    case CSR_MTVEC:
    {
        reg_value=0x00000001;           //基地址为0,向量模式
    }
    break;
    case CSR_MEDELEG:
    {

    }
    break;
    case CSR_MIDELEG:
    {

    }
    break;
    case CSR_MIP:
    {

    }
    break;
    case CSR_MIE:
    {

    }
    break;
    default:
    {

    }
    break;
    }
    return reg_value;
}

static inline  uint32_t hs_risc_v_core_rv32_csr_read(hs_risc_v_core_rv32_t *core,size_t address)
{
    if(core!=NULL && core->io!=NULL)
    {
        hs_risc_v_common_memory_word_t value;
        value.value=hs_risc_v_core_rv32_csr_read_default_value(core,address);
        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
        core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_CSR_READ | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_CORE,address,value.bytes,sizeof(value.bytes),core->usr);
        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
        return value.value;
    }
    return 0;
}

static inline  void hs_risc_v_core_rv32_csr_write(hs_risc_v_core_rv32_t *core,size_t address,uint32_t reg_value)
{
    if(core!=NULL && core->io!=NULL)
    {
        if(address==CSR_MISA)
        {
            /*
             * 支持通过misa动态使能/关闭"C"扩展。
             * 若关闭"C"会导致下一条指令的取指地址变为非32位对齐，
             * 则忽略本次关闭操作(保持IALIGN=16)。
             */
            if((reg_value&(1UL<<(2)))==0 &&
               hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32C) &&
               (hs_risc_v_core_rv32_pc_read(core)&0x2)!=0)
            {
                reg_value|=(1UL<<(2));
            }
            if((reg_value&(1UL<<(2)))!=0)
            {
                core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32C);
            }
            else
            {
                core->instruction_sets=hs_risc_v_common_instruction_set_sets_clear_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32C);
            }
        }
        hs_risc_v_common_memory_word_t value;
        value.value=reg_value;
        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
        core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_CSR_WRITE | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_CORE,address,value.bytes,sizeof(value.bytes),core->usr);
    }
}

/** \brief 检查CSR访问是否合法
 *
 * \param core 内核指针
 * \param csr CSR地址
 * \param write 是否为写访问
 * \return bool 是否合法
 *
 */
static inline bool hs_risc_v_core_rv32_csr_access_legal(hs_risc_v_core_rv32_t *core,uint32_t csr,bool write)
{
    if(core==NULL)
    {
        return false;
    }
    uint32_t priv=hs_risc_v_core_rv32_privilege_get(core);
    /*
     * CSR地址位[9:8]为访问所需的最低特权等级
     */
    if(priv < ((csr>>8)&0x3))
    {
        return false;
    }
    /*
     * CSR地址位[11:10]==3表示只读
     */
    if(write && (((csr>>10)&0x3)==3))
    {
        return false;
    }
    /*
     * satp在TVM=1时不可访问(S模式)
     */
    if(csr==CSR_SATP && priv<HS_RISC_V_CORE_RV32_PRIVILEGE_M)
    {
        if((hs_risc_v_core_rv32_csr_read(core,CSR_MSTATUS)&MSTATUS_TVM)!=0)
        {
            return false;
        }
    }
    /*
     * 用户态计数器访问受mcounteren/scounteren控制
     */
    if(priv<HS_RISC_V_CORE_RV32_PRIVILEGE_M &&
       ((csr>=CSR_CYCLE && csr<=CSR_CYCLE+31) ||
        (csr>=CSR_CYCLEH && csr<=CSR_CYCLEH+31)))
    {
        uint32_t bit=(csr&0x1F);
        if((hs_risc_v_core_rv32_csr_read(core,CSR_MCOUNTEREN)&(1UL<<bit))==0)
        {
            return false;
        }
        if(priv==HS_RISC_V_CORE_RV32_PRIVILEGE_U &&
           (hs_risc_v_core_rv32_csr_read(core,CSR_SCOUNTEREN)&(1UL<<bit))==0)
        {
            return false;
        }
    }
    return true;
}

/*
 * Sv32 MMU实现
 */
#define HS_RISC_V_CORE_RV32_PTE_V   (1UL<<0)
#define HS_RISC_V_CORE_RV32_PTE_R   (1UL<<1)
#define HS_RISC_V_CORE_RV32_PTE_W   (1UL<<2)
#define HS_RISC_V_CORE_RV32_PTE_X   (1UL<<3)
#define HS_RISC_V_CORE_RV32_PTE_U   (1UL<<4)
#define HS_RISC_V_CORE_RV32_PTE_G   (1UL<<5)
#define HS_RISC_V_CORE_RV32_PTE_A   (1UL<<6)
#define HS_RISC_V_CORE_RV32_PTE_D   (1UL<<7)

#define HS_RISC_V_CORE_RV32_MEM_ACCESS_FETCH 0
#define HS_RISC_V_CORE_RV32_MEM_ACCESS_LOAD  1
#define HS_RISC_V_CORE_RV32_MEM_ACCESS_STORE 2

static inline void hs_risc_v_core_rv32_exec_exception_raise_value(hs_risc_v_core_rv32_t *core,int cause,uint32_t instruction_pc,uint32_t instruction,uint32_t value);

/** \brief 物理地址内存访问(不经过MMU)
 *
 */
static inline bool hs_risc_v_core_rv32_mmu_physical_io(hs_risc_v_core_rv32_t *core,bool write,uint32_t pa,uint8_t *data,size_t len)
{
    if(core==NULL || core->io==NULL)
    {
        return false;
    }
    return core->io(core,(uint32_t)(write?HS_RISC_V_CORE_RV32_IO_MEMORY_WRITE:HS_RISC_V_CORE_RV32_IO_MEMORY_READ)|(uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_CORE,pa,data,len,core->usr);
}

/** \brief 产生页错误
 *
 */
static inline bool hs_risc_v_core_rv32_mmu_page_fault(hs_risc_v_core_rv32_t *core,uint32_t access,uint32_t va)
{
    int cause=HS_RISC_V_OPCODES_EXCEPTION_NAME(CAUSE_LOAD_PAGE_FAULT);
    if(access==HS_RISC_V_CORE_RV32_MEM_ACCESS_FETCH)
    {
        cause=HS_RISC_V_OPCODES_EXCEPTION_NAME(CAUSE_FETCH_PAGE_FAULT);
    }
    else if(access==HS_RISC_V_CORE_RV32_MEM_ACCESS_STORE)
    {
        cause=HS_RISC_V_OPCODES_EXCEPTION_NAME(CAUSE_STORE_PAGE_FAULT);
    }
    hs_risc_v_core_rv32_exec_exception_raise_value(core,cause,0,0,va);
    return false;
}

/** \brief Sv32地址翻译
 *
 * \param core 内核指针
 * \param va uint32_t 虚拟地址
 * \param access uint32_t 访问类型(HS_RISC_V_CORE_RV32_MEM_ACCESS_*)
 * \param pa uint32_t* 物理地址输出
 * \return bool 是否成功(失败时已产生页错误)
 *
 * 注意:若页表所在内存的IO操作未实现，则按无MMU方式处理，物理地址等于虚拟地址。
 */
static inline bool hs_risc_v_core_rv32_mmu_translate(hs_risc_v_core_rv32_t *core,uint32_t va,uint32_t access,uint32_t *pa)
{
    if(pa==NULL)
    {
        return false;
    }
    *pa=va;
    if(core==NULL)
    {
        return true;
    }
    uint32_t satp=hs_risc_v_core_rv32_csr_read(core,CSR_SATP);
    if((satp>>31)!=1)
    {
        /*
         * Bare模式,或者用户未实现satp CSR(默认值为0),按无MMU处理
         */
        return true;
    }

    uint32_t mstatus=hs_risc_v_core_rv32_csr_read(core,CSR_MSTATUS);
    uint32_t priv=hs_risc_v_core_rv32_privilege_get(core);
    if(access!=HS_RISC_V_CORE_RV32_MEM_ACCESS_FETCH && priv==HS_RISC_V_CORE_RV32_PRIVILEGE_M && (mstatus&MSTATUS_MPRV)!=0)
    {
        /*
         * MPRV:数据访问使用MPP指定的特权等级
         */
        priv=(mstatus&MSTATUS_MPP)>>11;
    }
    if(priv==HS_RISC_V_CORE_RV32_PRIVILEGE_M)
    {
        /*
         * M模式(且未启用MPRV)不进行地址翻译
         */
        return true;
    }

    uint32_t vpn1=(va>>22)&0x3FF;
    uint32_t vpn0=(va>>12)&0x3FF;
    uint32_t offset=va&0xFFF;
    uint32_t table=(satp&0x3FFFFF)<<12;

    for(int level=1; level>=0; level--)
    {
        uint32_t vpn=(level==1)?vpn1:vpn0;
        uint32_t pte_addr=table+vpn*4;
        hs_risc_v_common_memory_word_t pte;
        pte.value=0;
        if(!hs_risc_v_core_rv32_mmu_physical_io(core,false,pte_addr,pte.bytes,sizeof(pte.bytes)))
        {
            /*
             * 页表内存IO未实现，按无MMU处理
             */
            *pa=va;
            return true;
        }
        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(pte);

        if((pte.value&HS_RISC_V_CORE_RV32_PTE_V)==0 ||
           (((pte.value&HS_RISC_V_CORE_RV32_PTE_R)==0)&&((pte.value&HS_RISC_V_CORE_RV32_PTE_W)!=0)))
        {
            return hs_risc_v_core_rv32_mmu_page_fault(core,access,va);
        }

        if((pte.value&(HS_RISC_V_CORE_RV32_PTE_R|HS_RISC_V_CORE_RV32_PTE_X))!=0)
        {
            /*
             * 叶子PTE,检查权限
             */
            bool is_user=((pte.value&HS_RISC_V_CORE_RV32_PTE_U)!=0);
            bool sum=((mstatus&MSTATUS_SUM)!=0);
            bool mxr=((mstatus&MSTATUS_MXR)!=0);

            if(priv==HS_RISC_V_CORE_RV32_PRIVILEGE_U)
            {
                if(!is_user)
                {
                    return hs_risc_v_core_rv32_mmu_page_fault(core,access,va);
                }
            }
            else
            {
                /*
                 * S模式:取指不能访问U页;数据访问在SUM=1时可访问U页
                 */
                if(access==HS_RISC_V_CORE_RV32_MEM_ACCESS_FETCH)
                {
                    if(is_user)
                    {
                        return hs_risc_v_core_rv32_mmu_page_fault(core,access,va);
                    }
                }
                else if(is_user && !sum)
                {
                    return hs_risc_v_core_rv32_mmu_page_fault(core,access,va);
                }
            }

            if(access==HS_RISC_V_CORE_RV32_MEM_ACCESS_FETCH)
            {
                if((pte.value&HS_RISC_V_CORE_RV32_PTE_X)==0)
                {
                    return hs_risc_v_core_rv32_mmu_page_fault(core,access,va);
                }
            }
            else if(access==HS_RISC_V_CORE_RV32_MEM_ACCESS_LOAD)
            {
                if((pte.value&HS_RISC_V_CORE_RV32_PTE_R)==0 &&
                   !(mxr && (pte.value&HS_RISC_V_CORE_RV32_PTE_X)!=0))
                {
                    return hs_risc_v_core_rv32_mmu_page_fault(core,access,va);
                }
            }
            else
            {
                if((pte.value&HS_RISC_V_CORE_RV32_PTE_W)==0)
                {
                    return hs_risc_v_core_rv32_mmu_page_fault(core,access,va);
                }
            }

            uint32_t ppn=(pte.value>>10)&0xFFFFF;
            if(level==1)
            {
                /*
                 * 4MB超级页,PPN[0]必须为0
                 */
                if((ppn&0x3FF)!=0)
                {
                    return hs_risc_v_core_rv32_mmu_page_fault(core,access,va);
                }
                ppn=(ppn&0xFFFFFC00)|vpn0;
            }

            /*
             * 硬件更新A/D位
             */
            uint32_t new_pte=pte.value;
            if((new_pte&HS_RISC_V_CORE_RV32_PTE_A)==0)
            {
                new_pte|=HS_RISC_V_CORE_RV32_PTE_A;
            }
            if(access==HS_RISC_V_CORE_RV32_MEM_ACCESS_STORE && (new_pte&HS_RISC_V_CORE_RV32_PTE_D)==0)
            {
                new_pte|=HS_RISC_V_CORE_RV32_PTE_D;
            }
            if(new_pte!=pte.value)
            {
                hs_risc_v_common_memory_word_t new_pte_mem;
                new_pte_mem.value=new_pte;
                HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(new_pte_mem);
                hs_risc_v_core_rv32_mmu_physical_io(core,true,pte_addr,new_pte_mem.bytes,sizeof(new_pte_mem.bytes));
            }

            *pa=(ppn<<12)|offset;
            return true;
        }
        else
        {
            /*
             * 指向下一级页表
             */
            if(level==0)
            {
                return hs_risc_v_core_rv32_mmu_page_fault(core,access,va);
            }
            table=((pte.value>>10)&0xFFFFF)<<12;
        }
    }

    return hs_risc_v_core_rv32_mmu_page_fault(core,access,va);
}

/** \brief 带MMU的虚拟地址内存访问
 *
 * \param core 内核指针
 * \param opt uint32_t IO选项
 * \param va uint32_t 虚拟地址
 * \param data uint8_t* 数据指针
 * \param len size_t 数据长度
 * \param access uint32_t 访问类型
 * \param atomic bool 是否需要保持单次IO访问(AMO)
 * \param page_fault bool* 是否发生页错误，可为NULL
 * \return bool 是否访问成功
 *
 */
static inline bool hs_risc_v_core_rv32_mmu_mem_io(hs_risc_v_core_rv32_t *core,uint32_t opt,uint32_t va,uint8_t *data,size_t len,uint32_t access,bool atomic,bool *page_fault)
{
    if(page_fault!=NULL)
    {
        *page_fault=false;
    }
    if(core==NULL || core->io==NULL)
    {
        return false;
    }
    size_t done=0;
    bool ret=true;
    while(done<len)
    {
        uint32_t pa;
        if(!hs_risc_v_core_rv32_mmu_translate(core,va+done,access,&pa))
        {
            if(page_fault!=NULL)
            {
                *page_fault=true;
            }
            return false;
        }
        size_t chunk=len-done;
        if(!atomic)
        {
            size_t page_left=0x1000-((va+done)&0xFFF);
            if(chunk>page_left)
            {
                chunk=page_left;
            }
        }
        if(!core->io(core,opt,pa,data+done,chunk,core->usr))
        {
            ret=false;
        }
        done+=chunk;
    }
    return ret;
}

static inline  bool hs_risc_v_core_rv32_instruction_read(hs_risc_v_core_rv32_t *core,uint32_t *instruction)
{
    if(instruction==NULL)
    {
        return false;
    }
    *instruction=0;
    if(core==NULL || core->io==NULL)
    {
        return false;
    }
    uint32_t pc=hs_risc_v_core_rv32_pc_read(core);
    hs_risc_v_common_memory_halfword_t low;
    low.value=0;
    if(!hs_risc_v_core_rv32_mmu_mem_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_READ|(uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_CORE,pc,low.bytes,sizeof(low.bytes),HS_RISC_V_CORE_RV32_MEM_ACCESS_FETCH,false,NULL))
    {
        return false;
    }
    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(low);
    uint32_t value=low.value;
    if((low.value&0x3)==0x3)
    {
        hs_risc_v_common_memory_halfword_t high;
        high.value=0;
        if(!hs_risc_v_core_rv32_mmu_mem_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_READ|(uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_CORE,pc+2,high.bytes,sizeof(high.bytes),HS_RISC_V_CORE_RV32_MEM_ACCESS_FETCH,false,NULL))
        {
            return false;
        }
        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(high);
        value|=((uint32_t)high.value)<<16;
    }
    *instruction=value;
    return true;
}

#ifndef HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH
#define HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(NAME,CODE)      \
if(!is_instruction_processed)\
{\
    const hs_risc_v_opcodes_insn_t insn=HS_RISC_V_OPCODES_INSN_NAME(NAME);\
    if((instruction&insn.mask)==insn.match)\
    {\
        is_instruction_processed=true;\
        {\
            CODE;\
        }\
    }\
}

#endif // HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH

static inline  void hs_risc_v_core_rv32_exec_exception_raise_value(hs_risc_v_core_rv32_t *core,int cause,uint32_t instruction_pc,uint32_t instruction,uint32_t value)
{
    (void)instruction_pc;
    (void)instruction;
    //设置，下次指令执行时可能进入跳转相应处理函数。
    if(hs_risc_v_core_rv32_exception_raise(core,cause,false))
    {
        core->exception_value=value;
    }
}

static inline  void hs_risc_v_core_rv32_exec_exception_raise(hs_risc_v_core_rv32_t *core,int cause,uint32_t instruction_pc,uint32_t instruction)
{
    uint32_t value=0;
    if(cause==HS_RISC_V_OPCODES_EXCEPTION_NAME(CAUSE_ILLEGAL_INSTRUCTION))
    {
        value=instruction;
    }
    hs_risc_v_core_rv32_exec_exception_raise_value(core,cause,instruction_pc,instruction,value);
}

static inline uint32_t hs_risc_v_core_rv32_ialign(hs_risc_v_core_rv32_t *core)
{
    return (hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32C))?2:4;
}

static inline uint32_t hs_risc_v_core_rv32_pc_align(hs_risc_v_core_rv32_t *core,uint32_t pc)
{
    return pc & (~(hs_risc_v_core_rv32_ialign(core)-1));
}

static inline void hs_risc_v_core_rv32_exec_exception_mret(hs_risc_v_core_rv32_t *core)
{
    uint32_t mstatus=hs_risc_v_core_rv32_csr_read(core,CSR_MSTATUS);
    uint32_t mpp=(mstatus&MSTATUS_MPP)>>11;
    //从mepc恢复地址
    uint32_t mepc=hs_risc_v_core_rv32_pc_align(core,hs_risc_v_core_rv32_csr_read(core,CSR_MEPC));
    hs_risc_v_core_rv32_pc_write(core,mepc);
    /*
     * 复制MPIE到MIE,设置MPIE为1,MPP为U(MPRV在MPP!=M时清零)
     */
    uint32_t new_mstatus=mstatus;
    if((mstatus&MSTATUS_MPIE)!=0)
    {
        new_mstatus|=MSTATUS_MIE;
    }
    else
    {
        new_mstatus&=(~MSTATUS_MIE);
    }
    new_mstatus|=MSTATUS_MPIE;
    /*
     * 仅当MPP!=M时清除MPRV
     */
    if(mpp!=HS_RISC_V_CORE_RV32_PRIVILEGE_M)
    {
        new_mstatus&=(~MSTATUS_MPRV);
    }
    new_mstatus&=(~MSTATUS_MPP);
    hs_risc_v_core_rv32_csr_write(core,CSR_MSTATUS,new_mstatus);
    hs_risc_v_core_rv32_privilege_set(core,mpp);
}

static inline void hs_risc_v_core_rv32_exec_exception_sret(hs_risc_v_core_rv32_t *core)
{
    uint32_t mstatus=hs_risc_v_core_rv32_csr_read(core,CSR_MSTATUS);
    uint32_t spp=((mstatus&MSTATUS_SPP)!=0)?HS_RISC_V_CORE_RV32_PRIVILEGE_S:HS_RISC_V_CORE_RV32_PRIVILEGE_U;
    uint32_t sepc=hs_risc_v_core_rv32_pc_align(core,hs_risc_v_core_rv32_csr_read(core,CSR_SEPC));
    hs_risc_v_core_rv32_pc_write(core,sepc);
    uint32_t new_mstatus=mstatus;
    if((mstatus&MSTATUS_SPIE)!=0)
    {
        new_mstatus|=MSTATUS_SIE;
    }
    else
    {
        new_mstatus&=(~MSTATUS_SIE);
    }
    new_mstatus|=MSTATUS_SPIE;
    /*
     * SRET清除MPRV及SPP
     */
    new_mstatus&=(~(MSTATUS_MPRV|MSTATUS_SPP));
    hs_risc_v_core_rv32_csr_write(core,CSR_MSTATUS,new_mstatus);
    hs_risc_v_core_rv32_privilege_set(core,spp);
}

static inline void hs_risc_v_core_rv32_exec_exception_uret(hs_risc_v_core_rv32_t *core)
{
    /*
     * URET为N扩展指令,使用UEPC(0x041)恢复地址
     */
    uint32_t uepc=hs_risc_v_core_rv32_csr_read(core,0x041);
    hs_risc_v_core_rv32_pc_write(core,hs_risc_v_core_rv32_pc_align(core,uepc));
    hs_risc_v_core_rv32_privilege_set(core,HS_RISC_V_CORE_RV32_PRIVILEGE_U);
}

/** \brief 进入异常/中断处理流程
 *
 * \param core 内核指针
 * \param cause 异常/中断原因(中断为不含最高位的编号)
 * \param interrupt 是否为中断
 * \param epc 异常指令地址或中断返回地址
 * \param tval 异常附带值
 *
 */
static inline void hs_risc_v_core_rv32_exec_trap(hs_risc_v_core_rv32_t *core,uint32_t cause,bool interrupt,uint32_t epc,uint32_t tval)
{
    uint32_t priv=hs_risc_v_core_rv32_privilege_get(core);
    uint32_t mstatus=hs_risc_v_core_rv32_csr_read(core,CSR_MSTATUS);
    bool delegate=false;
    if(priv<HS_RISC_V_CORE_RV32_PRIVILEGE_M && cause<32)
    {
        uint32_t deleg=hs_risc_v_core_rv32_csr_read(core,interrupt?CSR_MIDELEG:CSR_MEDELEG);
        if((deleg&(1UL<<cause))!=0)
        {
            delegate=true;
        }
    }
    uint32_t cause_value=(interrupt?(cause|(1UL<<31)):cause);
    uint32_t tvec=0;
    if(delegate)
    {
        hs_risc_v_core_rv32_csr_write(core,CSR_SCAUSE,cause_value);
        hs_risc_v_core_rv32_csr_write(core,CSR_SEPC,hs_risc_v_core_rv32_pc_align(core,epc));
        hs_risc_v_core_rv32_csr_write(core,CSR_STVAL,tval);
        uint32_t trap_mstatus=mstatus;
        if((mstatus&MSTATUS_SIE)!=0)
        {
            trap_mstatus|=MSTATUS_SPIE;
        }
        else
        {
            trap_mstatus&=(~MSTATUS_SPIE);
        }
        if((priv&1)!=0)
        {
            trap_mstatus|=MSTATUS_SPP;
        }
        else
        {
            trap_mstatus&=(~MSTATUS_SPP);
        }
        trap_mstatus&=(~MSTATUS_SIE);
        hs_risc_v_core_rv32_csr_write(core,CSR_MSTATUS,trap_mstatus);
        hs_risc_v_core_rv32_privilege_set(core,HS_RISC_V_CORE_RV32_PRIVILEGE_S);
        tvec=hs_risc_v_core_rv32_csr_read(core,CSR_STVEC);
    }
    else
    {
        hs_risc_v_core_rv32_csr_write(core,CSR_MCAUSE,cause_value);
        hs_risc_v_core_rv32_csr_write(core,CSR_MEPC,hs_risc_v_core_rv32_pc_align(core,epc));
        hs_risc_v_core_rv32_csr_write(core,CSR_MTVAL,tval);
        uint32_t trap_mstatus=mstatus;
        if((mstatus&MSTATUS_MIE)!=0)
        {
            trap_mstatus|=MSTATUS_MPIE;
        }
        else
        {
            trap_mstatus&=(~MSTATUS_MPIE);
        }
        trap_mstatus&=(~MSTATUS_MIE);
        trap_mstatus&=(~MSTATUS_MPP);
        trap_mstatus|=((priv&0x3)<<11);
        hs_risc_v_core_rv32_csr_write(core,CSR_MSTATUS,trap_mstatus);
        hs_risc_v_core_rv32_privilege_set(core,HS_RISC_V_CORE_RV32_PRIVILEGE_M);
        tvec=hs_risc_v_core_rv32_csr_read(core,CSR_MTVEC);
    }
    uint32_t next_pc=tvec&(~0x3UL);
    if((tvec&0x3)!=0 && interrupt)
    {
        /*
         * 向量模式仅用于中断
         */
        next_pc+=4*cause;
    }
    hs_risc_v_core_rv32_pc_write(core,next_pc);
}

static inline  void hs_risc_v_core_rv32_exec_interrupt_raise(hs_risc_v_core_rv32_t *core,int cause)
{
    hs_risc_v_core_rv32_exception_raise(core,cause,true);
}

/** \brief 判断中断在当前特权等级及CSR设置下是否使能
 *
 * \param core 内核指针
 * \param cause 中断原因编号
 * \return bool 是否使能
 *
 */
static inline bool hs_risc_v_core_rv32_interrupt_enabled(hs_risc_v_core_rv32_t *core,uint32_t cause)
{
    uint32_t mstatus=hs_risc_v_core_rv32_csr_read(core,CSR_MSTATUS);
    uint32_t priv=hs_risc_v_core_rv32_privilege_get(core);
    bool m_enable=((mstatus&MSTATUS_MIE)!=0);
    bool s_enable=((mstatus&MSTATUS_SIE)!=0);
    if(cause==1 || cause==5 || cause==9)
    {
        uint32_t mideleg=hs_risc_v_core_rv32_csr_read(core,CSR_MIDELEG);
        if(priv<HS_RISC_V_CORE_RV32_PRIVILEGE_M && (mideleg&(1UL<<cause))!=0)
        {
            return s_enable;
        }
        return m_enable;
    }
    return m_enable;
}


static inline void hs_risc_v_core_rv32_exec_clint(hs_risc_v_core_rv32_t *core)
{
    if(core==NULL)
    {
        return;
    }

    /*
     * 此处只实现了clint的部分功能，用户需要手动实现软件中断，最终向mip写入相应的位
     */

    {
        /*
         * 处理定时器中断
         */
        uint32_t mip=hs_risc_v_core_rv32_csr_read(core,CSR_MIP);
        if(core->clint.mtimecmp > core->clint.mtime)
        {
            mip &= (~MIP_MTIP);
        }
        else
        {
            mip |= (MIP_MTIP);
        }
        hs_risc_v_core_rv32_csr_write(core,CSR_MIP,mip);
    }

    uint32_t mstatus=hs_risc_v_core_rv32_csr_read(core,CSR_MSTATUS);

    if(core->interrupt_pending==0)
    {
        uint32_t mip=hs_risc_v_core_rv32_csr_read(core,CSR_MIP);
        uint32_t mie=hs_risc_v_core_rv32_csr_read(core,CSR_MIE);
        uint32_t mideleg=hs_risc_v_core_rv32_csr_read(core,CSR_MIDELEG);
        uint32_t priv=hs_risc_v_core_rv32_privilege_get(core);
        uint32_t interrupt=(mip&mie);
        bool m_enable=((mstatus&MSTATUS_MIE)!=0);
        bool s_enable=((mstatus&MSTATUS_SIE)!=0);

        /*
         * 判断某中断在当前状态下是否使能。S模式中断(1,5,9)在非M模式且被委托时由SIE使能，否则由MIE使能。
         */
#define HS_RISC_V_CORE_RV32_INTERRUPT_ENABLED(CAUSE) \
        ( (((CAUSE)==1)||((CAUSE)==5)||((CAUSE)==9))? \
          ((priv<HS_RISC_V_CORE_RV32_PRIVILEGE_M && (mideleg&(1UL<<(CAUSE)))!=0)?s_enable:m_enable) : m_enable )

        /*
         * 按照MEI、MSI、MTI、SEI、SSI、STI的优先级顺序处理中断
         */
        if((interrupt & MIP_MEIP)!=0 && HS_RISC_V_CORE_RV32_INTERRUPT_ENABLED(11))
        {
            /*
             * 外部中断
             */
            hs_risc_v_core_rv32_exec_interrupt_raise(core,11);
        }
        else if((interrupt & MIP_MSIP)!=0 && HS_RISC_V_CORE_RV32_INTERRUPT_ENABLED(3))
        {
            /*
             * 软件中断
             */
            hs_risc_v_core_rv32_exec_interrupt_raise(core,3);
        }
        else if((interrupt & MIP_MTIP)!=0 && HS_RISC_V_CORE_RV32_INTERRUPT_ENABLED(7))
        {
            /*
             * 定时器中断
             */
            hs_risc_v_core_rv32_exec_interrupt_raise(core,7);
        }
        else if((interrupt & MIP_SEIP)!=0 && HS_RISC_V_CORE_RV32_INTERRUPT_ENABLED(9))
        {
            /*
             * 监管者外部中断
             */
            hs_risc_v_core_rv32_exec_interrupt_raise(core,9);
        }
        else if((interrupt & MIP_SSIP)!=0 && HS_RISC_V_CORE_RV32_INTERRUPT_ENABLED(1))
        {
            /*
             * 监管者软件中断
             */
            hs_risc_v_core_rv32_exec_interrupt_raise(core,1);
        }
        else if((interrupt & MIP_STIP)!=0 && HS_RISC_V_CORE_RV32_INTERRUPT_ENABLED(5))
        {
            /*
             * 监管者定时器中断
             */
            hs_risc_v_core_rv32_exec_interrupt_raise(core,5);
        }
#undef HS_RISC_V_CORE_RV32_INTERRUPT_ENABLED
    }
}

static inline void hs_risc_v_core_rv32_cycle_inc(hs_risc_v_core_rv32_t *core)
{
    if(core->flags.skip_counter_inc!=0)
    {
        /*
         * 本指令写入了计数器，不增加计数器
         */
        core->flags.skip_counter_inc=0;
        return;
    }
    {
        uint32_t cycle=hs_risc_v_core_rv32_csr_read(core,CSR_CYCLE);
        uint32_t cycleh=hs_risc_v_core_rv32_csr_read(core,CSR_CYCLEH);
        cycle++;
        hs_risc_v_core_rv32_csr_write(core,CSR_CYCLE,cycle);
        if(cycle==0)
        {
            cycleh++;
            hs_risc_v_core_rv32_csr_write(core,CSR_CYCLEH,cycleh);
        }
    }
    {
        uint32_t instret=hs_risc_v_core_rv32_csr_read(core,CSR_INSTRET);
        uint32_t instreth=hs_risc_v_core_rv32_csr_read(core,CSR_INSTRETH);
        instret++;
        hs_risc_v_core_rv32_csr_write(core,CSR_INSTRET,instret);
        if(instret==0)
        {
            instreth++;
            hs_risc_v_core_rv32_csr_write(core,CSR_INSTRETH,instreth);
        }
    }
}

static inline  bool hs_risc_v_core_rv32_exec_io(hs_risc_v_core_rv32_t *core,uint32_t opt,uint32_t address,uint8_t *data,size_t len)
{
    if(core==NULL || core->io==NULL)
    {
        return false;
    }
    if(core->flags.abort!=0)
    {
        /*
         * 当前指令已因页错误等异常终止,抑制后续总线访问
         */
        return false;
    }
    uint32_t full_opt=opt|(uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_CORE;
    uint32_t op=(opt&0x00FFFFFFUL);
    uint32_t access=HS_RISC_V_CORE_RV32_MEM_ACCESS_LOAD;
    bool is_memory=false;
    bool atomic=false;
    switch(op)
    {
    case HS_RISC_V_CORE_RV32_IO_MEMORY_READ:
    {
        access=HS_RISC_V_CORE_RV32_MEM_ACCESS_LOAD;
        is_memory=true;
    }
    break;
    case HS_RISC_V_CORE_RV32_IO_MEMORY_WRITE:
    {
        access=HS_RISC_V_CORE_RV32_MEM_ACCESS_STORE;
        is_memory=true;
    }
    break;
    case HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_LR:
    {
        access=HS_RISC_V_CORE_RV32_MEM_ACCESS_LOAD;
        is_memory=true;
        atomic=true;
    }
    break;
    case HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_SC:
    case HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_READ:
    case HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_WRITE:
    {
        access=HS_RISC_V_CORE_RV32_MEM_ACCESS_STORE;
        is_memory=true;
        atomic=true;
    }
    break;
    default:
    {
    }
    break;
    }
    if(is_memory)
    {
        bool page_fault=false;
        bool ret=hs_risc_v_core_rv32_mmu_mem_io(core,full_opt,address,data,len,access,atomic,&page_fault);
        if(page_fault)
        {
            core->flags.abort=1;
        }
        return ret;
    }
    //默认为内核操作
    return core->io(core,full_opt,address,data,len,core->usr);
}

static inline bool hs_risc_v_core_rv32_is_counter_csr(uint32_t csr)
{
    switch(csr)
    {
    case CSR_CYCLE:
    case CSR_TIME:
    case CSR_INSTRET:
    case CSR_CYCLEH:
    case CSR_TIMEH:
    case CSR_INSTRETH:
    case CSR_MCYCLE:
    case CSR_MINSTRET:
    case CSR_MCYCLEH:
    case CSR_MINSTRETH:
    {
        return true;
    }
    default:
    {
    }
    break;
    }
    return false;
}

static inline void hs_risc_v_core_rv32_csr_write_by_inst(hs_risc_v_core_rv32_t *core,uint32_t csr,uint32_t value)
{
    hs_risc_v_core_rv32_csr_write(core,csr,value);
    if(hs_risc_v_core_rv32_is_counter_csr(csr))
    {
        /*
         * 写入计数器时抑制本次指令的计数器自增
         */
        core->flags.skip_counter_inc=1;
    }
}

static inline uint32_t hs_risc_v_core_rv32_fcsr_read(hs_risc_v_core_rv32_t *core)
{
    return hs_risc_v_core_rv32_csr_read(core,CSR_FCSR)&0xFF;
}

static inline void hs_risc_v_core_rv32_fflags_commit(hs_risc_v_core_rv32_t *core,uint32_t fflags)
{
    uint32_t fcsr=hs_risc_v_core_rv32_fcsr_read(core);
    fcsr|=(fflags&0x1FUL);
    hs_risc_v_core_rv32_csr_write(core,CSR_FCSR,fcsr&0xFFUL);
}

static inline bool hs_risc_v_core_rv32_fp_rm_get(hs_risc_v_core_rv32_t *core,uint32_t instruction,hsoftfp_rounding_mode_t *rm)
{
    uint32_t rm_value=((instruction>>12)&0x7);
    if(rm_value==7)
    {
        rm_value=((hs_risc_v_core_rv32_fcsr_read(core)>>5)&0x7);
    }
    if(rm_value>4)
    {
        return false;
    }
    if(rm!=NULL)
    {
        *rm=(hsoftfp_rounding_mode_t)rm_value;
    }
    return true;
}

static inline bool hs_risc_v_core_rv32_fp_enabled(hs_risc_v_core_rv32_t *core)
{
    return (hs_risc_v_core_rv32_csr_read(core,CSR_MSTATUS)&MSTATUS_FS)!=0;
}

static inline void hs_risc_v_core_rv32_fp_dirty(hs_risc_v_core_rv32_t *core)
{
    if(core!=NULL && core->flags.abort!=0)
    {
        return;
    }
    uint32_t mstatus=hs_risc_v_core_rv32_csr_read(core,CSR_MSTATUS);
    if((mstatus&MSTATUS_FS)!=MSTATUS_FS)
    {
        mstatus&=(~MSTATUS_FS);
        mstatus|=MSTATUS_FS;
        hs_risc_v_core_rv32_csr_write(core,CSR_MSTATUS,mstatus);
    }
}

static inline uint64_t hs_risc_v_core_rv32_fp_read_d(hs_risc_v_core_rv32_t *core,uint32_t index)
{
    return hs_risc_v_core_rv32_f_register_read64(core,index);
}

static inline void hs_risc_v_core_rv32_fp_write_d(hs_risc_v_core_rv32_t *core,uint32_t index,uint64_t value)
{
    hs_risc_v_core_rv32_f_register_write64(core,index,value);
}

static inline uint32_t hs_risc_v_core_rv32_fp_read_s(hs_risc_v_core_rv32_t *core,uint32_t index)
{
    return hs_risc_v_core_rv32_f_register_read32(core,index);
}

/*
 * 读取单精度寄存器的原始低32位(不进行NaN-boxing检查),用于存储指令/FMV.X.W
 */
static inline uint32_t hs_risc_v_core_rv32_fp_read_s_raw(hs_risc_v_core_rv32_t *core,uint32_t index)
{
    if(hs_risc_v_core_rv32_has_d(core))
    {
        return (uint32_t)hs_risc_v_core_rv32_f_register_read64(core,index);
    }
    return hs_risc_v_core_rv32_f_register_read32(core,index);
}

static inline void hs_risc_v_core_rv32_fp_write_s(hs_risc_v_core_rv32_t *core,uint32_t index,uint32_t value)
{
    hs_risc_v_core_rv32_f_register_write32(core,index,value);
}

/*
 * 压缩指令符号扩展辅助函数
 */
static inline bool hs_risc_v_core_rv32_f32_is_nan(uint32_t value)
{
    return ((value&0x7F800000UL)==0x7F800000UL)&&((value&0x007FFFFFUL)!=0);
}

static inline bool hs_risc_v_core_rv32_f32_is_snan(uint32_t value)
{
    return hs_risc_v_core_rv32_f32_is_nan(value) && ((value&0x00400000UL)==0);
}

static inline uint32_t hs_risc_v_core_rv32_fmin_s(uint32_t a,uint32_t b,uint32_t *pfflags)
{
    if(hs_risc_v_core_rv32_f32_is_nan(a) || hs_risc_v_core_rv32_f32_is_nan(b))
    {
        if(hs_risc_v_core_rv32_f32_is_snan(a) || hs_risc_v_core_rv32_f32_is_snan(b))
        {
            *pfflags|=HSOFTFP_FFLAG_INVALID_OP;
        }
        if(hs_risc_v_core_rv32_f32_is_nan(a) && hs_risc_v_core_rv32_f32_is_nan(b))
        {
            return 0x7FC00000;
        }
        return hs_risc_v_core_rv32_f32_is_nan(a)?b:a;
    }
    uint32_t tmp_flags=0;
    bool a_lt_b=(hsoftfp_lt_sf32(a,b,&tmp_flags)!=0);
    bool b_lt_a=(hsoftfp_lt_sf32(b,a,&tmp_flags)!=0);
    if(!a_lt_b && !b_lt_a)
    {
        return ((a&HSOFTFP_FSIGN_MASK32)!=0)?a:b;
    }
    return a_lt_b?a:b;
}

static inline uint32_t hs_risc_v_core_rv32_fmax_s(uint32_t a,uint32_t b,uint32_t *pfflags)
{
    if(hs_risc_v_core_rv32_f32_is_nan(a) || hs_risc_v_core_rv32_f32_is_nan(b))
    {
        if(hs_risc_v_core_rv32_f32_is_snan(a) || hs_risc_v_core_rv32_f32_is_snan(b))
        {
            *pfflags|=HSOFTFP_FFLAG_INVALID_OP;
        }
        if(hs_risc_v_core_rv32_f32_is_nan(a) && hs_risc_v_core_rv32_f32_is_nan(b))
        {
            return 0x7FC00000;
        }
        return hs_risc_v_core_rv32_f32_is_nan(a)?b:a;
    }
    uint32_t tmp_flags=0;
    bool a_lt_b=(hsoftfp_lt_sf32(a,b,&tmp_flags)!=0);
    bool b_lt_a=(hsoftfp_lt_sf32(b,a,&tmp_flags)!=0);
    if(!a_lt_b && !b_lt_a)
    {
        return ((a&HSOFTFP_FSIGN_MASK32)!=0)?b:a;
    }
    return b_lt_a?a:b;
}

static inline bool hs_risc_v_core_rv32_f64_is_nan(uint64_t value)
{
    return ((value&0x7FF0000000000000ULL)==0x7FF0000000000000ULL)&&((value&0x000FFFFFFFFFFFFFULL)!=0);
}

static inline bool hs_risc_v_core_rv32_f64_is_snan(uint64_t value)
{
    return hs_risc_v_core_rv32_f64_is_nan(value) && ((value&0x0008000000000000ULL)==0);
}

static inline uint64_t hs_risc_v_core_rv32_fmin_d(uint64_t a,uint64_t b,uint32_t *pfflags)
{
    if(hs_risc_v_core_rv32_f64_is_nan(a) || hs_risc_v_core_rv32_f64_is_nan(b))
    {
        if(hs_risc_v_core_rv32_f64_is_snan(a) || hs_risc_v_core_rv32_f64_is_snan(b))
        {
            *pfflags|=HSOFTFP_FFLAG_INVALID_OP;
        }
        if(hs_risc_v_core_rv32_f64_is_nan(a) && hs_risc_v_core_rv32_f64_is_nan(b))
        {
            return 0x7FF8000000000000ULL;
        }
        return hs_risc_v_core_rv32_f64_is_nan(a)?b:a;
    }
    uint32_t tmp_flags=0;
    bool a_lt_b=(hsoftfp_lt_sf64(a,b,&tmp_flags)!=0);
    bool b_lt_a=(hsoftfp_lt_sf64(b,a,&tmp_flags)!=0);
    if(!a_lt_b && !b_lt_a)
    {
        return ((a&HSOFTFP_FSIGN_MASK64)!=0)?a:b;
    }
    return a_lt_b?a:b;
}

static inline uint64_t hs_risc_v_core_rv32_fmax_d(uint64_t a,uint64_t b,uint32_t *pfflags)
{
    if(hs_risc_v_core_rv32_f64_is_nan(a) || hs_risc_v_core_rv32_f64_is_nan(b))
    {
        if(hs_risc_v_core_rv32_f64_is_snan(a) || hs_risc_v_core_rv32_f64_is_snan(b))
        {
            *pfflags|=HSOFTFP_FFLAG_INVALID_OP;
        }
        if(hs_risc_v_core_rv32_f64_is_nan(a) && hs_risc_v_core_rv32_f64_is_nan(b))
        {
            return 0x7FF8000000000000ULL;
        }
        return hs_risc_v_core_rv32_f64_is_nan(a)?b:a;
    }
    uint32_t tmp_flags=0;
    bool a_lt_b=(hsoftfp_lt_sf64(a,b,&tmp_flags)!=0);
    bool b_lt_a=(hsoftfp_lt_sf64(b,a,&tmp_flags)!=0);
    if(!a_lt_b && !b_lt_a)
    {
        return ((a&HSOFTFP_FSIGN_MASK64)!=0)?b:a;
    }
    return b_lt_a?a:b;
}

static inline int32_t hs_risc_v_core_rv32_sign_extend(uint32_t value,uint32_t bits)
{
    uint32_t mask=(1UL<<(bits-1));
    return (int32_t)((value^mask)-mask);
}

/*
 * Zbb扩展辅助函数
 */
static inline uint32_t hs_risc_v_core_rv32_clz32(uint32_t value)
{
    uint32_t count=0;
    uint32_t mask=0x80000000UL;
    if(value==0)
    {
        return 32;
    }
    while((value&mask)==0)
    {
        count++;
        mask>>=1;
    }
    return count;
}

static inline uint32_t hs_risc_v_core_rv32_ctz32(uint32_t value)
{
    uint32_t count=0;
    if(value==0)
    {
        return 32;
    }
    while((value&1)==0)
    {
        count++;
        value>>=1;
    }
    return count;
}

static inline uint32_t hs_risc_v_core_rv32_cpop32(uint32_t value)
{
    uint32_t count=0;
    while(value!=0)
    {
        count+=value&1;
        value>>=1;
    }
    return count;
}

static inline uint32_t hs_risc_v_core_rv32_orc_b32(uint32_t value)
{
    uint32_t result=0;
    for(uint32_t i=0; i<4; i++)
    {
        if(((value>>(i*8))&0xFF)!=0)
        {
            result|=0xFFUL<<(i*8);
        }
    }
    return result;
}

static inline uint32_t hs_risc_v_core_rv32_rev8_32(uint32_t value)
{
    uint32_t result=0;
    for(uint32_t i=0; i<4; i++)
    {
        result|=((value>>(i*8))&0xFF)<<((3-i)*8);
    }
    return result;
}

static inline uint32_t hs_risc_v_core_rv32_rol32(uint32_t value,uint32_t shamt)
{
    shamt&=(sizeof(value)*8-1);
    if(shamt==0)
    {
        return value;
    }
    return (value<<shamt)|(value>>((sizeof(value)*8)-shamt));
}

static inline uint32_t hs_risc_v_core_rv32_ror32(uint32_t value,uint32_t shamt)
{
    shamt&=(sizeof(value)*8-1);
    if(shamt==0)
    {
        return value;
    }
    return (value>>shamt)|(value<<((sizeof(value)*8)-shamt));
}

/*
 * Zbc扩展辅助函数, mode:0=clmul,1=clmulh,2=clmulr
 */
static inline uint32_t hs_risc_v_core_rv32_clmul32(uint32_t a,uint32_t b,uint32_t mode)
{
    uint32_t result=0;
    if(mode==0)
    {
        for(uint32_t i=0; i<32; i++)
        {
            if(((b>>i)&1)!=0)
            {
                result^=a<<i;
            }
        }
    }
    else if(mode==1)
    {
        for(uint32_t i=1; i<32; i++)
        {
            if(((b>>i)&1)!=0)
            {
                result^=a>>(32-i);
            }
        }
    }
    else
    {
        for(uint32_t i=0; i<32; i++)
        {
            if(((b>>i)&1)!=0)
            {
                result^=a>>(31-i);
            }
        }
    }
    return result;
}

/*
 * Zbkb/Zbkx扩展辅助函数
 */
static inline uint32_t hs_risc_v_core_rv32_brev8_32(uint32_t value)
{
    uint32_t result=0;
    for(uint32_t i=0; i<4; i++)
    {
        uint8_t byte=(value>>(i*8))&0xFF;
        uint8_t reversed=0;
        for(uint32_t j=0; j<8; j++)
        {
            if((byte&(1<<j))!=0)
            {
                reversed|=1<<(7-j);
            }
        }
        result|=(uint32_t)reversed<<(i*8);
    }
    return result;
}

static inline uint32_t hs_risc_v_core_rv32_xperm32(uint32_t rs1,uint32_t rs2,uint32_t sz_log2)
{
    uint32_t result=0;
    uint32_t sz=1UL<<sz_log2;
    uint32_t mask=(1UL<<sz)-1;
    for(uint32_t i=0; i<32; i+=sz)
    {
        uint32_t pos=((rs2>>i)&mask)<<sz_log2;
        if(pos<32)
        {
            result|=((rs1>>pos)&mask)<<i;
        }
    }
    return result;
}

/*
 * Zbkb的zip/unzip在RV32上等价于SHAMT=xlen/2-1的shfli/unshfli
 */
static inline uint32_t hs_risc_v_core_rv32_zip(uint32_t value)
{
    uint32_t x=value;
    if((15&8)!=0)
    {
        x=(x&0xFF0000FF)|((x&0x00FF0000)>>8)|((x&0x0000FF00)<<8);
    }
    if((15&4)!=0)
    {
        x=(x&0xF00FF00F)|((x&0x0F000F00)>>4)|((x&0x00F000F0)<<4);
    }
    if((15&2)!=0)
    {
        x=(x&0xC3C3C3C3)|((x&0x30303030)>>2)|((x&0x0C0C0C0C)<<2);
    }
    if((15&1)!=0)
    {
        x=(x&0x99999999)|((x&0x44444444)>>1)|((x&0x22222222)<<1);
    }
    return x;
}

static inline uint32_t hs_risc_v_core_rv32_unzip(uint32_t value)
{
    uint32_t x=value;
    if((15&1)!=0)
    {
        x=(x&0x99999999)|((x&0x44444444)>>1)|((x&0x22222222)<<1);
    }
    if((15&2)!=0)
    {
        x=(x&0xC3C3C3C3)|((x&0x30303030)>>2)|((x&0x0C0C0C0C)<<2);
    }
    if((15&4)!=0)
    {
        x=(x&0xF00FF00F)|((x&0x0F000F00)>>4)|((x&0x00F000F0)<<4);
    }
    if((15&8)!=0)
    {
        x=(x&0xFF0000FF)|((x&0x00FF0000)>>8)|((x&0x0000FF00)<<8);
    }
    return x;
}

/** \brief 跳转到目标地址，必要时产生取指地址非对齐异常
 *
 * \param core 内核指针
 * \param target 目标地址
 * \param pc 当前指令地址(产生异常时的EPC)
 * \param instruction 当前指令
 *
 */
static inline void hs_risc_v_core_rv32_jump_to(hs_risc_v_core_rv32_t *core,uint32_t target,uint32_t pc,uint32_t instruction)
{
    uint32_t align=hs_risc_v_core_rv32_ialign(core);
    if((target&(align-1))!=0)
    {
        int cause=HS_RISC_V_OPCODES_EXCEPTION_NAME(CAUSE_MISALIGNED_FETCH);
        hs_risc_v_core_rv32_exec_exception_raise_value(core,cause,pc,instruction,target);
    }
    else
    {
        hs_risc_v_core_rv32_pc_write(core,target);
    }
}

static inline  void hs_risc_v_core_rv32_exec(hs_risc_v_core_rv32_t * core)
{
    if(core==NULL || core->io==NULL)
    {
        return;
    }

    /*
     * 处理WFI指令
     */
    if(core->flags.wfi_enable==1)
    {
        /*
         * 系统睡眠中
         */
        if(core->interrupt_pending!=0 || core->exception_pending!=0)
        {
            core->flags.wfi_enable=0;
        }
        return;
    }

    core->flags.abort=0;

    uint32_t pc=hs_risc_v_core_rv32_pc_read(core);
    uint32_t instruction=0;
    uint32_t next_pc=pc;
    bool     is_instruction_xret=false;         /**<指令是否是XRET，当其为XRET时跳过本次指令结束的异常处理*/

    //指令是否正确处理，当指令被正确处理时，需要将此标志置位true
    bool is_instruction_processed=false;

    if(!hs_risc_v_core_rv32_instruction_read(core,&instruction))
    {
        /*
         * 取指失败。若未由MMU产生页错误，则产生取指访问错误。
         */
        if(core->exception_pending==0)
        {
            int cause=HS_RISC_V_OPCODES_EXCEPTION_NAME(CAUSE_FETCH_ACCESS);
            hs_risc_v_core_rv32_exec_exception_raise_value(core,cause,pc,0,pc);
        }
        goto exec_exception_phase;
    }

    core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_INSTRUCTION_ENTER | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_HW,pc,(uint8_t*)&instruction,sizeof(instruction),core->usr);
    next_pc=pc+hs_risc_v_common_instruction_length(instruction);
    hs_risc_v_core_rv32_pc_write(core,next_pc);


    if((0x3&instruction)==0x3)
    {
        //RV32I/RV32E基本指令集

        /*
         *  使用低7位opcode初步分类
         */
        switch(0x7F&instruction)
        {
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_LOAD :
        {
            uint32_t rd=((instruction&INSN_FIELD_RD)>>7);
            uint32_t rs1=((instruction&INSN_FIELD_RS1)>>15);
            uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1);
            uint32_t i_imm=(((instruction >> 20)&((1ULL << (12))-1)) << (0))+(((instruction&(1ULL<<(31)))!=0)?(0xFFFFF000):0);
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(lw,
            {
                hs_risc_v_common_memory_word_t value;
                value.value=0;
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_READ,rs1_value+((int32_t)i_imm),value.bytes,sizeof(value.bytes));
                HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                hs_risc_v_core_rv32_x_register_write(core,rd,value.value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(lh,
            {
                hs_risc_v_common_memory_halfword_t value;
                value.value=0;
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_READ,rs1_value+((int32_t)i_imm),value.bytes,sizeof(value.bytes));
                HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                hs_risc_v_core_rv32_x_register_write(core,rd,(int32_t)value.s_value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(lb,
            {
                hs_risc_v_common_memory_byte_t value;
                value.value=0;
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_READ,rs1_value+((int32_t)i_imm),value.bytes,sizeof(value.bytes));
                //HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                hs_risc_v_core_rv32_x_register_write(core,rd,(int32_t)value.s_value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(lhu,
            {
                hs_risc_v_common_memory_halfword_t value;
                value.value=0;
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_READ,rs1_value+((int32_t)i_imm),value.bytes,sizeof(value.bytes));
                HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                hs_risc_v_core_rv32_x_register_write(core,rd,(uint32_t)value.value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(lbu,
            {
                hs_risc_v_common_memory_byte_t value;
                value.value=0;
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_READ,rs1_value+((int32_t)i_imm),value.bytes,sizeof(value.bytes));
                //HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                hs_risc_v_core_rv32_x_register_write(core,rd,(uint32_t)value.value);
            });
        }
        break;
        case  HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_STORE :
        {
            uint32_t rs1=((instruction&INSN_FIELD_RS1)>>15);
            uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1);
            uint32_t rs2=((instruction&INSN_FIELD_RS2)>>20);
            uint32_t rs2_value=hs_risc_v_core_rv32_x_register_read(core,rs2);
            uint32_t imm=(((instruction >> 7)&((1ULL << (5))-1)) << (0))+(((instruction >> 25)&((1ULL << (7))-1)) << (5))+(((instruction&(1ULL<<(31)))!=0)?(0xFFFFF000):0);
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sw,
            {
                hs_risc_v_common_memory_word_t value;
                value.value=rs2_value;
                HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_WRITE,rs1_value+((int32_t)imm),value.bytes,sizeof(value.bytes));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sh,
            {
                hs_risc_v_common_memory_halfword_t value;
                value.value=rs2_value;
                HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_WRITE,rs1_value+((int32_t)imm),value.bytes,sizeof(value.bytes));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sb,
            {
                hs_risc_v_common_memory_byte_t value;
                value.value=rs2_value;
                //HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_WRITE,rs1_value+((int32_t)imm),value.bytes,sizeof(value.bytes));
            });
        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_MADD:
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_BRANCH :
        {
            uint32_t rs1=((instruction&INSN_FIELD_RS1)>>15);
            uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1);
            uint32_t rs2=((instruction&INSN_FIELD_RS2)>>20);
            uint32_t rs2_value=hs_risc_v_core_rv32_x_register_read(core,rs2);
            uint32_t sb_imm=(((instruction >> 8)&((1ULL << (4))-1)) << (1))+(((instruction >> 25)&((1ULL << (6))-1)) << (5))+(((instruction >> 7)&((1ULL << (1))-1)) << (11))+(((instruction&(1ULL<<(31)))!=0)?(0xFFFFF000):0);
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(beq,
            {
                if(rs1_value==rs2_value)
                {
                    hs_risc_v_core_rv32_jump_to(core,pc+(int32_t)sb_imm,pc,instruction);
                }
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(bne,
            {
                if(rs1_value!=rs2_value)
                {
                    hs_risc_v_core_rv32_jump_to(core,pc+(int32_t)sb_imm,pc,instruction);
                }
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(blt,
            {
                if(((int32_t)rs1_value)<((int32_t)rs2_value))
                {
                    hs_risc_v_core_rv32_jump_to(core,pc+(int32_t)sb_imm,pc,instruction);
                }
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(bltu,
            {
                if(((uint32_t)rs1_value)<((uint32_t)rs2_value))
                {
                    hs_risc_v_core_rv32_jump_to(core,pc+(int32_t)sb_imm,pc,instruction);
                }
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(bge,
            {
                if(((int32_t)rs1_value)>=((int32_t)rs2_value))
                {
                    hs_risc_v_core_rv32_jump_to(core,pc+(int32_t)sb_imm,pc,instruction);
                }
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(bgeu,
            {
                if(((uint32_t)rs1_value)>=((uint32_t)rs2_value))
                {
                    hs_risc_v_core_rv32_jump_to(core,pc+(int32_t)sb_imm,pc,instruction);
                }
            });
        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_LOAD_FP:
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_STORE_FP:
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_MSUB :
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_JALR :
        {
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(jalr,
            {
                uint32_t rd=((instruction&INSN_FIELD_RD)>>7);
                uint32_t rs1=((instruction&INSN_FIELD_RS1)>>15);
                uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1);
                uint32_t i_imm=(((instruction >> 20)&((1ULL << (12))-1)) << (0))+(((instruction&(1ULL<<(31)))!=0)?(0xFFFFF000):0);
                uint32_t target=(rs1_value+(int32_t)i_imm)&(~1UL);
                hs_risc_v_core_rv32_jump_to(core,target,pc,instruction);
                hs_risc_v_core_rv32_x_register_write(core,rd,next_pc);
            })
        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_CUSTOM_0:
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_CUSTOM_1:
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_NMSUB :
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_RESERVED :
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_MISC_MEM:
        {
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fence,
            {
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_INSTRUCTION_FENCE_EXEC,pc,(uint8_t*)&instruction,sizeof(instruction));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(pause,
            {
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_INSTRUCTION_PAUSE_EXEC,pc,(uint8_t*)&instruction,sizeof(instruction));
            });
        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_AMO :
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_NMADD:
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_JAL:
        {
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(jal,
            {
                uint32_t rd=((instruction&INSN_FIELD_RD)>>7);
                uint32_t ju_imm=(((instruction >> 21)&((1ULL << (10))-1)) << (1))+(((instruction >> 20)&((1ULL << (1))-1)) << (11))+(((instruction >> 12)&((1ULL << (8))-1)) << (12))+(((instruction&(1ULL<<(31)))!=0)?(0xFFF00000):0);
                hs_risc_v_core_rv32_jump_to(core,pc+(int32_t)ju_imm,pc,instruction);
                hs_risc_v_core_rv32_x_register_write(core,rd,next_pc);
            })
        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_OP_IMM :
        {
            uint32_t rd=((instruction&INSN_FIELD_RD)>>7);
            uint32_t rs1=((instruction&INSN_FIELD_RS1)>>15);
            uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1);
            uint32_t i_imm=(((instruction >> 20)&((1ULL << (12))-1)) << (0))+(((instruction&(1ULL<<(31)))!=0)?(0xFFFFF000):0);
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(addi,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,((int32_t)rs1_value)+((int32_t)i_imm));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(slti,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,(((int32_t)rs1_value)<((int32_t)i_imm))?1:0);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sltiu,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,(((uint32_t)rs1_value)<((uint32_t)i_imm))?1:0);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(xori,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value^i_imm);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(ori,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value|i_imm);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(andi,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value&i_imm);
            });
            {
                uint32_t shamt=((instruction>>20)&0x1F);
                uint32_t funct3=((instruction>>12)&0x7);
                if((funct3==1 || funct3==5) && (instruction&(1UL<<25))!=0)
                {
                    /*
                     * RV32中shamt[5]必须为0
                     */
                    int cause=HS_RISC_V_OPCODES_EXCEPTION_NAME(CAUSE_ILLEGAL_INSTRUCTION);
                    hs_risc_v_core_rv32_exec_exception_raise(core,cause,pc,instruction);
                    is_instruction_processed=true;
                }
                else
                {
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(slli,
                    {
                        hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value<<shamt);
                    });
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(srli,
                    {
                        hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value>>shamt);
                    });
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(srai,
                    {
                        hs_risc_v_core_rv32_x_register_write(core,rd,((int64_t)((int32_t)rs1_value))>>shamt);
                    });
                }
            }
        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_OP  :
        {
            uint32_t rd=((instruction&INSN_FIELD_RD)>>7);
            uint32_t rs1=((instruction&INSN_FIELD_RS1)>>15);
            uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1);
            uint32_t rs2=((instruction&INSN_FIELD_RS2)>>20);
            uint32_t rs2_value=hs_risc_v_core_rv32_x_register_read(core,rs2);
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(add,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value+rs2_value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sub,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value-rs2_value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sll,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value<<(rs2_value&((sizeof(rs1_value)*8)-1)));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(slt,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,(((int32_t)rs1_value)<((int32_t)rs2_value))?1:0);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sltu,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,(((uint32_t)rs1_value)<((uint32_t)rs2_value))?1:0);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(xor,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value^rs2_value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(srl,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value>>(rs2_value&((sizeof(rs1_value)*8)-1)));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sra,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,((int64_t)((int32_t)rs1_value))>>(rs2_value&((sizeof(rs1_value)*8)-1)));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(or,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value|rs2_value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(and,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value&rs2_value);
            });
        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_OP_FP:
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_SYSTEM:
        {
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(ecall,
            {
                int cause=HS_RISC_V_OPCODES_EXCEPTION_NAME(CAUSE_MACHINE_ECALL);
                uint32_t priv=hs_risc_v_core_rv32_privilege_get(core);
                if(priv==HS_RISC_V_CORE_RV32_PRIVILEGE_U)
                {
                    cause=HS_RISC_V_OPCODES_EXCEPTION_NAME(CAUSE_USER_ECALL);
                }
                else if(priv==HS_RISC_V_CORE_RV32_PRIVILEGE_S)
                {
                    cause=HS_RISC_V_OPCODES_EXCEPTION_NAME(CAUSE_SUPERVISOR_ECALL);
                }
                hs_risc_v_core_rv32_exec_exception_raise(core,cause,pc,instruction);
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_INSTRUCTION_ECALL_EXEC,pc,(uint8_t*)&instruction,sizeof(instruction));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(ebreak,
            {
                int cause=HS_RISC_V_OPCODES_EXCEPTION_NAME(CAUSE_BREAKPOINT);
                hs_risc_v_core_rv32_exec_exception_raise_value(core,cause,pc,instruction,0);
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_INSTRUCTION_EBREAK_EXEC,pc,(uint8_t*)&instruction,sizeof(instruction));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(mret,
            {
                if(hs_risc_v_core_rv32_privilege_get(core)!=HS_RISC_V_CORE_RV32_PRIVILEGE_M)
                {
                    int cause=HS_RISC_V_OPCODES_EXCEPTION_NAME(CAUSE_ILLEGAL_INSTRUCTION);
                    hs_risc_v_core_rv32_exec_exception_raise(core,cause,pc,instruction);
                }
                else
                {
                    hs_risc_v_core_rv32_exec_exception_mret(core);
                    is_instruction_xret=true;
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_INSTRUCTION_MRET_EXEC,pc,(uint8_t*)&instruction,sizeof(instruction));
                }
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sret,
            {
                uint32_t mstatus=hs_risc_v_core_rv32_csr_read(core,CSR_MSTATUS);
                if(hs_risc_v_core_rv32_privilege_get(core)<HS_RISC_V_CORE_RV32_PRIVILEGE_S ||
                   (hs_risc_v_core_rv32_privilege_get(core)==HS_RISC_V_CORE_RV32_PRIVILEGE_S && (mstatus&MSTATUS_TSR)!=0))
                {
                    int cause=HS_RISC_V_OPCODES_EXCEPTION_NAME(CAUSE_ILLEGAL_INSTRUCTION);
                    hs_risc_v_core_rv32_exec_exception_raise(core,cause,pc,instruction);
                }
                else
                {
                    hs_risc_v_core_rv32_exec_exception_sret(core);
                    is_instruction_xret=true;
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_INSTRUCTION_SRET_EXEC,pc,(uint8_t*)&instruction,sizeof(instruction));
                }
            });
            if(instruction==0x00200073)
            {
                /*
                 * URET(N扩展),此处直接匹配编码
                 */
                if(!is_instruction_processed)
                {
                    is_instruction_processed=true;
                    hs_risc_v_core_rv32_exec_exception_uret(core);
                    is_instruction_xret=true;
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_INSTRUCTION_URET_EXEC,pc,(uint8_t*)&instruction,sizeof(instruction));
                }
            }
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sfence_vma,
            {
                uint32_t mstatus=hs_risc_v_core_rv32_csr_read(core,CSR_MSTATUS);
                if(hs_risc_v_core_rv32_privilege_get(core)<HS_RISC_V_CORE_RV32_PRIVILEGE_S ||
                   (hs_risc_v_core_rv32_privilege_get(core)==HS_RISC_V_CORE_RV32_PRIVILEGE_S && (mstatus&MSTATUS_TVM)!=0))
                {
                    int cause=HS_RISC_V_OPCODES_EXCEPTION_NAME(CAUSE_ILLEGAL_INSTRUCTION);
                    hs_risc_v_core_rv32_exec_exception_raise(core,cause,pc,instruction);
                }
                else
                {
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_INSTRUCTION_SFENCE_VMA_EXEC,pc,(uint8_t*)&instruction,sizeof(instruction));
                }
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(wfi,
            {
                uint32_t mstatus=hs_risc_v_core_rv32_csr_read(core,CSR_MSTATUS);
                if(hs_risc_v_core_rv32_privilege_get(core)<HS_RISC_V_CORE_RV32_PRIVILEGE_S && (mstatus&MSTATUS_TW)!=0)
                {
                    int cause=HS_RISC_V_OPCODES_EXCEPTION_NAME(CAUSE_ILLEGAL_INSTRUCTION);
                    hs_risc_v_core_rv32_exec_exception_raise(core,cause,pc,instruction);
                }
                else
                {
                    core->flags.wfi_enable=1;
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_INSTRUCTION_WFI_EXEC,pc,(uint8_t*)&instruction,sizeof(instruction));
                }
            });
        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_AUIPC:
        {
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(auipc,
            {
                uint32_t u_imm=(instruction&INSN_FIELD_IMM20);
                uint32_t rd=((instruction&INSN_FIELD_RD)>>7);
                hs_risc_v_core_rv32_x_register_write(core,rd,u_imm+pc);
            })
        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_LUI:
        {
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(lui,
            {
                uint32_t u_imm=(instruction&INSN_FIELD_IMM20);
                uint32_t rd=((instruction&INSN_FIELD_RD)>>7);
                hs_risc_v_core_rv32_x_register_write(core,rd,u_imm);
            })
        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_OP_V:
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_OP_VE :
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_OP_IMM_32:
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_OP_32 :
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_CUSTOM_3 :
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_CUSTOM_4:
        {

        }
        break;
        default:
        {

        }
        break;
        }
    }
    else
    {
        //RVC扩展指令集
        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32C))
        {
            uint32_t insn=instruction&0xFFFF;
            uint32_t rd=((insn>>7)&0x1F);
            uint32_t rs2=((insn>>2)&0x1F);
            uint32_t rd_p=(((insn>>2)&0x7)+8);
            uint32_t rs1_p=(((insn>>7)&0x7)+8);
            uint32_t rs2_p=(((insn>>2)&0x7)+8);
            uint32_t sp_value=hs_risc_v_core_rv32_x_register_read(core,2);
            /*
             * 压缩浮点指令同样要求mstatus.FS!=0
             */
            if((((insn&0x3)==0)||((insn&0x3)==2)) && ((((insn>>13)&0x7)&0x1)!=0) &&
               !hs_risc_v_core_rv32_fp_enabled(core))
            {
                int cause=HS_RISC_V_OPCODES_EXCEPTION_NAME(CAUSE_ILLEGAL_INSTRUCTION);
                hs_risc_v_core_rv32_exec_exception_raise(core,cause,pc,instruction);
                is_instruction_processed=true;
            }
            /*
             * Zcb扩展压缩指令
             */
            if(!is_instruction_processed &&
               hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZCB))
            {
                if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZBB))
                {
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(c_zext_b,
                    {
                        hs_risc_v_core_rv32_x_register_write(core,rs1_p,hs_risc_v_core_rv32_x_register_read(core,rs1_p)&0xFF);
                    });
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(c_sext_b,
                    {
                        hs_risc_v_core_rv32_x_register_write(core,rs1_p,(int32_t)(int8_t)hs_risc_v_core_rv32_x_register_read(core,rs1_p));
                    });
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(c_zext_h,
                    {
                        hs_risc_v_core_rv32_x_register_write(core,rs1_p,hs_risc_v_core_rv32_x_register_read(core,rs1_p)&0xFFFF);
                    });
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(c_sext_h,
                    {
                        hs_risc_v_core_rv32_x_register_write(core,rs1_p,(int32_t)(int16_t)hs_risc_v_core_rv32_x_register_read(core,rs1_p));
                    });
                }
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(c_not,
                {
                    hs_risc_v_core_rv32_x_register_write(core,rs1_p,~(hs_risc_v_core_rv32_x_register_read(core,rs1_p)));
                });
                if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32M))
                {
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(c_mul,
                    {
                        uint32_t rs1_v=hs_risc_v_core_rv32_x_register_read(core,rs1_p);
                        uint32_t rs2_v=hs_risc_v_core_rv32_x_register_read(core,rs2_p);
                        hs_risc_v_core_rv32_x_register_write(core,rs1_p,rs1_v*rs2_v);
                    });
                }
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(c_lbu,
                {
                    uint32_t offset=(((instruction>>5)&0x1)<<1)|((instruction>>6)&0x1);
                    uint32_t base=hs_risc_v_core_rv32_x_register_read(core,rs1_p);
                    hs_risc_v_common_memory_byte_t value;
                    value.value=0;
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_READ,base+offset,value.bytes,sizeof(value.bytes));
                    hs_risc_v_core_rv32_x_register_write(core,rd_p,(uint32_t)value.value);
                });
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(c_lhu,
                {
                    uint32_t offset=(((instruction>>5)&0x1)<<1);
                    uint32_t base=hs_risc_v_core_rv32_x_register_read(core,rs1_p);
                    hs_risc_v_common_memory_halfword_t value;
                    value.value=0;
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_READ,base+offset,value.bytes,sizeof(value.bytes));
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_x_register_write(core,rd_p,(uint32_t)value.value);
                });
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(c_lh,
                {
                    uint32_t offset=(((instruction>>5)&0x1)<<1);
                    uint32_t base=hs_risc_v_core_rv32_x_register_read(core,rs1_p);
                    hs_risc_v_common_memory_halfword_t value;
                    value.value=0;
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_READ,base+offset,value.bytes,sizeof(value.bytes));
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_x_register_write(core,rd_p,(int32_t)value.s_value);
                });
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(c_sb,
                {
                    uint32_t offset=(((instruction>>5)&0x1)<<1)|((instruction>>6)&0x1);
                    uint32_t base=hs_risc_v_core_rv32_x_register_read(core,rs1_p);
                    hs_risc_v_common_memory_byte_t value;
                    value.value=hs_risc_v_core_rv32_x_register_read(core,rs2_p);
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_WRITE,base+offset,value.bytes,sizeof(value.bytes));
                });
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(c_sh,
                {
                    uint32_t offset=(((instruction>>5)&0x1)<<1);
                    uint32_t base=hs_risc_v_core_rv32_x_register_read(core,rs1_p);
                    hs_risc_v_common_memory_halfword_t value;
                    value.value=hs_risc_v_core_rv32_x_register_read(core,rs2_p);
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_WRITE,base+offset,value.bytes,sizeof(value.bytes));
                });
            }
            /*
             *  使用象限初步分类
             */
            if(!is_instruction_processed)
            {
                switch(0x3&insn)
                {
                case 0:                     //象限0
                {
                    uint32_t funct3=((insn>>13)&0x7);
                    uint32_t base_value=hs_risc_v_core_rv32_x_register_read(core,rs1_p);
                    switch(funct3)
                    {
                    case 0:             //c.addi4spn
                    {
                        uint32_t nzuimm=(((insn>>7)&0xF)<<6)|(((insn>>11)&0x3)<<4)|(((insn>>5)&0x1)<<3)|(((insn>>6)&0x1)<<2);
                        if(nzuimm!=0)
                        {
                            hs_risc_v_core_rv32_x_register_write(core,rd_p,sp_value+nzuimm);
                            is_instruction_processed=true;
                        }
                    }
                    break;
                    case 1:             //c.fld
                    {
                        if(hs_risc_v_core_rv32_has_d(core))
                        {
                            uint32_t offset=(((insn>>10)&0x7)<<3)|(((insn>>5)&0x3)<<6);
                            hs_risc_v_common_memory_doubleword_t value;
                            value.value=0;
                            hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_READ,base_value+offset,value.bytes,sizeof(value.bytes));
                            HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                            hs_risc_v_core_rv32_fp_write_d(core,rd_p,value.value);
                            hs_risc_v_core_rv32_fp_dirty(core);
                            is_instruction_processed=true;
                        }
                    }
                    break;
                    case 2:             //c.lw
                    {
                        uint32_t offset=(((insn>>10)&0x7)<<3)|(((insn>>6)&0x1)<<2)|(((insn>>5)&0x1)<<6);
                        hs_risc_v_common_memory_word_t value;
                        value.value=0;
                        hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_READ,base_value+offset,value.bytes,sizeof(value.bytes));
                        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                        hs_risc_v_core_rv32_x_register_write(core,rd_p,value.value);
                        is_instruction_processed=true;
                    }
                    break;
                    case 3:             //c.flw
                    {
                        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32F))
                        {
                            uint32_t offset=(((insn>>10)&0x7)<<3)|(((insn>>6)&0x1)<<2)|(((insn>>5)&0x1)<<6);
                            hs_risc_v_common_memory_word_t value;
                            value.value=0;
                            hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_READ,base_value+offset,value.bytes,sizeof(value.bytes));
                            HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                            hs_risc_v_core_rv32_fp_write_s(core,rd_p,value.value);
                            hs_risc_v_core_rv32_fp_dirty(core);
                            is_instruction_processed=true;
                        }
                    }
                    break;
                    case 5:             //c.fsd
                    {
                        if(hs_risc_v_core_rv32_has_d(core))
                        {
                            uint32_t offset=(((insn>>10)&0x7)<<3)|(((insn>>5)&0x3)<<6);
                            hs_risc_v_common_memory_doubleword_t value;
                            value.value=hs_risc_v_core_rv32_fp_read_d(core,rs2_p);
                            HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                            hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_WRITE,base_value+offset,value.bytes,sizeof(value.bytes));
                            is_instruction_processed=true;
                        }
                    }
                    break;
                    case 6:             //c.sw
                    {
                        uint32_t offset=(((insn>>10)&0x7)<<3)|(((insn>>6)&0x1)<<2)|(((insn>>5)&0x1)<<6);
                        hs_risc_v_common_memory_word_t value;
                        value.value=hs_risc_v_core_rv32_x_register_read(core,rs2_p);
                        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                        hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_WRITE,base_value+offset,value.bytes,sizeof(value.bytes));
                        is_instruction_processed=true;
                    }
                    break;
                    case 7:             //c.fsw
                    {
                        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32F))
                        {
                            uint32_t offset=(((insn>>10)&0x7)<<3)|(((insn>>6)&0x1)<<2)|(((insn>>5)&0x1)<<6);
                            hs_risc_v_common_memory_word_t value;
                            value.value=hs_risc_v_core_rv32_fp_read_s_raw(core,rs2_p);
                            HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                            hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_WRITE,base_value+offset,value.bytes,sizeof(value.bytes));
                            is_instruction_processed=true;
                        }
                    }
                    break;
                    default:
                    {
                    }
                    break;
                    }
                }
                break;
                case 1:                     //象限1
                {
                    uint32_t funct3=((insn>>13)&0x7);
                    switch(funct3)
                    {
                    case 0:             //c.addi
                    {
                        int32_t imm=hs_risc_v_core_rv32_sign_extend((((insn>>12)&0x1)<<5)|((insn>>2)&0x1F),6);
                        hs_risc_v_core_rv32_x_register_write(core,rd,hs_risc_v_core_rv32_x_register_read(core,rd)+imm);
                        is_instruction_processed=true;
                    }
                    break;
                    case 1:             //c.jal (RV32)
                    {
                        int32_t imm=hs_risc_v_core_rv32_sign_extend((((insn>>12)&0x1)<<11)|(((insn>>11)&0x1)<<4)|(((insn>>9)&0x3)<<8)|(((insn>>8)&0x1)<<10)|(((insn>>7)&0x1)<<6)|(((insn>>6)&0x1)<<7)|(((insn>>3)&0x7)<<1)|(((insn>>2)&0x1)<<5),12);
                        hs_risc_v_core_rv32_x_register_write(core,1,next_pc);
                        hs_risc_v_core_rv32_jump_to(core,pc+imm,pc,instruction);
                        is_instruction_processed=true;
                    }
                    break;
                    case 2:             //c.li
                    {
                        if(rd!=0)
                        {
                            int32_t imm=hs_risc_v_core_rv32_sign_extend((((insn>>12)&0x1)<<5)|((insn>>2)&0x1F),6);
                            hs_risc_v_core_rv32_x_register_write(core,rd,imm);
                        }
                        is_instruction_processed=true;
                    }
                    break;
                    case 3:             //c.addi16sp/c.lui
                    {
                        if(rd==2)
                        {
                            uint32_t nzimm=(((insn>>12)&0x1)<<9)|(((insn>>6)&0x1)<<4)|(((insn>>5)&0x1)<<6)|(((insn>>3)&0x3)<<7)|(((insn>>2)&0x1)<<5);
                            if(nzimm!=0)
                            {
                                int32_t imm=hs_risc_v_core_rv32_sign_extend(nzimm,10);
                                hs_risc_v_core_rv32_x_register_write(core,2,sp_value+imm);
                            }
                            is_instruction_processed=true;
                        }
                        else if(rd!=0)
                        {
                            uint32_t imm6=(((insn>>12)&0x1)<<5)|((insn>>2)&0x1F);
                            if(imm6!=0)
                            {
                                int32_t imm=hs_risc_v_core_rv32_sign_extend(imm6<<12,18);
                                hs_risc_v_core_rv32_x_register_write(core,rd,imm);
                            }
                            is_instruction_processed=true;
                        }
                    }
                    break;
                    case 4:             //c.srli/c.srai/c.andi/c.sub/c.xor/c.or/c.and
                    {
                        uint32_t funct2=((insn>>10)&0x3);
                        if(funct2==0 || funct2==1)
                        {
                            /*
                             * RV32中shamt[5]必须为0
                             */
                            if(((insn>>12)&0x1)==0)
                            {
                                uint32_t shamt=((insn>>2)&0x1F);
                                uint32_t value=hs_risc_v_core_rv32_x_register_read(core,rs1_p);
                                if(funct2==0)
                                {
                                    hs_risc_v_core_rv32_x_register_write(core,rs1_p,value>>shamt);
                                }
                                else
                                {
                                    hs_risc_v_core_rv32_x_register_write(core,rs1_p,((int64_t)((int32_t)value))>>shamt);
                                }
                                is_instruction_processed=true;
                            }
                        }
                        else if(funct2==2)
                        {
                            int32_t imm=hs_risc_v_core_rv32_sign_extend((((insn>>12)&0x1)<<5)|((insn>>2)&0x1F),6);
                            hs_risc_v_core_rv32_x_register_write(core,rs1_p,hs_risc_v_core_rv32_x_register_read(core,rs1_p)&imm);
                            is_instruction_processed=true;
                        }
                        else
                        {
                            if(((insn>>12)&0x1)==0)
                            {
                                uint32_t rs2_value=hs_risc_v_core_rv32_x_register_read(core,rs2_p);
                                uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1_p);
                                switch((insn>>5)&0x3)
                                {
                                case 0:         //c.sub
                                {
                                    hs_risc_v_core_rv32_x_register_write(core,rs1_p,rs1_value-rs2_value);
                                }
                                break;
                                case 1:         //c.xor
                                {
                                    hs_risc_v_core_rv32_x_register_write(core,rs1_p,rs1_value^rs2_value);
                                }
                                break;
                                case 2:         //c.or
                                {
                                    hs_risc_v_core_rv32_x_register_write(core,rs1_p,rs1_value|rs2_value);
                                }
                                break;
                                default:        //c.and
                                {
                                    hs_risc_v_core_rv32_x_register_write(core,rs1_p,rs1_value&rs2_value);
                                }
                                break;
                                }
                                is_instruction_processed=true;
                            }
                        }
                    }
                    break;
                    case 5:             //c.j
                    {
                        int32_t imm=hs_risc_v_core_rv32_sign_extend((((insn>>12)&0x1)<<11)|(((insn>>11)&0x1)<<4)|(((insn>>9)&0x3)<<8)|(((insn>>8)&0x1)<<10)|(((insn>>7)&0x1)<<6)|(((insn>>6)&0x1)<<7)|(((insn>>3)&0x7)<<1)|(((insn>>2)&0x1)<<5),12);
                        hs_risc_v_core_rv32_jump_to(core,pc+imm,pc,instruction);
                        is_instruction_processed=true;
                    }
                    break;
                    case 6:             //c.beqz
                    case 7:             //c.bnez
                    {
                        int32_t imm=hs_risc_v_core_rv32_sign_extend((((insn>>12)&0x1)<<8)|(((insn>>10)&0x3)<<3)|(((insn>>5)&0x3)<<6)|(((insn>>3)&0x3)<<1)|(((insn>>2)&0x1)<<5),9);
                        uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1_p);
                        bool taken=(funct3==6)?(rs1_value==0):(rs1_value!=0);
                        if(taken)
                        {
                            hs_risc_v_core_rv32_jump_to(core,pc+imm,pc,instruction);
                        }
                        is_instruction_processed=true;
                    }
                    break;
                    default:
                    {
                    }
                    break;
                    }
                }
                break;
                case 2:                     //象限2
                {
                    uint32_t funct3=((insn>>13)&0x7);
                    switch(funct3)
                    {
                    case 0:             //c.slli
                    {
                        if(((insn>>12)&0x1)==0)
                        {
                            uint32_t shamt=((insn>>2)&0x1F);
                            hs_risc_v_core_rv32_x_register_write(core,rd,hs_risc_v_core_rv32_x_register_read(core,rd)<<shamt);
                            is_instruction_processed=true;
                        }
                    }
                    break;
                    case 1:             //c.fldsp
                    {
                        if(hs_risc_v_core_rv32_has_d(core))
                        {
                            uint32_t offset=(((insn>>12)&0x1)<<5)|(((insn>>5)&0x3)<<3)|(((insn>>2)&0x7)<<6);
                            hs_risc_v_common_memory_doubleword_t value;
                            value.value=0;
                            hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_READ,sp_value+offset,value.bytes,sizeof(value.bytes));
                            HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                            hs_risc_v_core_rv32_fp_write_d(core,rd,value.value);
                            hs_risc_v_core_rv32_fp_dirty(core);
                            is_instruction_processed=true;
                        }
                    }
                    break;
                    case 2:             //c.lwsp
                    {
                        if(rd!=0)
                        {
                            uint32_t offset=(((insn>>12)&0x1)<<5)|(((insn>>4)&0x7)<<2)|(((insn>>2)&0x3)<<6);
                            hs_risc_v_common_memory_word_t value;
                            value.value=0;
                            hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_READ,sp_value+offset,value.bytes,sizeof(value.bytes));
                            HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                            hs_risc_v_core_rv32_x_register_write(core,rd,value.value);
                            is_instruction_processed=true;
                        }
                    }
                    break;
                    case 3:             //c.flwsp
                    {
                        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32F))
                        {
                            uint32_t offset=(((insn>>12)&0x1)<<5)|(((insn>>4)&0x7)<<2)|(((insn>>2)&0x3)<<6);
                            hs_risc_v_common_memory_word_t value;
                            value.value=0;
                            hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_READ,sp_value+offset,value.bytes,sizeof(value.bytes));
                            HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                            hs_risc_v_core_rv32_fp_write_s(core,rd,value.value);
                            hs_risc_v_core_rv32_fp_dirty(core);
                            is_instruction_processed=true;
                        }
                    }
                    break;
                    case 4:             //c.jr/c.mv/c.ebreak/c.jalr/c.add
                    {
                        if(((insn>>12)&0x1)==0)
                        {
                            if(rs2==0)
                            {
                                if(rd!=0)
                                {
                                    hs_risc_v_core_rv32_jump_to(core,hs_risc_v_core_rv32_x_register_read(core,rd)&(~1UL),pc,instruction);
                                    is_instruction_processed=true;
                                }
                            }
                            else
                            {
                                if(rd!=0)
                                {
                                    hs_risc_v_core_rv32_x_register_write(core,rd,hs_risc_v_core_rv32_x_register_read(core,rs2));
                                    is_instruction_processed=true;
                                }
                            }
                        }
                        else
                        {
                            if(rd==0 && rs2==0)
                            {
                                int cause=HS_RISC_V_OPCODES_EXCEPTION_NAME(CAUSE_BREAKPOINT);
                                hs_risc_v_core_rv32_exec_exception_raise_value(core,cause,pc,instruction,0);
                                is_instruction_processed=true;
                            }
                            else if(rs2==0)
                            {
                                if(rd!=0)
                                {
                                    uint32_t target=hs_risc_v_core_rv32_x_register_read(core,rd)&(~1UL);
                                    hs_risc_v_core_rv32_x_register_write(core,1,next_pc);
                                    hs_risc_v_core_rv32_jump_to(core,target,pc,instruction);
                                    is_instruction_processed=true;
                                }
                            }
                            else
                            {
                                if(rd!=0)
                                {
                                    hs_risc_v_core_rv32_x_register_write(core,rd,hs_risc_v_core_rv32_x_register_read(core,rd)+hs_risc_v_core_rv32_x_register_read(core,rs2));
                                    is_instruction_processed=true;
                                }
                            }
                        }
                    }
                    break;
                    case 5:             //c.fsdsp
                    {
                        if(hs_risc_v_core_rv32_has_d(core))
                        {
                            uint32_t offset=(((insn>>10)&0x7)<<3)|(((insn>>7)&0x7)<<6);
                            hs_risc_v_common_memory_doubleword_t value;
                            value.value=hs_risc_v_core_rv32_fp_read_d(core,rs2);
                            HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                            hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_WRITE,sp_value+offset,value.bytes,sizeof(value.bytes));
                            is_instruction_processed=true;
                        }
                    }
                    break;
                    case 6:             //c.swsp
                    {
                        uint32_t offset=(((insn>>9)&0xF)<<2)|(((insn>>7)&0x3)<<6);
                        hs_risc_v_common_memory_word_t value;
                        value.value=hs_risc_v_core_rv32_x_register_read(core,rs2);
                        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                        hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_WRITE,sp_value+offset,value.bytes,sizeof(value.bytes));
                        is_instruction_processed=true;
                    }
                    break;
                    case 7:             //c.fswsp
                    {
                        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32F))
                        {
                            uint32_t offset=(((insn>>9)&0xF)<<2)|(((insn>>7)&0x3)<<6);
                            hs_risc_v_common_memory_word_t value;
                            value.value=hs_risc_v_core_rv32_fp_read_s_raw(core,rs2);
                            HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                            hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_WRITE,sp_value+offset,value.bytes,sizeof(value.bytes));
                            is_instruction_processed=true;
                        }
                    }
                    break;
                    default:
                    {
                    }
                    break;
                    }
                }
                break;
                default:                    //象限3（32位指令与更高位数指令）,理论上不会出现这种情况。
                {

                }
                break;
                }
            }
        }
    }

    //Zicbom/Zicboz扩展指令集
    if(!is_instruction_processed)
    {
        uint32_t rs1=((instruction&INSN_FIELD_RS1)>>15);
        uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1);
        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZICBOM))
        {
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(cbo_clean,
            {
                (void)rs1_value;
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(cbo_flush,
            {
                (void)rs1_value;
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(cbo_inval,
            {
                (void)rs1_value;
            });
        }
        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZICBOZ))
        {
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(cbo_zero,
            {
                uint32_t base=rs1_value&(~0x3FUL);
                uint8_t zeros[64];
                memset(zeros,0,sizeof(zeros));
                for(uint32_t offset=0; offset<64; offset+=8)
                {
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_WRITE,base+offset,zeros+offset,8);
                }
            });
        }
    }

    //Zawrs扩展指令集
    if(!is_instruction_processed)
    {
        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(wrs_nto,
        {
            (void)core;
        });
        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(wrs_sto,
        {
            (void)core;
        });
    }


    //Svinval扩展指令集
    if(!is_instruction_processed &&
       hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32SVINVAL))
    {
        /*
         * 本模拟器无地址翻译缓存，细粒度失效指令仅需正确执行即可。
         */
        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sinval_vma,
        {
            (void)core;
        });
        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sfence_w_inval,
        {
            (void)core;
        });
        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sfence_inval_ir,
        {
            (void)core;
        });
    }

    //Zifencei扩展指令集
    if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZIFENCEI)&& !is_instruction_processed)
    {
        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fence_i,
        {
            hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_INSTRUCTION_FENCE_I_EXEC,pc,(uint8_t*)&instruction,sizeof(instruction));
        })
    }

    //Zicsr扩展指令集
    if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZICSR) && !is_instruction_processed)
    {
        if((instruction&0x7F)==HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_SYSTEM)
        {
            uint32_t rd=((instruction&INSN_FIELD_RD)>>7);
            uint32_t rs1=((instruction&INSN_FIELD_RS1)>>15);
            uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1);
            uint32_t uimm=rs1;
            uint32_t csr=((instruction&INSN_FIELD_CSR)>>20);
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(csrrw,
            {
                if(!hs_risc_v_core_rv32_csr_access_legal(core,csr,true))
                {
                    int cause=HS_RISC_V_OPCODES_EXCEPTION_NAME(CAUSE_ILLEGAL_INSTRUCTION);
                    hs_risc_v_core_rv32_exec_exception_raise(core,cause,pc,instruction);
                }
                else
                {
                    if(rd!=0)
                    {
                        uint32_t csr_value=hs_risc_v_core_rv32_csr_read(core,csr);
                        hs_risc_v_core_rv32_x_register_write(core,rd,csr_value);
                    }
                    hs_risc_v_core_rv32_csr_write_by_inst(core,csr,rs1_value);
                }
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(csrrs,
            {
                if(!hs_risc_v_core_rv32_csr_access_legal(core,csr,rs1!=0))
                {
                    int cause=HS_RISC_V_OPCODES_EXCEPTION_NAME(CAUSE_ILLEGAL_INSTRUCTION);
                    hs_risc_v_core_rv32_exec_exception_raise(core,cause,pc,instruction);
                }
                else
                {
                    uint32_t csr_value=hs_risc_v_core_rv32_csr_read(core,csr);
                    if(rs1!=0)
                    {
                        hs_risc_v_core_rv32_csr_write_by_inst(core,csr,csr_value|rs1_value);
                    }
                    hs_risc_v_core_rv32_x_register_write(core,rd,csr_value);
                }
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(csrrc,
            {
                if(!hs_risc_v_core_rv32_csr_access_legal(core,csr,rs1!=0))
                {
                    int cause=HS_RISC_V_OPCODES_EXCEPTION_NAME(CAUSE_ILLEGAL_INSTRUCTION);
                    hs_risc_v_core_rv32_exec_exception_raise(core,cause,pc,instruction);
                }
                else
                {
                    uint32_t csr_value=hs_risc_v_core_rv32_csr_read(core,csr);
                    if(rs1!=0)
                    {
                        hs_risc_v_core_rv32_csr_write_by_inst(core,csr,csr_value&(~rs1_value));
                    }
                    hs_risc_v_core_rv32_x_register_write(core,rd,csr_value);
                }
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(csrrwi,
            {
                if(!hs_risc_v_core_rv32_csr_access_legal(core,csr,true))
                {
                    int cause=HS_RISC_V_OPCODES_EXCEPTION_NAME(CAUSE_ILLEGAL_INSTRUCTION);
                    hs_risc_v_core_rv32_exec_exception_raise(core,cause,pc,instruction);
                }
                else
                {
                    if(rd!=0)
                    {
                        uint32_t csr_value=hs_risc_v_core_rv32_csr_read(core,csr);
                        hs_risc_v_core_rv32_x_register_write(core,rd,csr_value);
                    }
                    hs_risc_v_core_rv32_csr_write_by_inst(core,csr,uimm);
                }
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(csrrsi,
            {
                if(!hs_risc_v_core_rv32_csr_access_legal(core,csr,uimm!=0))
                {
                    int cause=HS_RISC_V_OPCODES_EXCEPTION_NAME(CAUSE_ILLEGAL_INSTRUCTION);
                    hs_risc_v_core_rv32_exec_exception_raise(core,cause,pc,instruction);
                }
                else
                {
                    uint32_t csr_value=hs_risc_v_core_rv32_csr_read(core,csr);
                    if(uimm!=0)
                    {
                        hs_risc_v_core_rv32_csr_write_by_inst(core,csr,csr_value|uimm);
                    }
                    hs_risc_v_core_rv32_x_register_write(core,rd,csr_value);
                }
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(csrrci,
            {
                if(!hs_risc_v_core_rv32_csr_access_legal(core,csr,uimm!=0))
                {
                    int cause=HS_RISC_V_OPCODES_EXCEPTION_NAME(CAUSE_ILLEGAL_INSTRUCTION);
                    hs_risc_v_core_rv32_exec_exception_raise(core,cause,pc,instruction);
                }
                else
                {
                    uint32_t csr_value=hs_risc_v_core_rv32_csr_read(core,csr);
                    if(uimm!=0)
                    {
                        hs_risc_v_core_rv32_csr_write_by_inst(core,csr,csr_value&(~uimm));
                    }
                    hs_risc_v_core_rv32_x_register_write(core,rd,csr_value);
                }
            });
        }
    }



    //RV32M
    if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32M)&& !is_instruction_processed)
    {
        if((instruction&0x7F)==HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_OP)
        {
            //RV32M
            uint32_t rd=((instruction&INSN_FIELD_RD)>>7);
            uint32_t rs1=((instruction&INSN_FIELD_RS1)>>15);
            uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1);
            uint32_t rs2=((instruction&INSN_FIELD_RS2)>>20);
            uint32_t rs2_value=hs_risc_v_core_rv32_x_register_read(core,rs2);
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(mul,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,((uint64_t)(((int64_t)((int32_t)rs1_value))*((int64_t)((int32_t)rs2_value)))));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(mulh,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,((uint64_t)(((int64_t)((int32_t)rs1_value))*((int64_t)((int32_t)rs2_value)))) >> 32);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(mulhu,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,((uint64_t)(((uint64_t)((uint32_t)rs1_value))*((uint64_t)((uint32_t)rs2_value)))) >> 32);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(mulhsu,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,((uint64_t)(((int64_t)((int32_t)rs1_value))*((uint64_t)((uint32_t)rs2_value)))) >> 32);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(div,
            {
                int64_t lhs=(int32_t)rs1_value;
                int64_t rhs=(int32_t)rs2_value;
                if(rhs==0)
                {
                    hs_risc_v_core_rv32_x_register_write(core,rd,(uint32_t)(uint64_t)UINT64_MAX);
                }
                else if(lhs==INT32_MIN && rhs== -1)
                {
                    hs_risc_v_core_rv32_x_register_write(core,rd,(uint32_t)(uint64_t)lhs);
                }
                else
                {
                    hs_risc_v_core_rv32_x_register_write(core,rd,(uint32_t)(uint64_t)(lhs/rhs));
                }
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(divu,
            {
                uint64_t lhs=(uint32_t)rs1_value;
                uint64_t rhs=(uint32_t)rs2_value;
                if(rhs==0)
                {
                    hs_risc_v_core_rv32_x_register_write(core,rd,(uint32_t)(uint64_t)UINT64_MAX);
                }
                else
                {
                    hs_risc_v_core_rv32_x_register_write(core,rd,(uint32_t)(uint64_t)(lhs/rhs));
                }
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(rem,
            {
                int64_t lhs=(int32_t)rs1_value;
                int64_t rhs=(int32_t)rs2_value;
                if(rhs==0)
                {
                    hs_risc_v_core_rv32_x_register_write(core,rd,(uint32_t)(uint64_t)lhs);
                }
                else if(lhs==INT32_MIN && rhs== -1)
                {
                    hs_risc_v_core_rv32_x_register_write(core,rd,(uint32_t)(uint64_t)0);
                }
                else
                {
                    hs_risc_v_core_rv32_x_register_write(core,rd,(uint32_t)(uint64_t)(lhs%rhs));
                }
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(remu,
            {
                uint64_t lhs=(uint32_t)rs1_value;
                uint64_t rhs=(uint32_t)rs2_value;
                if(rhs==0)
                {
                    hs_risc_v_core_rv32_x_register_write(core,rd,(uint32_t)(uint64_t)(int64_t)(int32_t)rs1_value);
                }
                else
                {
                    hs_risc_v_core_rv32_x_register_write(core,rd,(uint32_t)(uint64_t)(lhs%rhs));
                }
            });
        }
    }

    //Zba/Zbb/Zbc/Zbs/Zicond扩展指令集
    if(!is_instruction_processed)
    {
        uint32_t rd=((instruction&INSN_FIELD_RD)>>7);
        uint32_t rs1=((instruction&INSN_FIELD_RS1)>>15);
        uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1);
        uint32_t rs2=((instruction&INSN_FIELD_RS2)>>20);
        uint32_t rs2_value=hs_risc_v_core_rv32_x_register_read(core,rs2);
        uint32_t shamt=((instruction>>20)&0x1F);
        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZBA))
        {
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sh1add,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,(rs1_value<<1)+rs2_value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sh2add,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,(rs1_value<<2)+rs2_value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sh3add,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,(rs1_value<<3)+rs2_value);
            });
        }
        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZBB))
        {
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(andn,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value&(~rs2_value));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(orn,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value|(~rs2_value));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(xnor,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,~(rs1_value^rs2_value));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(clz,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,hs_risc_v_core_rv32_clz32(rs1_value));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(ctz,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,hs_risc_v_core_rv32_ctz32(rs1_value));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(cpop,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,hs_risc_v_core_rv32_cpop32(rs1_value));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(max,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,((int32_t)rs1_value>(int32_t)rs2_value)?rs1_value:rs2_value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(maxu,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,(rs1_value>rs2_value)?rs1_value:rs2_value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(min,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,((int32_t)rs1_value<(int32_t)rs2_value)?rs1_value:rs2_value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(minu,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,(rs1_value<rs2_value)?rs1_value:rs2_value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sext_b,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,(int32_t)(int8_t)rs1_value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sext_h,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,(int32_t)(int16_t)rs1_value);
            });
            {
                const hs_risc_v_opcodes_insn_t insn_pack=HS_RISC_V_OPCODES_INSN_NAME(pack);
                if((instruction&insn_pack.mask)==insn_pack.match && ((instruction>>20)&0x1F)==0)
                {
                    /*
                     * RV32的zext.h编码为pack rd,rs1,x0
                     */
                    is_instruction_processed=true;
                    hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value&0xFFFF);
                }
            }
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(rol,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,hs_risc_v_core_rv32_rol32(rs1_value,rs2_value));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(ror,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,hs_risc_v_core_rv32_ror32(rs1_value,rs2_value));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(rori,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,hs_risc_v_core_rv32_ror32(rs1_value,shamt));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(orc_b,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,hs_risc_v_core_rv32_orc_b32(rs1_value));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(rev8_rv32,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,hs_risc_v_core_rv32_rev8_32(rs1_value));
            });
        }
        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZBC) ||
           hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZBKC))
        {
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(clmul,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,hs_risc_v_core_rv32_clmul32(rs1_value,rs2_value,0));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(clmulh,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,hs_risc_v_core_rv32_clmul32(rs1_value,rs2_value,1));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(clmulr,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,hs_risc_v_core_rv32_clmul32(rs1_value,rs2_value,2));
            });
        }
        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZBS))
        {
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(bset,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value|(1UL<<(rs2_value&0x1F)));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(bclr,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value&(~(1UL<<(rs2_value&0x1F))));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(binv,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value^(1UL<<(rs2_value&0x1F)));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(bext,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,(rs1_value>>(rs2_value&0x1F))&1);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(bseti,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value|(1UL<<shamt));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(bclri,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value&(~(1UL<<shamt)));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(binvi,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value^(1UL<<shamt));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(bexti,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,(rs1_value>>shamt)&1);
            });
        }
        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZICOND))
        {
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(czero_eqz,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,(rs2_value==0)?0:rs1_value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(czero_nez,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,(rs2_value!=0)?0:rs1_value);
            });
        }
    }


    //Zbkb/Zbkx扩展指令集
    if(!is_instruction_processed)
    {
        uint32_t rd=((instruction&INSN_FIELD_RD)>>7);
        uint32_t rs1=((instruction&INSN_FIELD_RS1)>>15);
        uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1);
        uint32_t rs2=((instruction&INSN_FIELD_RS2)>>20);
        uint32_t rs2_value=hs_risc_v_core_rv32_x_register_read(core,rs2);
        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZBKB))
        {
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(pack,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,(rs1_value&0xFFFF)|((rs2_value&0xFFFF)<<16));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(packh,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,(rs1_value&0xFF)|((rs2_value&0xFF)<<8));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(brev8,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,hs_risc_v_core_rv32_brev8_32(rs1_value));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(zip,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,hs_risc_v_core_rv32_zip(rs1_value));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(unzip,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,hs_risc_v_core_rv32_unzip(rs1_value));
            });
        }
        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZBKX))
        {
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(xperm4,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,hs_risc_v_core_rv32_xperm32(rs1_value,rs2_value,2));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(xperm8,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,hs_risc_v_core_rv32_xperm32(rs1_value,rs2_value,3));
            });
        }
    }


    //Zkne/Zknd/Zknh/Zksh/Zksed扩展指令集
    if(!is_instruction_processed)
    {
        uint32_t rd=((instruction&INSN_FIELD_RD)>>7);
        uint32_t rs1=((instruction&INSN_FIELD_RS1)>>15);
        uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1);
        uint32_t rs2=((instruction&INSN_FIELD_RS2)>>20);
        uint32_t rs2_value=hs_risc_v_core_rv32_x_register_read(core,rs2);
        uint32_t bs=(instruction>>30)&0x3;
        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZKNE))
        {
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(aes32esi,
            {
                uint8_t x=hs_risc_v_crypto_aes_enc_sbox[(uint8_t)(rs2_value>>(8*bs))];
                uint32_t u=x;
                u=hs_risc_v_core_rv32_ror32(u,32-8*bs);
                hs_risc_v_core_rv32_x_register_write(core,rd,u^rs1_value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(aes32esmi,
            {
                uint8_t x=hs_risc_v_crypto_aes_enc_sbox[(uint8_t)(rs2_value>>(8*bs))];
                uint32_t u=((uint32_t)hs_risc_v_crypto_aes_gfmul(x,3)<<24)|
                ((uint32_t)x<<16)|
                ((uint32_t)x<<8)|
                ((uint32_t)hs_risc_v_crypto_aes_gfmul(x,2)<<0);
                u=hs_risc_v_core_rv32_ror32(u,32-8*bs);
                hs_risc_v_core_rv32_x_register_write(core,rd,u^rs1_value);
            });
        }
        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZKND))
        {
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(aes32dsi,
            {
                uint8_t x=hs_risc_v_crypto_aes_dec_sbox[(uint8_t)(rs2_value>>(8*bs))];
                uint32_t u=x;
                u=hs_risc_v_core_rv32_ror32(u,32-8*bs);
                hs_risc_v_core_rv32_x_register_write(core,rd,u^rs1_value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(aes32dsmi,
            {
                uint8_t x=hs_risc_v_crypto_aes_dec_sbox[(uint8_t)(rs2_value>>(8*bs))];
                uint32_t u=((uint32_t)hs_risc_v_crypto_aes_gfmul(x,0xB)<<24)|
                ((uint32_t)hs_risc_v_crypto_aes_gfmul(x,0xD)<<16)|
                ((uint32_t)hs_risc_v_crypto_aes_gfmul(x,0x9)<<8)|
                ((uint32_t)hs_risc_v_crypto_aes_gfmul(x,0xE)<<0);
                u=hs_risc_v_core_rv32_ror32(u,32-8*bs);
                hs_risc_v_core_rv32_x_register_write(core,rd,u^rs1_value);
            });
        }
        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZKNH))
        {
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sha256sig0,
            {
                uint32_t a=rs1_value;
                uint32_t result=hs_risc_v_core_rv32_ror32(a,7)^hs_risc_v_core_rv32_ror32(a,18)^(a>>3);
                hs_risc_v_core_rv32_x_register_write(core,rd,result);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sha256sig1,
            {
                uint32_t a=rs1_value;
                uint32_t result=hs_risc_v_core_rv32_ror32(a,17)^hs_risc_v_core_rv32_ror32(a,19)^(a>>10);
                hs_risc_v_core_rv32_x_register_write(core,rd,result);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sha256sum0,
            {
                uint32_t a=rs1_value;
                uint32_t result=hs_risc_v_core_rv32_ror32(a,2)^hs_risc_v_core_rv32_ror32(a,13)^hs_risc_v_core_rv32_ror32(a,22);
                hs_risc_v_core_rv32_x_register_write(core,rd,result);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sha256sum1,
            {
                uint32_t a=rs1_value;
                uint32_t result=hs_risc_v_core_rv32_ror32(a,6)^hs_risc_v_core_rv32_ror32(a,11)^hs_risc_v_core_rv32_ror32(a,25);
                hs_risc_v_core_rv32_x_register_write(core,rd,result);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sha512sig0h,
            {
                uint32_t result=(rs1_value>>1)^(rs1_value>>7)^(rs1_value>>8)^(rs2_value<<31)^(rs2_value<<24);
                hs_risc_v_core_rv32_x_register_write(core,rd,result);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sha512sig0l,
            {
                uint32_t result=(rs1_value>>1)^(rs1_value>>7)^(rs1_value>>8)^(rs2_value<<31)^(rs2_value<<25)^(rs2_value<<24);
                hs_risc_v_core_rv32_x_register_write(core,rd,result);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sha512sig1h,
            {
                uint32_t result=(rs1_value<<3)^(rs1_value>>6)^(rs1_value>>19)^(rs2_value>>29)^(rs2_value<<13);
                hs_risc_v_core_rv32_x_register_write(core,rd,result);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sha512sig1l,
            {
                uint32_t result=(rs1_value<<3)^(rs1_value>>6)^(rs1_value>>19)^(rs2_value>>29)^(rs2_value<<26)^(rs2_value<<13);
                hs_risc_v_core_rv32_x_register_write(core,rd,result);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sha512sum0r,
            {
                uint32_t result=(rs1_value<<25)^(rs1_value<<30)^(rs1_value>>28)^(rs2_value>>7)^(rs2_value>>2)^(rs2_value<<4);
                hs_risc_v_core_rv32_x_register_write(core,rd,result);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sha512sum1r,
            {
                uint32_t result=(rs1_value<<23)^(rs1_value>>14)^(rs1_value>>18)^(rs2_value>>9)^(rs2_value<<18)^(rs2_value<<14);
                hs_risc_v_core_rv32_x_register_write(core,rd,result);
            });
        }
        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZKSH))
        {
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sm3p0,
            {
                uint32_t result=rs1_value^hs_risc_v_core_rv32_rol32(rs1_value,9)^hs_risc_v_core_rv32_rol32(rs1_value,17);
                hs_risc_v_core_rv32_x_register_write(core,rd,result);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sm3p1,
            {
                uint32_t result=rs1_value^hs_risc_v_core_rv32_rol32(rs1_value,15)^hs_risc_v_core_rv32_rol32(rs1_value,23);
                hs_risc_v_core_rv32_x_register_write(core,rd,result);
            });
        }
        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZKSED))
        {
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sm4ed,
            {
                uint32_t sb_out=hs_risc_v_crypto_sm4_sbox[(uint8_t)(rs2_value>>(8*bs))];
                uint32_t linear=sb_out^(sb_out<<8)^(sb_out<<2)^(sb_out<<18)^((sb_out&0x3F)<<26)^((sb_out&0xC0)<<10);
                uint32_t rotl=hs_risc_v_core_rv32_ror32(linear,32-8*bs);
                hs_risc_v_core_rv32_x_register_write(core,rd,rotl^rs1_value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sm4ks,
            {
                uint32_t sb_out=hs_risc_v_crypto_sm4_sbox[(uint8_t)(rs2_value>>(8*bs))];
                uint32_t x=sb_out^((sb_out&0x07)<<29)^((sb_out&0xFE)<<7)^((sb_out&0x01)<<23)^((sb_out&0xF8)<<13);
                uint32_t rotl=hs_risc_v_core_rv32_ror32(x,32-8*bs);
                hs_risc_v_core_rv32_x_register_write(core,rd,rotl^rs1_value);
            });
        }
    }


    //RV32A
    if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32A) && !is_instruction_processed)
    {
        if((instruction&0x7F)==HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_AMO)
        {
            uint32_t rd=((instruction&INSN_FIELD_RD)>>7);
            uint32_t rs1=((instruction&INSN_FIELD_RS1)>>15);
            uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1);
            uint32_t rs2=((instruction&INSN_FIELD_RS2)>>20);
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(lr_w,
            {
                hs_risc_v_common_memory_word_t value;
                value.value=0;
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_LR,rs1_value,value.bytes,sizeof(value.bytes));
                HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                hs_risc_v_core_rv32_x_register_write(core,rd,value.value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sc_w,
            {
                hs_risc_v_common_memory_word_t value;
                value.value=hs_risc_v_core_rv32_x_register_read(core,rs2);
                HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                bool have_reservation=hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_SC,rs1_value,value.bytes,sizeof(value.bytes));
                hs_risc_v_core_rv32_x_register_write(core,rd,have_reservation?0:1);
            });
            {
                uint32_t rs2_value=hs_risc_v_core_rv32_x_register_read(core,rs2);
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(amoswap_w,
                {
                    hs_risc_v_common_memory_word_t value;
                    value.value=0;
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_READ,rs1_value,value.bytes,sizeof(value.bytes));
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_x_register_write(core,rd,value.value);
                    value.value=rs2_value;
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_WRITE,rs1_value,value.bytes,sizeof(value.bytes));
                });
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(amoadd_w,
                {
                    hs_risc_v_common_memory_word_t value;
                    value.value=0;
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_READ,rs1_value,value.bytes,sizeof(value.bytes));
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_x_register_write(core,rd,value.value);
                    value.s_value+=(int32_t)rs2_value;
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_WRITE,rs1_value,value.bytes,sizeof(value.bytes));
                });
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(amoxor_w,
                {
                    hs_risc_v_common_memory_word_t value;
                    value.value=0;
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_READ,rs1_value,value.bytes,sizeof(value.bytes));
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_x_register_write(core,rd,value.value);
                    value.value^=rs2_value;
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_WRITE,rs1_value,value.bytes,sizeof(value.bytes));
                });
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(amoand_w,
                {
                    hs_risc_v_common_memory_word_t value;
                    value.value=0;
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_READ,rs1_value,value.bytes,sizeof(value.bytes));
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_x_register_write(core,rd,value.value);
                    value.value&=rs2_value;
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_WRITE,rs1_value,value.bytes,sizeof(value.bytes));
                });
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(amoor_w,
                {
                    hs_risc_v_common_memory_word_t value;
                    value.value=0;
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_READ,rs1_value,value.bytes,sizeof(value.bytes));
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_x_register_write(core,rd,value.value);
                    value.value|=rs2_value;
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_WRITE,rs1_value,value.bytes,sizeof(value.bytes));
                });
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(amomin_w,
                {
                    hs_risc_v_common_memory_word_t value;
                    value.value=0;
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_READ,rs1_value,value.bytes,sizeof(value.bytes));
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_x_register_write(core,rd,value.value);
                    if(value.s_value > ((int32_t)rs2_value))
                    {
                        value.value=rs2_value;
                    }
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_WRITE,rs1_value,value.bytes,sizeof(value.bytes));
                });
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(amomax_w,
                {
                    hs_risc_v_common_memory_word_t value;
                    value.value=0;
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_READ,rs1_value,value.bytes,sizeof(value.bytes));
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_x_register_write(core,rd,value.value);
                    if(value.s_value < ((int32_t)rs2_value))
                    {
                        value.value=rs2_value;
                    }
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_WRITE,rs1_value,value.bytes,sizeof(value.bytes));
                });
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(amominu_w,
                {
                    hs_risc_v_common_memory_word_t value;
                    value.value=0;
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_READ,rs1_value,value.bytes,sizeof(value.bytes));
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_x_register_write(core,rd,value.value);
                    if(value.value > ((uint32_t)rs2_value))
                    {
                        value.value=rs2_value;
                    }
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_WRITE,rs1_value,value.bytes,sizeof(value.bytes));
                });
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(amomaxu_w,
                {
                    hs_risc_v_common_memory_word_t value;
                    value.value=0;
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_READ,rs1_value,value.bytes,sizeof(value.bytes));
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_x_register_write(core,rd,value.value);
                    if(value.value < ((uint32_t)rs2_value))
                    {
                        value.value=rs2_value;
                    }
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_WRITE,rs1_value,value.bytes,sizeof(value.bytes));
                });
            }
        }
    }

    //Zacas/Zabha扩展指令集
    if(!is_instruction_processed && (instruction&0x7F)==HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_AMO)
    {
        uint32_t rd=((instruction&INSN_FIELD_RD)>>7);
        uint32_t rs1=((instruction&INSN_FIELD_RS1)>>15);
        uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1);
        uint32_t rs2=((instruction&INSN_FIELD_RS2)>>20);
        uint32_t rs2_value=hs_risc_v_core_rv32_x_register_read(core,rs2);
        uint32_t compare_value=hs_risc_v_core_rv32_x_register_read(core,rd);
        uint32_t funct3=((instruction>>12)&0x7);
        uint32_t funct5=((instruction>>27)&0x1F);
        bool amocas_enabled=hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZACAS);
        /*
         * Zacas: amocas.w，rd同时为比较值，rs2为交换值
         */
        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZACAS) &&
           funct5==0x05 && funct3==2)
        {
            hs_risc_v_common_memory_word_t value;
            value.value=0;
            hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_READ,rs1_value,value.bytes,sizeof(value.bytes));
            HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
            uint32_t old_value=value.value;
            if(compare_value==old_value)
            {
                value.value=rs2_value;
                HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_WRITE,rs1_value,value.bytes,sizeof(value.bytes));
            }
            hs_risc_v_core_rv32_x_register_write(core,rd,old_value);
            is_instruction_processed=true;
        }
        /*
         * Zacas: amocas.d，RV32使用寄存器对，rd/rd+1为比较值，rs2/rs2+1为交换值
         */
        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZACAS) &&
           funct5==0x05 && funct3==3)
        {
            hs_risc_v_common_memory_doubleword_t value;
            value.value=0;
            hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_READ,rs1_value,value.bytes,sizeof(value.bytes));
            HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
            uint64_t compare_value64=0;
            uint64_t swap_value64=0;
            if(rd!=0)
            {
                compare_value64=((uint64_t)hs_risc_v_core_rv32_x_register_read(core,rd+1)<<32)|(uint64_t)compare_value;
            }
            if(rs2!=0)
            {
                swap_value64=((uint64_t)hs_risc_v_core_rv32_x_register_read(core,rs2+1)<<32)|(uint64_t)rs2_value;
            }
            uint64_t old_value=value.value;
            if(compare_value64==old_value)
            {
                value.value=swap_value64;
                HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_WRITE,rs1_value,value.bytes,sizeof(value.bytes));
            }
            if(rd!=0)
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,(uint32_t)old_value);
                hs_risc_v_core_rv32_x_register_write(core,rd+1,(uint32_t)(old_value>>32));
            }
            is_instruction_processed=true;
        }
        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZABHA) &&
           funct3==0 && (funct5!=0x05 || amocas_enabled))
        {
            hs_risc_v_common_memory_byte_t value;
            value.value=0;
            hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_READ,rs1_value,value.bytes,sizeof(value.bytes));
            uint8_t old_value=value.value;
            uint8_t new_value=old_value;
            switch(funct5)
            {
            case 0x00:
            {
                new_value=(uint8_t)(old_value+(uint8_t)rs2_value);
            }
            break;
            case 0x01:
            {
                new_value=(uint8_t)rs2_value;
            }
            break;
            case 0x04:
            {
                new_value=old_value^((uint8_t)rs2_value);
            }
            break;
            case 0x08:
            {
                new_value=old_value|((uint8_t)rs2_value);
            }
            break;
            case 0x0C:
            {
                new_value=old_value&((uint8_t)rs2_value);
            }
            break;
            case 0x10:
            {
                if((int8_t)old_value>(int8_t)(uint8_t)rs2_value)
                {
                    new_value=(uint8_t)rs2_value;
                }
            }
            break;
            case 0x14:
            {
                if((int8_t)old_value<(int8_t)(uint8_t)rs2_value)
                {
                    new_value=(uint8_t)rs2_value;
                }
            }
            break;
            case 0x18:
            {
                if(old_value>(uint8_t)rs2_value)
                {
                    new_value=(uint8_t)rs2_value;
                }
            }
            break;
            case 0x1C:
            {
                if(old_value<(uint8_t)rs2_value)
                {
                    new_value=(uint8_t)rs2_value;
                }
            }
            break;
            case 0x05:
            {
                if((uint8_t)compare_value==old_value)
                {
                    new_value=(uint8_t)rs2_value;
                }
                else
                {
                    new_value=old_value;
                }
            }
            break;
            default:
            {
                new_value=old_value;
            }
            break;
            }
            value.value=new_value;
            hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_WRITE,rs1_value,value.bytes,sizeof(value.bytes));
            hs_risc_v_core_rv32_x_register_write(core,rd,(uint32_t)(int8_t)old_value);
            is_instruction_processed=true;
        }
        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZABHA) &&
           funct3==1 && (funct5!=0x05 || amocas_enabled))
        {
            hs_risc_v_common_memory_halfword_t value;
            value.value=0;
            hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_READ,rs1_value,value.bytes,sizeof(value.bytes));
            HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
            uint16_t old_value=value.value;
            uint16_t new_value=old_value;
            switch(funct5)
            {
            case 0x00:
            {
                new_value=(uint16_t)(old_value+(uint16_t)rs2_value);
            }
            break;
            case 0x01:
            {
                new_value=(uint16_t)rs2_value;
            }
            break;
            case 0x04:
            {
                new_value=old_value^((uint16_t)rs2_value);
            }
            break;
            case 0x08:
            {
                new_value=old_value|((uint16_t)rs2_value);
            }
            break;
            case 0x0C:
            {
                new_value=old_value&((uint16_t)rs2_value);
            }
            break;
            case 0x10:
            {
                if((int16_t)old_value>(int16_t)(uint16_t)rs2_value)
                {
                    new_value=(uint16_t)rs2_value;
                }
            }
            break;
            case 0x14:
            {
                if((int16_t)old_value<(int16_t)(uint16_t)rs2_value)
                {
                    new_value=(uint16_t)rs2_value;
                }
            }
            break;
            case 0x18:
            {
                if(old_value>(uint16_t)rs2_value)
                {
                    new_value=(uint16_t)rs2_value;
                }
            }
            break;
            case 0x1C:
            {
                if(old_value<(uint16_t)rs2_value)
                {
                    new_value=(uint16_t)rs2_value;
                }
            }
            break;
            case 0x05:
            {
                if((uint16_t)compare_value==old_value)
                {
                    new_value=(uint16_t)rs2_value;
                }
                else
                {
                    new_value=old_value;
                }
            }
            break;
            default:
            {
                new_value=old_value;
            }
            break;
            }
            value.value=new_value;
            HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
            hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_WRITE,rs1_value,value.bytes,sizeof(value.bytes));
            hs_risc_v_core_rv32_x_register_write(core,rd,(uint32_t)(int16_t)old_value);
            is_instruction_processed=true;
        }
    }


    //RV32F/RV32D(浮点指令)
    if((hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32F) ||
        hs_risc_v_core_rv32_has_d(core)) && !is_instruction_processed)
    {
        uint32_t opcode=(instruction&0x7F);
        bool is_fp_op=false;
        switch(opcode)
        {
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_LOAD_FP:
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_STORE_FP:
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_MADD:
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_MSUB:
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_NMSUB:
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_NMADD:
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_OP_FP:
        {
            is_fp_op=true;
        }
        break;
        default:
        {
        }
        break;
        }
        if(is_fp_op && !hs_risc_v_core_rv32_fp_enabled(core))
        {
            /*
             * mstatus.FS==0时浮点指令非法
             */
            int cause=HS_RISC_V_OPCODES_EXCEPTION_NAME(CAUSE_ILLEGAL_INSTRUCTION);
            hs_risc_v_core_rv32_exec_exception_raise(core,cause,pc,instruction);
            is_instruction_processed=true;
        }
        else if(is_fp_op)
        {
            uint32_t rd=((instruction&INSN_FIELD_RD)>>7);
            uint32_t rs1=((instruction&INSN_FIELD_RS1)>>15);
            uint32_t rs2=((instruction&INSN_FIELD_RS2)>>20);
            uint32_t rs3=((instruction&INSN_FIELD_RS3)>>27);
            uint32_t flags=0;
            hsoftfp_rounding_mode_t rm=HSOFTFP_RM_RNE;

            if(opcode==HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_LOAD_FP)
            {
                uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1);
                uint32_t i_imm=(((instruction >> 20)&((1ULL << (12))-1)) << (0))+(((instruction&(1ULL<<(31)))!=0)?(0xFFFFF000):0);
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(flw,
                {
                    hs_risc_v_common_memory_word_t value;
                    value.value=0;
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_READ,rs1_value+((int32_t)i_imm),value.bytes,sizeof(value.bytes));
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_fp_write_s(core,rd,value.value);
                    hs_risc_v_core_rv32_fp_dirty(core);
                });
                if(hs_risc_v_core_rv32_has_d(core))
                {
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fld,
                    {
                        hs_risc_v_common_memory_doubleword_t value;
                        value.value=0;
                        hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_READ,rs1_value+((int32_t)i_imm),value.bytes,sizeof(value.bytes));
                        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                        hs_risc_v_core_rv32_fp_write_d(core,rd,value.value);
                        hs_risc_v_core_rv32_fp_dirty(core);
                    });
                }
            }
            else if(opcode==HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_STORE_FP)
            {
                uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1);
                uint32_t imm=(((instruction >> 7)&((1ULL << (5))-1)) << (0))+(((instruction >> 25)&((1ULL << (7))-1)) << (5))+(((instruction&(1ULL<<(31)))!=0)?(0xFFFFF000):0);
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fsw,
                {
                    hs_risc_v_common_memory_word_t value;
                    value.value=hs_risc_v_core_rv32_fp_read_s_raw(core,rs2);
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_WRITE,rs1_value+((int32_t)imm),value.bytes,sizeof(value.bytes));
                });
                if(hs_risc_v_core_rv32_has_d(core))
                {
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fsd,
                    {
                        hs_risc_v_common_memory_doubleword_t value;
                        value.value=hs_risc_v_core_rv32_fp_read_d(core,rs2);
                        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                        hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_WRITE,rs1_value+((int32_t)imm),value.bytes,sizeof(value.bytes));
                    });
                }
            }
            else if(opcode==HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_MADD ||
                    opcode==HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_MSUB ||
                    opcode==HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_NMSUB ||
                    opcode==HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_NMADD)
            {
                if(!hs_risc_v_core_rv32_fp_rm_get(core,instruction,&rm))
                {
                    int cause=HS_RISC_V_OPCODES_EXCEPTION_NAME(CAUSE_ILLEGAL_INSTRUCTION);
                    hs_risc_v_core_rv32_exec_exception_raise(core,cause,pc,instruction);
                    is_instruction_processed=true;
                }
                else
                {
                    uint32_t a=hs_risc_v_core_rv32_fp_read_s(core,rs1);
                    uint32_t b=hs_risc_v_core_rv32_fp_read_s(core,rs2);
                    uint32_t c=hs_risc_v_core_rv32_fp_read_s(core,rs3);
                    uint32_t result=0;
                    switch(opcode)
                    {
                    case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_MADD:
                    {
                        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fmadd_s,
                        {
                            result=hsoftfp_fma_sf32(a,b,c,rm,&flags);
                        });
                    }
                    break;
                    case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_MSUB:
                    {
                        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fmsub_s,
                        {
                            result=hsoftfp_fma_sf32(a,b,c^HSOFTFP_FSIGN_MASK32,rm,&flags);
                        });
                    }
                    break;
                    case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_NMSUB:
                    {
                        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fnmsub_s,
                        {
                            result=hsoftfp_fma_sf32(a,b,c^HSOFTFP_FSIGN_MASK32,rm,&flags)^HSOFTFP_FSIGN_MASK32;
                        });
                    }
                    break;
                    default:
                    {
                        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fnmadd_s,
                        {
                            result=hsoftfp_fma_sf32(a,b,c,rm,&flags)^HSOFTFP_FSIGN_MASK32;
                        });
                    }
                    break;
                    }
                    if(is_instruction_processed)
                    {
                        hs_risc_v_core_rv32_fp_write_s(core,rd,result);
                        hs_risc_v_core_rv32_fflags_commit(core,flags);
                        hs_risc_v_core_rv32_fp_dirty(core);
                    }
                    if(!is_instruction_processed && hs_risc_v_core_rv32_has_d(core))
                    {
                        uint64_t da=hs_risc_v_core_rv32_fp_read_d(core,rs1);
                        uint64_t db=hs_risc_v_core_rv32_fp_read_d(core,rs2);
                        uint64_t dc=hs_risc_v_core_rv32_fp_read_d(core,rs3);
                        uint64_t dresult=0;
                        flags=0;
                        switch(opcode)
                        {
                        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_MADD:
                        {
                            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fmadd_d,
                            {
                                dresult=hsoftfp_fma_sf64(da,db,dc,rm,&flags);
                            });
                        }
                        break;
                        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_MSUB:
                        {
                            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fmsub_d,
                            {
                                dresult=hsoftfp_fma_sf64(da,db,dc^HSOFTFP_FSIGN_MASK64,rm,&flags);
                            });
                        }
                        break;
                        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_NMSUB:
                        {
                            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fnmsub_d,
                            {
                                dresult=hsoftfp_fma_sf64(da,db,dc^HSOFTFP_FSIGN_MASK64,rm,&flags)^HSOFTFP_FSIGN_MASK64;
                            });
                        }
                        break;
                        default:
                        {
                            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fnmadd_d,
                            {
                                dresult=hsoftfp_fma_sf64(da,db,dc,rm,&flags)^HSOFTFP_FSIGN_MASK64;
                            });
                        }
                        break;
                        }
                        if(is_instruction_processed)
                        {
                            hs_risc_v_core_rv32_fp_write_d(core,rd,dresult);
                            hs_risc_v_core_rv32_fflags_commit(core,flags);
                            hs_risc_v_core_rv32_fp_dirty(core);
                        }
                    }
                }
            }
            else if(opcode==HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_OP_FP)
            {
                uint32_t a=hs_risc_v_core_rv32_fp_read_s(core,rs1);
                uint32_t b=hs_risc_v_core_rv32_fp_read_s(core,rs2);
                if(hs_risc_v_core_rv32_fp_rm_get(core,instruction,&rm))
                {
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fadd_s,
                    {
                        uint32_t result=hsoftfp_add_sf32(a,b,rm,&flags);
                        hs_risc_v_core_rv32_fp_write_s(core,rd,result);
                    });
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fsub_s,
                    {
                        uint32_t result=hsoftfp_sub_sf32(a,b,rm,&flags);
                        hs_risc_v_core_rv32_fp_write_s(core,rd,result);
                    });
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fmul_s,
                    {
                        uint32_t result=hsoftfp_mul_sf32(a,b,rm,&flags);
                        hs_risc_v_core_rv32_fp_write_s(core,rd,result);
                    });
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fdiv_s,
                    {
                        uint32_t result=hsoftfp_div_sf32(a,b,rm,&flags);
                        hs_risc_v_core_rv32_fp_write_s(core,rd,result);
                    });
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fsqrt_s,
                    {
                        uint32_t result=hsoftfp_sqrt_sf32(a,rm,&flags);
                        hs_risc_v_core_rv32_fp_write_s(core,rd,result);
                    });
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fcvt_w_s,
                    {
                        int32_t result=hsoftfp_cvt_sf32_i32(a,rm,&flags);
                        hs_risc_v_core_rv32_x_register_write(core,rd,result);
                    });
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fcvt_wu_s,
                    {
                        uint32_t result=hsoftfp_cvt_sf32_u32(a,rm,&flags);
                        hs_risc_v_core_rv32_x_register_write(core,rd,result);
                    });
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fcvt_s_w,
                    {
                        int32_t value=(int32_t)hs_risc_v_core_rv32_x_register_read(core,rs1);
                        uint32_t result=hsoftfp_cvt_i32_sf32(value,rm,&flags);
                        hs_risc_v_core_rv32_fp_write_s(core,rd,result);
                    });
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fcvt_s_wu,
                    {
                        uint32_t value=hs_risc_v_core_rv32_x_register_read(core,rs1);
                        uint32_t result=hsoftfp_cvt_u32_sf32(value,rm,&flags);
                        hs_risc_v_core_rv32_fp_write_s(core,rd,result);
                    });
                    if(hs_risc_v_core_rv32_has_d(core))
                    {
                        uint64_t da=hs_risc_v_core_rv32_fp_read_d(core,rs1);
                        uint64_t db=hs_risc_v_core_rv32_fp_read_d(core,rs2);
                        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fadd_d,
                        {
                            uint64_t result=hsoftfp_add_sf64(da,db,rm,&flags);
                            hs_risc_v_core_rv32_fp_write_d(core,rd,result);
                        });
                        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fsub_d,
                        {
                            uint64_t result=hsoftfp_sub_sf64(da,db,rm,&flags);
                            hs_risc_v_core_rv32_fp_write_d(core,rd,result);
                        });
                        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fmul_d,
                        {
                            uint64_t result=hsoftfp_mul_sf64(da,db,rm,&flags);
                            hs_risc_v_core_rv32_fp_write_d(core,rd,result);
                        });
                        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fdiv_d,
                        {
                            uint64_t result=hsoftfp_div_sf64(da,db,rm,&flags);
                            hs_risc_v_core_rv32_fp_write_d(core,rd,result);
                        });
                        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fsqrt_d,
                        {
                            uint64_t result=hsoftfp_sqrt_sf64(da,rm,&flags);
                            hs_risc_v_core_rv32_fp_write_d(core,rd,result);
                        });
                        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fcvt_w_d,
                        {
                            int32_t result=hsoftfp_cvt_sf64_i32(da,rm,&flags);
                            hs_risc_v_core_rv32_x_register_write(core,rd,result);
                        });
                        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fcvt_wu_d,
                        {
                            uint32_t result=hsoftfp_cvt_sf64_u32(da,rm,&flags);
                            hs_risc_v_core_rv32_x_register_write(core,rd,result);
                        });
                        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fcvt_d_w,
                        {
                            int32_t value=(int32_t)hs_risc_v_core_rv32_x_register_read(core,rs1);
                            uint64_t result=hsoftfp_cvt_i32_sf64(value,rm,&flags);
                            hs_risc_v_core_rv32_fp_write_d(core,rd,result);
                        });
                        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fcvt_d_wu,
                        {
                            uint32_t value=hs_risc_v_core_rv32_x_register_read(core,rs1);
                            uint64_t result=hsoftfp_cvt_u32_sf64(value,rm,&flags);
                            hs_risc_v_core_rv32_fp_write_d(core,rd,result);
                        });
                        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fcvt_s_d,
                        {
                            uint32_t result=hsoftfp_cvt_sf64_sf32(da,rm,&flags);
                            hs_risc_v_core_rv32_fp_write_s(core,rd,result);
                        });
                        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fcvt_d_s,
                        {
                            uint64_t result=hsoftfp_cvt_sf32_sf64(a,&flags);
                            hs_risc_v_core_rv32_fp_write_d(core,rd,result);
                        });
                    }
                }
                else
                {
                    /*
                     * 舍入模式为保留编码
                     */
                    int cause=HS_RISC_V_OPCODES_EXCEPTION_NAME(CAUSE_ILLEGAL_INSTRUCTION);
                    hs_risc_v_core_rv32_exec_exception_raise(core,cause,pc,instruction);
                    is_instruction_processed=true;
                }

                /*
                 * 不涉及舍入的操作
                 */
                if(!is_instruction_processed)
                {
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fsgnj_s,
                    {
                        uint32_t result=(a&(~HSOFTFP_FSIGN_MASK32))|(b&HSOFTFP_FSIGN_MASK32);
                        hs_risc_v_core_rv32_fp_write_s(core,rd,result);
                    });
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fsgnjn_s,
                    {
                        uint32_t result=(a&(~HSOFTFP_FSIGN_MASK32))|((~b)&HSOFTFP_FSIGN_MASK32);
                        hs_risc_v_core_rv32_fp_write_s(core,rd,result);
                    });
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fsgnjx_s,
                    {
                        uint32_t result=(a&(~HSOFTFP_FSIGN_MASK32))|((a^b)&HSOFTFP_FSIGN_MASK32);
                        hs_risc_v_core_rv32_fp_write_s(core,rd,result);
                    });
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fmin_s,
                    {
                        uint32_t result=hs_risc_v_core_rv32_fmin_s(a,b,&flags);
                        hs_risc_v_core_rv32_fp_write_s(core,rd,result);
                    });
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fmax_s,
                    {
                        uint32_t result=hs_risc_v_core_rv32_fmax_s(a,b,&flags);
                        hs_risc_v_core_rv32_fp_write_s(core,rd,result);
                    });
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fmv_x_w,
                    {
                        hs_risc_v_core_rv32_x_register_write(core,rd,hs_risc_v_core_rv32_fp_read_s_raw(core,rs1));
                    });
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fmv_w_x,
                    {
                        hs_risc_v_core_rv32_fp_write_s(core,rd,hs_risc_v_core_rv32_x_register_read(core,rs1));
                    });
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(feq_s,
                    {
                        bool result=hsoftfp_eq_quiet_sf32(a,b,&flags)!=0;
                        hs_risc_v_core_rv32_x_register_write(core,rd,result?1:0);
                    });
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(flt_s,
                    {
                        bool result=hsoftfp_lt_sf32(a,b,&flags)!=0;
                        hs_risc_v_core_rv32_x_register_write(core,rd,result?1:0);
                    });
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fle_s,
                    {
                        bool result=hsoftfp_le_sf32(a,b,&flags)!=0;
                        hs_risc_v_core_rv32_x_register_write(core,rd,result?1:0);
                    });
                    HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fclass_s,
                    {
                        uint32_t result=hsoftfp_fclass_sf32(a);
                        hs_risc_v_core_rv32_x_register_write(core,rd,result);
                    });
                    if(hs_risc_v_core_rv32_has_d(core))
                    {
                        uint64_t da=hs_risc_v_core_rv32_fp_read_d(core,rs1);
                        uint64_t db=hs_risc_v_core_rv32_fp_read_d(core,rs2);
                        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fsgnj_d,
                        {
                            uint64_t result=(da&(~HSOFTFP_FSIGN_MASK64))|(db&HSOFTFP_FSIGN_MASK64);
                            hs_risc_v_core_rv32_fp_write_d(core,rd,result);
                        });
                        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fsgnjn_d,
                        {
                            uint64_t result=(da&(~HSOFTFP_FSIGN_MASK64))|((~db)&HSOFTFP_FSIGN_MASK64);
                            hs_risc_v_core_rv32_fp_write_d(core,rd,result);
                        });
                        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fsgnjx_d,
                        {
                            uint64_t result=(da&(~HSOFTFP_FSIGN_MASK64))|((da^db)&HSOFTFP_FSIGN_MASK64);
                            hs_risc_v_core_rv32_fp_write_d(core,rd,result);
                        });
                        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fmin_d,
                        {
                            uint64_t result=hs_risc_v_core_rv32_fmin_d(da,db,&flags);
                            hs_risc_v_core_rv32_fp_write_d(core,rd,result);
                        });
                        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fmax_d,
                        {
                            uint64_t result=hs_risc_v_core_rv32_fmax_d(da,db,&flags);
                            hs_risc_v_core_rv32_fp_write_d(core,rd,result);
                        });
                        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(feq_d,
                        {
                            bool result=hsoftfp_eq_quiet_sf64(da,db,&flags)!=0;
                            hs_risc_v_core_rv32_x_register_write(core,rd,result?1:0);
                        });
                        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(flt_d,
                        {
                            bool result=hsoftfp_lt_sf64(da,db,&flags)!=0;
                            hs_risc_v_core_rv32_x_register_write(core,rd,result?1:0);
                        });
                        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fle_d,
                        {
                            bool result=hsoftfp_le_sf64(da,db,&flags)!=0;
                            hs_risc_v_core_rv32_x_register_write(core,rd,result?1:0);
                        });
                        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fclass_d,
                        {
                            uint32_t result=hsoftfp_fclass_sf64(da);
                            hs_risc_v_core_rv32_x_register_write(core,rd,result);
                        });
                    }
                }
                if(is_instruction_processed)
                {
                    hs_risc_v_core_rv32_fflags_commit(core,flags);
                    hs_risc_v_core_rv32_fp_dirty(core);
                }
            }
        }
    }

    if(!is_instruction_processed)
    {
        //指令未处理，可能是custom指令
        if(!hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_CUSTOM_INSTRUCTION_EXEC,pc,(uint8_t*)&instruction,sizeof(instruction)))
        {
            int cause=HS_RISC_V_OPCODES_EXCEPTION_NAME(CAUSE_ILLEGAL_INSTRUCTION);    //非法指令
            hs_risc_v_core_rv32_exec_exception_raise(core,cause,pc,instruction);
        }
    }

    core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_INSTRUCTION_EXIT | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_HW,pc,(uint8_t*)&instruction,sizeof(instruction),core->usr);

exec_exception_phase:
    /*
     * 异常处理
     */
    if(!is_instruction_xret)
    {
        if(core->exception_pending!=0)
        {
            core->exception_pending &= (~(1UL << 31));
            uint32_t cause=core->exception_pending;
            core->exception_pending=0;
            hs_risc_v_core_rv32_exec_trap(core,cause,false,pc,core->exception_value);
            core->exception_value=0;
        }
        else if(core->interrupt_pending!=0)
        {
            uint32_t cause=core->interrupt_pending&(~(1UL << 31));
            if(hs_risc_v_core_rv32_interrupt_enabled(core,cause))
            {
                core->interrupt_pending=0;
                hs_risc_v_core_rv32_exec_trap(core,cause,true,hs_risc_v_core_rv32_pc_read(core),0);
            }
        }
    }

    /*
     * 增加cycle
     */
    hs_risc_v_core_rv32_cycle_inc(core);
    core->flags.abort=0;
}


void hs_risc_v_core_rv32_tick(hs_risc_v_core_rv32_t * core,size_t cycles)
{
    if(core==NULL || core->io==NULL)
    {
        return;
    }

    /*
     * 处理clint
     */
    hs_risc_v_core_rv32_exec_clint(core);

    while(cycles--)
    {
        core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_TICK_ENTER | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_HW,hs_risc_v_core_rv32_pc_read(core),(uint8_t*)&cycles,sizeof(cycles),core->usr);

        hs_risc_v_core_rv32_exec(core);

        core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_TICK_EXIT | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_HW,hs_risc_v_core_rv32_pc_read(core),(uint8_t*)&cycles,sizeof(cycles),core->usr);

        if(core->flags.wfi_enable!=0)
        {
            /*
             * WFI指令执行中，直接返回
             */
            break;
        }
    };
}


void hs_risc_v_core_rv32_reset(hs_risc_v_core_rv32_t * core)
{
    if(core!=NULL && core->io!=NULL)
    {
        {
            hs_risc_v_core_rv32_io_t    io=core->io;
            void* usr=core->usr;
            memset(core,0,sizeof(hs_risc_v_core_rv32_t));
            core->usr=usr;
            core->io=io;
        }
        {
            //初始化指令集支持
            core->instruction_sets=HS_RISC_V_COMMON_INSTRUCTION_SET_RV32I;
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZIFENCEI);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZICSR);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32M);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32A);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32C);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32F);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32D);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZBA);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZBB);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZBC);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZBS);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZICOND);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZCB);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZICBOM);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZICBOZ);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZICBOP);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZAWRS);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZACAS);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZABHA);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZKNE);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZKND);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZKNH);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZKSED);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZKSH);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZBKB);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZBKC);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZBKX);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32SVINVAL);
            core->flags.privilege=HS_RISC_V_CORE_RV32_PRIVILEGE_M;
        }
        {
            uint32_t val=0;
            core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_RESET | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_HW,0,(uint8_t *)&val,sizeof(val),core->usr);
        }
        {
            //初始化PC值
            hs_risc_v_common_memory_word_t pc;
            pc.value=0;
            core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_ENTRY_PC_READ | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_HW,0,pc.bytes,sizeof(pc.bytes),core->usr);
            /*
             *   由于传入传出皆为字节数据，因此可不进行字节序修复
             */
            core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_PC_REGISTER_WRITE | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_HW,0,pc.bytes,sizeof(pc.bytes),core->usr);
        }
        {
            //初始化SP值(X2)
            hs_risc_v_common_memory_word_t x;
            x.value=0;
            core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_INITIAL_SP_READ | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_HW,0,x.bytes,sizeof(x.bytes),core->usr);
            if(x.value!=0)
            {
                /*
                 *   由于传入传出皆为字节数据，因此可不进行字节序修复
                 */
                core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_X_REGISTER_WRITE | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_HW,2,x.bytes,sizeof(x.bytes),core->usr);
            }
        }
    }
}

bool hs_risc_v_core_rv32_io(hs_risc_v_core_rv32_t *core,hs_risc_v_core_rv32_io_opt_t opt,uint32_t address,uint8_t *data,size_t len)
{
    if(core!=NULL && core->io!=NULL)
    {
        return core->io(core,opt|(uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_EXTERN,address,data,len,core->usr);
    }
    return false;
}

uint32_t hs_risc_v_core_rv32_sets_get(hs_risc_v_core_rv32_t *core)
{
    if(core==NULL)
    {
        return 0;
    }

    core->instruction_sets=hs_risc_v_common_instruction_set_sets_format(core->instruction_sets);

    return core->instruction_sets;
}

uint32_t hs_risc_v_core_rv32_sets_disable_set(hs_risc_v_core_rv32_t *core,hs_risc_v_common_instruction_set_t ins_set)
{
    if(core==NULL)
    {
        return 0;
    }

    core->instruction_sets=hs_risc_v_common_instruction_set_sets_clear_set(core->instruction_sets,ins_set);

    return core->instruction_sets;
}

bool hs_risc_v_core_rv32_exception_raise(hs_risc_v_core_rv32_t *core,int cause,bool interrupt)
{
    if(core==NULL || core->io == NULL)
    {
        return false;
    }
    if(cause > 31 || cause < 0)
    {
        return false;
    }
    bool ret=true;
    if(interrupt)
    {
        /*
         * 每次只能执行一个中断，无抢占
         */
        if(core->interrupt_pending ==0 )
        {
            core->interrupt_pending = (cause| (1ULL << 31));
        }
        else
        {
            ret=false;
        }
    }
    else
    {
        /*
         * 每次只能等待一个异常
         */
        if(core->exception_pending ==0 )
        {
            core->exception_pending = (cause| (1ULL << 31));
        }
        else
        {
            ret=false;
        }
    }
    return ret;
}

bool hs_risc_v_core_rv32_exception_clear(hs_risc_v_core_rv32_t *core,bool interrupt)
{
    bool ret=false;
    if(core == NULL)
    {
        return ret;
    }
    if(interrupt)
    {
        if(core->interrupt_pending!=0)
        {
            core->interrupt_pending=0;
            ret=true;
        }
    }
    else
    {
        if(core->exception_pending!=0)
        {
            core->exception_pending=0;
            ret=true;
        }
    }
    return ret;
}


bool hs_risc_v_core_rv32_wfi(hs_risc_v_core_rv32_t *core)
{
    bool ret=false;
    if(core!=NULL)
    {
        ret=(core->flags.wfi_enable!=0);
    }
    return ret;
}

void hs_risc_v_core_rv32_wfi_clear(hs_risc_v_core_rv32_t *core)
{
    if(core!=NULL)
    {
        core->flags.wfi_enable=0;
    }
}

uint64_t * hs_risc_v_core_rv32_clint_mtime(hs_risc_v_core_rv32_t *core)
{
    if(core!=NULL)
    {
        return &core->clint.mtime;
    }
    return NULL;
}


uint64_t * hs_risc_v_core_rv32_clint_mtimecmp(hs_risc_v_core_rv32_t *core)
{
    if(core!=NULL)
    {
        return &core->clint.mtimecmp;
    }
    return NULL;
}
