
#ifndef __SIGNAL_HELPER_H__
#define __SIGNAL_HELPER_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
#include "stdbool.h"

void sigint_init(void);

void sigint_reg(void (*sigint_handler)(void));

#ifdef __cplusplus
}
#endif

#endif // __SIGNAL_HELPER_H__
