#ifndef SPEAKER_TEST
#define SPEAKER_TEST

#include "setup.h"

bool test_speaker()
{
    audio.connecttospeech("This is a unit testing of module name speaker. If you heard the audio then test pass", "en");
    audio.loop();
    delay(2000);
}

#endif