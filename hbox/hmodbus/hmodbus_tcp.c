/***************************************************************
 * Name:      tcp.c
 * Purpose:   modbus tcp
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-09-02
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hmodbus.h"

/** \brief 从TCP的ADU数据中提取PDU数据
 *
 * \param adu uint8_t* ADU数据帧
 * \param adu_length size_t ADU数据帧长度
 * \param cb modbus_tcp_pdu_callback_t PDU数据帧回调
 * \param usr void* 用户参数,用于PDU数据帧回调
 * \return bool 是否成功提取
 *
 */
bool modbus_tcp_get_pdu_from_adu(const uint8_t *adu,size_t adu_length,modbus_tcp_pdu_callback_t cb,void *usr)
{
    if(adu==NULL || adu_length < 7)
    {
        return false;
    }

    uint16_t TId=adu[0];
    TId<<=8;
    TId+=adu[1];

    uint16_t PId=adu[2];
    PId<<=8;
    PId+=adu[3];

    uint16_t Length=adu[4];
    Length<<=8;
    Length+=adu[5];

    uint8_t node_address=adu[6];

    if(PId != 0 || (Length+6 != adu_length))
    {
        return false;
    }

    if(cb!=NULL)
    {
        return cb(TId,node_address,&adu[7],adu_length-7,usr);
    }

    return false;
}

/** \brief 向TCP的ADU数据中设置PDU数据
 *
 * \param adu uint8_t* ADU数据指针
 * \param adu_length size_t ADU长度
 * \param TId uint16_t 传输ID，用于区分请求与响应是否为一对
 * \param node_address uint8_t 节点地址
 * \param pdu const uint8_t* PDU数据指针
 * \param pdu_length size_t PDU长度
 * \return size_t 实际ADU长度
 *
 */
size_t modbus_tcp_set_pdu_to_adu(uint8_t *adu,size_t adu_length,uint16_t TId,uint8_t node_address,const uint8_t *pdu,size_t pdu_length)
{
    if(adu==NULL || pdu == NULL || pdu_length == 0 || pdu_length+7 > adu_length)
    {
        return 0;
    }

    adu[0]=(TId>>8);
    adu[1]=(TId&0xFF);
    adu[2]=0;
    adu[3]=0;
    adu[4]=((pdu_length+1)>>8);
    adu[5]=((pdu_length+1)&0xFF);
    adu[6]=node_address;//一般用于转发
    if(&adu[7]!=pdu)
    {
        memcpy(&adu[7],pdu,pdu_length);
    }
    return pdu_length+7;
}

/** \brief 向TCP的ADU数据中设置异常PDU数据
 *
 * \param adu uint8_t* ADU数据指针
 * \param adu_length size_t ADU长度
 * \param TId uint16_t 传输ID，用于区分请求与响应是否为一对
 * \param node_address uint8_t 节点地址
 * \param function_code uint8_t 功能码
 * \param exception_code uint8_t 异常代码
 * \return size_t 实际ADU长度
 *
 */
size_t modbus_tcp_set_exception_pdu_to_adu(uint8_t *adu,size_t adu_length,uint16_t TId,uint8_t node_address,uint8_t function_code,uint8_t exception_code)
{
    uint8_t pdu[2]= {function_code,exception_code};
    if(pdu[0] < MODBUS_FC_EXCEPTION_BASE)
    {
        pdu[0] += MODBUS_FC_EXCEPTION_BASE;
    }
    return modbus_tcp_set_pdu_to_adu(adu,adu_length,TId,node_address,pdu,sizeof(pdu));
}
