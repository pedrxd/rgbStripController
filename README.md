# RgbStrip Controller for Arduino
**This program can be used for control some ilumination elements** like **rgb** and **leds** on your home by your own adding lot of features and possibilities due the usage of **Arduino**. It was created firstly for just rgb but I am adding more things.

## Features

 - **Control rgb lights API**

  Smooth transition, rainbow, strike color, strike random color, breathing color

 - **Control led light API**

  turn on/off, strike

 - **Powerfull command parser**


## Command syntax
The basic structure is:
> &lt;var&gt;&lt;value&gt;

Some var don't need a value, and can be concated in the same command.
<var><value><var><var><value>
For a fast response from arduino a ',' is required where the command terminated.

### Available Var

Var | Value | Usage
--- | --- | ---
'c'| mode,red,green,blue  | Change color, smoothly if mode is 1 and directly if 0
'r'| red [0-255] | Change red intensity.
'g'| green [0-255]| Change green intensity.
'b'| blue [0-255]| Change blue intensity.
'd'| time [Microseconds]| Change duration of animations.
'a'|  britghness [0-255] | Change brightness of strip
'm'| mode [0-4] | Change mode. 0:StaticColor;1:Rainbow;2:Strike;3:RandomStrike;4:Breathing
's'| _none_ | Poweroff rgb smoothly
'l'| mode [0-3] | Control secundary led. 0:turnoff;1:turnon;2:strike;3:switch(ON/OFF)

### Examples:
 - Set red high, blue high, green high with 100 of brightness.

	r255b255g255a100,
 - Set red high, blue 100, and off it smoothly on 2 seconds.

	r255b100d2000s,
 - Set rainbow mode

	m1,
