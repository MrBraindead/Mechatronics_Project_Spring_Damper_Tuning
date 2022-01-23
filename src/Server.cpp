#include <Arduino.h>
#include <WiFi.h>
#include <esp_task_wdt.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>

#include "Server.h"
#include "main.h"

bool ledState = 0;
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char *index_html;

void notifyClients()
{
    ws.textAll(String(ledState));
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t length)
{
    AwsFrameInfo *info = (AwsFrameInfo *) arg;
    if (info->final && info->index == 0 && info->len == length && info->opcode == WS_TEXT)
    {
        data[length] = 0;
        if (strcmp((char *) data, "toggle") == 0)
        {
            ledState = !ledState;
            notifyClients();
        }
        else{
            Serial.println(reinterpret_cast<char*>(data));
        }
    }
}

void EventHandle(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t length)
{
    switch (type)
    {
        case WS_EVT_CONNECT:
            Serial.printf("WebSocket %s: client %u connected from %s\n", server->url(), client->id(),
                          client->remoteIP().toString().c_str());
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("WebSocket %s: client %u disconnected", server->url(), client->id());
            break;
        case WS_EVT_DATA:
            handleWebSocketMessage(arg, data, length);
            break;
        case WS_EVT_ERROR:
            Serial.printf("WebSocket %s: client %u error %u: %s\n", server->url(), client->id(), *((uint16_t *) arg),
                          (char *) data);
            break;
        case WS_EVT_PONG:
            break;
    }
}

void initWebSocket()
{
    ws.onEvent(EventHandle);
    server.addHandler(&ws);
}

String processor(const String &var)
{
    Serial.println(var);
    if (var == "STATE")
    {
        if (ledState)
        {
            return "ON";
        }
        return "OFF";
    }
    return {};
}

void ServerTask(void *parameter)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 100;
    xLastWakeTime = xTaskGetTickCount();

    if(!SPIFFS.begin(true)){
        Serial.println("An Error has occurred while mounting SPIFFS");
        vTaskDelete(nullptr);
    }

    File file = SPIFFS.open("/page.html");
    if(!file){
        Serial.println("Failed to open file for reading");
        vTaskDelete(nullptr);
    }

    while(file.available()){
        index_html = reinterpret_cast<const char *>(file.read());
    }
    file.close();

    int i = 0;
    initWebSocket();

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(SPIFFS, "/page.html", String(), false, processor);
    });

    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(SPIFFS, "/style.css");
    });

    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(SPIFFS, "/script.js");
    });


    server.begin();

    while (true)
    {
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        i++;
        ws.cleanupClients();
        digitalWrite(ledPin, ledState);
        taskYIELD();
    }
}