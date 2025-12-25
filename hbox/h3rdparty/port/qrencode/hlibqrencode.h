/***************************************************************
 * Name:      hlibqrencode.h
 * Purpose:   声明hlibqrencode接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-12-25
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HLIBQRENCODE_H_INCLUDE__
#define __HLIBQRENCODE_H_INCLUDE__
#include "h3rdparty_port_qrencode.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


typedef enum
{
    HLIBQRENCODE_QR_CODE_NUL = -1,   ///< Terminator (NUL character). Internal use only
    HLIBQRENCODE_QR_CODE_NUM = 0,    ///< Numeric mode
    HLIBQRENCODE_QR_CODE_AN,         ///< Alphabet-numeric mode
    HLIBQRENCODE_QR_CODE_8,          ///< 8-bit data mode
    HLIBQRENCODE_QR_CODE_KANJI,      ///< Kanji (shift-jis) mode
    HLIBQRENCODE_QR_CODE_STRUCTURE,  ///< Internal use only
    HLIBQRENCODE_QR_CODE_ECI,        ///< ECI mode
    HLIBQRENCODE_QR_CODE_FNC1FIRST,  ///< FNC1, first position
    HLIBQRENCODE_QR_CODE_FNC1SECOND, ///< FNC1, second position
} hlibqrencode_encode_mode_t;

typedef enum
{
    HLIBQRENCODE_QR_ECLEVEL_L = 0, ///< lowest
    HLIBQRENCODE_QR_ECLEVEL_M,
    HLIBQRENCODE_QR_ECLEVEL_Q,
    HLIBQRENCODE_QR_ECLEVEL_H      ///< highest
} hlibqrencode_eclevel_t;

struct hlibqrencode_qrcode;
typedef struct hlibqrencode_qrcode hlibqrencode_qrcode_t;
struct hlibqrencode_qrcode
{
    int version;         ///< version of the symbol
    int width;           ///< width of the symbol
    unsigned char *data; ///< symbol data
    /*
      MSB 76543210 LSB
       |||||||`- 1=black/0=white
       ||||||`-- 1=ecc/0=data code area
       |||||`--- format information
       ||||`---- version information
       |||`----- timing pattern
       ||`------ alignment pattern
       |`------- finder pattern and separator
       `-------- non-data modules (format, timing, etc.)
    */
};

hlibqrencode_qrcode_t *hlibqrencode_encode_string(const char *string, int version, hlibqrencode_eclevel_t level, hlibqrencode_encode_mode_t hint, int casesensitive);

hlibqrencode_qrcode_t *hlibqrencode_encode_string_8bit(const char *string, int version, hlibqrencode_eclevel_t level);

hlibqrencode_qrcode_t *hlibqrencode_encode_string_MQR(const char *string, int version, hlibqrencode_eclevel_t level, hlibqrencode_encode_mode_t hint, int casesensitive);

hlibqrencode_qrcode_t *hlibqrencode_encode_string8bit_MQR(const char *string, int version, hlibqrencode_eclevel_t level);

hlibqrencode_qrcode_t *hlibqrencode_encode_data(int size, const unsigned char *data, int version, hlibqrencode_eclevel_t level);

hlibqrencode_qrcode_t *hlibqrencode_encode_data_MQR(int size, const unsigned char *data, int version, hlibqrencode_eclevel_t level);

void hlibqrencode_free(hlibqrencode_qrcode_t *qrcode);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HLIBQRENCODE_H_INCLUDE__
