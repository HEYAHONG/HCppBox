/***************************************************************
 * Name:      moxie_moxiebox_el.h
 * Purpose:   声明moxie_moxiebox小端虚拟机模型接口(仿moxiebox)
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-16
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

/*
 * 典型调用顺序:
 *
 * 1. machine_mem = malloc(hs_moxie_moxiebox_el_size());
 *    machine = hs_moxie_moxiebox_el_init(machine_mem);
 *
 * 2. 添加代码/数据/外设内存区域并加载程序:
 *    hs_moxie_moxiebox_el_add_memory() 或
 *    hs_moxie_moxiebox_el_add_memory_callback() 添加内存区域;
 *    hs_moxie_moxiebox_el_load_elf()/hs_moxie_moxiebox_el_load_elf_memory() 使用helf加载Moxie ELF;
 *    hs_moxie_moxiebox_el_load_elf_with_buffer() 按PT_LOAD段自动添加用户缓冲区并加载;
 *    hs_moxie_moxiebox_el_set_entry() 设置程序入口地址。
 *
 * 3. prepare_size = hs_moxie_moxiebox_el_prepare_size(machine);
 *    prepare_mem = malloc(prepare_size);
 *    hs_moxie_moxiebox_el_prepare(machine,prepare_mem,prepare_size);
 *
 * 4. 可选:
 *    hs_moxie_moxiebox_el_set_swi() 设置外部服务回调。
 *
 * 5. 运行:
 *    while(!hs_moxie_moxiebox_el_stopped(machine))
 *        hs_moxie_moxiebox_el_step(machine,elapsed_us,count);
 *
 * 6. 复位/销毁:
 *    hs_moxie_moxiebox_el_reset(machine);   // 关闭未关闭文件并清理hmmap申请的内存
 *    hs_moxie_moxiebox_el_deinit(machine);  // 关闭未关闭文件并清理hmmap申请的内存
 *    free(prepare_mem);
 *    free(machine_mem);
 *
 * 注意:
 * - add_memory()/add_memory_callback()/set_entry()必须在prepare()之前调用;
 * - prepare()之后不可再添加内存区域;
 * - 用户提供的机器结构缓冲区、prepare缓冲区以及添加的内存缓冲区必须保持有效，
 *   直到deinit()完成;
 * - 本库可使用helf加载Moxie ELF到用户提供的内存区域，但不会分配或释放用户缓冲区。
 */
#ifndef __HS_MOXIE_MOXIEBOX_EL_H__
#define __HS_MOXIE_MOXIEBOX_EL_H__
#include "moxie_core_el.h"
#include "moxie_moxiebox_common.h"
#include "hruntime.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#ifndef HS_MOXIE_MOXIEBOX_EL_PAGE_SIZE
#define HS_MOXIE_MOXIEBOX_EL_PAGE_SIZE         4096UL
#endif
#ifndef HS_MOXIE_MOXIEBOX_EL_STACK_SIZE
#define HS_MOXIE_MOXIEBOX_EL_STACK_SIZE        (64UL*1024UL)
#endif
#ifndef HS_MOXIE_MOXIEBOX_EL_MAX_MEMORY
#define HS_MOXIE_MOXIEBOX_EL_MAX_MEMORY        64
#endif
#ifndef HS_MOXIE_MOXIEBOX_EL_TAG_SIZE
#define HS_MOXIE_MOXIEBOX_EL_TAG_SIZE          24
#endif
#ifndef HS_MOXIE_MOXIEBOX_EL_MAX_FD
#define HS_MOXIE_MOXIEBOX_EL_MAX_FD            64
#endif
#ifndef HS_MOXIE_MOXIEBOX_EL_PATH_MAX
#define HS_MOXIE_MOXIEBOX_EL_PATH_MAX          256
#endif
#ifndef HS_MOXIE_MOXIEBOX_EL_IO_BUFFER_SIZE
#define HS_MOXIE_MOXIEBOX_EL_IO_BUFFER_SIZE    256
#endif


/*
 * 说明:
 * - 所有swi首先调用外部服务回调，回调返回true表示已接管（包括SYS_EXIT与SYS_MMAP）。
 * - 回调未处理时，SYS_EXIT与SYS_MMAP由虚拟机内部处理，其余swi进入体系结构异常流程。
 * - SYS_MMAP通过hmmap分配主机内存，并将moxie保护/映射标志转换为HMMAN_*标志。
 * - 内置实现包括：reset/read/write/open/close/lseek/mkdir/gettimeofday/settimeofday/mmap/munmap/msync/fsync/fdatasync/getrandom/exit。
 * - 其余系统调用号仅在此定义，由用户SWI回调选择实现；回调返回false时进入体系结构异常流程。
 * - 文件描述符通过虚拟机内部fd_table映射到主机文件描述符；复位时关闭虚拟机打开但未关闭的文件。
 * - 文件描述符0/1/2为标准输入/输出/错误，虚拟机无需open即可直接使用，close不会关闭主机标准流。
 * - 系统调用号及标志定义见moxie_moxiebox_common.h；open/lseek/getrandom/msync标志在虚拟机内部转换为host标志。
 */

typedef struct hs_moxie_moxiebox_el hs_moxie_moxiebox_el_t;

struct hs_moxie_moxiebox_el_memory;
typedef struct hs_moxie_moxiebox_el_memory hs_moxie_moxiebox_el_memory_t;

/** \brief moxiebox外部服务回调
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \param usr void* 用户指针
 * \param number uint32_t swi中断号
 * \return bool 是否已处理。返回true时内核按已处理异常继续运行，返回false时进入体系结构异常处理流程。
 *
 */
typedef bool (*hs_moxie_moxiebox_el_swi_t)(hs_moxie_moxiebox_el_t *machine,void *usr,uint32_t number);

/** \brief moxiebox内存读取回调
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \param memory hs_moxie_moxiebox_el_memory_t* 内存区域指针
 * \param usr void* 区域用户指针
 * \param address uint32_t 虚拟机内部地址
 * \param data void* 数据指针
 * \param len size_t 数据长度
 * \return size_t 已读取长度
 *
 */
typedef size_t (*hs_moxie_moxiebox_el_memory_read_t)(hs_moxie_moxiebox_el_t *machine,hs_moxie_moxiebox_el_memory_t *memory,void *usr,uint32_t address,void *data,size_t len);

/** \brief moxiebox内存写入回调
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \param memory hs_moxie_moxiebox_el_memory_t* 内存区域指针
 * \param usr void* 区域用户指针
 * \param address uint32_t 虚拟机内部地址
 * \param data const void* 数据指针
 * \param len size_t 数据长度
 * \return size_t 已写入长度
 *
 */
typedef size_t (*hs_moxie_moxiebox_el_memory_write_t)(hs_moxie_moxiebox_el_t *machine,hs_moxie_moxiebox_el_memory_t *memory,void *usr,uint32_t address,const void *data,size_t len);

/*
 * 内存区域
 */
struct hs_moxie_moxiebox_el_memory
{
    uint32_t    start;                          /**< 起始地址 */
    uint32_t    length;                         /**< 长度 */
    uint8_t    *data;                           /**< 数据指针，回调方式时为NULL */
    struct
    {
        uint32_t    readonly:1;                 /**< 是否只读 */
        uint32_t    mmap_backed:1;              /**< 是否由mmap分配 */
    } flags;
    void       *usr;                            /**< 回调用户指针 */
    hs_moxie_moxiebox_el_memory_read_t  read;      /**< 读取回调，NULL表示不使用回调读取 */
    hs_moxie_moxiebox_el_memory_write_t write;     /**< 写入回调，NULL表示不使用回调写入 */
    char        tags[HS_MOXIE_MOXIEBOX_EL_TAG_SIZE];/**< 内存描述标签 */
};

/*
 * 文件描述符映射项
 */
typedef struct
{
    int         host_fd;                    /**< 主机文件描述符，-1表示未使用 */
    uint32_t    owned:1;                    /**< 是否由虚拟机打开 */
} hs_moxie_moxiebox_el_fd_t;

/*
 * 虚拟CPU模型(小端)
 */
struct hs_moxie_moxiebox_el
{
    /*
     * 内存区域
     */
    hs_moxie_moxiebox_el_memory_t memory[HS_MOXIE_MOXIEBOX_EL_MAX_MEMORY];
    size_t      memory_count;

    /*
     * 运行参数
     */
    uint32_t    entry;                  /**< 入口地址 */
    uint32_t    stack_top;              /**< 栈顶地址 */
    uint32_t    mapdesc_addr;           /**< 内存描述表地址 */
    uint32_t    heap_next;              /**< 下一次mmap的候选地址 */
    uint32_t    heap_base;              /**< 复位后mmap候选地址的基值 */
    uint32_t    heap_count;             /**< 已分配mmap区域数量 */

    /*
     * 文件描述符映射表
     */
    hs_moxie_moxiebox_el_fd_t fd_table[HS_MOXIE_MOXIEBOX_EL_MAX_FD];

    /*
     * 必须实现的内部寄存器
     */
    uint32_t    regs[16];               /**< 通用寄存器 */
    uint32_t    sregs[256];             /**< 特殊寄存器 */
    uint32_t    pc;                     /**< PC寄存器 */
    int         exit_code;              /**< 退出码 */

    /*
     * 状态
     */
    struct
    {
        uint32_t    started:1;          /**< 是否已开始执行 */
        uint32_t    prepared:1;         /**< 是否已准备栈与内存描述表 */
        uint32_t    stopped:1;          /**< 是否已停止 */
        uint32_t    halted:1;           /**< 是否因异常暂停 */
        uint32_t    return_pending:1;   /**< 是否已写入返回缓冲区地址 */
        uint32_t    return_valid:1;     /**< 返回缓冲区地址与长度是否有效 */
        uint32_t    reset_pending:1;    /**< 等待执行虚拟机复位 */
    } flags;

    /*
     * 外部服务回调
     */
    void       *swi_usr;
    hs_moxie_moxiebox_el_swi_t swi;

    /*
     * 内核对象存储
     */
    union
    {
        uint8_t     bytes[HS_MOXIE_CORE_EL_SIZE()];
        uint64_t    align;
    } core;
};

/** \brief hs_moxie_moxiebox_el_t结构体大小
 *
 * \return size_t 结构体大小
 *
 */
size_t hs_moxie_moxiebox_el_size(void);

/** \brief 初始化虚拟机模型
 *
 * \param mem void* 大小至少为hs_moxie_moxiebox_el_size()的内存
 * \return hs_moxie_moxiebox_el_t* 成功返回指针，失败返回NULL
 *
 */
hs_moxie_moxiebox_el_t *hs_moxie_moxiebox_el_init(void *mem);

/** \brief 关闭未关闭文件、释放虚拟机模型内部由hmmap申请的内存（用户提供的缓冲区不会被释放）
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 *
 */
void hs_moxie_moxiebox_el_deinit(hs_moxie_moxiebox_el_t *machine);

/** \brief 检查虚拟机模型配置
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \return bool 是否有效(至少加载了一个ELF并且入口非0)
 *
 */
bool hs_moxie_moxiebox_el_check(hs_moxie_moxiebox_el_t *machine);

/** \brief 设置程序入口地址
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \param entry uint32_t 入口地址
 *
 */
void hs_moxie_moxiebox_el_set_entry(hs_moxie_moxiebox_el_t *machine,uint32_t entry);

/** \brief 添加内存区域(由用户提供缓冲区，本函数不复制数据)
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \param address uint32_t 起始地址
 * \param data void* 缓冲区指针
 * \param length size_t 缓冲区长度
 * \param readonly bool 是否只读
 * \param name const char* 区域名称(用于内存描述表标签，可为NULL)
 * \return bool 是否添加成功
 *
 */
bool hs_moxie_moxiebox_el_add_memory(hs_moxie_moxiebox_el_t *machine,uint32_t address,void *data,size_t length,bool readonly,const char *name);

/** \brief 添加回调方式内存区域
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \param address uint32_t 起始地址
 * \param length size_t 区域长度
 * \param read hs_moxie_moxiebox_el_memory_read_t 读取回调，可为NULL
 * \param write hs_moxie_moxiebox_el_memory_write_t 写入回调，为NULL时区域为只读
 * \param usr void* 区域用户指针
 * \param name const char* 区域名称(用于内存描述表标签，可为NULL)
 * \return bool 是否添加成功
 *
 */
bool hs_moxie_moxiebox_el_add_memory_callback(hs_moxie_moxiebox_el_t *machine,uint32_t address,size_t length,
        hs_moxie_moxiebox_el_memory_read_t read,
        hs_moxie_moxiebox_el_memory_write_t write,
        void *usr,const char *name);

/*
 * ELF加载(使用helf)
 *
 * - 仅支持Moxie可执行ELF(ELF32,机器类型为Moxie或旧版Moxie,类型为ET_EXEC)。
 * - 加载时只处理PT_LOAD段:文件内容写入各段虚拟地址，文件中不存在但内存中存在的部分(BSS)清零。
 * - 必须在hs_moxie_moxiebox_el_prepare()之前调用。
 * - 本库不分配内存:目标地址必须已由用户通过add_memory()/add_memory_callback()提供，
 *   或者使用load_elf_with_buffer()由用户提供一整块缓冲区。
 */

/** \brief 获取ELF可加载段(PT_LOAD)覆盖的地址范围
 *
 * \param input_file helf_file_input_t* helf输入文件
 * \param base uint32_t* 最小虚拟地址(所有PT_LOAD段的起点)
 * \param length size_t* 覆盖长度(base到最大虚拟地址末尾)
 * \return bool 是否成功
 *
 */
bool hs_moxie_moxiebox_el_elf_memory_range_get(helf_file_input_t *input_file,uint32_t *base,size_t *length);

/** \brief 将ELF可加载段(PT_LOAD)加载到已添加的内存区域
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \param input_file helf_file_input_t* helf输入文件
 * \return bool 是否加载成功
 *
 */
bool hs_moxie_moxiebox_el_load_elf(hs_moxie_moxiebox_el_t *machine,helf_file_input_t *input_file);

/** \brief 使用用户缓冲区按PT_LOAD段自动添加内存区域并加载ELF
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \param input_file helf_file_input_t* helf输入文件
 * \param buffer void* 用户提供的机器内存缓冲区，需覆盖elf_memory_range_get()给出的范围
 * \param length size_t 缓冲区长度
 * \return bool 是否加载成功
 *
 * \note 本函数会为每个PT_LOAD段添加一个内存区域，区域直接指向buffer内部，
 *       不会复制或释放buffer。调用前machine中不应存在与ELF地址范围重叠的内存区域。
 *
 */
bool hs_moxie_moxiebox_el_load_elf_with_buffer(hs_moxie_moxiebox_el_t *machine,helf_file_input_t *input_file,void *buffer,size_t length);

/** \brief 加载内存中的Moxie ELF映像到已添加的内存区域
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \param data const void* ELF文件映像
 * \param length size_t ELF文件映像长度
 * \return bool 是否加载成功
 *
 */
bool hs_moxie_moxiebox_el_load_elf_memory(hs_moxie_moxiebox_el_t *machine,const void *data,size_t length);

/** \brief 获取准备运行环境所需的缓冲区大小(请在添加完内存并设置入口后调用)
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \return size_t 所需缓冲区大小，0表示无法准备
 *
 */
size_t hs_moxie_moxiebox_el_prepare_size(hs_moxie_moxiebox_el_t *machine);

/** \brief 准备运行环境(栈与内存描述表)
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \param mem void* 用户提供的缓冲区，大小至少为hs_moxie_moxiebox_el_prepare_size()返回值
 * \param len size_t 缓冲区长度
 * \return bool 是否准备成功
 *
 */
bool hs_moxie_moxiebox_el_prepare(hs_moxie_moxiebox_el_t *machine,void *mem,size_t len);

/** \brief 复位虚拟机
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 *
 */
void hs_moxie_moxiebox_el_reset(hs_moxie_moxiebox_el_t *machine);

/** \brief 虚拟机步进
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \param elapsed_us uint32_t 本次步进经过的时间(微秒)，累加到定时器time
 * \param count size_t 本次最多执行的指令条数
 * \return int 0=正常运行，1=已停止(退出或异常)，-1=参数错误或配置检查失败
 *
 */
int hs_moxie_moxiebox_el_step(hs_moxie_moxiebox_el_t *machine,uint32_t elapsed_us,size_t count);

/** \brief 获取内核指针
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \return hs_moxie_core_el_t* 内核指针
 *
 */
hs_moxie_core_el_t *hs_moxie_moxiebox_el_core(hs_moxie_moxiebox_el_t *machine);

/** \brief 是否已停止
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \return bool 是否已停止
 *
 */
bool hs_moxie_moxiebox_el_stopped(hs_moxie_moxiebox_el_t *machine);

/** \brief 是否因异常暂停
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \return bool 是否因异常暂停
 *
 */
bool hs_moxie_moxiebox_el_halted(hs_moxie_moxiebox_el_t *machine);

/** \brief 获取退出码
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \return int 退出码
 *
 */
int hs_moxie_moxiebox_el_exit_code(hs_moxie_moxiebox_el_t *machine);

/** \brief 获取返回缓冲区
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \param address uint32_t* 返回缓冲区地址
 * \param length uint32_t* 返回缓冲区长度
 * \return bool 是否有效
 *
 */
bool hs_moxie_moxiebox_el_return_buffer(hs_moxie_moxiebox_el_t *machine,uint32_t *address,uint32_t *length);

/** \brief 设置swi外部服务回调
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \param swi hs_moxie_moxiebox_el_swi_t 回调
 * \param usr void* 用户指针
 *
 */
void hs_moxie_moxiebox_el_set_swi(hs_moxie_moxiebox_el_t *machine,hs_moxie_moxiebox_el_swi_t swi,void *usr);

/** \brief 读取虚拟机内存
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \param address uint32_t 地址
 * \param data void* 数据
 * \param len size_t 长度
 * \return bool 是否成功
 *
 */
bool hs_moxie_moxiebox_el_memory_read(hs_moxie_moxiebox_el_t *machine,uint32_t address,void *data,size_t len);

/** \brief 写入虚拟机内存
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \param address uint32_t 地址
 * \param data const void* 数据
 * \param len size_t 长度
 * \return bool 是否成功
 *
 */
bool hs_moxie_moxiebox_el_memory_write(hs_moxie_moxiebox_el_t *machine,uint32_t address,const void *data,size_t len);

/** \brief 将虚拟机内部地址翻译为主机地址
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \param address uint32_t 虚拟机内部地址
 * \param len size_t 长度
 * \param want_write bool 是否用于写入
 * \return void* 主机地址，NULL表示无法直接翻译(如回调方式内存)
 *
 */
void *hs_moxie_moxiebox_el_memory_to_host(hs_moxie_moxiebox_el_t *machine,uint32_t address,size_t len,bool want_write);

/** \brief 读取通用寄存器
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \param index uint32_t 寄存器编号
 * \return uint32_t 寄存器值
 *
 */
uint32_t hs_moxie_moxiebox_el_register_read(hs_moxie_moxiebox_el_t *machine,uint32_t index);

/** \brief 写入通用寄存器
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \param index uint32_t 寄存器编号
 * \param value uint32_t 值
 *
 */
void hs_moxie_moxiebox_el_register_write(hs_moxie_moxiebox_el_t *machine,uint32_t index,uint32_t value);

/** \brief 读取特殊寄存器
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \param index uint32_t 寄存器编号
 * \return uint32_t 寄存器值
 *
 */
uint32_t hs_moxie_moxiebox_el_sreg_read(hs_moxie_moxiebox_el_t *machine,uint32_t index);

/** \brief 写入特殊寄存器
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \param index uint32_t 寄存器编号
 * \param value uint32_t 值
 *
 */
void hs_moxie_moxiebox_el_sreg_write(hs_moxie_moxiebox_el_t *machine,uint32_t index,uint32_t value);

/** \brief 获取定时器计数(time)
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \return uint64_t 定时器计数
 *
 */
uint64_t hs_moxie_moxiebox_el_timer_time_get(hs_moxie_moxiebox_el_t *machine);

/** \brief 设置定时器计数(time)
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \param value uint64_t 定时器计数
 * \return bool 是否成功
 *
 */
bool hs_moxie_moxiebox_el_timer_time_set(hs_moxie_moxiebox_el_t *machine,uint64_t value);

/** \brief 获取定时器比较值(timecmp)
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \return uint64_t 定时器比较值
 *
 */
uint64_t hs_moxie_moxiebox_el_timer_timecmp_get(hs_moxie_moxiebox_el_t *machine);

/** \brief 设置定时器比较值(timecmp)
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \param value uint64_t 定时器比较值
 * \return bool 是否成功
 *
 */
bool hs_moxie_moxiebox_el_timer_timecmp_set(hs_moxie_moxiebox_el_t *machine,uint64_t value);

/** \brief 触发软件中断(mie.MSIE使能时生效)
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \return bool 是否成功
 *
 */
bool hs_moxie_moxiebox_el_software_interrupt_raise(hs_moxie_moxiebox_el_t *machine);

/** \brief 清除软件中断
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \return bool 是否成功
 *
 */
bool hs_moxie_moxiebox_el_software_interrupt_clear(hs_moxie_moxiebox_el_t *machine);

/** \brief 软件中断是否等待处理
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \return bool 是否等待处理
 *
 */
bool hs_moxie_moxiebox_el_software_interrupt_pending(hs_moxie_moxiebox_el_t *machine);

/** \brief 触发外部中断(mie.MEIE使能时生效)
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \return bool 是否成功
 *
 */
bool hs_moxie_moxiebox_el_external_interrupt_raise(hs_moxie_moxiebox_el_t *machine);

/** \brief 清除外部中断
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \return bool 是否成功
 *
 */
bool hs_moxie_moxiebox_el_external_interrupt_clear(hs_moxie_moxiebox_el_t *machine);

/** \brief 外部中断是否等待处理
 *
 * \param machine hs_moxie_moxiebox_el_t* 虚拟机指针
 * \return bool 是否等待处理
 *
 */
bool hs_moxie_moxiebox_el_external_interrupt_pending(hs_moxie_moxiebox_el_t *machine);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HS_MOXIE_MOXIEBOX_EL_H__
