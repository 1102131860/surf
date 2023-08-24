#ifndef _MAX30102_BASICREAD_H
#define _MAX30102_BASICREAD_H

#include <DFRobot_MAX30102.h>

/**
* Include two parts
* 1. check whether connect to max30102
* 2. configuration
*/
extern "C" void init_MAX30102();

extern "C" float getRed();

extern "C" float getIr();

#endif