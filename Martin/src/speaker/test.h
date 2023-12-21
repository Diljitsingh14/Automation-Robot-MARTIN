#ifndef SPEAKER_TEST
#define SPEAKER_TEST

#include "setup.h"
#include "methods.h"
#include "local_storage/setup.h"

bool test_speaker(bool is_simple_test = true)
{
    if (is_simple_test)
    {
        audio.connecttospeech("This is a unit testing of module name speaker. If you heard the audio then test pass", "en");
        audio.loop();
        return true;
    }
    WavFile = SD.open("/heavenly.wav"); // Open the wav file
    if (WavFile == false)
        Serial.println("Could not open 'heavenly.wav'");
    else
    {
        WavFile.read((byte *)&WavHeader, 44);                    // Read in the WAV header, which is first 44 bytes of the file.
                                                                 // We have to typecast to bytes for the "read" function
        DumpWAVHeader(&WavHeader);                               // Dump the header data to serial, optional!
        if (ValidWavData(&WavHeader))                            // optional if your sure the WAV file will be valid.
            i2s_set_sample_rates(i2s_num, WavHeader.SampleRate); // set sample rate
    }

    PlayWav();
}

#endif