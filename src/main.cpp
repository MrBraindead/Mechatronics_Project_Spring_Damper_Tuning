/***********************************************************************************************************************
 *  *
 *
 * references: Charts by Highchart https://www.highcharts.com licenced under a Student License (Creative Commons (CC) Attribution-Non-Commercial) *
 *
 * author: Mika Schmitt scmi1066@h-ka.de *
 * usage: boot up the ESP32 -> Connect to the AP with the SSID "ESP32" -> Open Website on 192.168.4.1 *
 * last modified: 25.01.2022 *
***********************************************************************************************************************/

#include <esp_task_wdt.h>
#include <SPIFFS.h>
#include <DNSServer.h>
#include "FileHandleTask.h"
#include "SensorReadTask.h"
#include "Server.h"
#include "main.h"
#include <queue>

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

void setup()
{
    Serial.begin(115200);

    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    if(!SPIFFS.begin(true)){
        Serial.println("An Error has occurred while mounting SPIFFS");
        vTaskDelete(nullptr);
    }

    WiFi.softAP(ssid, password);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AccessPoint IP: ");
    Serial.println(IP);
    WiFi.onEvent(ClientConnected, SYSTEM_EVENT_AP_STACONNECTED);
    WiFi.onEvent(ClientDisconnected, SYSTEM_EVENT_AP_STADISCONNECTED);

    xTaskCreatePinnedToCore(SensorReadTask, "Sensor", 10000, (void*)&data_queue, 1, &TaskHandles[Tasks::Sensor], 1);
    //vTaskSuspend(TaskHandles[Tasks::Sensor]);
    xTaskCreatePinnedToCore(FileHandleTask, "Files", 10000, (void*)&data_queue, 1, &TaskHandles[Tasks::Files], 0);
    xTaskCreatePinnedToCore(ServerTask, "Server", 10000, nullptr, 0, &TaskHandles[Tasks::Server], 0);
    vTaskSuspend(TaskHandles[Tasks::Server]);

    vTaskDelete(xTaskGetIdleTaskHandle());
}

void loop()
{
    taskYIELD();
}
