#include "transistor.h"

#define UNLOCL_TIME 2000

Transistor::Transistor(uint8_t pin) : pin(pin), state(false), currentTime(0)
{
    pinMode(pin, OUTPUT);
    off();
}

void Transistor::on()
{
    if (!state)
    {
        digitalWrite(pin, HIGH);
        state = true;
    }
}

void Transistor::off()
{
    if (state)
    {
        digitalWrite(pin, LOW);
        state = false;
    }
}

void Transistor::toggle()
{
    if (state)
    {
        off();
    }
    else
    {
        on();
    }
}

bool Transistor::getState()
{
    return state;
}

void Transistor::unlock()
{
    unsigned long now = millis();

    if (!state)
    {
        on();
        currentTime = now;
    }
    else if (now - currentTime >= UNLOCL_TIME)
    {
        off();
        TransistorOpen = false; // Скидаємо прапорець після вимкнення
    }
}

void Transistor::setTransistorOpen(bool state)
{
    TransistorOpen = state;
}

bool Transistor::isTransistorOpen() const
{
    return TransistorOpen;
}