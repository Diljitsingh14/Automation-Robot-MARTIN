// console.cpp
#include "./Log/console.h"
#include "config.h"

Console::Console()
{
    Serial.begin(SERIAL_BAUD_RATE);
}

template <typename T>
void Console::log(T msg)
{
    Serial.println(msg);
}

template <typename T, typename... Args>
void Console::log(const char *format, T value, Args... args)
{
    // Format and print the log message
    Serial.printf(format, value, args...);
    Serial.println();
}

// Define the external instance of the Console class
Console console;