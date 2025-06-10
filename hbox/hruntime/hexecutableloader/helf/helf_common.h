/***************************************************************
 * Name:      helf_common.h
 * Purpose:   声明helf_common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-09
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HELF_COMMON_H__
#define __HELF_COMMON_H__
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


#define HELF_EI_MAG0        0   /* Magic number, byte 0. */
#define HELF_EI_MAG1        1   /* Magic number, byte 1. */
#define HELF_EI_MAG2        2   /* Magic number, byte 2. */
#define HELF_EI_MAG3        3   /* Magic number, byte 3. */
#define HELF_EI_CLASS       4   /* Class of machine. */
#define HELF_EI_DATA        5   /* Data format. */
#define HELF_EI_VERSION     6   /* ELF format version. */
#define HELF_EI_OSABI       7   /* Operating system / ABI identification */
#define HELF_EI_ABIVERSION  8   /* ABI version */
#define HELF_OLD_EI_BRAND   8   /* Start of architecture identification. */
#define HELF_EI_PAD         9   /* Start of padding (per SVR4 ABI). */
#define HELF_EI_NIDENT      16  /* Size of e_ident array. */

/*
 * ELF标识(处于文件起始位置)
 */
typedef uint8_t helf_e_ident_t[HELF_EI_NIDENT];


/** \brief ELF判断是否是ELF
 *
 * \param e_ident helf_e_ident_t ELF标识
 * \return bool 是否是ELF
 *
 */
bool helf_is_elf(helf_e_ident_t e_ident);


#define HELF_ELFCLASS_NONE      0   /* Unknown class. */
#define HELF_ELFCLASS_32        1   /* 32-bit architecture. */
#define HELF_ELFCLASS_64        2   /* 64-bit architecture. */

/** \brief ELF获取ELFCLASS
 *
 * \param e_ident helf_e_ident_t ELF标识
 * \return uint8_t ELFCLASS,见HELF_ELFCLASS_*
 *
 */
uint8_t helf_elfclass_get(helf_e_ident_t e_ident);


#define HELF_ELFDATA_NONE   0   /* Unknown data format. */
#define HELF_ELFDATA_2_LSB  1   /* 2's complement little-endian. */
#define HELF_ELFDATA_2_MSB  2   /* 2's complement big-endian. */

/** \brief ELF获取ELFDATA
 *
 * \param e_ident helf_e_ident_t ELF标识
 * \return uint8_t ELFDATA,见HELF_ELFDATA_*
 *
 */
uint8_t helf_elfdata_get(helf_e_ident_t e_ident);

#define HELF_EV_NONE        0
#define HELF_EV_CURRENT     1

/** \brief ELF获取ELFVERSION
 *
 * \param e_ident helf_e_ident_t ELF标识
 * \return uint8_t ELFVERSION,见HELF_EV_*
 *
 */
uint8_t helf_elfversion_get(helf_e_ident_t e_ident);


#define HELF_ELFOSABI_NONE          0           /* UNIX System V ABI */
#define HELF_ELFOSABI_HPUX          1           /* HP-UX operating system */
#define HELF_ELFOSABI_NETBSD        2           /* NetBSD */
#define HELF_ELFOSABI_LINUX         3           /* GNU/Linux */
#define HELF_ELFOSABI_HURD          4           /* GNU/Hurd */
#define HELF_ELFOSABI_86OPEN        5           /* 86Open common IA32 ABI */
#define HELF_ELFOSABI_SOLARIS       6           /* Solaris */
#define HELF_ELFOSABI_AIX           7           /* AIX */
#define HELF_ELFOSABI_IRIX          8           /* IRIX */
#define HELF_ELFOSABI_FREEBSD       9           /* FreeBSD */
#define HELF_ELFOSABI_TRU64         10          /* TRU64 UNIX */
#define HELF_ELFOSABI_MODESTO       11          /* Novell Modesto */
#define HELF_ELFOSABI_OPENBSD       12          /* OpenBSD */
#define HELF_ELFOSABI_OPENVMS       13          /* Open VMS */
#define HELF_ELFOSABI_NSK           14          /* HP Non-Stop Kernel */
#define HELF_ELFOSABI_AROS          15          /* Amiga Research OS */
#define HELF_ELFOSABI_FENIXOS       16          /* FenixOS */
#define HELF_ELFOSABI_CLOUDABI      17          /* Nuxi CloudABI */
#define HELF_ELFOSABI_OPENVOS       18          /* Stratus Technologies OpenVOS */
#define HELF_ELFOSABI_ARM_AEABI     64          /* ARM EABI */
#define HELF_ELFOSABI_ARM           97          /* ARM */
#define HELF_ELFOSABI_STANDALONE    255         /* Standalone (embedded) application */

#define HELF_ELFOSABI_SYSV      HELF_ELFOSABI_NONE          /* symbol used in old spec */
#define HELF_ELFOSABI_MONTEREY  HELF_ELFOSABI_AIX           /* Monterey */
#define HELF_ELFOSABI_GNU       HELF_ELFOSABI_LINUX


/** \brief ELF获取ELFOSABI
 *
 * \param e_ident helf_e_ident_t ELF标识
 * \return uint8_t ELFOSABI,见HELF_ELFOSABI_*
 *
 */
uint8_t helf_elfosabi_get(helf_e_ident_t e_ident);


/** \brief ELF获取ELFABIVERSION
 *
 * \param e_ident helf_e_ident_t ELF标识
 * \return uint8_t ELFABIVERSION
 *
 */
uint8_t helf_elfabiversion_get(helf_e_ident_t e_ident);

typedef struct helf_file_input helf_file_input_t;

/** \brief ELF输入文件读取
 *
 * \param input helf_file_input_t* 输入文件
 * \param address uintptr_t 文件内部的地址
 * \param buffer void* 缓冲区地址
 * \param buffer_length size_t 缓冲区大小
 * \return size_t 已读取的大小
 *
 */
typedef size_t (*helf_file_input_read_t)(helf_file_input_t *input,uintptr_t address,void *buffer,size_t buffer_length);

struct helf_file_input
{
    helf_file_input_read_t read;
    void *usr;
};

/** \brief ELF输入文件初始化
 *
 * \param input helf_file_input_t*     输入文件
 * \param read helf_file_input_read_t  文件读取操作
 * \param usr void* 用户操作
 *
 */
void helf_file_input_init(helf_file_input_t *input,helf_file_input_read_t read,void *usr);

/** \brief ELF输入文件读取
 *
 * \param input helf_file_input_t* 输入文件
 * \param address uintptr_t 文件内部的地址
 * \param buffer void* 缓冲区地址
 * \param buffer_length size_t 缓冲区大小
 * \return size_t 已读取的大小
 *
 */
size_t helf_file_input_read(helf_file_input_t *input,uintptr_t address,void *buffer,size_t buffer_length);

/** \brief ELF文件是否为大端
 *
 * \param input helf_file_input_t* 输入文件
 * \return bool 是否为大端,通常情况下,小端模式更加常见。
 *
 */
bool helf_file_input_is_big_endian(helf_file_input_t *input_file);

/** \brief ELF文件是否为32位
 *
 * \param input helf_file_input_t* 输入文件
 * \return bool 是否为32位
 *
 */
bool helf_file_input_is_32_bits(helf_file_input_t *input_file);

typedef struct helf_elf32_file_header helf_elf32_file_header_t;
struct helf_elf32_file_header
{
    helf_e_ident_t      e_ident;        /* ELF "magic number" */
    uint16_t            e_type;         /* Identifies object file type */
    uint16_t            e_machine;      /* Specifies required architecture */
    uint32_t            e_version;      /* Identifies object file version */
    uint32_t            e_entry;        /* Entry point virtual address */
    uint32_t            e_phoff;        /* Program header table file offset */
    uint32_t            e_shoff;        /* Section header table file offset */
    uint32_t            e_flags;        /* Processor-specific flags */
    uint16_t            e_ehsize;       /* ELF header size in bytes */
    uint16_t            e_phentsize;    /* Program header table entry size */
    uint16_t            e_phnum;        /* Program header table entry count */
    uint16_t            e_shentsize;    /* Section header table entry size */
    uint16_t            e_shnum;        /* Section header table entry count */
    uint16_t            e_shstrndx;     /* Section header string table index */
};

/** \brief ELF读取32位头
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param header helf_elf32_file_header_t* 32位文件头
 * \return bool 是否读取成功
 *
 */
bool helf_file_input_32_bits_header_get(helf_file_input_t *input_file,helf_elf32_file_header_t *header);

typedef struct helf_elf32_program_header helf_elf32_program_header_t;
struct helf_elf32_program_header
{
    uint32_t	p_type;		    /* Identifies program segment type */
    uint32_t	p_offset;		/* Segment file offset */
    uint32_t	p_vaddr;		/* Segment virtual address */
    uint32_t	p_paddr;		/* Segment physical address */
    uint32_t	p_filesz;		/* Segment size in file */
    uint32_t	p_memsz;		/* Segment size in memory */
    uint32_t	p_flags;		/* Segment flags */
    uint32_t	p_align;		/* Segment alignment, file & memory */
};

typedef struct helf_elf32_section_header helf_elf32_section_header_t;
struct helf_elf32_section_header
{
    uint32_t	sh_name;		    /* Section name, index in string tbl */
    uint32_t	sh_type;		    /* Type of section */
    uint32_t	sh_flags;		    /* Miscellaneous section attributes */
    uint32_t	sh_addr;		    /* Section virtual addr at execution */
    uint32_t	sh_offset;		    /* Section file offset */
    uint32_t	sh_size;		    /* Size of section in bytes */
    uint32_t	sh_link;		    /* Index of another section */
    uint32_t	sh_info;		    /* Additional section information */
    uint32_t	sh_addralign;	    /* Section alignment */
    uint32_t	sh_entsize;		    /* Entry size if section holds table */
};

typedef struct helf_elf32_symbol_header helf_elf32_symbol_header_t;
struct helf_elf32_symbol_header
{
    uint32_t  st_name;                /* Symbol name (string tbl index) */
    uint32_t  st_value;               /* Symbol value */
    uint32_t  st_size;                /* Symbol size */
    uint8_t   st_info;                /* Symbol type and binding */
    uint8_t   st_other;               /* Symbol visibility */
    uint16_t  st_shndx;               /* Section index */
};

/** \brief ELF文件是否为64位
 *
 * \param input helf_file_input_t* 输入文件
 * \return bool 是否为64位
 *
 */
bool helf_file_input_is_64_bits(helf_file_input_t *input_file);

typedef struct helf_elf64_file_header helf_elf64_file_header_t;
struct helf_elf64_file_header
{
    helf_e_ident_t  e_ident;            /* ELF "magic number" */
    uint16_t        e_type;             /* Identifies object file type */
    uint16_t        e_machine;          /* Specifies required architecture */
    uint32_t        e_version;          /* Identifies object file version */
    uint64_t        e_entry;            /* Entry point virtual address */
    uint64_t        e_phoff;            /* Program header table file offset */
    uint64_t        e_shoff;            /* Section header table file offset */
    uint32_t        e_flags;            /* Processor-specific flags */
    uint16_t        e_ehsize;           /* ELF header size in bytes */
    uint16_t        e_phentsize;        /* Program header table entry size */
    uint16_t        e_phnum;            /* Program header table entry count */
    uint16_t        e_shentsize;        /* Section header table entry size */
    uint16_t        e_shnum;            /* Section header table entry count */
    uint16_t        e_shstrndx;         /* Section header string table index */
};

typedef struct helf_elf64_program_header helf_elf64_program_header_t;
struct helf_elf64_program_header
{
    uint32_t	p_type;		    /* Identifies program segment type */
    uint32_t	p_flags;		/* Segment flags */
    uint64_t	p_offset;		/* Segment file offset */
    uint64_t	p_vaddr;		/* Segment virtual address */
    uint64_t	p_paddr;		/* Segment physical address */
    uint64_t	p_filesz;		/* Segment size in file */
    uint64_t	p_memsz;		/* Segment size in memory */
    uint64_t	p_align;		/* Segment alignment, file & memory */
};

typedef struct helf_elf64_section_header helf_elf64_section_header_t;
struct helf_elf64_section_header
{
    uint32_t	sh_name;		/* Section name, index in string tbl */
    uint32_t	sh_type;		/* Type of section */
    uint64_t	sh_flags;		/* Miscellaneous section attributes */
    uint64_t	sh_addr;		/* Section virtual addr at execution */
    uint64_t	sh_offset;		/* Section file offset */
    uint64_t	sh_size;		/* Size of section in bytes */
    uint32_t	sh_link;		/* Index of another section */
    uint32_t	sh_info;		/* Additional section information */
    uint64_t	sh_addralign;	/* Section alignment */
    uint64_t	sh_entsize;		/* Entry size if section holds table */
};

typedef struct helf_elf64_symbol_header helf_elf64_symbol_header_t;
struct helf_elf64_symbol_header
{
    uint32_t st_name;                 /* Symbol name (string tbl index) */
    uint8_t st_info;                  /* Symbol type and binding */
    uint8_t st_other;                 /* Symbol visibility */
    uint16_t st_shndx;                /* Section index */
    uint64_t st_value;                /* Symbol value */
    uint64_t st_size;                 /* Symbol size */
};

/** \brief ELF读取64位头
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param header helf_elf64_file_header_t* 64位文件头
 * \return bool 是否读取成功
 *
 */
bool helf_file_input_64_bits_header_get(helf_file_input_t *input_file,helf_elf64_file_header_t *header);

/*
 * e_type
 */
#define HELF_ET_NONE        0           /* Unknown type. */
#define HELF_ET_REL         1           /* Relocatable. */
#define HELF_ET_EXEC        2           /* Executable. */
#define HELF_ET_DYN         3           /* Shared object. */
#define HELF_ET_CORE        4           /* Core file. */
#define HELF_ET_LOOS        0xfe00      /* First operating system specific. */
#define HELF_ET_HIOS        0xfeff      /* Last operating system-specific. */
#define HELF_ET_LOPROC      0xff00      /* First processor-specific. */
#define HELF_ET_HIPROC      0xffff      /* Last processor-specific. */

/*
 * e_machine
 */
#define HELF_EM_NONE        0               /* Unknown machine. */
#define HELF_EM_M32         1               /* AT&T WE32100. */
#define HELF_EM_SPARC       2               /* Sun SPARC. */
#define HELF_EM_386         3               /* Intel i386. */
#define HELF_EM_68K         4               /* Motorola 68000. */
#define HELF_EM_88K         5               /* Motorola 88000. */
#define HELF_EM_IAMCU       6               /* Intel MCU. */
#define HELF_EM_860         7               /* Intel i860. */
#define HELF_EM_MIPS        8               /* MIPS R3000 Big-Endian only. */
#define HELF_EM_S370        9               /* IBM System/370. */
#define HELF_EM_MIPS_RS3_LE 10              /* MIPS R3000 Little-Endian. */
#define HELF_EM_PARISC      15              /* HP PA-RISC. */
#define HELF_EM_VPP500      17              /* Fujitsu VPP500. */
#define HELF_EM_SPARC32PLUS 18              /* SPARC v8plus. */
#define HELF_EM_960         19              /* Intel 80960. */
#define HELF_EM_PPC         20              /* PowerPC 32-bit. */
#define HELF_EM_PPC64       21              /* PowerPC 64-bit. */
#define HELF_EM_S390        22              /* IBM System/390. */
#define HELF_EM_V800        36              /* NEC V800. */
#define HELF_EM_FR20        37              /* Fujitsu FR20. */
#define HELF_EM_RH32        38              /* TRW RH-32. */
#define HELF_EM_RCE         39              /* Motorola RCE. */
#define HELF_EM_ARM         40              /* ARM. */
#define HELF_EM_SH          42              /* Hitachi SH. */
#define HELF_EM_SPARCV9     43              /* SPARC v9 64-bit. */
#define HELF_EM_TRICORE     44              /* Siemens TriCore embedded processor. */
#define HELF_EM_ARC         45              /* Argonaut RISC Core. */
#define HELF_EM_H8_300      46              /* Hitachi H8/300. */
#define HELF_EM_H8_300H     47              /* Hitachi H8/300H. */
#define HELF_EM_H8S         48              /* Hitachi H8S. */
#define HELF_EM_H8_500      49              /* Hitachi H8/500. */
#define HELF_EM_IA_64       50              /* Intel IA-64 Processor. */
#define HELF_EM_MIPS_X      51              /* Stanford MIPS-X. */
#define HELF_EM_COLDFIRE    52              /* Motorola ColdFire. */
#define HELF_EM_68HC12      53              /* Motorola M68HC12. */
#define HELF_EM_MMA         54              /* Fujitsu MMA. */
#define HELF_EM_PCP         55              /* Siemens PCP. */
#define HELF_EM_NCPU        56              /* Sony nCPU. */
#define HELF_EM_NDR1        57              /* Denso NDR1 microprocessor. */
#define HELF_EM_STARCORE    58              /* Motorola Star*Core processor. */
#define HELF_EM_ME16        59              /* Toyota ME16 processor. */
#define HELF_EM_ST100       60              /* STMicroelectronics ST100 processor. */
#define HELF_EM_TINYJ       61              /* Advanced Logic Corp. TinyJ processor. */
#define HELF_EM_X86_64      62              /* Advanced Micro Devices x86-64 */
#define HELF_EM_AMD64       HELF_EM_X86_64  /* Advanced Micro Devices x86-64 (compat) */
#define HELF_EM_PDSP        63              /* Sony DSP Processor. */
#define HELF_EM_FX66        66              /* Siemens FX66 microcontroller. */
#define HELF_EM_ST9PLUS     67              /* STMicroelectronics ST9+ 8/16 microcontroller. */
#define HELF_EM_ST7         68              /* STmicroelectronics ST7 8-bit  microcontroller. */
#define HELF_EM_68HC16      69              /* Motorola MC68HC16 microcontroller. */
#define HELF_EM_68HC11      70              /* Motorola MC68HC11 microcontroller. */
#define HELF_EM_68HC08      71              /* Motorola MC68HC08 microcontroller. */
#define HELF_EM_68HC05      72              /* Motorola MC68HC05 microcontroller. */
#define HELF_EM_SVX         73              /* Silicon Graphics SVx. */
#define HELF_EM_ST19        74              /* STMicroelectronics ST19 8-bit mc. */
#define HELF_EM_VAX         75              /* Digital VAX. */
#define HELF_EM_CRIS        76              /* Axis Communications 32-bit embedded   processor. */
#define HELF_EM_JAVELIN     77              /* Infineon Technologies 32-bit embedded processor. */
#define HELF_EM_FIREPATH    78              /* Element 14 64-bit DSP Processor. */
#define HELF_EM_ZSP         79              /* LSI Logic 16-bit DSP Processor. */
#define HELF_EM_MMIX        80              /* Donald Knuth's educational 64-bit proc. */
#define HELF_EM_HUANY       81              /* Harvard University machine-independent  object files. */
#define HELF_EM_PRISM       82              /* SiTera Prism. */
#define HELF_EM_AVR         83              /* Atmel AVR 8-bit microcontroller. */
#define HELF_EM_FR30        84              /* Fujitsu FR30. */
#define HELF_EM_D10V        85              /* Mitsubishi D10V. */
#define HELF_EM_D30V        86              /* Mitsubishi D30V. */
#define HELF_EM_V850        87              /* NEC v850. */
#define HELF_EM_M32R        88              /* Mitsubishi M32R. */
#define HELF_EM_MN10300     89              /* Matsushita MN10300. */
#define HELF_EM_MN10200     90              /* Matsushita MN10200. */
#define HELF_EM_PJ          91              /* picoJava. */
#define HELF_EM_OPENRISC    92              /* OpenRISC 32-bit embedded processor. */
#define HELF_EM_ARC_A5      93              /* ARC Cores Tangent-A5. */
#define HELF_EM_XTENSA      94              /* Tensilica Xtensa Architecture. */
#define HELF_EM_VIDEOCORE   95              /* Alphamosaic VideoCore processor. */
#define HELF_EM_TMM_GPP     96              /* Thompson Multimedia General Purpose   Processor. */
#define HELF_EM_NS32K       97              /* National Semiconductor 32000 series. */
#define HELF_EM_TPC         98              /* Tenor Network TPC processor. */
#define HELF_EM_SNP1K       99              /* Trebia SNP 1000 processor. */
#define HELF_EM_ST200       100             /* STMicroelectronics ST200 microcontroller. */
#define HELF_EM_IP2K        101             /* Ubicom IP2xxx microcontroller family. */
#define HELF_EM_MAX         102             /* MAX Processor. */
#define HELF_EM_CR          103             /* National Semiconductor CompactRISC  microprocessor. */
#define HELF_EM_F2MC16      104             /* Fujitsu F2MC16. */
#define HELF_EM_MSP430      105             /* Texas Instruments embedded microcontroller  msp430. */
#define HELF_EM_BLACKFIN    106             /* Analog Devices Blackfin (DSP) processor. */
#define HELF_EM_SE_C33      107             /* S1C33 Family of Seiko Epson processors. */
#define HELF_EM_SEP         108             /* Sharp embedded microprocessor. */
#define HELF_EM_ARCA        109             /* Arca RISC Microprocessor. */
#define HELF_EM_UNICORE     110             /* Microprocessor series from PKU-Unity Ltd. and MPRC of Peking University */
#define HELF_EM_AARCH64     183             /* AArch64 (64-bit ARM) */
#define HELF_EM_RISCV       243             /* RISC-V */
/* Non-standard or deprecated. */
#define HELF_EM_486         6               /* Intel i486. */
#define HELF_EM_MIPS_RS4_BE 10              /* MIPS R4000 Big-Endian */
#define HELF_EM_ALPHA_STD   41              /* Digital Alpha (standard value). */
#define HELF_EM_ALPHA       0x9026          /* Alpha (written in the absence of an ABI) */



#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HELF_COMMON_H__
