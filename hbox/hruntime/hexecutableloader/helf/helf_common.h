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


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HELF_COMMON_H__
