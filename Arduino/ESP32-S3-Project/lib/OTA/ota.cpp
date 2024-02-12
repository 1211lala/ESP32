#include "ota.h"

AsyncWebServer AsyncWebserver(80);

/*
"<form action=\"/update\" method=\"GET\">
    <input type=\"submit\" value=\"update\">
</form>"
*/
void handleRoot (AsyncWebServerRequest *request)
{
    request->send(200, "text/html", "<form action=\"/UART\" method=\"GET\"><input type=\"submit\" value=\"UART\"></form>");
}

void handleUart (AsyncWebServerRequest *request)
{
   Serial.printf("uart\r\n");
   
   request->send(200, "text/html", "<form action=\"/UART\" method=\"GET\"><input type=\"submit\" value=\"UART\"></form>");
   request->send(303);
}

void async_elegant_ota_start(void)
{
    AsyncWebserver.on("/", HTTP_GET,  handleRoot);

    AsyncWebserver.on("/UART", HTTP_GET,  handleUart);

    AsyncElegantOTA.begin(&AsyncWebserver);
    
    AsyncWebserver.begin();
}