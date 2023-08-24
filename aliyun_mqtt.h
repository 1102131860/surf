/*
  Aliyun_mqtt.h - Library for connect to Aliyun MQTT server with authentication by
  product key, device name and device secret.
  https://www.alibabacloud.com/help/product/30520.htm
*/
 
#ifndef _ALIYUN_MATT_H
#define _ALIYUN_MATT_H
 
#include "Arduino.h"
#include "wifi_config.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
 
// Following definition should your own
#define PRODUCT_KEY "k00l2FcaqIp" //替换自己的PRODUCT_KEY
#define DEVICE_NAME "esp32" //替换自己的DEVICE_NAME
#define DEVICE_SECRET "d4d1ef542e3d1f86b020906bffc80277" //替换自己的DEVICE_SECRET
#define INSTANCE_ID "iot-06z00cz822e4ko8"    //替换自己的实例的ID

// Following part is the definition of MQTT connection configuration
#define TIMESTAMP "1689744396421" //自己强行定义一个特殊的时间戳（看阿里云平台的）
#define MQTT_PASSWORD "e1deb7bbed81d4d5c5184868c1c8b3a0cb85de9a7258009947c8602b13afbab4" //自己强行定义一个密码,根据强心定义的时间戳
#define MQTT_PORT 1883 //自己的MQTT的接口

// 以下是传输的属性，根据自己的定义修改，在post中要使用
#define ATTRIBUTE_X "X_acceleration"
#define ATTRIBUTE_Y "Y_acceleration"
#define ATTRIBUTE_Z "Z_acceleration"
#define ATTRIBUTE_Red "Red"
#define ATTRIBUTE_IR "IR"
#define ATTRIBUTE_ECG "ECG"

//以下根据上面的定义而来不需修改
#define MQTT_HOST_URL INSTANCE_ID ".mqtt.iothub.aliyuncs.com" // "iot-06z00cz822e4ko8.mqtt.iothub.aliyuncs.com"
#define MQTT_USER_NAME DEVICE_NAME "&" PRODUCT_KEY // "esp32&k00l2FcaqIp"
#define MQTT_CLINET_ID PRODUCT_KEY "." DEVICE_NAME "|securemode=2,signmethod=hmacsha256,timestamp=" TIMESTAMP "|" // "k00l2FcaqIp.esp32|securemode=2,signmethod=hmacsha256,timestamp=1689744396421|"

#define DEV_VERSION "S-TH-WIFI-v1.0-20190220"             //固件版本信息
#define ALINK_BODY_FORMAT "{\"id\":\"123\",\"version\":\"1.0\",\"method\":\"%s\",\"params\":%s}"
#define ALINK_TOPIC_PROP_POST "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post"
#define ALINK_TOPIC_PROP_POSTRSP "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post_reply"
#define ALINK_TOPIC_PROP_SET "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/service/property/set"
#define ALINK_METHOD_PROP_POST "thing.event.property.post"
#define ALINK_TOPIC_DEV_INFO "/ota/device/inform/" PRODUCT_KEY "/" DEVICE_NAME ""
#define ALINK_VERSION_FROMA "{\"id\": 123,\"params\": {\"version\": \"%s\"}}"


/**
 * Use tools here to calculate HMAC256: http://tool.oschina.net/encrypt?type=2
 * The calculated result should be defined as constants and passed when call this function.
 */
extern "C" bool connect_aliyun_mqtt();

/**
* This function is used to work as a function pointer used in the mqtt.setCallback function.
*/
extern "C" void mqtt_callback(
    char *topic, 
    byte *payload, 
    unsigned int length);

/**
* This is used to initlize mqtt connection, just call mqtt_callback to set the client mointor
*/
extern "C" void init_mqtt();

/**
* This is used to show the version of mqtt post
*/
extern "C" void mqtt_version_post();

/**
* This is used to check whether the mqtt connection is kept 
*/
extern "C" void mqtt_check_connect();
 
/**
* This is used to post the message
*/
extern "C" void mqtt_post_ECG(float ecg);

/**
* This is used to post PPG (IR and Red) signal
*/
extern "C" void mqtt_post_PPG(float Red, float IR);

/**
* This is used to post the XYZ accelerations
*/
extern "C" void mqtt_post_XYZ(float* data);

/**
* This is used to send all the float data;
*/
extern "C" void mqtt_post_all_data(float* data);

#endif