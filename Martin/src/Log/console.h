// console.h
#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <Arduino.h>

class Console
{
public:
    Console();
    template <typename T>
    void log(T msg);
    template <typename T, typename... Args>
    void log(const char *format, T value, Args... args);
};

// Declare the external instance of the Console class
extern Console console;

#endif
