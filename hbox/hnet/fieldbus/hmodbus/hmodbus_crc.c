/***************************************************************
 * Name:      crc.c
 * Purpose:   modbus crc校验
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-09-02
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hmodbus.h"

static uint16_t hmodbus_crc16(const uint8_t *arr_buff,size_t len)
{
    uint16_t crc=0xFFFF;
    size_t  i, j;
    for ( j=0; j<len; j++)
    {
        crc=crc ^*arr_buff++;
        for ( i=0; i<8; i++)
        {
            if( ( crc&0x0001) >0)
            {
                crc=crc>>1;
                crc=crc^ 0xa001;        /**< modbus crc 表达式0x8005 按位反转 */
            }
            else
                crc=crc>>1;
        }
    }
    return ( crc);
}



/** \brief 检查一帧数据的crc
 *
 * \param adu 整帧数据(包含CRC)的指针
 * \param adu_length 长度(包含CRC)
 * \return CRC是否通过
 *
 */
bool modbus_rtu_adu_check_crc(const uint8_t *adu,size_t adu_length)
{
    if(adu_length<=2 || adu==NULL)
    {
        return false;
    }
    uint16_t crc1=hmodbus_crc16(adu,adu_length-2);
    uint16_t crc2=adu[adu_length-1];
    crc2<<=8;
    crc2+=adu[adu_length-2];
    return crc1==crc2;
}



/** \brief 在数据帧末尾的添加crc校验
 *
 * \param adu 整帧数据的指针，需要在末尾留两个字节以填写CRC
 * \param adu_length 长度(包含CRC)
 * \return 是否调用成功
 *
 */
bool modbus_rtu_adu_append_crc(uint8_t *adu,size_t adu_length)
{
    if(adu_length<=2 || adu==NULL)
    {
        return false;
    }
    uint16_t crc=hmodbus_crc16(adu,adu_length-2);
    adu[adu_length-2]=(crc&0xff);
    adu[adu_length-1]=(crc>>8);
    return true;
}




