/***********************************************************************************************************************
 * author: Mika Schmitt scmi1066@h-ka.de *
 * last modified: 26.01.2022 *
***********************************************************************************************************************/

#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

static bool running;
const int ledPin = 32;

void ToggleSensors(bool state);

#endif
