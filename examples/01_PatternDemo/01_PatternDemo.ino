/*
LedBlinker - an Arduino library just for flashing a (status) led

Created by Istvan Klezli, 2021-Jan-03

MIT License

Copyright (c) 2021 Istvan Klezli

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include <LedBlinker.h>


// The LED object created here, the GPIO pin number and active state (active low or high) set
LedBlinker Led {LED_BUILTIN, LedBlinker::ActiveLevel::ACTIVE_HIGH};
// NOTE: Arduino Uno has a builtin LED (on pin 13) that needs HIGH level, in contrast to
// the LED on an ESP-01 board (connected to pin 2), which needs LOW output.


// Further LedBlinker instances can be created if needed, e.g.
// LedBlinker GreenLed {8, LedBlinker::ActiveLevel::ACTIVE_HIGH};
// LedBlinker RedLed {9, LedBlinker::ActiveLevel::ACTIVE_HIGH};


// just some ways to define/rename some patterns if you want
#define MY_ALERT_BLINKING                LedBlinker::Pattern::SPEED_MAX
constexpr uint32_t MY_STANDBY_BLINKING = static_cast<uint32_t>(LedBlinker::Pattern::ONE_SHORT_FLASH);
constexpr uint32_t MY_FANCY_BLINKING =   1484 * __LINE__ + 33;


void setup()
{
	Serial.begin(9600);

	// You could change the phase time here and/or set a new pattern
	// (default would be 100ms and OFF)
	//
	// Led.setPhaseTime(50);
	// Led.setPattern(ONE_SHORT_FLASH);

}

void loop()
{

	// If you want to alter the rhythm of the blinking then just change it by
	// calling the setPattern() method any location of your code. The update()
	// method needs to be invoked periodically.

	// This is just a simple example here for calling the update() method:
	// put it into the loop() function. If your loop execution time is predicable
	// and very short compared to the LED's phase (i.e. refresh) time, you can
	// apply this solution. Otherwise, if a more precise timing needed and/or you
	// loop can take long, let call the update() method by an interrupt/timer-based
	// routine.

    Led.update();

	// If you have more LedBlinker objects, you can just call updateAll() method to
	// update each of them. For example, the GreenLed, RedLed objects above could be
	// updated via this single call:
	//
	// LedBlinker::updateAll();
	// This is not available for AVR boards like Arduino Uno (but works on ESP8266/ESP32
	// boards) due to missing support of std::vector, which is used in library code.



	// DEMO. This part just iterates through the blinking patterns using setPattern()
	// (and also setPhaseTime()), ~10 seconds spent with each pattern
	static uint8_t lastPattern = UINT8_MAX;

	uint8_t pattern = ((millis() >> 10) % 110) / 10; // results in counting from 0 to 10
	if (lastPattern == pattern)
		return;

	lastPattern = pattern;

	switch (pattern)
	{

	case 0:
	  Led.setPhaseTime();
	  Serial.println("(case 0) very slow blinking (f=0.5Hz, 50% duty)");
	  Led.setPattern(LedBlinker::Pattern::SPEED_VERY_SLOW);
	  break;

	case 1:
	  Serial.println("(case 1) slow blinking (f=1Hz, 50% duty)");
	  Led.setPattern(LedBlinker::Pattern::SPEED_SLOW);
	  break;

	case 2:
	  Serial.println("(case 2) fast blinking (f=2.5Hz, 50% duty)");
	  Led.setPattern(LedBlinker::Pattern::SPEED_MEDIUM);
	  break;

	case 3:
	  Serial.println("(case 3) very fast blinking (f=5Hz, 50% duty)");
	  Led.setPattern(MY_ALERT_BLINKING); // see macro definition above
	  break;

	case 4:
	  Serial.println("(case 4) just a short flash in each 2 seconds");
	  Led.setPattern(MY_STANDBY_BLINKING); // see constexpr above
	  break;

	case 5:
	  Serial.println("(case 5) my fancy blinking pattern 1");
	  Led.setPattern(0b001110000010100, 15);
	  break;

	case 6:
	  Serial.println("(case 6) my fancy blinking pattern 2");
	  Led.setPattern(MY_FANCY_BLINKING);
	  break;

	case 7:
	  Serial.println("(case 7) permanent ON");
	  Led.setPattern(LedBlinker::Pattern::ON);	// or call setOn()
	  break;

	case 8:
	  Serial.println("(case 8) super fast...");
	  Led.setPhaseTime(50);
	  Led.setPattern(0b01, 2);
	  break;

	case 9:
	  Serial.println("(case 9) hyper mega super fast...");
	  Led.setPhaseTime(25);
	  Led.setPattern(0b01, 2);
	  break;

	case 10:
	  Serial.println("(case 10) Don't panic, everything is OK, the LED is just switched off...");
	  Led.setOff();
	  break;

	}


}

