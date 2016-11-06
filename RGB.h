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


#ifndef RGB_h
#define RGB_h
#include <Arduino.h>


enum animationType
{
  A_NOTHING,
  A_SMOOTHCHANGE,
};

typedef struct {
  byte r;
  byte g;
  byte b;
} rgbColor;

typedef struct
{
  animationType at;
  
  long startat;
  int duration;
  
  rgbColor color;
  rgbColor startcolor;
  float rgbPerMillis[3];
} animation;

typedef struct{
  byte REDP;
  byte GREENP;
  byte BLUEP;
  
  animation animation;
  
  rgbColor  currentColor;
  byte      brightness;
} rgbStrip;

class RGB
{
public:
  RGB(byte r, byte g, byte b);
  setRGB(rgbColor& color);
  rgbColor getRGB(void);
  setRed(byte r);
  setGreen(byte g);
  setBlue(byte b);
  setBrightness(byte a);
  byte getRed(void);
  byte getBlue(void);
  byte getGreen(void);
  byte getBrightness(void);
  startAnimation(animationType at, int duration, rgbColor& color);
  processAnimation(void);
  animationType isAnimationRunning(void);



private:
  rgbStrip strip;
  calculateFadeFrames(void);
  byte calculateBrightness(byte from);
};

#endif

copyRGBColor(rgbColor& from, rgbColor& to)
{
  to.r = from.r;
  to.g = from.g;
  to.b = from.b;
}

//*****************************************
//     MAIN API FOR RGB CONTROL
//*****************************************
RGB::RGB(byte r, byte g, byte b)
{
  //Set pins
  strip.REDP = r;
  strip.GREENP = g;
  strip.BLUEP = b;

  //Setup the brightness to the higher value
  strip.brightness = 255;
  //Set pinMode
  pinMode(r, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(b, OUTPUT);
}
RGB::setRGB(rgbColor& color)
{
  copyRGBColor(color, strip.currentColor);
  //Writing colors
  analogWrite(strip.REDP, calculateBrightness(color.r));
  analogWrite(strip.GREENP, calculateBrightness(color.g));
  analogWrite(strip.BLUEP, calculateBrightness(color.b));
}
rgbColor RGB::getRGB(void)
{
  return strip.currentColor;
}


RGB::setRed(byte r)
{
  analogWrite(strip.REDP, calculateBrightness(r));
  strip.currentColor.r = r;
}

RGB::setGreen(byte g)
{
  analogWrite(strip.GREENP, calculateBrightness(g));
  strip.currentColor.g = g;
}
RGB::setBlue(byte b)
{
  analogWrite(strip.BLUEP, calculateBrightness(b));
  strip.currentColor.b = b;
}
RGB::setBrightness(byte a)
{
  strip.brightness = a;
  setRGB(strip.currentColor);
}
byte RGB::getRed(void)
{
  return strip.currentColor.r;
}
byte RGB::getBlue(void)
{
  return strip.currentColor.b;
}
byte RGB::getGreen(void)
{
  return strip.currentColor.g;
}
byte RGB::getBrightness(void)
{
  return strip.brightness;
}
/*
 * This function is a dirty hack for set brigthness
 * if you can control only R G B and no the positive. 
 * 
 * It is recommended to use a fourth control on the positive
 * for the brigthness. 
 */
byte RGB::calculateBrightness(byte from)
{
  //Dont do any operation if is at high state (255)
  if(strip.brightness == 255)
    return from;
  return ((float)from/(float)255)*(float)strip.brightness;
}
//****************************
//End of the main api
//****************************



//****************************
//Animation api
//****************************

/////////////////////////////////////////////////////
//  Calculate the rgb changes needed per milliseconds
/////////////////////////////////////////////////////
RGB::calculateFadeFrames(void)
{
   strip.animation.rgbPerMillis[1] = (float)((strip.animation.color.g - getGreen()) /(float)strip.animation.duration);
   strip.animation.rgbPerMillis[2] = (float)((strip.animation.color.b - getBlue()) /(float)strip.animation.duration);
   strip.animation.rgbPerMillis[0] = (float)((strip.animation.color.r - getRed()) /(float)strip.animation.duration);
}

///////////////////////////////////7
//  Initialize a animation
////////////////////////////////////
RGB::startAnimation(animationType at, int duration, rgbColor& color)
{
    strip.animation.at = at;
    strip.animation.duration = duration;
    strip.animation.startat = millis();
    copyRGBColor(color, strip.animation.color);
    copyRGBColor(strip.currentColor, strip.animation.startcolor);
    if(at == A_SMOOTHCHANGE) 
      calculateFadeFrames();
}

////////////////////////////////////
// Return current animation running
////////////////////////////////////
animationType RGB::isAnimationRunning(void)
{
  return strip.animation.at;
}

//////////////////////////////////////////
// Needed one time on the main loop for
// calculate the next change for the rgb
//////////////////////////////////////////
RGB::processAnimation(void)
{
  long time = millis() - strip.animation.startat;
  switch (strip.animation.at)
  {
    case A_SMOOTHCHANGE:
    {
      //Calculate color that the strip should have at this moment.
      if(time < strip.animation.duration)
      {
        setGreen((time * strip.animation.rgbPerMillis[1]) + strip.animation.startcolor.g);
        setBlue((time * strip.animation.rgbPerMillis[2]) + strip.animation.startcolor.b);
        setRed((time * strip.animation.rgbPerMillis[0]) + strip.animation.startcolor.r);
      }
      else
      {
        setRGB(strip.animation.color);
        strip.animation.at = A_NOTHING;
      }
    }
    break;
  }
}
//****************************
//End of the animation api
//****************************


