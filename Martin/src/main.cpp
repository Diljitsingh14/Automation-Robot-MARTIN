#include "Arduino.h"
#include "config.h"
#include "mic/test.h"
#include "mic/setup.h"
#include "network/wifi_setup.h"
#include "network/test.h"

void setup()
{
  Serial.begin(SERIAL_BAUD_RATE);
  // setup_mic();
  setup_wifi();
}

void loop()
{
  // test_mic();
  test_internet();
}