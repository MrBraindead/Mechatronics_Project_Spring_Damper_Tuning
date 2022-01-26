/***********************************************************************************************************************
 *  *
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
        if(!((std::queue<char>*)parameter)->empty())
        {
            File file = SPIFFS.open("/data.bin", FILE_APPEND);
            if(!file)
            {
                Serial.println("Could not open file data.bin for writing");
                vTaskDelete(nullptr);
            }
            while(!((std::queue<char>*)parameter)->empty())
            {
                file.print((char)((std::queue<char>*)parameter)->front());
                ((std::queue<char>*)parameter)->pop();
            }
            Serial.println("Emptied queue");
            file.close();
            taskYIELD();
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
