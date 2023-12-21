#ifndef SPEAKER_SETUP_H
#define SPEAKER_SETUP_H

#include "Arduino.h"
#include "driver/i2s.h"
#include "config.h"
#include "Audio.h"

Audio audio;

static const i2s_port_t i2s_num = I2S_SPEAKER_PORT;

void i2s_speaker_driver_install()
{
    const i2s_config_t i2s_config =
        {
            .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
            .sample_rate = 44100, // Note, this will be changed later
            .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
            .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
            .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
            .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // high interrupt priority
            .dma_buf_count = 8,                       // 8 buffers
            .dma_buf_len = 64,                        // 64 bytes per buffer, so 8K of buffer space
            .use_apll = 0,
            .tx_desc_auto_clear = true,
            .fixed_mclk = -1};
    i2s_driver_install(i2s_num, &i2s_config, 0, NULL);
}

void i2s_speaker_pin_setup()
{
    const i2s_pin_config_t pin_config =
        {
            .bck_io_num = I2S_SPEAKER_BCLK,   // The bit clock connectiom, goes to pin 27 of ESP32
            .ws_io_num = I2S_SPEAKER_LRC,     // Word select, also known as word select or left right clock
            .data_out_num = I2S_SPEAKER_DOUT, // Data out from the ESP32, connect to DIN on 38357A
            .data_in_num = I2S_PIN_NO_CHANGE  // we are not interested in I2S data into the ESP32
        };
    i2s_set_pin(i2s_num, &pin_config);
}

bool setup_speaker(bool is_audio_lib = true)
{
    Serial.print("Setup Speaker I2S connection: mode = ");
    Serial.print((String)(is_audio_lib ? "audio lib" : "core"));
    Serial.println();

    if (is_audio_lib)
    {
        audio.setPinout(I2S_SPEAKER_BCLK, I2S_SPEAKER_LRC, I2S_SPEAKER_DOUT);
        audio.setVolume(100);
        return true;
    }
    delay(1000);
    i2s_speaker_driver_install();
    i2s_speaker_pin_setup();
}

#endif