/***************************************************************
 * Name:      h3rdparty_mqtt_packket.c
 * Purpose:   引入第三方源代码文件
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-01-01
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "h3rdparty.h"
#include "hdefaults.h"

#ifndef H3RDPARTY_USING_SYSTEM_PAHO_MQTT_EMBEDDED_C_PACKET
#include "3rdparty/paho.mqtt.embedded-c/MQTTPacket/src/MQTTConnectClient.c"
#ifdef min
#undef min
#endif
#include "3rdparty/paho.mqtt.embedded-c/MQTTPacket/src/MQTTConnectServer.c"
#ifdef min
#undef min
#endif
#include "3rdparty/paho.mqtt.embedded-c/MQTTPacket/src/MQTTDeserializePublish.c"
#ifdef min
#undef min
#endif
#include "3rdparty/paho.mqtt.embedded-c/MQTTPacket/src/MQTTFormat.c"
#ifdef min
#undef min
#endif
#include "3rdparty/paho.mqtt.embedded-c/MQTTPacket/src/MQTTPacket.c"
#ifdef min
#undef min
#endif
#include "3rdparty/paho.mqtt.embedded-c/MQTTPacket/src/MQTTSerializePublish.c"
#ifdef min
#undef min
#endif
#include "3rdparty/paho.mqtt.embedded-c/MQTTPacket/src/MQTTSubscribeClient.c"
#ifdef min
#undef min
#endif
#include "3rdparty/paho.mqtt.embedded-c/MQTTPacket/src/MQTTSubscribeServer.c"
#ifdef min
#undef min
#endif
#include "3rdparty/paho.mqtt.embedded-c/MQTTPacket/src/MQTTUnsubscribeClient.c"
#ifdef min
#undef min
#endif
#include "3rdparty/paho.mqtt.embedded-c/MQTTPacket/src/MQTTUnsubscribeServer.c"
#endif // H3RDPARTY_USING_SYSTEM_PAHO_MQTT_EMBEDDED_C_PACKET

