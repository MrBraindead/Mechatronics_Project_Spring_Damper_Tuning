#include <esp_task_wdt.h>

#include "SensorReadTask.h"
#include "Server.h"
#include "main.h"

const char *ssid = "ESP32";
const char *password = "123456789";

char sample_size = 10;

enum Tasks { Sensor, Server, Optimizer };

TaskHandle_t TaskHandles[2] = {nullptr, nullptr};

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

    WiFi.softAP(ssid, password);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AccessPoint IP: ");
    Serial.println(IP);
    WiFi.onEvent(ClientConnected, SYSTEM_EVENT_AP_STACONNECTED);
    WiFi.onEvent(ClientDisconnected, SYSTEM_EVENT_AP_STADISCONNECTED);

    xTaskCreatePinnedToCore(SensorReadTask, "Sensor", 10000, (void*)&sample_size, 1, &TaskHandles[Tasks::Sensor], 1);
    vTaskSuspend(TaskHandles[Tasks::Sensor]);

    xTaskCreatePinnedToCore(ServerTask, "Server", 10000, nullptr, 1, &TaskHandles[Tasks::Server], 0);
    vTaskSuspend(TaskHandles[Tasks::Server]);

    //xTaskCreatePinnedToCore(OptimizeTasks, "OptimizeTasks", 10000, (void*)&TaskHandles, 1, TaskHandles[2], 0);
    esp_task_wdt_delete(xTaskGetIdleTaskHandle());
}

void loop()
{

}