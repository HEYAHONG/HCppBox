#ifndef __SYSCON_H__
#define __SYSCON_H__
#include "stdint.h"
#include "stdlib.h"

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined(SYSCON)
#define SYSCON (*(volatile uint32_t *)0x11100000)
#endif

#define SYSCON_HALT()       do { SYSCON = 0x5555; } while(0)
#define SYSCON_REBOOT()     do { SYSCON = 0x7777; } while(0)

#ifdef __cplusplus
}
#endif

#endif // __SYSCON_H__

