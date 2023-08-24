#include "adx355.h"

//uninitalised pointers to SPI objects
//vspi also needs to send data through SPI in the function read and write register
SPIClass * vspi = NULL;

void init_SPI(){
  vspi = new SPIClass(VSPI);
  vspi->begin(VSPI_SCLK, VSPI_MISO, VSPI_MOSI, VSPI_SS); //SCLK, MISO, MOSI, SS
  // Initalize the data ready and chip select pins:
  pinMode(CHIP_SELECT_PIN, OUTPUT);
}

void init_ADX355(){
  // firstly initlize SPI, so that can write or read data
  init_SPI();

  //Configure ADXL355:
  // must Reset firstly to clear the data
  writeRegister(RESET, RESET_COMMAND);
  //must delay a while for clear data
  delay(1000);

  writeRegister(RANGE, RANGE_8G); // 4G
  /*Note that while configuring the ADXL355 in an application, all configuration registers must 
   *be programmed before enabling measurement mode in the POWER_CTL register
   */
  writeRegister(POWER_CTL, MEASURE_MODE); // Enable measure mode 
  // must delay to boost AXDL355 and press the enable button on ESP32 development board
  delay(1000);
}

void readXYZ(float* readedData){
  int axisAddresses[] = {XDATA1, XDATA2, XDATA3, YDATA1, YDATA2, YDATA3, ZDATA1, ZDATA2, ZDATA3};
  int axisMeasures[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  int dataSize = 9;

  // Read accelerometer data
  readMultipleData(axisAddresses, dataSize, axisMeasures);

  // Split data
  int xdata = (axisMeasures[0] >> 4) | (axisMeasures[1] << 4) | (axisMeasures[2] << 12);
  int ydata = (axisMeasures[3] >> 4) | (axisMeasures[4] << 4) | (axisMeasures[5] << 12);
  int zdata = (axisMeasures[6] >> 4) | (axisMeasures[7] << 4) | (axisMeasures[8] << 12);
  
  // Apply two complement
  // pay attention here, the xyz data is 32-bit 2's complemention, while the raw data is 20-bit 2's complemention
  if (xdata >= 0x80000) {
    //xdata = ~xdata + 1;
    xdata |= 0xFFF00000;
  }
  if (ydata >= 0x80000) {
    //ydata = ~ydata + 1;
    ydata |= 0xFFF00000; 
  }
  if (zdata >= 0x80000) {
    //zdata = ~zdata + 1;
    zdata |= 0xFFF00000;
  }

  // Serial.print(xdata);
  // Serial.print(",");
  // Serial.print(ydata);
  // Serial.print(",");
  // Serial.println(zdata);

  // convert bits into gravity acceleration (accelemeter)
  readedData[0] = xdata * SCALE_8G; // 单位是g
  readedData[1] = ydata * SCALE_8G;
  readedData[2] = zdata * SCALE_8G;

  // Serial.print(readedData[0], 6);
  // Serial.print(",");
  // Serial.print(readedData[1], 6);
  // Serial.print(",");
  // Serial.println(readedData[2], 6);
}

void writeRegister(uint8_t thisRegister, uint8_t thisValue){
  uint8_t dataToSend = (thisRegister << 1) | WRITE_BYTE;
  vspi->beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE0)); // The timing scheme follows the clock polarity (CPOL) = 0 and clock phase (CPHA) = 0. namely, model0
  digitalWrite(vspi->pinSS(), LOW); // pull Chip_select pin slow to prep other end for transfer
  vspi->transfer(dataToSend); // register address
  vspi->transfer(thisValue);  // data will be written
  digitalWrite(vspi->pinSS(), HIGH); // pull Chip_select pin high to signify end of data transfer
  vspi->endTransaction();
}

unsigned int readRegister(uint8_t thisRegister) {
  unsigned int result = 0;
  uint8_t dataToSend = (thisRegister << 1) | READ_BYTE;
  vspi->beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE0)); 
  digitalWrite(vspi->pinSS(), LOW);
  vspi->transfer(dataToSend);     // register address
  result = vspi->transfer(0x00);  // pass null to adxl355 and receive data from adxl355 
  digitalWrite(vspi->pinSS(), HIGH);
  vspi->endTransaction();
  return result;
}

void readMultipleData(int *addresses, int dataSize, int *readedData) {
  vspi->beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE0)); 
  digitalWrite(vspi->pinSS(), LOW);
  for(int i = 0; i < dataSize; i = i + 1) {
    uint8_t dataToSend = (addresses[i] << 1) | READ_BYTE;
    vspi->transfer(dataToSend);
    readedData[i] = vspi->transfer(0x00);
  }
  digitalWrite(vspi->pinSS(), HIGH);
  vspi->endTransaction();
}
