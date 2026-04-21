/***************************************************************
 * Name:      hminirv32ima.c
 * Purpose:   实现hminirv32ima接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-04-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hminirv32ima.h"

static void hminirv32ima_state_csr_write(struct hminirv32ima_state *state,uint32_t csrno,uint32_t csrvalue)
{
    (void)state;
    (void)csrno;
    (void)csrvalue;
}

static void hminirv32ima_state_csr_read(struct hminirv32ima_state *state,uint32_t csrno,uint32_t* csrvalue)
{
    (void)state;
    (void)csrno;
    (void)csrvalue;
}


static size_t hminirv32ima_memory_load(const struct hminirv32ima_memory *mem,uintptr_t ram_addr,void *ptr,size_t length)
{
    (void)mem;
    (void)ram_addr;
    (void)ptr;
    (void)length;
    return 0;
}

static size_t hminirv32ima_memory_store(const struct hminirv32ima_memory *mem,uintptr_t ram_addr,const void *ptr,size_t length)
{
    (void)mem;
    (void)ram_addr;
    (void)ptr;
    (void)length;
    return 0;
}

static size_t hminirv32ima_mmio_load(const struct hminirv32ima_mmio *mmio,uintptr_t mmio_addr,void *ptr,size_t length)
{
    (void)mmio;
    (void)mmio_addr;
    (void)ptr;
    (void)length;
    return 0;
}

static size_t hminirv32ima_mmio_store(const struct hminirv32ima_mmio *mmio,uintptr_t mmio_addr,const void *ptr,size_t length)
{
    (void)mmio;
    (void)mmio_addr;
    (void)ptr;
    (void)length;
    return 0;
}


void hminirv32ima_core_init(hminirv32ima_core_t *core,size_t ram_size)
{
    if(core!=NULL)
    {
        memset(core,0,sizeof(*core));

        /*
         * 初始化状态
         */
        core->state.extraflags |= 3;                       /**< 默认特权为机器级 */
        core->state.csr_read=hminirv32ima_state_csr_read;
        core->state.csr_write=hminirv32ima_state_csr_write;

        /*
         * 初始化内存
         */
        core->memory.ram_base=0x80000000;
        core->memory.ram_size=ram_size;
        core->memory.mem_load=hminirv32ima_memory_load;
        core->memory.mem_store=hminirv32ima_memory_store;
        core->state.regs[2]=core->memory.ram_base+core->memory.ram_size;/**< 初始化SP指针 */

        /*
         * 初始化MMIO
         */
        core->mmio.mmio_base=0x10000000;
        core->mmio.mmio_size= 0x2000000;
        core->mmio.mmio_load=hminirv32ima_mmio_load;
        core->mmio.mmio_store=hminirv32ima_mmio_store;
    }
}

#define MINIRV32_RAM_IMAGE_OFFSET                      (core->memory.ram_base)
#define MINI_RV32_RAM_SIZE                             (core->memory.ram_size)
#define MINIRV32_MMIO_RANGE(n)                         ((core->mmio.mmio_base) <= (n) && (n) < (core->mmio.mmio_base+core->mmio.mmio_size))
#define MINIRV32_HANDLE_MEM_STORE_CONTROL(addr,rs)     do {core->mmio.mmio_store(&core->mmio,addr,&rs,sizeof(rs));} while(0)
#define MINIRV32_HANDLE_MEM_LOAD_CONTROL(addr,rs)      do {core->mmio.mmio_load(&core->mmio,addr,&rs,sizeof(rs));}  while(0)
#define MINIRV32_OTHERCSR_WRITE(csrno,csrval)          do {core->state.csr_write(&core->state,csrno,csrval);}      while(0)
#define MINIRV32_OTHERCSR_READ(csrno,csrval)           do {core->state.csr_read(&core->state,csrno,&csrval);}      while(0)

#define MINIRV32_CUSTOM_MEMORY_BUS 1
static inline void hminirv32ima_store4(hminirv32ima_core_t *core,size_t offset,uint32_t val)
{
    core->memory.mem_store(&core->memory,offset+core->memory.ram_base,&val,sizeof(val));
}
#define MINIRV32_STORE4( ofs, val ) hminirv32ima_store4(core,ofs,val)
static inline void hminirv32ima_store2(hminirv32ima_core_t *core,size_t offset,uint16_t val)
{
    core->memory.mem_store(&core->memory,offset+core->memory.ram_base,&val,sizeof(val));
}
#define MINIRV32_STORE2( ofs, val ) hminirv32ima_store2(core,ofs,val)
static inline void hminirv32ima_store1(hminirv32ima_core_t *core,size_t offset,uint8_t val)
{
    core->memory.mem_store(&core->memory,offset+core->memory.ram_base,&val,sizeof(val));
}
#define MINIRV32_STORE1( ofs, val ) hminirv32ima_store1(core,ofs,val)
static inline uint32_t hminirv32ima_load4(hminirv32ima_core_t *core,size_t offset)
{
    uint32_t val=0;
    core->memory.mem_load(&core->memory,offset+core->memory.ram_base,&val,sizeof(val));
    return val;
}
#define MINIRV32_LOAD4( ofs ) hminirv32ima_load4(core,ofs)
static inline uint16_t hminirv32ima_load2(hminirv32ima_core_t *core,size_t offset)
{
    uint16_t val=0;
    core->memory.mem_load(&core->memory,offset+core->memory.ram_base,&val,sizeof(val));
    return val;
}
#define MINIRV32_LOAD2( ofs ) hminirv32ima_load2(core,ofs)
static inline uint8_t hminirv32ima_load1(hminirv32ima_core_t *core,size_t offset)
{
    uint8_t val=0;
    core->memory.mem_load(&core->memory,offset+core->memory.ram_base,&val,sizeof(val));
    return val;
}
#define MINIRV32_LOAD1( ofs ) hminirv32ima_load1(core,ofs)
static inline int16_t hminirv32ima_load2_signed(hminirv32ima_core_t *core,size_t offset)
{
    int16_t val=0;
    core->memory.mem_load(&core->memory,offset+core->memory.ram_base,&val,sizeof(val));
    return val;
}
#define MINIRV32_LOAD2_SIGNED( ofs ) hminirv32ima_load2_signed(core,ofs)
static inline int8_t hminirv32ima_load1_signed(hminirv32ima_core_t *core,size_t offset)
{
    int8_t val=0;
    core->memory.mem_load(&core->memory,offset+core->memory.ram_base,&val,sizeof(val));
    return val;
}
#define MINIRV32_LOAD1_SIGNED( ofs ) hminirv32ima_load1_signed(core,ofs)

#define MINIRV32_CUSTOM_INTERNALS 1
#define CSR( x ) core->state.x
#define SETCSR( x, val ) { core->state.x = val; }
#define REG( x ) core->state.regs[x]
#define REGSET( x, val ) { core->state.regs[x] = val; }

#define MINIRV32_STEPPROTO static int32_t hminirv32ima_step_internal(hminirv32ima_core_t *core,uint32_t vProcAddress, uint32_t elapsedUs, int count )

#define MINIRV32_IMPLEMENTATION 1

#include H3RDPARTY_MINI_RV32IMA_HEADER

int hminirv32ima_core_step(hminirv32ima_core_t *core,uint32_t elapsedUs, int insn_count )
{
    if(core==NULL)
    {
        return -1;
    }
    return hminirv32ima_step_internal(core,0,elapsedUs,insn_count);
}
