/***************************************************************
 * Name:      hlibqrencode.c
 * Purpose:   实现hlibqrencode接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-12-25
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hlibqrencode.h"
#include H3RDPARTY_LIBQRENCODE_HEADER

hlibqrencode_qrcode_t *hlibqrencode_encode_string(const char *string, int version, hlibqrencode_eclevel_t level, hlibqrencode_encode_mode_t hint, int casesensitive)
{
    return (hlibqrencode_qrcode_t *)QRcode_encodeString(string,version,(QRecLevel)level,(QRencodeMode)hint,casesensitive);
}

hlibqrencode_qrcode_t *hlibqrencode_encode_string_8bit(const char *string, int version, hlibqrencode_eclevel_t level)
{
    return (hlibqrencode_qrcode_t *)QRcode_encodeString8bit(string,version,(QRecLevel)level);
}

hlibqrencode_qrcode_t *hlibqrencode_encode_string_MQR(const char *string, int version, hlibqrencode_eclevel_t level, hlibqrencode_encode_mode_t hint, int casesensitive)
{
    return (hlibqrencode_qrcode_t *)QRcode_encodeStringMQR(string,version,(QRecLevel)level,(QRencodeMode)hint,casesensitive);
}

hlibqrencode_qrcode_t *hlibqrencode_encode_string8bit_MQR(const char *string, int version, hlibqrencode_eclevel_t level)
{
    return (hlibqrencode_qrcode_t *)QRcode_encodeString8bitMQR(string,version,(QRecLevel)level);
}

hlibqrencode_qrcode_t *hlibqrencode_encode_data(int size, const unsigned char *data, int version, hlibqrencode_eclevel_t level)
{
    return (hlibqrencode_qrcode_t *)QRcode_encodeData(size,data,version,(QRecLevel)level);
}

hlibqrencode_qrcode_t *hlibqrencode_encode_data_MQR(int size, const unsigned char *data, int version, hlibqrencode_eclevel_t level)
{
    return (hlibqrencode_qrcode_t *)QRcode_encodeDataMQR(size,data,version,(QRecLevel)level);
}

void hlibqrencode_free(hlibqrencode_qrcode_t *qrcode)
{
    QRcode_free((QRcode *)qrcode);
}
