#include <Arduino.h>
#include <esp_task_wdt.h>

#include "SensorReadTask.h"
#include "Server.h"
#include "main.h"

const char* ssid = "ESP32";
const char* password = "123456789";

TaskHandle_t SensorTaskHandle = NULL;
TaskHandle_t ServerTaskHandle = NULL;

WiFiServer server(80);

void setup() {
    Serial.begin(115200);

    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    WiFi.softAP(ssid, password);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AccessPoint IP: ");
    Serial.println(IP);

    server.begin();

    xTaskCreatePinnedToCore(SensorReadTask, "Sensor", 10000, NULL, 1, &SensorTaskHandle,  1);
    vTaskSuspend(SensorTaskHandle);

    //xTaskCreatePinnedToCore(ServerTask, "Server", 10000, (void*)&server, 0, &ServerTaskHandle,  0);
}
void loop() {
    //taskYIELD();
}

