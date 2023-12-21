// setup.h
#ifndef SETUP_H
#define SETUP_H

#include <driver/i2s.h>
#include "config.h"
#include "Arduino.h"

// int16_t sBuffer[bufferLen];

void i2s_install()
{
    i2s_config_t i2s_config = {
        .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = 8000,
        .bits_per_sample = i2s_bits_per_sample_t(16),
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
        .intr_alloc_flags = 0, // default interrupt priority
        .dma_buf_count = 8,
        .dma_buf_len = bufferLen,
        .use_apll = false};
    i2s_driver_install(I2S_MIC_PORT, &i2s_config, 0, NULL);
}

void i2s_setpin()
{
    const i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_MIC_CLOCK,
        .ws_io_num = I2S_MIC_WORD_SELECT,
        .data_out_num = -1,
        .data_in_num = I2S_MIC_DATA};
    i2s_set_pin(I2S_MIC_PORT, &pin_config);
}

void setup_mic()
{
    Serial.print("Setup I2S ...");
    delay(1000);
    i2s_install();
    i2s_setpin();
    i2s_start(I2S_MIC_PORT);
    delay(500);
}

#endif // SETUP_H