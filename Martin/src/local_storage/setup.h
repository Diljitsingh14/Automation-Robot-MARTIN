#ifndef LS_SETUP
#define LS_SETUP

#include "SD.h"
#include "config.h"

bool setup_ls()
{
    pinMode(SD_CS, OUTPUT);
    digitalWrite(SD_CS, HIGH); // SD card chips select, must use GPIO 5 (ESP32 SS)
    if (!SD.begin(SD_CS))
    {
        Serial.println("Error talking to SD card!");
        return false;
    };

    return true;
}

#endif