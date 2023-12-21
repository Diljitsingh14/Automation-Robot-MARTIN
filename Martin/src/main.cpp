#include "Arduino.h"
#include "config.h"
#include "mic/test.h"
#include "mic/setup.h"

void setup()
{
  Serial.begin(SERIAL_BAUD_RATE);
  setup_mic();
}

void loop()
{
  test_mic();
}