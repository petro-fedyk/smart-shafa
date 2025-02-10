#include "mainControll.h"

void MainControll::mainControll()
{
    if (isCorrectPin)
    {
        Transistor::on();
        keyPadControll::keyPadLoop(isCorrectPin(true));
    }
}