//
// Created by mikas on 26/01/2022.
//

#include "main.h"
#include "Server.h"

[[noreturn]]void ToggleSensors(void * parameter)
{
    TaskHandle_t *handle = (TaskHandle_t*) parameter;
    while(true)
    {
        running = !running;
        // turn ON
        if(running)
        {
            vTaskResume(handle[Tasks::Sensor]);
            notifyClients(running);
            vTaskDelete(nullptr);
        }
        // turn OFF
        vTaskSuspend(handle[Tasks::Sensor]);
        notifyClients(running);
        vTaskDelete(nullptr);
    }
}