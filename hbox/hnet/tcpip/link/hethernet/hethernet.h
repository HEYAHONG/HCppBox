/***************************************************************
 * Name:      hethernet.h
 * Purpose:   声明hethernet接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-09-30
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HETHERNET_H__
#define __HETHERNET_H__

/*
 * 由IEEE 802.3描述，以太网的数据包包含以下部分
 *      -PREAMBLE：7字节,不属于以太网数据帧，一般由硬件直接处理
 *      -SFD:1字节，不属于以太网数据帧，一般由硬件直接处理
 *      -DESTINATION ADDRESS：6字节，目标地址，属于以太网数据帧
 *      -SOURCE ADDRESS：6字节，源地址，属于以太网数据帧
 *      -LENGTH/TYPE:2字节，长度或者类型，属于以太网数据帧，大端模式
 *      -MAC CLIENT DATA：数据，46字节至1500或者1504或者1982字节，属于以太网数据帧
 *      -FCS：4字节，帧校验，属于以太网数据帧。一般为CRC32，多数硬件可以自动处理,若硬件不能自动处理应当在接口移植中实现FCS添加或者自动移除。位31先发（与其它数据帧项不同，MSB先发），因此应根据实际硬件启用或者填充CRC32。
 *      -EXTENSION:扩展，不属于以太网数据帧。
 */

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


struct hethernet_hwaddr;
typedef struct hethernet_hwaddr hethernet_hwaddr_t;
struct hethernet_hwaddr
{
    uint8_t addr[6];                    /**< 48位地址，最高位为I/G标志，次高位为U/L标志，由于以太网传输时字节低位在前，故而第一字节的位0为I/G标志，位1为U/L标志  */
};

typedef enum
{
    HETHERNET_HWADDR_I_G_I=0,           /**< 单播地址  */
    HETHERNET_HWADDR_I_G_G=1            /**< 组播（广播）地址 */
} hethernet_hwaddr_i_g_t;

/** \brief 地址I/G标志
 *
 * \param addr hethernet_hwaddr_t* 地址
 * \param new_value hethernet_hwaddr_i_g_t* 新标志，为NULL表示不设置新标志
 * \return hethernet_hwaddr_i_g_t 地址原I/G标志
 *
 */
hethernet_hwaddr_i_g_t hethernet_hwaddr_i_g(hethernet_hwaddr_t *addr,hethernet_hwaddr_i_g_t *new_value);

typedef enum
{
    HETHERNET_HWADDR_U_L_U=0,           /**< 全局地址，由生产商烧录  */
    HETHERNET_HWADDR_U_L_L=1            /**< 本地地址，由用户自行分配，一般情况下可用于虚拟网卡或者随机地址（防止MAC地址追踪） */
} hethernet_hwaddr_u_l_t;

/** \brief 地址U/L标志
 *
 * \param addr hethernet_hwaddr_t* 地址
 * \param new_value hethernet_hwaddr_u_l_t* 新标志
 * \return hethernet_hwaddr_u_l_t 地址原U/L标志，为NULL表示不设置新标志
 *
 */
hethernet_hwaddr_u_l_t hethernet_hwaddr_u_l(hethernet_hwaddr_t *addr,hethernet_hwaddr_u_l_t *new_value);


/*
 * 广播地址（全1）
 */
#define HETHERNET_HWADDR_BROADCAST_ADDR0 (0xFF)
#define HETHERNET_HWADDR_BROADCAST_ADDR1 (0xFF)
#define HETHERNET_HWADDR_BROADCAST_ADDR2 (0xFF)
#define HETHERNET_HWADDR_BROADCAST_ADDR3 (0xFF)
#define HETHERNET_HWADDR_BROADCAST_ADDR4 (0xFF)
#define HETHERNET_HWADDR_BROADCAST_ADDR5 (0xFF)

/** \brief 是否为广播地址
 *
 * \param addr hethernet_hwaddr_t* 地址
 * \return bool 是否广播
 *
 */
bool hethernet_hwaddr_is_broadcast(hethernet_hwaddr_t *addr);


/*
 * IPV4组播地址
 * 高24位为01-00-5E,25位为0,低23位映射IPV4地址的低23位
 */
#define HETHERNET_HWADDR_IPV4_MULTICAST_ADDR0 (0x01)
#define HETHERNET_HWADDR_IPV4_MULTICAST_ADDR1 (0x00)
#define HETHERNET_HWADDR_IPV4_MULTICAST_ADDR2 (0x5E)

/** \brief 是否为IPV4组播地址
 *
 * \param addr hethernet_hwaddr_t* 地址
 * \return bool IPV4组播地址
 *
 */
bool hethernet_hwaddr_is_ipv4_multicast(hethernet_hwaddr_t *addr);

/*
 * IPV6组播地址
 * 高16位为33-33,低32位映射IPV6地址的低32位
 */
#define HETHERNET_HWADDR_IPV6_MULTICAST_ADDR0 (0x33)
#define HETHERNET_HWADDR_IPV6_MULTICAST_ADDR1 (0x33)

/** \brief 是否为IPV6组播地址
 *
 * \param addr hethernet_hwaddr_t* 地址
 * \return bool IPV6组播地址
 *
 */
bool hethernet_hwaddr_is_ipv6_multicast(hethernet_hwaddr_t *addr);

/** \brief 长度/类型解码
 *
 * \param data[2] uint8_t 数据
 * \return uint16_t 长度/类型
 *
 */
uint16_t hethernet_length_type_decode(const uint8_t data[2]);

/** \brief 长度/类型编码
 *
 * \param data[2] uint8_t 数据
 * \param length_type uint16_t 长度/类型
 *
 */
void hethernet_length_type_encode(uint8_t data[2],uint16_t length_type);

/*
 * 长度/类型
 */
typedef enum
{
    HETHERNET_LENGTH_TYPE_MIN_LENGTH=46,
    HETHERNET_LENGTH_TYPE_BASE_MAX_LENGTH=1500,
    HETHERNET_LENGTH_TYPE_Q_TAGGED_MAX_LENGTH=1504,
    HETHERNET_LENGTH_TYPE_ENVELOPE_MAX_LENGTH=1982,
    HETHERNET_LENGTH_TYPE_TYPE_START=0x0600,
    HETHERNET_LENGTH_TYPE_IPV4=0x0800,
    HETHERNET_LENGTH_TYPE_X_75=0x0801,
    HETHERNET_LENGTH_TYPE_X_25=0x0802,
    HETHERNET_LENGTH_TYPE_ARP=0x0806,
    HETHERNET_LENGTH_TYPE_RELAY_ARP=0x0808,
    HETHERNET_LENGTH_TYPE_WOL=0x0842,
    HETHERNET_LENGTH_TYPE_TRILL=0x22F3,
    HETHERNET_LENGTH_TYPE_L2_IS_IS=0x22F4,
    HETHERNET_LENGTH_TYPE_TRANS_ETHER_BRIDGING=0x6558,
    HETHERNET_LENGTH_TYPE_RAW_FRAME_RELAY=0x6559,
    HETHERNET_LENGTH_TYPE_RARP=0x8035,
    HETHERNET_LENGTH_TYPE_APPLETALK=0x089B,
    HETHERNET_LENGTH_TYPE_IEEE_802_1Q_CUSTOM_TAG=0x8100,
    HETHERNET_LENGTH_TYPE_NOVELL_NETWARE_IPX_SPX=0x8137,
    HETHERNET_LENGTH_TYPE_NOVELL_INC=0x8138,
    HETHERNET_LENGTH_TYPE_SNMP_OE=0x814C,
    HETHERNET_LENGTH_TYPE_IPV6=0x86DD,
    HETHERNET_LENGTH_TYPE_TCP_IP_COMPRESSION=0x876B,
    HETHERNET_LENGTH_TYPE_IP_AUTONOMOUS_SYSTEMS=0x876C,
    HETHERNET_LENGTH_TYPE_SECURE_DATA=0x876D,
    HETHERNET_LENGTH_TYPE_EPON=0x8808,
    HETHERNET_LENGTH_TYPE_PPP=0x880B,
    HETHERNET_LENGTH_TYPE_GSMP=0x880C,
    HETHERNET_LENGTH_TYPE_MPLS=0x8847,
    HETHERNET_LENGTH_TYPE_MPLS_UPSTREAM_ASSIGNED_LABEL=0x8848,
    HETHERNET_LENGTH_TYPE_PPPOE_DISCOVERY=0x8863,
    HETHERNET_LENGTH_TYPE_PPPOE_SESSION=0x8864,
    HETHERNET_LENGTH_TYPE_JUMBO=0x8870,
    HETHERNET_LENGTH_TYPE_IEEE_802_1X=0x888E,
    HETHERNET_LENGTH_TYPE_PROFINET=0x8892,
    HETHERNET_LENGTH_TYPE_ETHERCAT=0x88A4,
    HETHERNET_LENGTH_TYPE_IEEE_802_1Q_S_TAG=0x88A8,
    HETHERNET_LENGTH_TYPE_IEEE_802_OUI_EXTENDED_ETHERTYPE=0x88B7,
    HETHERNET_LENGTH_TYPE_IEEE_802_11I=0x88C7,
    HETHERNET_LENGTH_TYPE_IEEE_802_1AB=0x88CC,
    HETHERNET_LENGTH_TYPE_SERCOS=0x88CD,
    HETHERNET_LENGTH_TYPE_MRP=0x88E3,
    HETHERNET_LENGTH_TYPE_IEEE_802_1AE=0x88E5,
    HETHERNET_LENGTH_TYPE_IEEE_802_1Q_MVRP=0x88F5,
    HETHERNET_LENGTH_TYPE_IEEE_802_1Q_MMRP=0x88F6,
    HETHERNET_LENGTH_TYPE_PTP=0x88F7,
    HETHERNET_LENGTH_TYPE_TRILL_FGL=0x893B,
    HETHERNET_LENGTH_TYPE_TRILL_RBRIDGE_CHANNEL=0x8946,
    HETHERNET_LENGTH_TYPE_802_1AD=0x9100,
} hethernet_length_type_t;

/*
 * 以太网帧头
 */
struct hethernet_header;
typedef struct hethernet_header hethernet_header_t;
struct hethernet_header
{
    hethernet_hwaddr_t dst;
    hethernet_hwaddr_t src;
    uint8_t length_type[2];
};


/** \brief 以太网CRC32
 *
 * \param frame const uint8_t* 帧长度
 * \param frame_len size_t 帧长度（不含FCS）
 * \return uint32_t CRC32，具体如何填充请根据硬件实际处理
 *
 */
uint32_t hethernet_crc32_calculate(const uint8_t *frame,size_t frame_len);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HETHERNET_H__
