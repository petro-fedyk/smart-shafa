#ifndef TRANSISTOR_H
#define TRANSISTOR_H

#include <Arduino.h>

class Transistor
{
private:
    uint8_t pin;
    bool state;

public:
    Transistor(uint8_t pin);
    void on();
    void off();
    void toggle();
    bool getState();
};

#endif