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
 * 重定位的类型根据架构的不同而不同，下面列出一些常用的类型(X86_64、I386)
 */
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


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HCOFF_COMMON_H__
