#ifndef _ADX355_H
#define _ADX355_H

#include "Arduino.h"
#include <SPI.h>

// Memory register addresses:
#define DEVID_AD                 0x00 // This register contains the Analog Devices ID, 0xAD.
#define DEVID_MST                0x01 // This register contains the Analog Devices MEMS ID, 0x1D
#define PARTID                   0x02 // This register contains the device ID, 0xED (355 octal).
#define REVID                    0x03 // This register contains the product revision ID, beginning with 0x00 and incrementing for each subsequent revision.
#define STATUS                   0x04 // This register includes bits that describe the various conditions of the ADXL355. 
                                      // Bit 4: NVM_BUSY; Bit 3: Activity; Bit 2: FIFO_OVR; Bit 1: FIFO_FULL; Bit 0: DATA_RDY; Bit 7 to 5 is reserved
#define FIFO_ENTRIES             0x05 // This register indicates the number of valid data samples present in the FIFO buffer. This number ranges from 0 to 96. 
#define TEMP2                    0x06 // uncalibrated temperature data; Bit 7 to 4 is reserved, Bit 3 to 0 is temperature [11:8]
#define TEMP1                    0x07 // temperature [7:0]
#define XDATA3                   0x08 // XDATA, Bits[19:12]
#define XDATA2                   0x09 // XDATA, Bits[11:4]
#define XDATA1                   0x0A // XDATA, Bits[3:0]; Reserved
#define YDATA3                   0x0B // YDATA, Bits[19:12]
#define YDATA2                   0x0C // YDATA, Bits[11:4]
#define YDATA1                   0x0D // YDATA, Bits[3:0]; Reserved
#define ZDATA3                   0x0E // ZDATA, Bits[19:12]
#define ZDATA2                   0x0F // ZDATA, Bits[11:4]
#define ZDATA1                   0x10 // ZDATA, Bits[3:0]; Reserved
#define FIFO_DATA                0x11 // From 0x11-Ox1D, 13 registers can use for user-defined data store
                                      // There are 96 21-bit locations in the FIFO. Each location contains 20 bits of data and a marker bit for the x-axis data.
#define OFFSET_X_H               0x1E // OFFSET_X, Bits[15:8]
#define OFFSET_X_L               0x1F // OFFSET_X, Bits[7:0]
#define OFFSET_Y_H               0x20 // OFFSET_Y, Bits[15:8]
#define OFFSET_Y_L               0x21 // OFFSET_Y, Bits[7:0]
#define OFFSET_Z_H               0x22 // OFFSET_Z, Bits[15:8]
#define OFFSET_Z_L               0x23 // OFFSET_Z, Bits[7:0]
#define ACT_EN                   0x24 // Reserved, ACT_Z, ACT_Y, ACT_Z
#define ACT_THRESH_H             0x25 // ACT_THRESH, Bits[15:8]
#define ACT_THRESH_L             0x26 // ACT_THRESH, Bits[7:0]
#define ACT_COUNT                0x27 
#define FILTER                   0x28 // Bit7 is reserved, Bit 6 to 4 are HPF_CONNER, Bit 3 to 0 are ODR_LPF
#define FIFO_SAMPLES             0x29 // Bit7 is reserved, Bit 6 to 1 FIFO_SAMPLES 
#define INT_MAP                  0x2A
#define SYNC                     0x2B
#define RANGE                    0x2C // Bit 7 is I2C_HS, Bit 6 is INT_POL, Bit 5 to 2 is reserved, Bit 1 and 0 is Range (01:±2g, 10:±4g, 11:±8g) 
#define POWER_CTL                0x2D // Bit 7 to 3 is reserved, 
                                      // Bit 2 DRDY_OFF, Set to 1 to force the DRDY output to 0 in modes where it is normally signal data ready.
                                      // Bit 1 TEMP_OFF, Set to 1 to disable temperature processing. Temperature processing is also disabled when standby = 1. 
                                      // Bit 0 Standby or measurement mode.
                                      // 1 : standby mode;  In standby mode, the device is in a low power state, and the temperature and acceleration datapaths 
                                      // are not operating. In addition, digital functions, including FIFO pointers, reset.
                                      // 0 : measurement
#define SELF_TEST                0X2E
#define RESET                    0x2F

/***********************************************************************************************************************************/
// Device values
#define RANGE_2G                 0x01
#define RANGE_4G                 0x02
#define RANGE_8G                 0x03
#define MEASURE_MODE             0x06 // Only accelerometer; 0x06 = 0b 0000 0110; Data ready to output; temperature disable; Measurement mode; if want to change, refer to the datasheet
#define RESET_COMMAND            0x52 // Write Code 0x52 to reset the device, similar to a power-on reset (POR) 

#define SCALE_2G                 0.0000039 // 3.9ug/LSB ; (2-(-2))/(2^20)=0.000003814
#define SCALE_4G                 0.0000078
#define SCALE_8G                 0.0000156

// Operations
#define READ_BYTE                0x01  // /W/R = 1
#define WRITE_BYTE               0x00

/************************************************************************************************************************************/
// Pins used for the connection with the sensor
#define CHIP_SELECT_PIN          17
#define VSPI_MISO                MISO    //19 (default)
#define VSPI_MOSI                MOSI    //23 (default)
#define VSPI_SCLK                SCK     //18 (default)
#define VSPI_SS                  CHIP_SELECT_PIN      // ss = 5 (default); 这里改为 CHIP_SELECT_PIN (GPIO17)

#if CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
#define VSPI FSPI
#endif

// the SPI transder frequency (clock)
#define SPI_CLK                   1000000 // 1 MHz

/**
* Write registry in specific device address 
*/
extern "C" void writeRegister(
    uint8_t thisRegister, 
    uint8_t thisValue);

/**
* Read registry in specific device address
*/
extern "C" unsigned int readRegister(
    uint8_t thisRegister);

/**
* Read multiple registries
*/
extern "C" void readMultipleData(
    int *addresses, 
    int dataSize, 
    int *readedData);

/**
* Read X, Y, Z axis data
* @param readedData a pointer pointing to 3-float memory store
*/
extern "C" void readXYZ(float* readedData);

/**
* Initalize the SPI configuration
*/
extern "C" void init_SPI();

/**
* Initialize the ADX355
* this function will call init_SPI so don't need to call init_SPI again
*/
extern "C" void init_ADX355();

#endif