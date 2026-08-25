#ifndef __BLT_CONFIG_H__
#define __BLT_CONFIG_H__

#include "hdefaults.h"

/*
 * dummy配置
 */
#ifndef BOOT_CPU_XTAL_SPEED_KHZ
/*
 * 此项用于定时器及某些外设初始化
 */
#define BOOT_CPU_XTAL_SPEED_KHZ     80000
#endif
#ifndef BOOT_CPU_SYSTEM_SPEED_KHZ
/*
 * 此项用于定时器及某些外设初始化
 */
#define BOOT_CPU_SYSTEM_SPEED_KHZ   80000
#endif
#ifndef BOOT_CPU_BYTE_ORDER_MOTOROLA
#if((HDEFAULTS_ENDIAN_ORDER)==(HDEFAULTS_ENDIAN_ORDER_BIG))
#define BOOT_CPU_BYTE_ORDER_MOTOROLA  (1)
#else
#define BOOT_CPU_BYTE_ORDER_MOTOROLA  (0)
#endif
#endif
#ifndef BOOT_NVM_SIZE_KB
/*
 * 此项必须由用户配置为实际存储的大小
 */
#define BOOT_NVM_SIZE_KB  1
#endif

/*
 * 网络默认配置
 */
#if  BOOT_COM_NET_ENABLE  > (0)
#if !defined(BOOT_COM_NET_PORT)
/** \brief Configure the port that the TCP/IP server listens on */
#define BOOT_COM_NET_PORT                 (1000)
#endif
#if !defined(BOOT_COM_NET_DHCP_ENABLE)
/** \brief Enable/disable DHCP client for automatically obtaining an IP address. */
#define BOOT_COM_NET_DHCP_ENABLE          (1)
#endif
#if !defined(BOOT_COM_NET_IPADDR0)
/** \brief Configure the 1st byte of the IP address */
#define BOOT_COM_NET_IPADDR0              (192)
#endif
#if !defined(BOOT_COM_NET_IPADDR1)
/** \brief Configure the 2nd byte of the IP address */
#define BOOT_COM_NET_IPADDR1              (168)
#endif
#if !defined(BOOT_COM_NET_IPADDR2)
/** \brief Configure the 3rd byte of the IP address */
#define BOOT_COM_NET_IPADDR2              (178)
#endif
#if !defined(BOOT_COM_NET_IPADDR3)
/** \brief Configure the 4th byte of the IP address */
#define BOOT_COM_NET_IPADDR3              (50)
#endif
#if !defined(BOOT_COM_NET_NETMASK0)
/** \brief Configure the 1st byte of the network mask */
#define BOOT_COM_NET_NETMASK0             (255)
#endif
#if !defined(BOOT_COM_NET_NETMASK1)
/** \brief Configure the 2nd byte of the network mask */
#define BOOT_COM_NET_NETMASK1             (255)
#endif
#if !defined(BOOT_COM_NET_NETMASK2)
/** \brief Configure the 3rd byte of the network mask */
#define BOOT_COM_NET_NETMASK2             (255)
#endif
#if !defined(BOOT_COM_NET_NETMASK3)
/** \brief Configure the 4th byte of the network mask */
#define BOOT_COM_NET_NETMASK3             (0)
#endif
#if !defined(BOOT_COM_NET_GATEWAY0)
/** \brief Configure the 1st byte of the gateway address */
#define BOOT_COM_NET_GATEWAY0             (192)
#endif
#if !defined(BOOT_COM_NET_GATEWAY1)
/** \brief Configure the 2nd byte of the gateway address */
#define BOOT_COM_NET_GATEWAY1             (168)
#endif
#if !defined(BOOT_COM_NET_GATEWAY2)
/** \brief Configure the 3rd byte of the gateway address */
#define BOOT_COM_NET_GATEWAY2             (178)
#endif
#if !defined(BOOT_COM_NET_GATEWAY3)
/** \brief Configure the 4th byte of the gateway address */
#define BOOT_COM_NET_GATEWAY3             (1)
#endif
#if !defined(BOOT_COM_NET_DEFERRED_INIT_ENABLE)
/** \brief Enable/disable the deferred initialization mechanism. When enabled, the
 *         communication interface is only initialized when: (a) no valid user program
 *         is detected, or (b) when CpuUserProgramStartHook() returns BLT_FALSE. Your
 *         bootloader application can explicitly initialize the communication interface
 *         by calling ComDeferredInit().
 */
#define BOOT_COM_NET_DEFERRED_INIT_ENABLE (1)
#endif
#endif

/*
 * CAN通信默认配置
 */
#if BOOT_COM_CAN_ENABLE  > (0)
#if !defined(BOOT_COM_CAN_BAUDRATE)
/** \brief Configure the desired CAN baudrate. */
#define BOOT_COM_CAN_BAUDRATE           (500000)
#endif
#if !defined(BOOT_COM_CAN_TX_MSG_ID)
/** \brief Configure CAN message ID target->host. */
#define BOOT_COM_CAN_TX_MSG_ID          (0x7E1 /*| 0x80000000*/)
#endif
#if !defined(BOOT_COM_CAN_RX_MSG_ID)
/** \brief Configure CAN message ID host->target. */
#define BOOT_COM_CAN_RX_MSG_ID          (0x667 /*| 0x80000000*/)
#endif
#if !defined(BOOT_COM_CAN_FD_ENABLE)
/** \brief Configure CAN classic (0) or CAN FD (1). */
#define BOOT_COM_CAN_FD_ENABLE          (0)
#endif
#if !defined(BOOT_COM_CAN_FD_BRS_BAUDRATE)
/** \brief Configure the CAN FD data baudrate for the bitrate switch.  */
#define BOOT_COM_CAN_FD_BRS_BAUDRATE    (2000000)
#endif
#if !defined(BOOT_COM_CAN_CHANNEL_INDEX)
/** \brief Select the desired CAN peripheral as a zero based index. */
#define BOOT_COM_CAN_CHANNEL_INDEX      (0)
#endif
#endif


/*
 * 串口默认配置
 */
#if BOOT_COM_RS232_ENABLE  > (0)
#if !defined( BOOT_COM_RS232_BAUDRATE)
/** \brief Configure the desired communication speed. */
#define BOOT_COM_RS232_BAUDRATE          (57600)
#endif
#if !defined(BOOT_COM_RS232_CHANNEL_INDEX)
/** \brief Select the desired UART peripheral as a zero based index. */
#define BOOT_COM_RS232_CHANNEL_INDEX     (0)
#endif
#endif

/*
 * ModbusRTU默认设置
 */
#if BOOT_COM_MBRTU_ENABLE > (0)
#if !defined(BOOT_COM_MBRTU_BAUDRATE)
/** \brief Configure the desired communication speed. */
#define BOOT_COM_MBRTU_BAUDRATE          (57600)
#endif
#if !defined(BOOT_COM_MBRTU_STOPBITS)
/** \brief Configure the desired number of stopbits (1 or 2). */
#define BOOT_COM_MBRTU_STOPBITS          (1)
#endif
#if !defined(BOOT_COM_MBRTU_PARITY)
/** \brief Configure the desired parity (0 for none, 1 for odd, 2 for even). */
#define BOOT_COM_MBRTU_PARITY            (2)
#endif
#if !defined(BOOT_COM_MBRTU_CHANNEL_INDEX)
/** \brief Select the desired UART peripheral as a zero based index:
 *         - Set to 0 to use USART1 for RS485 communication in combination with the
 *           Waveshare RS485/CAN shield.
 *         - Set to 1 to use USART2 for RS232 communication through the USB-CDC class
 *           of the on-board ST-Link/V2.1 debugger.
 */
#define BOOT_COM_MBRTU_CHANNEL_INDEX     (1)
#endif
#if !defined(BOOT_COM_MBRTU_NODE_ID)
/** \brief The 8-bit node identifier of this node. Should be between 1 and 247. */
#define BOOT_COM_MBRTU_NODE_ID           (1)
#endif
#endif

#endif


