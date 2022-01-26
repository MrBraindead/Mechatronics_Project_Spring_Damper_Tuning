/***********************************************************************************************************************
 * This Task runs the WebServer and handles messages on the Websocket. *
 *
 * author: Mika Schmitt scmi1066@h-ka.de *
 * last modified: 25.01.2022 *
***********************************************************************************************************************/

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

void sendDataUri()
{
    ws.textAll("/data.bin");
}

void notifyClients(bool state)
{
    if(state)
    {
        ws.textAll("SensorON");
        return;
    }
    ws.textAll("SensorOFF");
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t length)
{
    AwsFrameInfo *info = (AwsFrameInfo *) arg;
    if (info->final && info->index == 0 && info->len == length && info->opcode == WS_TEXT)
    {
        data[length] = 0;
        if (strcmp((char *) data, "getData") == 0)
        {
            sendDataUri();
            return;
        }
        if(strcmp((char *) data, "toggleSensors") == 0)
        {
            running = !running;
            ToggleSensors(running);
            notifyClients(running);
            return;
        }
        if(strcmp((char *) data, "clear") == 0)
        {
            File file = SPIFFS.open("/data.bin", FILE_WRITE);
            if(file)
            {
                file.print("");
                file.close();
            }
            return;
        }
        Serial.println(reinterpret_cast<char*>(data));
    }
}

void EventHandle(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t length)
{
    switch (type)
    {
        case WS_EVT_CONNECT:
            Serial.printf("WebSocket %s: client %u connected from %s\n", server->url(), client->id(), client->remoteIP().toString().c_str());
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("WebSocket %s: client %u disconnected", server->url(), client->id());
            break;
        case WS_EVT_DATA:
            handleWebSocketMessage(arg, data, length);
            break;
        case WS_EVT_ERROR:
            Serial.printf("WebSocket %s: client %u error %u: %s\n", server->url(), client->id(), *((uint16_t *) arg), (char *) data);
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

void ServerTask(void *parameter)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 100;
    xLastWakeTime = xTaskGetTickCount();

    int i = 0;
    initWebSocket();

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(SPIFFS, "/page.html", String(), false);
    });
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(SPIFFS, "/style.css");
    });
    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(SPIFFS, "/script.js");
    });
    server.on("/highcharts.js", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(SPIFFS, "/highcharts.js");
    });
    server.on("/data.bin", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(SPIFFS, "/data.bin");
    });

    server.begin();

    while (true)
    {
        ws.cleanupClients();
        digitalWrite(ledPin, ledState);
        taskYIELD();
    }
}
