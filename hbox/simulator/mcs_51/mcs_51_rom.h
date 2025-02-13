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

#ifdef __cplusplus
extern "C"
{
#endif

/*
 *  helloworld程序(见rom/helloworld目录)
 */
extern const unsigned char  hs_mcs_51_rom_helloworld[];
extern unsigned int         hs_mcs_51_rom_helloworld_len;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HS_MCS_51_ROM_H__

