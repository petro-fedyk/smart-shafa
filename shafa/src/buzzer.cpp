#include "buzzer.h"

myBuzzer::myBuzzer(uint8_t pin)
{
    buzzPin = pin;
    pinMode(buzzPin, OUTPUT);
}

void myBuzzer::greatingSound()
{
    pinMode(buzzPin, OUTPUT);
    for (int i = 0; i < gratingTone; i++)
    {
        tone(buzzPin, frequencyGR, timeGR);
    }
    Serial.println("buzzer greating");
}

void myBuzzer::successSound()
{
    for (int i = 0; i < successTone; i++)
    {
        tone(buzzPin, frequencySUC, timeSUC);
    }
}

void myBuzzer::unsuccessSound()
{
    for (int i = 0; i < unsuccessTone; i++)
    {
        tone(buzzPin, frequencyUnSUC, timeUnSUC);
    }
}

void myBuzzer::alarmSound()
{
    for (int i = 0; i < alarmTone; i++)
    {
        tone(buzzPin, frequencyAlarm, timeAlarm);
    }
    Serial.println("alarm sound");
}