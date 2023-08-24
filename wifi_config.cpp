#include "wifi_config.h"

void init_wifi() {
  //Serial.begin(115200);  //波特率设置你要在你的串口中设置同样的波特率不然就是乱码
  WiFi.mode(WIFI_STA);  // station模式，wifi工作在无线终端模式
  WiFi.begin(WIFI_SSID, WIFI_PASSWD); //这个就是TCP连接一样的通过wifi名字和密码绑定wifi
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi does not connect, try again ...");
    delay(1000);
  }

  Serial.println("Wifi is connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(1000);  
}