/***************************************************************
 * Name:      hcoff_common.h
 * Purpose:   声明hcoff_common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCOFF_COMMON_H__
#define __HCOFF_COMMON_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus




typedef struct hcoff_fileheader hcoff_fileheader_t;
struct hcoff_fileheader
{
    uint16_t f_magic;	    /* magic number			*/
    uint16_t f_nscns;	    /* number of sections		*/
    uint32_t f_timdat;	    /* time & date stamp		*/
    uint32_t f_symptr;	    /* file pointer to symtab	*/
    uint32_t f_nsyms;	    /* number of symtab entries	*/
    uint16_t f_opthdr;	    /* sizeof(optional hdr)		*/
    uint16_t f_flags;	    /* flags			*/
};


/*
 * hcoff_fileheader_t幻数(注意:对于不同的架构，可能会区分大小端,可使用幻数的大小端区分文件的大小端)
 */
#define HCOFF_FILEHEADER_F_MAGIC_UNKNOWN           0x0000
#define HCOFF_FILEHEADER_F_MAGIC_ALPHA             0x0184
#define HCOFF_FILEHEADER_F_MAGIC_ALPHA64           0x0284
#define HCOFF_FILEHEADER_F_MAGIC_AM33              0x01d3
#define HCOFF_FILEHEADER_F_MAGIC_AMD64             0x8664
#define HCOFF_FILEHEADER_F_MAGIC_ARM               0x01c0
#define HCOFF_FILEHEADER_F_MAGIC_ARM64             0xaa64
#define HCOFF_FILEHEADER_F_MAGIC_ARMNT             0x01c4
#define HCOFF_FILEHEADER_F_MAGIC_AXP64             HCOFF_FILEHEADER_F_MAGIC_ALPHA64
#define HCOFF_FILEHEADER_F_MAGIC_CEE               0xc0ee
#define HCOFF_FILEHEADER_F_MAGIC_CEF               0x0cef
#define HCOFF_FILEHEADER_F_MAGIC_EBC               0x0ebc
#define HCOFF_FILEHEADER_F_MAGIC_I386              0x014c
#define HCOFF_FILEHEADER_F_MAGIC_IA64              0x0200
#define HCOFF_FILEHEADER_F_MAGIC_LOONGARCH64       0x6264
#define HCOFF_FILEHEADER_F_MAGIC_M32R              0x9041
#define HCOFF_FILEHEADER_F_MAGIC_M68K              0x0268
#define HCOFF_FILEHEADER_F_MAGIC_MIPS16            0x0266
#define HCOFF_FILEHEADER_F_MAGIC_MIPSFPU           0x0366
#define HCOFF_FILEHEADER_F_MAGIC_MIPSFPU16         0x0466
#define HCOFF_FILEHEADER_F_MAGIC_POWERPC           0x01f0
#define HCOFF_FILEHEADER_F_MAGIC_POWERPCFP         0x01f1
#define HCOFF_FILEHEADER_F_MAGIC_R10000            0x0168
#define HCOFF_FILEHEADER_F_MAGIC_R3000             0x0162
#define HCOFF_FILEHEADER_F_MAGIC_R4000             0x0166
#define HCOFF_FILEHEADER_F_MAGIC_RISCV32           0x5032
#define HCOFF_FILEHEADER_F_MAGIC_RISCV64           0x5064
#define HCOFF_FILEHEADER_F_MAGIC_SH3               0x01a2
#define HCOFF_FILEHEADER_F_MAGIC_SH3DSP            0x01a3
#define HCOFF_FILEHEADER_F_MAGIC_SH3E              0x01a4
#define HCOFF_FILEHEADER_F_MAGIC_SH4               0x01a6
#define HCOFF_FILEHEADER_F_MAGIC_SH5               0x01a8
#define HCOFF_FILEHEADER_F_MAGIC_THUMB             0x01c2
#define HCOFF_FILEHEADER_F_MAGIC_TRICORE           0x0520
#define HCOFF_FILEHEADER_F_MAGIC_WCEMIPSV2         0x0169


/*
 * hcoff_fileheader_t标志位
 */
#define HCOFF_FILEHEADER_F_FLAGS_F_EXEC                 (0x0002)
#define HCOFF_FILEHEADER_F_FLAGS_IMAGE_FILE_SYSTEM      (0x1000)
#define HCOFF_FILEHEADER_F_FLAGS_IMAGE_FILE_DLL         (0x2000)


/** \brief COFF读取文件头
 *
 * \param fileheader hcoff_fileheader_t*    COFF文件头
 * \param fileheader_bytes uint8_t*         COFF文件头字节指针
 * \param fileheader_bytes_length size_t    COFF文件头大小,小于sizeof(hcoff_fileheader_t)时将失败
 * \return bool 是否成功读取
 *
 */
bool hcoff_fileheader_read(hcoff_fileheader_t *fileheader,const uint8_t* fileheader_bytes,size_t fileheader_bytes_length);


/** \brief COFF文件头获取节在文件中地址
 *
 * \param fileheader hcoff_fileheader_t* COFF文件头
 * \return uintptr_t 节在文件中地址
 *
 */
uintptr_t hcoff_fileheader_section_offset_get(hcoff_fileheader_t *fileheader);

/** \brief COFF文件头获取节数量
 *
 * \param fileheader hcoff_fileheader_t* COFF文件头
 * \return size_t   节数量
 *
 */
size_t hcoff_fileheader_section_count_get(hcoff_fileheader_t *fileheader);


/** \brief COFF文件头获取是否是可重定位的对象文件
 *
 * \param fileheader hcoff_fileheader_t* COFF文件头
 * \return bool 是否是可重定位的对象文件
 *
 */
bool hcoff_fileheader_is_relocatable_object_file(hcoff_fileheader_t *fileheader);


typedef struct hcoff_file_input hcoff_file_input_t;

/** \brief COFF输入文件读取
 *
 * \param input hcoff_file_input_t* 输入文件
 * \param address uintptr_t 文件内部的地址
 * \param buffer void* 缓冲区地址
 * \param buffer_length size_t 缓冲区大小
 * \return size_t 已读取的大小
 *
 */
typedef size_t (*hcoff_file_input_read_t)(hcoff_file_input_t *input,uintptr_t address,void *buffer,size_t buffer_length);

struct hcoff_file_input
{
    hcoff_file_input_read_t read;
    void *usr;
};

/** \brief COFF输入文件初始化
 *
 * \param input hcoff_file_input_t*     输入文件
 * \param read hcoff_file_input_read_t  文件读取操作
 * \param usr void* 用户操作
 *
 */
void hcoff_file_input_init(hcoff_file_input_t *input,hcoff_file_input_read_t read,void *usr);

/** \brief COFF输入文件读取
 *
 * \param input hcoff_file_input_t* 输入文件
 * \param address uintptr_t 文件内部的地址
 * \param buffer void* 缓冲区地址
 * \param buffer_length size_t 缓冲区大小
 * \return size_t 已读取的大小
 *
 */
size_t hcoff_file_input_read(hcoff_file_input_t *input,uintptr_t address,void *buffer,size_t buffer_length);


/** \brief COFF文件是否为大端
 *
 * \param input hcoff_file_input_t* 输入文件
 * \return bool 是否为大端,通常情况下,小端模式更加常见。
 *
 */
bool hcoff_file_input_is_big_endian(hcoff_file_input_t *input_file);

typedef struct hcoff_sectionheader hcoff_sectionheader_t;
struct hcoff_sectionheader
{
    uint8_t s_name[8];      /* section name				*/
    uint32_t s_paddr;       /* physical address, aliased s_nlib 	*/
    uint32_t s_vaddr;       /* virtual address			*/
    uint32_t s_size;        /* section size				*/
    uint32_t s_scnptr;      /* file ptr to raw data for section 	*/
    uint32_t s_relptr;      /* file ptr to relocation		*/
    uint32_t s_lnnoptr;     /* file ptr to line numbers		*/
    uint16_t s_nreloc;      /* number of relocation entries		*/
    uint16_t s_nlnno;       /* number of line number entries	*/
    uint32_t s_flags;       /* flags				*/
};

/*
 * hcoff_sectionheader节名。
 * 注意:
 *     --通常嵌入式时会启用GCC/Clang的-fdata-sections -ffunctiong-sections,此处只表示节名的第一部分(以.或者$分割),如.text.foo与.text$mn均表示.text节
 *     --节名可以不是唯一的，即多个不同的节可使用同一个节名。
 */
#define HCOFF_SECTIONHEADER_S_NAME_TEXT         ".text"         /**< 代码节,存储可执行代码 */
#define HCOFF_SECTIONHEADER_S_NAME_RDATA        ".rdata"        /**< 只读数据节,存储只读数据(如字符串) */
#define HCOFF_SECTIONHEADER_S_NAME_PDATA        ".pdata"        /**< 异常表节 */
#define HCOFF_SECTIONHEADER_S_NAME_XDATA        ".xdata"        /**< 异常处理表节 */
#define HCOFF_SECTIONHEADER_S_NAME_IDATA        ".idata"        /**< 导入表节 */
#define HCOFF_SECTIONHEADER_S_NAME_EDATA        ".edata"        /**< 导出表节 */
#define HCOFF_SECTIONHEADER_S_NAME_RSRC         ".rsrc"         /**< 资源节 */
#define HCOFF_SECTIONHEADER_S_NAME_CRT          ".crt"          /**< C运行时节，通常用于可执行文件 */
#define HCOFF_SECTIONHEADER_S_NAME_TLS          ".tls"          /**< 线程局部变量节 */
#define HCOFF_SECTIONHEADER_S_NAME_RELOC        ".reloc"        /**< 可执行文件的重定位节 */
#define HCOFF_SECTIONHEADER_S_NAME_SDATA        ".sdata"        /**< 短数据节,短可读可写数据,通常用于具有全局指针的架构 */
#define HCOFF_SECTIONHEADER_S_NAME_DATA         ".data"         /**< 数据节，可读可写数据，通常用于全局变量 */
#define HCOFF_SECTIONHEADER_S_NAME_BSS          ".bss"          /**< 未初始化的数据节 */




/*
 * hcoff_sectionheader_t标志
 */
#define HCOFF_SECTIONHEADER_S_FLAGS_STYP_DATA                   (1 << 6)
#define HCOFF_SECTIONHEADER_S_FLAGS_IMAGE_SCN_MEM_DISCARDABLE   (1 << 25)
#define HCOFF_SECTIONHEADER_S_FLAGS_IMAGE_SCN_MEM_SHARED        (1 << 28)
#define HCOFF_SECTIONHEADER_S_FLAGS_IMAGE_SCN_MEM_READ          (1 << 30)


/** \brief COFF文件读取节头
 *
 * \param sectionheader hcoff_sectionheader_t* 节头
 * \param index size_t 引索，从0开始
 * \param input_file hcoff_file_input_t* 输入文件
 * \return bool 是否读取成功
 *
 */
bool hcoff_sectionheader_read(hcoff_sectionheader_t *sectionheader,size_t index,hcoff_file_input_t *input_file);

/** \brief COFF文件读取节名称(当名称长度超过7时，需要从字符串表中获取名称字符串)
 *
 *
 * \param sectionheader hcoff_sectionheader_t* 节头
 * \param input_file hcoff_file_input_t* 输入文件
 * \param namebuf void* 用于存储名称的缓冲
 * \param namebulen size_t 用于存储名称的缓冲大小
 * \return const char* 失败返回NULL
 *
 */
const char *hcoff_sectionheader_name_read(const hcoff_sectionheader_t *sectionheader,hcoff_file_input_t *input_file,void *namebuf,size_t namebulen);


/** \brief COFF文件匹配节名
 *
 * \param section_name const char* 待匹配的节名,具体见 HCOFF_SECTIONHEADER_S_NAME_*
 * \param sectionheader const hcoff_sectionheader_t* 节头
 * \param input_file hcoff_file_input_t* 输入文件
 * \return bool 是否匹配成功
 *
 */
bool hcoff_sectionheader_name_match(const char *section_name,const hcoff_sectionheader_t *sectionheader,hcoff_file_input_t *input_file);

/*
 * hcoff_section_relocation_t并不适用于所有COFF文件,某些架构的COFF文件需要在结构体末尾添加额外的程序。
 */
typedef struct hcoff_section_relocation hcoff_section_relocation_t;
struct hcoff_section_relocation
{
    uint32_t r_vaddr;         /* Virtual address of reference */
    uint32_t r_symndx;        /* Index into symbol table	*/
    uint16_t r_type;          /* Relocation type		*/
};

/*
 * 重定位的类型根据架构的不同而不同，下面列出一些常用的类型(aarch64、X86_64、I386)
 */
#define HCOFF_SECTION_RELOCATION_R_TYPE_AARCH64_ARM64_ABSOLUTE        0x0000    /* No relocation required */
#define HCOFF_SECTION_RELOCATION_R_TYPE_AARCH64_ARM64_ADDR32          0x0001    /* The 32-bit VA of the target. */
#define HCOFF_SECTION_RELOCATION_R_TYPE_AARCH64_ARM64_ADDR32NB        0x0002    /* The 32-bit RVA of the target. */
#define HCOFF_SECTION_RELOCATION_R_TYPE_AARCH64_ARM64_BRANCH26        0x0003    /* The 26-bit relative displacement to the target, for B and BL instructions. */
#define HCOFF_SECTION_RELOCATION_R_TYPE_AARCH64_ARM64_PAGEBASE_REL21  0x0004    /* The page base of the target, for ADRP instruction. */
#define HCOFF_SECTION_RELOCATION_R_TYPE_AARCH64_ARM64_REL21           0x0005    /* The 12-bit relative displacement to the target, for instruction ADR */
#define HCOFF_SECTION_RELOCATION_R_TYPE_AARCH64_ARM64_PAGEOFFSET_12A  0x0006    /* The 12-bit page offset of the target, for instructions ADD/ADDS (immediate) with zero shift. */
#define HCOFF_SECTION_RELOCATION_R_TYPE_AARCH64_ARM64_PAGEOFFSET_12L  0x0007    /* The 12-bit page offset of the target, for instruction LDR (indexed, unsigned immediate). */
#define HCOFF_SECTION_RELOCATION_R_TYPE_AARCH64_ARM64_SECREL          0x0008    /* The 32-bit offset of the target from the beginning of its section. This is used to support debugging information and static thread local storage. */
#define HCOFF_SECTION_RELOCATION_R_TYPE_AARCH64_ARM64_SECREL_LOW12A   0x0009    /* Bit 0:11 of section offset of the target, for instructions ADD/ADDS (immediate) with zero shift. */
#define HCOFF_SECTION_RELOCATION_R_TYPE_AARCH64_ARM64_SECREL_HIGH12A  0x000A    /* Bit 12:23 of section offset of the target, for instructions ADD/ADDS (immediate) with zero shift. */
#define HCOFF_SECTION_RELOCATION_R_TYPE_AARCH64_ARM64_SECREL_LOW12L   0x000B    /* Bit 0:11 of section offset of the target, for instruction LDR (indexed, unsigned immediate). */
#define HCOFF_SECTION_RELOCATION_R_TYPE_AARCH64_ARM64_TOKEN           0x000C    /* CLR token */
#define HCOFF_SECTION_RELOCATION_R_TYPE_AARCH64_ARM64_SECTION         0x000D    /* The 16-bit section index of the section that contains the target. This is used to support debugging information. */
#define HCOFF_SECTION_RELOCATION_R_TYPE_AARCH64_ARM64_ADDR64          0x000E    /* The 64-bit VA of the relocation target. */
#define HCOFF_SECTION_RELOCATION_R_TYPE_AARCH64_ARM64_BRANCH19        0x000F    /* 19 bit offset << 2 & sign ext. for conditional B */
#define HCOFF_SECTION_RELOCATION_R_TYPE_AARCH64_ARM64_BRANCH14        0x0010    /* The 14-bit offset to the relocation target, for instructions TBZ and TBNZ. */
#define HCOFF_SECTION_RELOCATION_R_TYPE_AARCH64_ARM64_REL32           0x0011    /* The 32-bit relative address from the byte following the relocation. */
#define HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_ABS            0           /* Reference is absolute, no relocation is necessary.  */
#define HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_DIR64          1           /* 64-bit address (VA).  */
#define HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_DIR32          2           /* 32-bit address (VA) R_DIR32.  */
#define HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_IMAGEBASE      3           /* 32-bit absolute ref w/o base R_IMAGEBASE.  */
#define HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG        4           /* 32-bit relative address from byte following reloc R_PCRLONG.  */
#define HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_1      5           /* 32-bit relative address from byte distance 1 from reloc.  */
#define HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_2      6           /* 32-bit relative address from byte distance 2 from reloc.  */
#define HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_3      7           /* 32-bit relative address from byte distance 3 from reloc.  */
#define HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_4      8           /* 32-bit relative address from byte distance 4 from reloc.  */
#define HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_5      9           /* 32-bit relative address from byte distance 5 from reloc.  */
#define HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_SECTION        10          /* Section index.  */
#define HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_SECREL         11          /* 32 bit offset from base of section containing target HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_SECREL.  */
#define HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_SECREL7        12          /* 7 bit unsigned offset from base of section containing target.  */
#define HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_TOKEN          13          /* 32 bit metadata token.  */
#define HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRQUAD        14          /* Pseude PC64 relocation - Note: not specified by MS/AMD but need for gas pc-relative 64bit wide relocation generated by ELF.  */
#define HCOFF_SECTION_RELOCATION_R_TYPE_I386_DIR32		            6
#define HCOFF_SECTION_RELOCATION_R_TYPE_I386_IMAGEBASE	            7
#define HCOFF_SECTION_RELOCATION_R_TYPE_I386_SECTION	            10
#define HCOFF_SECTION_RELOCATION_R_TYPE_I386_SECREL32	            11
#define HCOFF_SECTION_RELOCATION_R_TYPE_I386_RELBYTE	            15
#define HCOFF_SECTION_RELOCATION_R_TYPE_I386_RELWORD	            16
#define HCOFF_SECTION_RELOCATION_R_TYPE_I386_RELLONG	            17
#define HCOFF_SECTION_RELOCATION_R_TYPE_I386_PCRBYTE	            18
#define HCOFF_SECTION_RELOCATION_R_TYPE_I386_PCRWORD	            19
#define HCOFF_SECTION_RELOCATION_R_TYPE_I386_PCRLONG	            20


/** \brief COFF读取节的重定位信息
 * 注意:并不适用于所有COFF文件,对于hcoff_section_relocation_t有额外的成员的架构需要自行实现读重定位信息
 *
 * \param relocation hcoff_section_relocation_t*  重定位信息
 * \param index size_t 引索,从0开始,小于s_nreloc。
 * \param sectionheader const hcoff_sectionheader_t* 节头
 * \param input_file hcoff_file_input_t* 输入文件
 * \return bool 是否成功
 *
 */
bool hcoff_section_relocation_read(hcoff_section_relocation_t *relocation,size_t index,const hcoff_sectionheader_t *sectionheader,hcoff_file_input_t *input_file);

/** \brief COFF判断是否为符号（而不是辅助信息）
 *
 * \param index size_t 符号表的引索
 * \param input_file hcoff_file_input_t* 输入文件
 * \return bool 是否为符号（而非辅助信息）
 *
 */
bool hcoff_symbol_is_symbol(size_t index,hcoff_file_input_t *input_file);

typedef struct hcoff_symbol_entry hcoff_symbol_entry_t;
struct hcoff_symbol_entry
{
    union
    {
        uint8_t e_name[8];
        struct
        {
            uint32_t e_zeroes;
            uint32_t e_offset;
        } e;
    } e;

    uint32_t e_value;
    uint16_t e_scnum;
    uint16_t e_type;
    uint8_t e_sclass;
    uint8_t  e_numaux;
};

#define HCOFF_SYMBOL_ENTRY_E_TYPE_SYMBOL_KEEP          (1U << 5)   /*由链接器使用*/

#define HCOFF_SYMBOL_ENTRY_E_SCLASS_EFCN        0xff    /* physical end of function	*/
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_NULL        0
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_AUTO        1       /* automatic variable		*/
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_EXT         2       /* external symbol		*/
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_STAT        3       /* static			*/
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_REG         4       /* register variable		*/
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_EXTDEF      5       /* external definition		*/
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_LABEL       6       /* label			*/
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_ULABEL      7       /* undefined label		*/
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_MOS         8       /* member of structure		*/
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_ARG         9       /* function argument		*/
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_STRTAG      10      /* structure tag		*/
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_MOU         11      /* member of union		*/
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_UNTAG       12      /* union tag			*/
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_TPDEF       13      /* type definition		*/
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_USTATIC     14      /* undefined static		*/
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_ENTAG       15      /* enumeration tag		*/
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_MOE         16      /* member of enumeration	*/
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_REGPARM     17      /* register parameter		*/
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_FIELD       18      /* bit field			*/
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_AUTOARG     19      /* auto argument		*/
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_LASTENT     20      /* dummy entry (end of block)	*/
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_BLOCK       100     /* ".bb" or ".eb"		*/
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_FCN         101     /* ".bf" or ".ef"		*/
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_EOS         102     /* end of structure		*/
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_FILE        103     /* file name			*/
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_LINE        104     /* line # reformatted as symbol table entry */
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_ALIAS       105     /* duplicate tag		*/
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_HIDDEN      106     /* ext symbol in dmert public lib */
#define HCOFF_SYMBOL_ENTRY_E_SCLASS_WEAKEXT     127     /* weak symbol -- GNU extension.  */


/** \brief COFF读取符号入口
 *
 * \param symbol_entry hcoff_symbol_entry_t* 符号入口
 * \param index size_t 引索
 * \param input_file hcoff_file_input_t* 输入文件
 * \return bool 是否成功
 *
 */
bool hcoff_symbol_entry_read(hcoff_symbol_entry_t *symbol_entry,size_t index,hcoff_file_input_t *input_file);

/** \brief COFF文件读取符号名称(当名称长度超过8时，需要从字符串表中获取名称字符串)
 *
 *
 * \param symbol_entry hcoff_symbol_entry_t* 节头
 * \param input_file hcoff_file_input_t* 输入文件
 * \param namebuf void* 用于存储名称的缓冲
 * \param namebulen size_t 用于存储名称的缓冲大小，需要大于8
 * \return const char* 失败返回NULL
 *
 */
const char *hcoff_symbol_entry_name_read(const hcoff_symbol_entry_t *symbol_entry,hcoff_file_input_t *input_file,void *namebuf,size_t namebulen);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HCOFF_COMMON_H__
