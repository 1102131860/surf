#include "aliyun_mqtt.h"
#include "adx355.h"
#include "max30102_basicRead.h"
#include "ad8232.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //initilze sensors
  init_ADX355();
  init_MAX30102();
  init_AD8232();
  //initlize networks and iot
  init_wifi();
  init_mqtt();
  // FreeRTOS tasks start
  //create_FreeRTOS_Tasks();
}

void loop() {
  float readedData[3]; // create a three-float memory store, just a pointer without inilization
  readXYZ(readedData);
  //mqtt_post_XYZ(readedData);

  float red = getRed();
  float ir = getIr();
  //mqtt_post_PPG(red, ir);

  float ECG = getECG();
  //mqtt_post_ECG(ECG);

  float allData[6] = {readedData[0], readedData[1], readedData[2], red, ir, ECG};
  Serial.printf("%f,%f,%f,%f,%f,%f\n",
                 allData[0], allData[1], 
                 allData[2], allData[3],
                 allData[4], allData[5]);
  mqtt_post_all_data(allData);  
}

void create_FreeRTOS_Tasks(){
  xTaskCreate(
      task_XYZ_accelerations,     /* Task function. */
      "Task_XYZ_accelerations",   /* String with name of task. */
      10000,                      /* Stack size in bytes. */
      NULL,                       /* Parameter passed as input of the task */
      1,                          /* Priority of the task. */
      NULL);                      /* Task handle. */

  xTaskCreate(
      task_PPG,                   /* Task function. */
      "Task_PPG",                  /* String with name of task. */
      10000,                      /* Stack size in bytes. */
      NULL,                       /* Parameter passed as input of the task */
      1,                          /* Priority of the task. */
      NULL);                      /* Task handle. */
  
  xTaskCreate(
      task_ECG,                   /* Task function. */
      "Task_ECG",                 /* String with name of task. */
      10000,                      /* Stack size in bytes. */
      NULL,                       /* Parameter passed as input of the task */
      1,                          /* Priority of the task. */
      NULL);                      /* Task handle. */
}

void task_XYZ_accelerations(void *parameter){
  float readedData[3];
  readXYZ(readedData);
  mqtt_post_XYZ(readedData);
}

void task_PPG(void *parameter){
  float red = getRed();
  float ir = getIr();
  mqtt_post_PPG(red, ir);
}

void task_ECG(void *parameter){
  float ECG = getECG();
  mqtt_post_ECG(ECG);
}