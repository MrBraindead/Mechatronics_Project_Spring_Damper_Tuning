#ifndef UNTITLED1_SERVER_H
#define UNTITLED1_SERVER_H

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

[[noreturn]] void ServerTask( void * pvParameters );

#endif //UNTITLED1_SERVER_H
