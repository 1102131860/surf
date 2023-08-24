/*
  Aliyun_mqtt.h - Library for connect to Aliyun MQTT server.
*/

#include "aliyun_mqtt.h"

// these two variables should put in this cpp file, otherwise mutiple definition error will occur
WiFiClient espClient;
PubSubClient mqttClient(espClient);

bool connect_aliyun_mqtt() {
  //connect to mqtt server
  mqttClient.setServer(MQTT_HOST_URL, MQTT_PORT);

  byte mqttConnectTryCnt = 5;
  while (!mqttClient.connected() && mqttConnectTryCnt > 0) {
    Serial.println("Connecting to MQTT Server ...");
    if (mqttClient.connect(MQTT_CLINET_ID, MQTT_USER_NAME, MQTT_PASSWORD)) {
      Serial.println("MQTT Connected!");
      return true;
    } else {
      byte errCode = mqttClient.state();
      Serial.print("MQTT connect failed, error code:");
      Serial.println(errCode);
      switch(errCode){
        case MQTT_CONNECT_BAD_PROTOCOL:
          Serial.println("MQTT_CONNECT_BAD_PROTOCOL");
        break;
        case MQTT_CONNECT_BAD_CLIENT_ID:
          Serial.println("MQTT_CONNECT_BAD_CLIENT_ID");
        break;
        case MQTT_CONNECT_BAD_CREDENTIALS:
          Serial.println("MQTT_CONNECT_BAD_CREDENTIALS");
        break;
        case MQTT_CONNECT_UNAUTHORIZED:
          Serial.println("MQTT_CONNECT_UNAUTHORIZED");
        break;
        default:
          Serial.println("Other issues, please refer to the mqtt errode code");
      }
      Serial.println("Check the mqttBroker, mqttClientID, mqttUserName and password again");
      break; // don't need to try connect to mqtt server (no dinimishment), just check the configuration
      delay(3000);
    }
    mqttConnectTryCnt -= 1;
  }
  return false;
}

void mqtt_callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  payload[length] = '\0';
  Serial.println((char *)payload);

  if (strstr(topic, ALINK_TOPIC_PROP_SET)) {
    StaticJsonDocument<100> doc;
    auto error = deserializeJson(doc, payload); // parse the json payload
    if (error) {
      Serial.println("parse object failed");
      Serial.print(F("deserializeJson() failed with code"));
      Serial.println(error.c_str());
      return;
    }
  }
  //delay(1000);
}

void init_mqtt(){
  mqttClient.setCallback(mqtt_callback);
}

void mqtt_version_post() {
  char param[512];
  char jsonBuf[1024];

  sprintf(param, "{\"id\": 123,\"params\": {\"version\": \"%s\"}}", DEV_VERSION);
  // sprintf(jsonBuf, ALINK_BODY_FORMAT, ALINK_METHOD_PROP_POST, param);
  Serial.println(param);
  mqttClient.publish(ALINK_TOPIC_DEV_INFO, param);
}

void mqtt_check_connect() {
  while (!mqttClient.connected())  //如果没有连接成功
  {
    while (connect_aliyun_mqtt()) { //则持续连接阿里云
      Serial.println("MQTT connection succeed!");
      //client.subscribe(ALINK_TOPIC_PROP_POSTRSP);
      mqttClient.subscribe(ALINK_TOPIC_PROP_SET);

      Serial.println("subscribe done");
      mqtt_version_post();
    }
  }
}

void mqtt_post_ECG(float ecg){
  char param[512];
  char jsonBuf[1024];

  //Firstly, check the connection is kept or not
  mqtt_check_connect();
  //Secondly, post the ecg to designated attribute
  sprintf(param,"{\"%s\":%f}", ATTRIBUTE_ECG, ecg); // 将数据转化为param，再转化为json
  sprintf(jsonBuf, ALINK_BODY_FORMAT, ALINK_METHOD_PROP_POST, param);
  Serial.println(jsonBuf);
  mqttClient.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
  //keep the mqttclient monitor on to spy on the received message
  //mqttClient.loop(); //comment it out if only foucs on sending without aliyun receiving or not
}

void mqtt_post_PPG(float Red, float IR){
  char param[512];
  char jsonBuf[1024];

  //Firstly, check the connection is kept or not
  mqtt_check_connect();
  //Secondly, post the ecg to designated attribute
  sprintf(param,"{\"%s\":%f,\"%s\":%f}", 
                ATTRIBUTE_Red, Red, 
                ATTRIBUTE_IR, IR); // 将数据转化为param，再转化为json
  sprintf(jsonBuf, ALINK_BODY_FORMAT, ALINK_METHOD_PROP_POST, param);
  Serial.println(jsonBuf);
  mqttClient.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
  //keep the mqttclient monitor on to spy on the received message
  //mqttClient.loop(); //comment it out if only foucs on sending without aliyun receiving or not
}

void mqtt_post_XYZ(float* data){
  char param[512];
  char jsonBuf[1024];

  // check mqtt kept connected well
  mqtt_check_connect();
  sprintf(param, "{\"%s\":%f,\"%s\":%f,\"%s\":%f}", 
                  ATTRIBUTE_X, data[0], 
                  ATTRIBUTE_Y, data[1], 
                  ATTRIBUTE_Z, data[2]);
  sprintf(jsonBuf, ALINK_BODY_FORMAT, ALINK_METHOD_PROP_POST, param);
  Serial.println(jsonBuf);
  mqttClient.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
  //receive the reply message from aliyun server
  //mqttClient.loop();
}

void mqtt_post_all_data(float* data){
  char param[512];
  char jsonBuf[1024];

  // check mqtt kept connected well
  mqtt_check_connect();
  sprintf(param, "{\"%s\":%f,\"%s\":%f,\"%s\":%f,\"%s\":%f,\"%s\":%f,\"%s\":%f}", 
                    ATTRIBUTE_X, data[0], 
                    ATTRIBUTE_Y, data[1], 
                    ATTRIBUTE_Z, data[2],
                    ATTRIBUTE_Red, data[3],
                    ATTRIBUTE_IR, data[4],
                    ATTRIBUTE_ECG, data[5]);
  sprintf(jsonBuf, ALINK_BODY_FORMAT, ALINK_METHOD_PROP_POST, param);
  Serial.println(jsonBuf);
  mqttClient.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
  //receive the reply message from aliyun server
  //mqttClient.loop();
}