/***********************************************************************************************************************
 * author: Mika Schmitt scmi1066@h-ka.de *
 * last modified: 26.01.2022 *
***********************************************************************************************************************/

#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

static bool running;    // true if sensors are on
const char buttonPin = 25;
enum Tasks { Sensor, Server, Files };

void ToggleSensors();

#endif
