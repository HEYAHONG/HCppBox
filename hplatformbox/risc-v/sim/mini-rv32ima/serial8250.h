#ifndef __SERIAL8250_H__
#define __SERIAL8250_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"
#include "stdlib.h"

#if !defined(CONSOLE_8250_BASE)
#define CONSOLE_8250_BASE (0x10000000)
#endif

typedef struct
{
    volatile uint8_t rbr_thr;
    volatile uint8_t ier;
    volatile uint8_t iir_fcr;
    volatile uint8_t lcr;
    volatile uint8_t mcr;
    volatile uint8_t lsr;
    volatile uint8_t msr;
    volatile uint8_t scr;
} serial8250_t;

int serial8250_getchar(volatile serial8250_t *serial);
int serial8250_putchar(volatile serial8250_t *serial,uint8_t ch);

#ifdef __cplusplus
}
#endif

#endif // __SERIAL8250_H__

