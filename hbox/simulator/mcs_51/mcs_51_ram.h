/***************************************************************
 * Name:      mcs_51_ram.h
 * Purpose:   声明mcs_51_ram接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-02-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HS_MCS_51_RAM_H__
#define __HS_MCS_51_RAM_H__
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "mcs_51_core.h"
#ifdef __cplusplus
extern "C"
{
#endif


/** \brief 极简模型，注意：只能选取一种作为最终使用的模型
 *
 *
 */
struct hs_mcs_51_ram_model_tiny;
typedef struct hs_mcs_51_ram_model_tiny  hs_mcs_51_ram_model_tiny_t;
struct hs_mcs_51_ram_model_tiny
{
    uint8_t ram[256];                   //内部128字节+SFR
};


/** \brief  MCS-51 RAM总线IO操作(一般由总线上的主设备(如CPU)调用),注意：需要在外设操作前调用
 *
  * \param core hs_mcs_51_core_t*                   MCS-51内核指针，见hs_mcs_51_io_t。
 * \param address uint16_t                          地址，见hs_mcs_51_io_t。
 * \param opt hs_mcs_51_io_opt_t                    IO操作选项，见hs_mcs_51_io_t。
 * \param data uint8_t*                             数据长度，见hs_mcs_51_io_t。
 * \param length uint16_t                           数据长度，见hs_mcs_51_io_t。
 * \param usr void*                                 用户指针，见hs_mcs_51_io_t。
 * \param ram hs_mcs_51_ram_model_tiny_t*           MCS-51 RAM指针
 *
 */
void hs_mcs_51_ram_model_tiny_bus_io(hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr,hs_mcs_51_ram_model_tiny_t*  ram);


/** \brief small模型，注意：只能选取一种作为最终使用的模型
 *
 *
 */
struct hs_mcs_51_ram_model_small;
typedef struct hs_mcs_51_ram_model_small  hs_mcs_51_ram_model_small_t;
struct hs_mcs_51_ram_model_small
{
    hs_mcs_51_ram_model_tiny_t base;
    uint8_t ram[128];                  //内部RAM高128字节
};

/** \brief  MCS-51 RAM总线IO操作(一般由总线上的主设备(如CPU)调用),注意：需要在外设操作前调用
 *
  * \param core hs_mcs_51_core_t*                   MCS-51内核指针，见hs_mcs_51_io_t。
 * \param address uint16_t                          地址，见hs_mcs_51_io_t。
 * \param opt hs_mcs_51_io_opt_t                    IO操作选项，见hs_mcs_51_io_t。
 * \param data uint8_t*                             数据长度，见hs_mcs_51_io_t。
 * \param length uint16_t                           数据长度，见hs_mcs_51_io_t。
 * \param usr void*                                 用户指针，见hs_mcs_51_io_t。
 * \param ram hs_mcs_51_ram_model_small_t*          MCS-51 RAM指针
 *
 */
void hs_mcs_51_ram_model_small_bus_io(hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr,hs_mcs_51_ram_model_small_t*  ram);


/** \brief medium模型，注意：只能选取一种作为最终使用的模型
 *
 *
 */
struct hs_mcs_51_ram_model_medium;
typedef struct hs_mcs_51_ram_model_medium  hs_mcs_51_ram_model_medium_t;
struct hs_mcs_51_ram_model_medium
{
    hs_mcs_51_ram_model_small_t base;
    uint8_t xram[256];                  //外部低256字节
};


/** \brief  MCS-51 RAM总线IO操作(一般由总线上的主设备(如CPU)调用),注意：需要在外设操作前调用
 *
  * \param core hs_mcs_51_core_t*                   MCS-51内核指针，见hs_mcs_51_io_t。
 * \param address uint16_t                          地址，见hs_mcs_51_io_t。
 * \param opt hs_mcs_51_io_opt_t                    IO操作选项，见hs_mcs_51_io_t。
 * \param data uint8_t*                             数据长度，见hs_mcs_51_io_t。
 * \param length uint16_t                           数据长度，见hs_mcs_51_io_t。
 * \param usr void*                                 用户指针，见hs_mcs_51_io_t。
 * \param ram hs_mcs_51_ram_model_medium_t*         MCS-51 RAM指针
 *
 */
void hs_mcs_51_ram_model_medium_bus_io(hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr,hs_mcs_51_ram_model_medium_t*  ram);

/** \brief large模型，注意：只能选取一种作为最终使用的模型
 *
 *
 */
struct hs_mcs_51_ram_model_large;
typedef struct hs_mcs_51_ram_model_large  hs_mcs_51_ram_model_large_t;
struct hs_mcs_51_ram_model_large
{
    hs_mcs_51_ram_model_medium_t base;
    uint8_t xram[64*1024-256];          //外部64K字节(除开低256字节)
};

/** \brief  MCS-51 RAM总线IO操作(一般由总线上的主设备(如CPU)调用),注意：需要在外设操作前调用
 *
  * \param core hs_mcs_51_core_t*                   MCS-51内核指针，见hs_mcs_51_io_t。
 * \param address uint16_t                          地址，见hs_mcs_51_io_t。
 * \param opt hs_mcs_51_io_opt_t                    IO操作选项，见hs_mcs_51_io_t。
 * \param data uint8_t*                             数据长度，见hs_mcs_51_io_t。
 * \param length uint16_t                           数据长度，见hs_mcs_51_io_t。
 * \param usr void*                                 用户指针，见hs_mcs_51_io_t。
 * \param ram hs_mcs_51_ram_model_large_t*          MCS-51 RAM指针
 *
 */
void hs_mcs_51_ram_model_large_bus_io(hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr,hs_mcs_51_ram_model_large_t*  ram);

/** \brief huge模型，注意：只能选取一种作为最终使用的模型
 *
 *
 */
struct hs_mcs_51_ram_model_huge;
typedef struct hs_mcs_51_ram_model_huge  hs_mcs_51_ram_model_huge_t;
struct hs_mcs_51_ram_model_huge
{
    hs_mcs_51_ram_model_large_t base;
};

/** \brief  MCS-51 RAM总线IO操作(一般由总线上的主设备(如CPU)调用),注意：需要在外设操作前调用
 *
  * \param core hs_mcs_51_core_t*                   MCS-51内核指针，见hs_mcs_51_io_t。
 * \param address uint16_t                          地址，见hs_mcs_51_io_t。
 * \param opt hs_mcs_51_io_opt_t                    IO操作选项，见hs_mcs_51_io_t。
 * \param data uint8_t*                             数据长度，见hs_mcs_51_io_t。
 * \param length uint16_t                           数据长度，见hs_mcs_51_io_t。
 * \param usr void*                                 用户指针，见hs_mcs_51_io_t。
 * \param ram hs_mcs_51_ram_model_huge_t*           MCS-51 RAM指针
 *
 */
void hs_mcs_51_ram_model_huge_bus_io(hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr,hs_mcs_51_ram_model_huge_t*  ram);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HS_MCS_51_RAM_H__

