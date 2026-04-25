/***************************************************************
 * Name:      hshell_ctlseq.h
 * Purpose:   声明hshell_ctlseq接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-03-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSHELL_CTLSEQ_H__
#define __HSHELL_CTLSEQ_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


typedef enum
{
    //ECMA-48 C0 set
    HSHELL_CTLSEQ_CONTROL_SET_C0_NUL=0x0,
    HSHELL_CTLSEQ_CONTROL_SET_C0_SOH=0x1,
    HSHELL_CTLSEQ_CONTROL_SET_C0_STX=0x2,
    HSHELL_CTLSEQ_CONTROL_SET_C0_ETX=0x3,
    HSHELL_CTLSEQ_CONTROL_SET_C0_EOT=0x4,
    HSHELL_CTLSEQ_CONTROL_SET_C0_ENQ=0x5,
    HSHELL_CTLSEQ_CONTROL_SET_C0_ACK=0x6,
    HSHELL_CTLSEQ_CONTROL_SET_C0_BEL=0x7,
    HSHELL_CTLSEQ_CONTROL_SET_C0_BS=0x8,
    HSHELL_CTLSEQ_CONTROL_SET_C0_HT=0x9,
    HSHELL_CTLSEQ_CONTROL_SET_C0_LF=0xA,
    HSHELL_CTLSEQ_CONTROL_SET_C0_VT=0xB,
    HSHELL_CTLSEQ_CONTROL_SET_C0_FF=0xC,
    HSHELL_CTLSEQ_CONTROL_SET_C0_CR=0xD,
    HSHELL_CTLSEQ_CONTROL_SET_C0_SO=0xE,
    HSHELL_CTLSEQ_CONTROL_SET_C0_LS1=HSHELL_CTLSEQ_CONTROL_SET_C0_SO,
    HSHELL_CTLSEQ_CONTROL_SET_C0_SI=0xF,
    HSHELL_CTLSEQ_CONTROL_SET_C0_LS0=HSHELL_CTLSEQ_CONTROL_SET_C0_SI,
    HSHELL_CTLSEQ_CONTROL_SET_C0_DLE=0x10,
    HSHELL_CTLSEQ_CONTROL_SET_C0_DC1=0x11,
    HSHELL_CTLSEQ_CONTROL_SET_C0_DC2=0x12,
    HSHELL_CTLSEQ_CONTROL_SET_C0_DC3=0x13,
    HSHELL_CTLSEQ_CONTROL_SET_C0_DC4=0x14,
    HSHELL_CTLSEQ_CONTROL_SET_C0_NAK=0x15,
    HSHELL_CTLSEQ_CONTROL_SET_C0_SYN=0x16,
    HSHELL_CTLSEQ_CONTROL_SET_C0_ETB=0x17,
    HSHELL_CTLSEQ_CONTROL_SET_C0_CAN=0x18,
    HSHELL_CTLSEQ_CONTROL_SET_C0_EM=0x19,
    HSHELL_CTLSEQ_CONTROL_SET_C0_SUB=0x1A,
    HSHELL_CTLSEQ_CONTROL_SET_C0_ESC=0x1B,
    HSHELL_CTLSEQ_CONTROL_SET_C0_IS4=0x1C,
    HSHELL_CTLSEQ_CONTROL_SET_C0_IS3=0x1D,
    HSHELL_CTLSEQ_CONTROL_SET_C0_IS2=0x1E,
    HSHELL_CTLSEQ_CONTROL_SET_C0_IS1=0x1F,

    //删除字符（Delete字符）
    HSHELL_CTLSEQ_CONTROL_SET_DEL=0x7F,

    //ECMA-48 C1 set（部分）
    HSHELL_CTLSEQ_CONTROL_SET_C1_IND=0x84,          /**< 转义序列：ESC D */
    HSHELL_CTLSEQ_CONTROL_SET_C1_NEL=0x85,          /**< 转义序列：ESC E */
    HSHELL_CTLSEQ_CONTROL_SET_C1_HTS=0x88,          /**< 转义序列：ESC H */
    HSHELL_CTLSEQ_CONTROL_SET_C1_RI=0x8D,           /**< 转义序列：ESC M */
    HSHELL_CTLSEQ_CONTROL_SET_C1_SS2=0x8E,          /**< 转义序列：ESC N */
    HSHELL_CTLSEQ_CONTROL_SET_C1_SS3=0x8D,          /**< 转义序列：ESC O */
    HSHELL_CTLSEQ_CONTROL_SET_C1_DCS=0x90,          /**< 转义序列：ESC P */
    HSHELL_CTLSEQ_CONTROL_SET_C1_SPA=0x96,          /**< 转义序列：ESC V */
    HSHELL_CTLSEQ_CONTROL_SET_C1_EPA=0x97,          /**< 转义序列：ESC W */
    HSHELL_CTLSEQ_CONTROL_SET_C1_SOS=0x98,          /**< 转义序列：ESC X */
    HSHELL_CTLSEQ_CONTROL_SET_C1_DECID=0x8A,        /**< 转义序列：ESC Z */
    HSHELL_CTLSEQ_CONTROL_SET_C1_CSI=0x9B,          /**< 转义序列：ESC [ */
    HSHELL_CTLSEQ_CONTROL_SET_C1_ST=0x9C,           /**< 转义序列：ESC \ */
    HSHELL_CTLSEQ_CONTROL_SET_C1_OSC=0x9D,          /**< 转义序列：ESC ] */
    HSHELL_CTLSEQ_CONTROL_SET_C1_PM=0x9E,           /**< 转义序列：ESC ^ */
    HSHELL_CTLSEQ_CONTROL_SET_C1_APC=0x9F,          /**< 转义序列：ESC _ */


    HSHELL_CTLSEQ_CONTROL_SET_INVAILD=-1            /**< 无效控制字符 */
} hshell_ctlseq_control_set_t;

/** \brief  hshell控制序列获取控制字符
 *
 * \param control_bytes uint8_t* 控制字节
 * \param length size_t 控制字节长度
 * \return hshell_ctlseq_control_set_t 控制集合中的字符,-1表示出错
 *
 */
hshell_ctlseq_control_set_t hshell_ctlseq_control_set_get_from_bytes(uint8_t *control_bytes,size_t length);




#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HSHELL_CTLSEQ_H__
