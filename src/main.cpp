/***********************************************************************************************************************
 *  *
 *
 * references: Charts by Highcharts https://www.highcharts.com licenced under a Student License (Creative Commons (CC) Attribution-Non-Commercial) *
 *
 * author: Mika Schmitt scmi1066@h-ka.de *
 * usage: boot up the ESP32 -> Connect to the AP with the SSID "ESP32" -> Open Website on 192.168.4.1 *
 * last modified: 25.01.2022 *
***********************************************************************************************************************/

#include <esp_task_wdt.h>
#include <SPIFFS.h>
#include <queue>
#include "FileHandleTask.h"
#include "SensorReadTask.h"
#include "Server.h"
#include "main.h"
#include "soc/rtc_wdt.h"


const char *ssid = "ESP32";
const char *password = "123456789";

static std::queue<char> data_queue;

enum Tasks { Sensor, Server, Files };
TaskHandle_t TaskHandles[3] = {nullptr, nullptr, nullptr};

void ClientConnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("Client connected to AP");
    if(eTaskGetState(TaskHandles[Tasks::Server]) == eSuspended)
    {
        Serial.println("Staring Server");
        vTaskResume(TaskHandles[Tasks::Server]);
    }
}

void ClientDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("Client disconnected from AP");
    if(!WiFi.softAPgetStationNum())
    {
        Serial.println("No Client Connected to AP. Stopping Server");
        vTaskSuspend(TaskHandles[Tasks::Server]);
    }
}

void ToggleSensors(bool state)
{
    // turn ON
    if(state)
    {
        vTaskResume(TaskHandles[Tasks::Sensor]);
        return;
    }
    // turn OFF
    vTaskSuspend(TaskHandles[Tasks::Sensor]);
}

void setup()
{
    Serial.begin(115200);

    running = false;

    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    if(!SPIFFS.begin(true)){
        Serial.println("An Error has occurred while mounting SPIFFS");
        vTaskDelete(nullptr);
    }

    // Starting AccessPoint and printing the AccessPoints IP Address to the console.
    // The IP Address is needed to connect to the website.
    WiFi.softAP(ssid, password);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AccessPoint IP: ");
    Serial.println(IP);

    // Create Wi-Fi events
    WiFi.onEvent(ClientConnected, SYSTEM_EVENT_AP_STACONNECTED);
    WiFi.onEvent(ClientDisconnected, SYSTEM_EVENT_AP_STADISCONNECTED);

    // Create tasks.
    // All tasks are immediately suspended. They will be enabled when needed.
    xTaskCreatePinnedToCore(SensorReadTask, "Sensor", 10000, (void*)&data_queue, 1, &TaskHandles[Tasks::Sensor], 1);
    vTaskSuspend(TaskHandles[Tasks::Sensor]);
    xTaskCreatePinnedToCore(FileHandleTask, "Files", 10000, (void*)&data_queue, 1, &TaskHandles[Tasks::Files], 0);
    xTaskCreatePinnedToCore(ServerTask, "Server", 10000, nullptr, 0, &TaskHandles[Tasks::Server], 0);
    vTaskSuspend(TaskHandles[Tasks::Server]);
}

void loop()
{
    rtc_wdt_feed();
    vTaskDelay(pdMS_TO_TICKS(100));
}
