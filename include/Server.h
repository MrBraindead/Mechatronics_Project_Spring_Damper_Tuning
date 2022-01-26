/***********************************************************************************************************************
 * author: Mika Schmitt scmi1066@h-ka.de *
 * last modified: 19.01.2022 *
***********************************************************************************************************************/

#ifndef SERVER_H
#define SERVER_H

#include <WiFi.h>

void notifyClients(bool state);
void ServerTask( void * pvParameters );

#endif
