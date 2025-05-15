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
 * \param fileheader_bytes_length size_t    COFF文件头大小
 * \return bool 是否成功读取
 *
 */
bool hcoff_fileheader_read(hcoff_fileheader_t *fileheader,uint8_t* fileheader_bytes,size_t fileheader_bytes_length);

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



#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HCOFF_COMMON_H__
