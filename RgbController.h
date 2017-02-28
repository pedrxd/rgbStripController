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


#ifndef RGBCONTROLLER_h
#define RGBCONTROLLER_h

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
  long duration;

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

class RgbController
{
public:
  RgbController(byte r, byte g, byte b);
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
  startAnimation(animationType at, long duration, rgbColor& color);
  stopAnimation(void);
  processAnimation(void);
  animationType isAnimationRunning(void);



private:
  rgbStrip strip;
  calculateFadeFrames(void);
  byte calculateBrightness(byte from);
};

void copyRGBColor(rgbColor& from, rgbColor& to);
#endif
