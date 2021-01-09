# LedBlinker
**An Arduino library for flashing one or more LEDs. The goal of this library is to provide a simple mechanism to control status LEDs of your project: just configure the LED object (GPIO pin, active level) and change the blinking pattern (as bitmask) and its speed any time you want a new "Tempo and Rhythm"**

For instance, you have a green (operation mode) and a red (error state) LED and want to show different blinking in case of different operational and error situations, e.g. a periodical flashing of the green LED for stand-by and normal blinking for normal operation. And the flashing speed of the red LED could indicate for example the severity of the error... or show whether the error is acknowledged... or whatever...

Tested on Arduino Uno board and also on ESP-based Wemos D1 and ESP-01 boards.

## TL;DR
```C++
#include <LedBlinker.h>

LedBlinker greenLed   {12, LedBlinker::ActiveLevel::ACTIVE_HIGH};
LedBlinker yellowLed  {13, LedBlinker::ActiveLevel::ACTIVE_HIGH};
LedBlinker redLed     {14, LedBlinker::ActiveLevel::ACTIVE_HIGH};

void setup()
{
	greenLed.setPattern(0b001,3);
	greenLed.setPhaseTime(500);

	yellowLed.setPattern(0b010,3);
	yellowLed.setPhaseTime(500);

	redLed.setPattern(0b100,3);
	redLed.setPhaseTime(500);
}

void loop()
{
	LedBlinker::updateAll();
}
```

![Demo sketch running on a Wemos D1 (ESP8266) board](https://github.com/i-kl/LedBlinker/blob/main/examples/02_MultipleLeds/02_MultipleLeds.gif)

## Description
See also the header file and the ![1st example sketch](https://github.com/i-kl/LedBlinker/blob/main/examples/01_PatternDemo/01_PatternDemo.ino)


### Class and contructor
```C++
class LedBlinker
```
One instance of this class can handle one LED attached to a given GPIO pin. The pin number and the active high or low state must be defined in the constructor.
You can write this for using the built-in LED (assuming active high level, i.e. the GPIO pin must be switched to HIGH to feed the LED)
```C++
LedBlinker led {LED_BUILTIN, LedBlinker::ActiveLevel::ACTIVE_HIGH};
```
### Blinking pattern - bit mask
Also you need to setup the blinking pattern, which is a bit mask where bit `1` represents the ON state. There are some predefined ones like `SPEED_MEDIUM` (which is a blinking with freq 2.5Hz and 50% duty cycle, i.e. symmetrical square wavee) or `ONE_SHORT_FLASH` (which is just a 100ms flash in each 2 seconds).
```C++
led.setPattern(LedBlinker::Pattern::SPEED_MEDIUM);
```
These predefined patterns (i.e. ON/OFF bit masks) contain **20** used bits. If you want to use an own pattern then pass the corresponding bitmask (preferably as binary or hexadecimal number) and define also the number of used bits in that pattern.

```C++
led.setPattern(0b001110000010100, 15); // super fancy blinking, 15-bit long pattern
```
Or just use a macro (or any content or non-constant variable)
```C++
#define MY_ALERT_BLINKING    0x55555555
...
led.setPattern(MY_ALERT_BLINKING, 32);
```

### setOn, setOff
They are just shortcuts for predefined `111...` and `000...` bit masks.
```C++
if (<my condition>)
  led.setOn();
else
  led.setOff();
```

### Phase time
The phase time indicates how long a bit in the pattern has an effect. The default phase time is **100ms**. As mentioned above, the length of the predefined patterns is 20. This means, that the blinking period of the predefined patterns is *phase-time* * *number-of-bits* = **2 seconds**. Of course, you can use other phase time (and pattern-lenght up to 32). To change the phase time call `setPhaseTime(t)` where t is in milliseconds.
```C++
led.setPhaseTime(25);
```
Note: the phase time must be set to zero if the update() is timed by an interrupt routine, see below.

### Update 
The update() method needs to be invoked periodically! There are more ways to do this. A simple solution would be calling the `update()` method in the Arduino's `loop()` function. If your `loop()` execution time is predicable and short compared to the LED's phase (i.e. refresh) time, you can do this. Otherwise, if a more precise timing needed and/or you `loop()` can take long then call the `update()` method from an interrupt/timer-based routine. In this case, the **phase time must be set to zero** since the timing is done by the interrupt configured by you.

### Multiple LEDs - updateAll()
For each individual LED (attached to different GPIO pins) you must create a `LedBlinker` instance. If you do so the class methods `updateAll()`, `setAllOn()`, `setAllOff()` can be used in order to refresh/command each LED in one single function call. This is unfortunately not supported on AVR based boards (due to lack of `std::vector` used in the library code).

See also the ![2nd example sketch](https://github.com/i-kl/LedBlinker/blob/main/examples/02_MultipleLeds/02_MultipleLeds.ino)
