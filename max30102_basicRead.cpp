/*
Macro definition opions in sensor configuration 
sampleAverage: SAMPLEAVG_1 SAMPLEAVG_2 SAMPLEAVG_4 
               SAMPLEAVG_8 SAMPLEAVG_16 SAMPLEAVG_32
ledMode:       MODE_REDONLY  MODE_RED_IR  MODE_MULTILED
sampleRate:    PULSEWIDTH_69 PULSEWIDTH_118 PULSEWIDTH_215 PULSEWIDTH_411
pulseWidth:    SAMPLERATE_50 SAMPLERATE_100 SAMPLERATE_200 SAMPLERATE_400
               SAMPLERATE_800 SAMPLERATE_1000 SAMPLERATE_1600 SAMPLERATE_3200
adcRange:      ADCRANGE_2048 ADCRANGE_4096 ADCRANGE_8192 ADCRANGE_16384
*/

#include "max30102_basicRead.h"

DFRobot_MAX30102 particleSensor;

void init_MAX30102(){
  /*!
   *@brief Init sensor 
   *@param pWire IIC bus pointer object and construction device, can both pass or not pass parameters (Wire in default)
   *@param i2cAddr Chip IIC address (0x57 in default)
   *@return true or false
   */
  while (!particleSensor.begin()) {
    Serial.println("MAX30102 was not found");
    delay(1000);
  }

  /*!
   *@brief Use macro definition to configure sensor
   *@param ledBrightness LED brightness, default value: 0x1F（6.4mA), Range: 0~255（0=Off, 255=50mA）
   *@param sampleAverage Average multiple samples then draw once, reduce data throughput, default 4 samples average
   *@param ledMode LED mode, default to use red light and IR at the same time 
   *@param sampleRate Sampling rate, default 400 samples every second 
   *@param pulseWidth Pulse width: the longer the pulse width, the wider the detection range. Default to be Max range; LED ADC Resolution is 18 bits
   *@param adcRange SpO2 Measurement Range, default 4096 (nA), 15.63(pA) per LSB, 12 bits
   */
  particleSensor.sensorConfiguration(/*ledBrightness=*/0x1F, /*sampleAverage=*/SAMPLEAVG_4, \
                                  /*ledMode=*/MODE_MULTILED, /*sampleRate=*/SAMPLERATE_400, \
                                  /*pulseWidth=*/PULSEWIDTH_411, /*adcRange=*/ADCRANGE_4096);
}

float getRed(){
  uint32_t int_red = 262144 - particleSensor.getRed(); // 2^18 = 262,144, inverse the pulse
  //Serial.print(int_red);
  //Serial.print(",");
  float red = (float) int_red * 6.4 / 262144; //单位是mA, (6.4mA/(2^18)单位是mA)
  // Serial.print(red, 6);
  // Serial.print(",");
  return red;
}

float getIr(){
  uint32_t int_ir = 262144 - particleSensor.getIR();
  //Serial.println(int_ir);
  float ir = (float) int_ir * 6.4 / 262144; //单位是mA
  // Serial.println(ir, 6);
  return ir;
}