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

#ifndef LEDCONTROLLER_h
#define LEDCONTROLLER_h

enum ledState
{
  STATE_OFF,
  STATE_ON,
};
enum ledMode
{
  MODE_NORMAL,
  MODE_STRIKE
};

class LedController
{
public:
  LedController(byte ledPin);
  turnOn(void);
  turnOff(void);
  strike(int duration);

  ledState getState(void);
  ledMode  getMode(void);

  processStrike();
private:
  byte ledPin;
  ledState ledState;
  ledMode   ledMode;
  int duration;
  unsigned long time;
};

#endif
