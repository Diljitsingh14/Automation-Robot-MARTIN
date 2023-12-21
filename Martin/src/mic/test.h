// test.h
#ifndef TEST_H
#define TEST_H

#include <driver/i2s.h>
#include "Arduino.h"
#include "config.h"
// #include "setup.h"

int16_t sBuffer[bufferLen];

void test_mic()
{
    size_t byteIn = 0;
    esp_err_t result = i2s_read(I2S_MIC_PORT, &sBuffer, bufferLen, &byteIn, portMAX_DELAY);
    if (result == ESP_OK)
    {
        int samples_read = byteIn / 8;
        if (samples_read > 0)
        {
            float mean = 0;
            for (int i = 0; i < samples_read; i++)
            {
                mean += (sBuffer[i]);
            }
            mean /= samples_read;
            Serial.println(mean);
        }
    }
}

#endif // TEST_H