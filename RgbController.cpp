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

void copyRGBColor(rgbColor& from, rgbColor& to)
{
  to.r = from.r;
  to.g = from.g;
  to.b = from.b;
}

RgbController::RgbController(byte r, byte g, byte b)
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

RgbController::setRGB(rgbColor& color)
{
  copyRGBColor(color, strip.currentColor);
  //Writing colors
  analogWrite(strip.REDP, calculateBrightness(color.r));
  analogWrite(strip.GREENP, calculateBrightness(color.g));
  analogWrite(strip.BLUEP, calculateBrightness(color.b));
}

rgbColor RgbController::getRGB(void)
{
  return strip.currentColor;
}

RgbController::setRed(byte r)
{
  analogWrite(strip.REDP, calculateBrightness(r));
  strip.currentColor.r = r;
}

RgbController::setGreen(byte g)
{
  analogWrite(strip.GREENP, calculateBrightness(g));
  strip.currentColor.g = g;
}
RgbController::setBlue(byte b)
{
  analogWrite(strip.BLUEP, calculateBrightness(b));
  strip.currentColor.b = b;
}
RgbController::setBrightness(byte a)
{
  strip.brightness = a;
  setRGB(strip.currentColor);
}
byte RgbController::getRed(void)
{
  return strip.currentColor.r;
}
byte RgbController::getBlue(void)
{
  return strip.currentColor.b;
}
byte RgbController::getGreen(void)
{
  return strip.currentColor.g;
}
byte RgbController::getBrightness(void)
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
byte RgbController::calculateBrightness(byte from)
{
  //Dont do any operation if is at high state (255)
  if(strip.brightness == 255)
    return from;
  return ((float)from/(float)255)*(float)strip.brightness;
}

// Calculate the rgb changes needed per milliseconds
RgbController::calculateFadeFrames(void)
{
   strip.animation.rgbPerMillis[1] = (float)((strip.animation.color.g - getGreen()) /(float)strip.animation.duration);
   strip.animation.rgbPerMillis[2] = (float)((strip.animation.color.b - getBlue()) /(float)strip.animation.duration);
   strip.animation.rgbPerMillis[0] = (float)((strip.animation.color.r - getRed()) /(float)strip.animation.duration);
}

//For start animation
RgbController::startAnimation(animationType at, int duration, rgbColor& color)
{
    strip.animation.at = at;
    strip.animation.duration = duration;
    strip.animation.startat = millis();
    copyRGBColor(color, strip.animation.color);
    copyRGBColor(strip.currentColor, strip.animation.startcolor);
    if(at == A_SMOOTHCHANGE)
      calculateFadeFrames();
}

//Return animation running
animationType RgbController::isAnimationRunning(void)
{
  return strip.animation.at;
}
//Stop animation running
RgbController::stopAnimation(void)
{
  strip.animation.at = A_NOTHING;
}

//Needed one time on the main loop for control the animation if needed
RgbController::processAnimation(void)
{
  unsigned long time = millis() - strip.animation.startat;
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
      //If time has terminated, set the color directly.
      else
      {
        setRGB(strip.animation.color);
        strip.animation.at = A_NOTHING;
      }
    }
    break;
  }
}
