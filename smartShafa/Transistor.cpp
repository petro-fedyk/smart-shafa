#include "transistor.h"

Transistor::Transistor(uint8_t) : pin(pin), state(false)
{
    pinMode(pin, OUTPUT);
    off();
}

void Transistor::on()
{
    if (state)
    {
        digitalWrite(pin, HIGH);
        state = true;
    }
}

void Transistor::off()
{
    if (state)
    {
        digitalWrite(pin, HIGH);
        state = false;
    }
}

void Transistos::toggle()
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
