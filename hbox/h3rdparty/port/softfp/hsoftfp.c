/***************************************************************
 * Name:      hsoftfp.c
 * Purpose:   实现hsoftfp接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hsoftfp.h"
#include H3RDPARTY_SOFTFP_HEADER

hsoftfp_float32_t hsoftfp_add_sf32(hsoftfp_float32_t a,hsoftfp_float32_t b,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return add_sf32(a,b,(RoundingModeEnum)rm,pfflags);
}

hsoftfp_float32_t hsoftfp_sub_sf32(hsoftfp_float32_t a,hsoftfp_float32_t b,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return sub_sf32(a,b,(RoundingModeEnum)rm,pfflags);
}

hsoftfp_float32_t hsoftfp_mul_sf32(hsoftfp_float32_t a,hsoftfp_float32_t b,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return mul_sf32(a,b,(RoundingModeEnum)rm,pfflags);
}

hsoftfp_float32_t hsoftfp_div_sf32(hsoftfp_float32_t a,hsoftfp_float32_t b,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return div_sf32(a,b,(RoundingModeEnum)rm,pfflags);
}

hsoftfp_float32_t hsoftfp_sqrt_sf32(hsoftfp_float32_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return sqrt_sf32(a,(RoundingModeEnum)rm,pfflags);
}

hsoftfp_float32_t hsoftfp_fma_sf32(hsoftfp_float32_t a,hsoftfp_float32_t b,hsoftfp_float32_t c,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return fma_sf32(a,b,c,(RoundingModeEnum)rm,pfflags);
}

hsoftfp_float32_t hsoftfp_min_sf32(hsoftfp_float32_t a,hsoftfp_float32_t b,uint32_t *pfflags)
{
    return min_sf32(a,b,pfflags);
}

hsoftfp_float32_t hsoftfp_max_sf32(hsoftfp_float32_t a,hsoftfp_float32_t b,uint32_t *pfflags)
{
    return max_sf32(a,b,pfflags);
}

int hsoftfp_eq_quiet_sf32(hsoftfp_float32_t a,hsoftfp_float32_t b,uint32_t *pfflags)
{
    return eq_quiet_sf32(a,b,pfflags);
}

int hsoftfp_le_sf32(hsoftfp_float32_t a,hsoftfp_float32_t b,uint32_t *pfflags)
{
    return le_sf32(a,b,pfflags);
}

int hsoftfp_lt_sf32(hsoftfp_float32_t a,hsoftfp_float32_t b,uint32_t *pfflags)
{
    return lt_sf32(a,b,pfflags);
}

uint32_t hsoftfp_fclass_sf32(hsoftfp_float32_t a)
{
    return fclass_sf32(a);
}

hsoftfp_float64_t hsoftfp_cvt_sf32_sf64(hsoftfp_float32_t a,uint32_t *pfflags)
{
    return cvt_sf32_sf64(a,pfflags);
}

hsoftfp_float32_t hsoftfp_cvt_sf64_sf32(hsoftfp_float64_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return cvt_sf64_sf32(a,(RoundingModeEnum)rm,pfflags);
}

int32_t hsoftfp_cvt_sf32_i32(hsoftfp_float32_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return cvt_sf32_i32(a,(RoundingModeEnum)rm,pfflags);
}

uint32_t hsoftfp_cvt_sf32_u32(hsoftfp_float32_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return cvt_sf32_u32(a,(RoundingModeEnum)rm,pfflags);
}

int64_t hsoftfp_cvt_sf32_i64(hsoftfp_float32_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return cvt_sf32_i64(a,(RoundingModeEnum)rm,pfflags);
}

uint64_t hsoftfp_cvt_sf32_u64(hsoftfp_float32_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return cvt_sf32_u64(a,(RoundingModeEnum)rm,pfflags);
}

hsoftfp_float32_t hsoftfp_cvt_i32_sf32(int32_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return cvt_i32_sf32(a,(RoundingModeEnum)rm,pfflags);
}

hsoftfp_float32_t hsoftfp_cvt_u32_sf32(uint32_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return cvt_u32_sf32(a,(RoundingModeEnum)rm,pfflags);
}

hsoftfp_float32_t hsoftfp_cvt_i64_sf32(int64_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return cvt_i64_sf32(a,(RoundingModeEnum)rm,pfflags);
}

hsoftfp_float32_t hsoftfp_cvt_u64_sf32(uint64_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return cvt_u64_sf32(a,(RoundingModeEnum)rm,pfflags);
}

hsoftfp_float64_t hsoftfp_add_sf64(hsoftfp_float64_t a,hsoftfp_float64_t b,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return add_sf64(a,b,(RoundingModeEnum)rm,pfflags);
}

hsoftfp_float64_t hsoftfp_sub_sf64(hsoftfp_float64_t a,hsoftfp_float64_t b,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return sub_sf64(a,b,(RoundingModeEnum)rm,pfflags);
}

hsoftfp_float64_t hsoftfp_mul_sf64(hsoftfp_float64_t a,hsoftfp_float64_t b,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return mul_sf64(a,b,(RoundingModeEnum)rm,pfflags);
}

hsoftfp_float64_t hsoftfp_div_sf64(hsoftfp_float64_t a,hsoftfp_float64_t b,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return div_sf64(a,b,(RoundingModeEnum)rm,pfflags);
}

hsoftfp_float64_t hsoftfp_sqrt_sf64(hsoftfp_float64_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return sqrt_sf64(a,(RoundingModeEnum)rm,pfflags);
}

hsoftfp_float64_t hsoftfp_fma_sf64(hsoftfp_float64_t a,hsoftfp_float64_t b,hsoftfp_float64_t c,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return fma_sf64(a,b,c,(RoundingModeEnum)rm,pfflags);
}

hsoftfp_float64_t hsoftfp_min_sf64(hsoftfp_float64_t a,hsoftfp_float64_t b,uint32_t *pfflags)
{
    return min_sf64(a,b,pfflags);
}

hsoftfp_float64_t hsoftfp_max_sf64(hsoftfp_float64_t a,hsoftfp_float64_t b,uint32_t *pfflags)
{
    return max_sf64(a,b,pfflags);
}

int hsoftfp_eq_quiet_sf64(hsoftfp_float64_t a,hsoftfp_float64_t b,uint32_t *pfflags)
{
    return eq_quiet_sf64(a,b,pfflags);
}

int hsoftfp_le_sf64(hsoftfp_float64_t a,hsoftfp_float64_t b,uint32_t *pfflags)
{
    return le_sf64(a,b,pfflags);
}

int hsoftfp_lt_sf64(hsoftfp_float64_t a,hsoftfp_float64_t b,uint32_t *pfflags)
{
    return lt_sf64(a,b,pfflags);
}

uint32_t hsoftfp_fclass_sf64(hsoftfp_float64_t a)
{
    return fclass_sf64(a);
}

int32_t hsoftfp_cvt_sf64_i32(hsoftfp_float64_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return cvt_sf64_i32(a,(RoundingModeEnum)rm,pfflags);
}

uint32_t hsoftfp_cvt_sf64_u32(hsoftfp_float64_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return cvt_sf64_u32(a,(RoundingModeEnum)rm,pfflags);
}

int64_t hsoftfp_cvt_sf64_i64(hsoftfp_float64_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return cvt_sf64_i64(a,(RoundingModeEnum)rm,pfflags);
}

uint64_t hsoftfp_cvt_sf64_u64(hsoftfp_float64_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return cvt_sf64_u64(a,(RoundingModeEnum)rm,pfflags);
}

hsoftfp_float64_t hsoftfp_cvt_i32_sf64(int32_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return cvt_i32_sf64(a,(RoundingModeEnum)rm,pfflags);
}

hsoftfp_float64_t hsoftfp_cvt_u32_sf64(uint32_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return cvt_u32_sf64(a,(RoundingModeEnum)rm,pfflags);
}

hsoftfp_float64_t hsoftfp_cvt_i64_sf64(int64_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return cvt_i64_sf64(a,(RoundingModeEnum)rm,pfflags);
}

hsoftfp_float64_t hsoftfp_cvt_u64_sf64(uint64_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags)
{
    return cvt_u64_sf64(a,(RoundingModeEnum)rm,pfflags);
}
