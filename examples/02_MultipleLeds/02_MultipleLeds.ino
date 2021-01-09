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


// **************************************************************************
// Please see example "01_PatternDemo" first! Here the usage of multiple LEDs
// is shown only, the methods and different patterns are explained at example
// "01_PatternDemo".
//
// The updateAll() method is not supported for AVR based boards like Arduino
// Uno! Use ESP8266 or ESP32 for this demo.
// **************************************************************************


// LedBlinker instances
LedBlinker greenLed  {12, LedBlinker::ActiveLevel::ACTIVE_HIGH};
LedBlinker yellowLed {13, LedBlinker::ActiveLevel::ACTIVE_HIGH};
LedBlinker redLed    {14, LedBlinker::ActiveLevel::ACTIVE_HIGH};

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
	LedBlinker::updateAll(); // only for non-AVR boards
}

