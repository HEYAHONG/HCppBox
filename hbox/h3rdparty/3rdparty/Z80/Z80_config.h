#if !defined(Z80_CONFIG_H)
#define Z80_CONFIG_H 1
#include "stdint.h"
#include "stdlib.h"


//启用静态API
#define Z80_STATIC   1

//使用本地头文件
#define Z80_WITH_LOCAL_HEADER 1

//配置Z80库
#define Z80_WITH_EXECUTE                    1
#define Z80_WITH_FULL_IM0                   1
#define Z80_WITH_IM0_RETX_NOTIFICATIONS     1
#define Z80_WITH_PARITY_COMPUTATION         1
#define Z80_WITH_PRECOMPUTED_DAA            1
#define Z80_WITH_Q                          1
#define Z80_WITH_SPECIAL_RESET              1
#define Z80_WITH_UNOFFICIAL_RETI            1
#define Z80_WITH_ZILOG_NMOS_LD_A_IR_BUG     1

//Zeta库
#include "Z/macros/language.h"
#include "Z/types/bitwise.h"
#include "Z/constants/pointer.h"
#include "Z/macros/bitwise.h"
#include "Z/macros/structure.h"


#endif
