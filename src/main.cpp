/***********************************************************************************************************************
 * This program is a utility to aid with tuning and calibrating spring damper systems. It reads data from two linear
 * potentiometers and provides a website where the data is depicted in two charts for easy analysis.
 *
 * references: Charts by Highcharts https://www.highcharts.com licenced under a Student License (Creative Commons (CC) Attribution-Non-Commercial) *
 *
 * author: Mika Schmitt scmi1066@h-ka.de *
 * usage: boot up the ESP32 -> Connect to the AP with the SSID "ESP32". The password is 123456789 -> Open Website on 192.168.4.1 *
 * last modified: 26.01.2022 *
***********************************************************************************************************************/

#include <esp_task_wdt.h>
#include <SPIFFS.h>
#include <queue>
#include "FileHandleTask.h"
#include "SensorReadTask.h"
#include "Server.h"
#include "main.h"
#include "soc/rtc_wdt.h"
#include "ToggleSensorsTask.h"


const char *ssid = "ESP32";
const char *password = "123456789";

static std::queue<char> data_queue; // queue where data from the sensors is stored before beeing written to a file

TaskHandle_t TaskHandles[3] = {nullptr, nullptr, nullptr}; // Handles for Sensor Server and File Tasks

void ClientConnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    // When a Client connects to the AP and the Server-Task is suspended the Servertask is resumed and the Server is started
    Serial.println("Client connected to AP");
    if(eTaskGetState(TaskHandles[Tasks::Server]) == eSuspended)
    {
        Serial.println("Staring Server");
        vTaskResume(TaskHandles[Tasks::Server]);
    }
}

void ClientDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    // When the last client disconnects from the AP the Server-Task is suspended
    Serial.println("Client disconnected from AP");
    if(!WiFi.softAPgetStationNum())
    {
        Serial.println("No Client Connected to AP. Stopping Server");
        vTaskSuspend(TaskHandles[Tasks::Server]);
    }
}

// This function is run when the Button is pressed and starts a Task that toggles the Sensors on or off
void IRAM_ATTR RunToggleSensorTask()
{
    xTaskCreatePinnedToCore(ToggleSensors, "SensorToggle", 10000, (void *)TaskHandles, 0, nullptr, 1);
}

void setup()
{
    Serial.begin(115200);

    running = false;

    pinMode(buttonPin, INPUT);

    // Mounting the file system
    if(!SPIFFS.begin(true)){
        Serial.println("An Error has occurred while mounting SPIFFS");
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
    xTaskCreatePinnedToCore(ServerTask, "Server", 10000, (void *)TaskHandles, 0, &TaskHandles[Tasks::Server], 0);
    vTaskSuspend(TaskHandles[Tasks::Server]);

    // Interrupt for the Button that will toggle the Sensors
    attachInterrupt(buttonPin, RunToggleSensorTask ,RISING);
}

void loop()
{
    // Feeding the Watchdog so it doesn't trigger
    rtc_wdt_feed();
    vTaskDelay(pdMS_TO_TICKS(100));
}
