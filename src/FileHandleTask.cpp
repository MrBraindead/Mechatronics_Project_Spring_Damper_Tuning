/***********************************************************************************************************************
 * Stores the data collected by the sensors to a file *
 *
 * author: Mika Schmitt scmi1066@h-ka.de *
 * last modified: 25.01.2022 *
***********************************************************************************************************************/

#include "SPIFFS.h"
#include "main.h"
#include "FileHandleTask.h"
#include <queue>

[[noreturn]] void FileHandleTask(void * parameter)
{
    while(true)
    {
        // if there are elements in the queue
        if(!((std::queue<char>*)parameter)->empty())
        {
            // try to open file
            File file = SPIFFS.open("/data.bin", FILE_APPEND);
            if(!file)
            {
                Serial.println("Could not open file data.bin for writing");
                vTaskDelete(nullptr);
            }
            // while there are elements in the queue
            while(!((std::queue<char>*)parameter)->empty())
            {
                // write elements to file and remove from the queue
                file.print((char)((std::queue<char>*)parameter)->front());
                ((std::queue<char>*)parameter)->pop();
            }
            file.close();
            taskYIELD();
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
