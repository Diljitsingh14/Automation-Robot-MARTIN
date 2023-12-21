#include "Arduino.h"
#include "config.h"
#include "mic/test.h"
#include "mic/setup.h"
#include "network/wifi_setup.h"
#include "network/test.h"
#include "speaker/setup.h"
#include "speaker/test.h"

void setup()
{
  Serial.begin(SERIAL_BAUD_RATE);
  // setup_mic();
  setup_wifi();
  setup_speaker();
}

void loop()
{
  // test_mic();
  // test_internet();
  test_speaker();
}