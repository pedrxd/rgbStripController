/*
*
*   Copyright (C) 2016 Pedro Ruiz
*
*   This file is part of rgbStrip.
*
*   rgbStrip is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   rgbStrip is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with rgbStrip.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#include <Arduino.h>
#include "LedController.h"

LedController::LedController (byte pin)
{
  ledPin = pin;
  pinMode(ledPin, OUTPUT);
}

LedController::turnOn(void)
{
  digitalWrite(ledPin, HIGH);
  ledState = STATE_ON;
  ledMode  = MODE_NORMAL;
}
LedController::turnOff(void)
{
  digitalWrite(ledPin, LOW);
  ledState = STATE_OFF;
  ledMode  = MODE_NORMAL;
}
LedController::strike(int delay)
{
  duration = delay;
  ledMode  = MODE_STRIKE;
}
ledMode   LedController::getMode(void)
{
  return ledMode;
}
ledState LedController::getState(void)
{
  return ledState;
}
LedController::processStrike()
{
  if(getMode() == MODE_STRIKE)
  {
    if(time < millis())
    {
      time = millis() + duration;
      if(!getState())
        turnOn();
      else
        turnOff();
    }
  }
}
