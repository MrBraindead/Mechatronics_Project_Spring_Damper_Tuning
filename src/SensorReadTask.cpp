/***********************************************************************************************************************
 * This task reads the output of the sensors, processes the information and the send the data to another class that is
 * tasked with storing the data *
 *
 * author: Mika Schmitt scmi1066@h-ka.de *
 * last modified: 26.01.2022 *
***********************************************************************************************************************/

#include <Arduino.h>
#include <esp_task_wdt.h>
#include "SensorReadTask.h"
#include <queue>

void SensorReadTask( void * parameter )
{
    TickType_t lastWakeTime;
    const TickType_t period = 10; // Specifies the period the task runs at in milliseconds
    pinMode(potentiometer, INPUT);
    while(true)
    {
        try
        {
            // Runs the Task at a given period
            vTaskDelayUntil(&lastWakeTime, period);

            unsigned int input = analogRead(potentiometer);
            // Projecting 12 bit value to 8 bit by shifting 4 to the right
            // This way we only have to store and send half the data.
            // We lose 4 bits of precision but this is totally fine
            // in this application
            ((std::queue<char>*)parameter)->push((char)(input >> 4));
        }
        catch(std::exception &e)
        {
            // The only exception that is expected here is when the stack is full. This should never happen but if it does
            // then there probably is a critical error somewhere else in the code and there is no point in running this task.
            // Therefore, it is being suspended.
            Serial.printf("Error %s", e.what());
            vTaskSuspend(nullptr);
        }
    }
}
