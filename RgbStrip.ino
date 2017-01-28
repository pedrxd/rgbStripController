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
#include "RgbController.h"
#include "LedController.h"

//Initializate strip
RgbController strip = RgbController(3,9,10);
LedController led   = LedController(4);

rgbColor white = {255,255,255};
rgbColor black = {0,0,0};

//Full rainbow colors -
rgbColor rainbow[7] =
{
  {255,255,255},
  {255,255,0},
  {255,0,255},
  {255,0,0},
  {0,255,255},
  {0,255,0},
  {0,0,255}
};

enum rgbModes
{
  M_STATICCOLOR,
  M_RAINBOW,
  M_STRIKE,
  M_RANDOMSTRIKE,
  M_BREATHING,
  M_MAXMODES
};

void setup()
{
  Serial.begin(9600);

  //Switch off the strip when starting.
  strip.setRGB(black);

  //Setting frequency to max for no blinks on led.
  TCCR1B = TCCR1B & 0b11111000 | 0x01;  //10 / 9 -> 31250Hz
  TCCR2B = TCCR2B & 0b11111000 | 0x01;  //3      -> 31250Hz
}

//Used for process modes
int step = 0;
unsigned long time = 0;
rgbColor lcolor;

int duration = 1200;
rgbModes mode = M_STATICCOLOR;

void commandParser()
{
  while(Serial.available() > 0)
  {
    switch(Serial.read())
    {
      //Turn on/off secondary light
      case 'l':
      {
        switch(Serial.parseInt())
        {
          case 0:
            led.turnOff();
          case 1:
            led.turnOn();
          case 2:
            led.strike(duration);
        }
      }break;
      //Colors , send a complete rgb color and with smoothchange mode if selected
      case 'c':
      {
        rgbColor motion;
        if(Serial.parseInt() == 1)
        {
          if(strip.isAnimationRunning() == A_NOTHING)
          {
            motion.r = Serial.parseInt();
            motion.g = Serial.parseInt();
            motion.b = Serial.parseInt();
            strip.startAnimation(A_SMOOTHCHANGE, duration, motion);
          }
        }else{
            motion.r = Serial.parseInt();
            motion.g = Serial.parseInt();
            motion.b = Serial.parseInt();
            strip.setRGB(motion);
        }
      }break;
      //duration of all animations or things that depend on time
      case 'd':
      {
        duration = Serial.parseInt();
      }break;
      //Change brithgness of the strip
      case 'a':
      {
        strip.setBrightness((byte)Serial.parseInt());
      }break;
      //Set color red
      case 'r':
      {
        strip.setRed((byte)Serial.parseInt());
        mode = M_STATICCOLOR;
      }break;
      //Set color green
      case 'g':
      {
        strip.setGreen((byte)Serial.parseInt());
        mode = M_STATICCOLOR;
      }break;
      //Set color blue
      case 'b':
      {
        strip.setBlue((byte)Serial.parseInt());
        mode = M_STATICCOLOR;
      }break;
      //Set a mode
      case 'm':
      {
        step = 0;
        time = 0;
        lcolor.r = strip.getRGB().r;
        lcolor.b = strip.getRGB().b;
        lcolor.g = strip.getRGB().g;
        mode = Serial.parseInt();
        strip.stopAnimation();
      }break;
      //Set mode to STATICCOLOR and turnoff the strip
      case 's':
      {
        mode = M_STATICCOLOR;
        strip.startAnimation(A_SMOOTHCHANGE, duration, black);
      }break;
    }
  }
}

void loop()
{
  //Check if is necesary to process some animation
  //non-block
  strip.processAnimation();
  //Check if necesary update the state of led
  led.processStrike();

  //Read and parse commands from the bluetooth module
  commandParser();

  //Execute mode
  switch(mode)
  {
    case M_RAINBOW:
    {
      if(strip.isAnimationRunning() == A_NOTHING)
      {
        strip.startAnimation(A_SMOOTHCHANGE, duration, rainbow[step++]);
        if(step > 6)
          step = 0;
      }
    }break;
    case M_STRIKE:
    {
      //If it is the time for update
      if(time < millis())
      {
        if(step)
        {
          step = 0;
          time = (2.0/3.0)*duration+ millis(); //Time off is 2/3 the time on
          strip.setRGB(lcolor);
        }else{
          step = 1;
          time = duration + millis();
          strip.setRGB(black);
        }
      }
    }break;
    case M_RANDOMSTRIKE:
    {
      //If it is the time for update
      if(time < millis())
      {
        int newcolor;
        //Get when the next step is needed
        time = millis()+duration;

        //Check if it is not the same color
        do
        {
          newcolor = random(6);
        }while(step == newcolor);

        step = newcolor;
        strip.setRGB(rainbow[step]);
      }

    }break;
    case M_BREATHING:
    {
      if(strip.isAnimationRunning() == A_NOTHING)
      {
        if(step)
        {
          strip.startAnimation(A_SMOOTHCHANGE, duration, lcolor);
          step = 0;
        }else
        {
          strip.startAnimation(A_SMOOTHCHANGE, duration, black);
          step = 1;
        }
      }
    }break;
  }
}
