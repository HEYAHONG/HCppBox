#if !defined(Z80_CONFIG_H)
#define Z80_CONFIG_H 1
#include "stdint.h"
#include "stdlib.h"

/*
 * Arm Compiler 5
 */
#if defined(__ARMCC_VERSION) && !defined(__clang__)
#define Z_COMPILER      Z_COMPILER_UNKNOWN
/*
 * 默认32位,64位需要单独定义
 */
#if (!defined(Z_COMPILER_64_BITS)) &&  ((SIZE_MAX) <= (UINT32_MAX))
#define Z_COMPILER_ISA  Z_ISA_AARCH32
#define Z_COMPILER_DATA_MODEL Z_DATA_MODEL_ILP32
#else
#define Z_COMPILER_ISA  Z_ISA_AARCH64
#define Z_COMPILER_DATA_MODEL Z_DATA_MODEL_LP64
#endif
/*
 * 默认小端模式，大端模式需要单独定义
 */
#if !defined(Z_COMPILER_BIG_ENDIANNESS)
#define Z_COMPILER_ISA_INTEGRAL_ENDIANNESS Z_ENDIANNESS_LITTLE
#define Z_COMPILER_BIT_FIELD_ORDER_ALL     Z_ORDER_REVERSED
#else
#define Z_COMPILER_ISA_INTEGRAL_ENDIANNESS Z_ENDIANNESS_BIG
#define Z_COMPILER_BIT_FIELD_ORDER_ALL     Z_ORDER_NORMAL
#endif

#endif

/*
 *  修复Windows下的警告
 */
#if defined(WIN32) || defined(_WIN32)
#undef IN
#undef OUT
#endif

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
