/* 
* Following section is used to initilize the wifi configuration
*/
#ifndef _WIFI_CONFIG_H
#define _WIFI_CONFIG_H

#include "Arduino.h"
#include <WiFi.h>
// Following definition shoule be yourself configuration
#define WIFI_SSID "wcble" // your wifi name
#define WIFI_PASSWD "123456789" // your wifi password

// initialize wifi configuration
extern "C" void init_wifi();

#endif