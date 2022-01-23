#include <Arduino.h>
#include <esp_task_wdt.h>
#include "SensorReadTask.h"

void SensorReadTask( void * parameter )
{
    while(true)
    {
        sleep(1);
        taskYIELD();
    }
    /*const char sample_size = *((char *) parameter);
    pinMode(poti, INPUT);
    float sum;
    while(true){
        try
        {
            sum = 0;
            for(int i = 0; i < sample_size; i++)
                sum += digitalRead(poti);
            Serial.print(sum / sample_size);
            taskYIELD();
        }
        catch(std::exception e)
        {
            Serial.printf("Error %s", e.what());
        }
    }*/
}