#include "modbus.h"

/** \brief 从RTU的ADU数据中提取PDU数据
 *
 * \param adu uint8_t* ADU数据帧
 * \param adu_length size_t ADU数据帧长度
 * \param cb modbus_rtu_pdu_callback_t PDU数据帧回调
 * \param usr void* 用户参数,用于PDU数据帧回调
 * \return bool 是否成功提取
 *
 */
bool modbus_rtu_get_pdu_from_adu(const uint8_t *adu,size_t adu_length,modbus_rtu_pdu_callback_t cb,void *usr)
{
    if(!modbus_rtu_adu_check_crc(adu,adu_length))
    {
        return false;
    }

    if(adu_length <= 3)
    {
        return false;
    }

    uint8_t node_address=adu[0];

    if(cb!=NULL)
    {
        return cb(node_address,&adu[1],adu_length-3,usr);
    }

    return false;
}

/** \brief 向RTU的ADU数据中设置PDU数据
 *
 * \param adu uint8_t* ADU数据指针
 * \param adu_length size_t ADU长度
 * \param node_address uint8_t 节点地址
 * \param pdu const uint8_t* PDU数据指针
 * \param pdu_length size_t PDU长度
 * \return size_t 实际ADU长度
 *
 */
size_t modbus_rtu_set_pdu_to_adu(uint8_t *adu,size_t adu_length,uint8_t node_address,const uint8_t *pdu,size_t pdu_length)
{
    if(adu==NULL || pdu == NULL || pdu_length == 0 || pdu_length +3 > adu_length)
    {
        return 0;
    }

    adu[0]=node_address;

    if(&adu[1]!=pdu)
    {
        memcpy(&adu[1],pdu,pdu_length);
    }

    modbus_rtu_adu_append_crc(adu,pdu_length+3);

    return pdu_length+3;
}


