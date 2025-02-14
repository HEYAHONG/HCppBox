/***************************************************************
 * Name:      mcs_51_rom.h
 * Purpose:   声明mcs_51_rom接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-02-13
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HS_MCS_51_ROM_H__
#define __HS_MCS_51_ROM_H__
#include "stdio.h"
#include "string.h"
#include "mcs_51_core.h"
#ifdef __cplusplus
extern "C"
{
#endif

struct hs_mcs_51_rom;
typedef struct hs_mcs_51_rom hs_mcs_51_rom_t;

struct hs_mcs_51_rom
{
    const unsigned char *code;  /**< 程序指针 */
    unsigned int  len;          /**< 程序长度 */
};

/** \brief MCS-51 ROM初始化参数
 *
 *
 */
#define HS_MCS_51_ROM_INITIALIZER {NULL,0}

/** \brief  MCS-51 ROM总线IO操作(一般由总线上的主设备(如CPU)调用)
 *
  * \param core hs_mcs_51_core_t*       MCS-51内核指针，见hs_mcs_51_io_t。
 * \param address uint16_t              地址，见hs_mcs_51_io_t。
 * \param opt hs_mcs_51_io_opt_t        IO操作选项，见hs_mcs_51_io_t。
 * \param data uint8_t*                 数据长度，见hs_mcs_51_io_t。
 * \param length uint16_t               数据长度，见hs_mcs_51_io_t。
 * \param usr void*                     用户指针，见hs_mcs_51_io_t。
 * \param rom hs_mcs_51_rom_t*          MCS-51 ROM指针,当指针为空时将跳转至复位地址。
 *
 */
void hs_mcs_51_rom_bus_io(hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr,hs_mcs_51_rom_t *rom);


/*
 *  helloworld程序(见rom/helloworld目录)
 */
extern const hs_mcs_51_rom_t hs_mcs_51_rom_helloworld;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HS_MCS_51_ROM_H__

