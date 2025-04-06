#ifndef TRANSISTOR_H
#define TRANSISTOR_H

#include <Arduino.h>

class Transistor
{
public:
    Transistor(uint8_t pin);
    void on();
    void off();
    void toggle();
    bool getState();
    void unlock();

    // Методи для роботи з прапорцем TransistorOpen
    void setTransistorOpen(bool state);
    bool isTransistorOpen() const;

private:
    uint8_t pin;
    bool state;
    unsigned long currentTime = 0;
    bool TransistorOpen = false; // Прапорець для контролю стану
};

#endif