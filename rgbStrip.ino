/*
*   
*   Copyright 2016 Pedro Ruiz
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

#include "RGB.h"

//Initializate strip
RGB strip = RGB(5,3,6);


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
};

void setup()
{
  Serial.begin(9600);

  //Switch off the strip when starting.
  strip.setRGB(black);
}

//Used for process modes
int step = 0;
unsigned long time = 0;
rgbColor lcolor;

int duration = 1200;
rgbModes mode = M_STATICCOLOR;

void loop()
{
  //Check if is necesary to process some change on the Color
  //non-block
  strip.processAnimation();

  //Read and parse commands from the bluetooth module
  while(Serial.available() > 0)
  {
    switch(Serial.read())
    {
      //Duration of the animations
      case 'd':
      {
        duration = Serial.parseInt();
      }break;
      case 'a':
      {
        strip.setBrightness((byte)Serial.parseInt());
      }break;
      case 'r':
      {
        strip.setRed((byte)Serial.parseInt());
        mode = M_STATICCOLOR;
      }break;
      case 'g':
      {
        strip.setGreen((byte)Serial.parseInt());
        mode = M_STATICCOLOR;
      }break;
      case 'b':
      {
        strip.setBlue((byte)Serial.parseInt());
        mode = M_STATICCOLOR;
      }break;
      case 'm':
      {
        step = 0;
        time = 0;
        lcolor.r = strip.getRGB().r;
        lcolor.b = strip.getRGB().b;
        lcolor.g = strip.getRGB().g;
        mode = Serial.parseInt();
      }break;
      //Set mode to STATICCOLOR and turnoff the strip
      case 's':
      {
        mode = M_STATICCOLOR;
        strip.startAnimation(A_SMOOTHCHANGE, duration, black);
      }break;
    }
  }


  //Execute mode 
  switch(mode)
  {
    case M_RAINBOW:
    {
      if(strip.isAnimationRunning() == A_NOTHING && step == 0)
      {
        strip.startAnimation(A_SMOOTHCHANGE, duration, rainbow[step]);
        step = 1;
      }
      if(strip.isAnimationRunning() == A_NOTHING && step == 1)
      {
        strip.startAnimation(A_SMOOTHCHANGE, duration, rainbow[step]);
        step = 2;
      }
      if(strip.isAnimationRunning() == A_NOTHING && step == 2)
      {
        strip.startAnimation(A_SMOOTHCHANGE, duration, rainbow[step]);
        step = 3;
      }
      if(strip.isAnimationRunning() == A_NOTHING && step == 3)
      {
        strip.startAnimation(A_SMOOTHCHANGE, duration, rainbow[step]);
        step = 4;
      }
      if(strip.isAnimationRunning() == A_NOTHING && step == 4)
      {
        strip.startAnimation(A_SMOOTHCHANGE, duration, rainbow[step]);
        step = 5;
      }
      if(strip.isAnimationRunning() == A_NOTHING && step == 5)
      {
        strip.startAnimation(A_SMOOTHCHANGE, duration, rainbow[step]);
        step = 6;
      }
      if(strip.isAnimationRunning() == A_NOTHING && step == 6)
      {
        strip.startAnimation(A_SMOOTHCHANGE, duration, rainbow[step]);
        step = 0;
      }
      break;
    }
    case M_STRIKE:
    {
      //If it is the time for update
      if(time < millis())
      {
        if(step)
        {
          step = 0;
          time = (2.0/3.0)*duration+ millis();
          strip.setRGB(lcolor);
        }else{
          step = 1;
          time = duration + millis();
          strip.setRGB(black);
        }
      }
      break;
      break;
    }
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
      break;
    }
  }
}


