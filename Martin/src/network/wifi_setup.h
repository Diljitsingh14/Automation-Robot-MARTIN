#ifndef WIFI_SETUP
#define WIFI_SETUP

#include "config.h"
#include "Arduino.h"
#include <WiFi.h>

bool setup_wifi()
{
    Serial.println((String) "Connecting to Wifi : " + WIFI_SSID);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(500);

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.print(WiFi.localIP());

    return true;
}

#endif