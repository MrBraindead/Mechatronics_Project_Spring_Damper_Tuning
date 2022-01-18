#include <Arduino.h>
#include <esp_task_wdt.h>
#include "SensorReadTask.h"

[[noreturn]] void SensorReadTask( void * pvParameters ){
    pinMode(poti, INPUT);
    while(true){
        float t = analogRead(poti);
        Serial.print(t);
        taskYIELD();
    }
}