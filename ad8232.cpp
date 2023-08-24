#include "ad8232.h"

void init_AD8232(){
  pinMode(L0_POSITIVE, INPUT); // Setup for leads off detection LO +
  pinMode(L0_NEGATIVE, INPUT); // Setup for leads off detection LO -
}

float getECG(){
  uint32_t ECG_value = analogRead(ANALOG_INPUT);
  // Serial.println(ECG_value);
  float ECG = (float) ECG_value * (2.95-0.05) / (4095 - 0); // 单位是V (Vout最大是2.95V, 最小是2V, ESP_32 ADC是12bit)
  // Serial.println(ECG, 6);
  return ECG;
}