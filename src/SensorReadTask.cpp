/***********************************************************************************************************************
 * This task reads the output of the sensors, processes the information and the send the data to another class that is
 * tasked with storing the data *
 *
 * author: Mika Schmitt scmi1066@h-ka.de *
 * last modified: 25.01.2022 *
***********************************************************************************************************************/

#include <Arduino.h>
#include <esp_task_wdt.h>
#include "SensorReadTask.h"
#include <queue>

void SensorReadTask( void * parameter )
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 50;
    pinMode(potentiometer, INPUT);
    while(true)
    {
        try
        {
            unsigned long t_zero = millis();
            for(int i = 0; i < 1000; i++)
            {
                vTaskDelayUntil(&xLastWakeTime, xFrequency);

                unsigned int input = analogRead(potentiometer);
                // Projecting 12 bit value to 8 bit by shifting 4 to the right
                // This way we only have to store and send half the data.
                // We lose 4 bits of precision but this is totally fine
                // in this application
                ((std::queue<char>*)parameter)->push((char)(input >> 4));
            }
            unsigned long delta = (millis() - t_zero);
            Serial.println(delta);
        }
        catch(std::exception &e)
        {
            Serial.printf("Error %s", e.what());
            vTaskDelete(nullptr);
        }
    }
}
