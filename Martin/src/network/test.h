#ifndef WIFI_TEST
#define WIFI_TEST

#include "WiFi.h"
#include "Arduino.h"
#include "HTTPClient.h"

bool test_internet()
{
    HTTPClient http;
    Serial.println("Testing Internet HTTP Connection...");

    // Begin Http
    http.begin("http://example.com/index.html");
    // get status
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0)
    {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK)
        {
            String payload = http.getString();
            Serial.println(payload);
            http.end();
            return true;
        }
    }

    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    http.end();
    return false;
}

#endif