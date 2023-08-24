#ifndef _AD8232_H
#define _AD8232_H

#include "Arduino.h"

// pins configuration
#define L0_POSITIVE 16
#define L0_NEGATIVE 4
#define ANALOG_INPUT 36

/**
* initilize AD8232
*/
extern "C" void init_AD8232();

/**
* get ECG signal from AnalogRead
*/
extern "C" float getECG();

#endif