/***********************************************************************************************************************
 * This Task toggles the Senors on or off. *
 *
 * author: Mika Schmitt scmi1066@h-ka.de *
 * last modified: 26.01.2022 *
***********************************************************************************************************************/
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