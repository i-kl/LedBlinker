/*
LedBlinker - an Arduino library just for flashing a (status) LED

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

#pragma once

#include "Arduino.h"
#ifndef __AVR__
#include <vector>
#endif


/// One instance of this class can handle one LED attached to a given GPIO pin.
/// The pin number and the active high or low state must be defined in the constructor.
/// The blinking pattern and the phase time can be defined after the creation of the
/// LedBlinker instance. The update() method must be called periodically.
/// In case of more LedBlinker instances (e.g. LEDs on different GPIO pins)
/// the class methods updateAll(), setAllOn(), setAllOff() can be used in order to
/// refresh/command each LED instances (not for AVR based boards)
class LedBlinker
{

public:

static const uint8_t    DEFAULT_PATTERN_LENGTH = 20;  /// the pattern has 20 used bits by default
static const uint32_t   DEFAULT_PHASE_TIME_MS  = 100; /// phase time 100ms ==> a pattern takes 20 x 100ms
                                                      /// = 2 seconds by default. See also setPhaseTime() 
                                                      /// and setPattern() below.

/// Some general predefined patterns (each 20-bit)
enum class Pattern : uint32_t
{
	ON                  = UINT32_MAX,
	OFF                 = 0,
	SPEED_MAX           = 0b01010101010101010101,
	SPEED_MEDIUM        = 0b00110011001100110011,
	SPEED_SLOW          = 0b00000111110000011111,
	SPEED_VERY_SLOW     = 0b00000000001111111111,
	ONE_SHORT_FLASH     = 0b00000000000000000001,
	ONE_LONG_FLASH      = 0b00000000000000000011,
	TWO_SHORT_FLASHES   = 0b00000000010000000001,
};



/// Defines what output voltage level is needed at the GPIO pin which the LED is attached to.
enum class ActiveLevel : uint8_t
{
	ACTIVE_HIGH,  /// active high LED (cathode grounded, anode switched, like Arduino's built-in LED usually)
	ACTIVE_LOW,   /// active low LED (anode on Vcc, cathode switched, like ESP moduls' built-in LED usually)
};

	/// Constructor needs the GPIO pin number the LED attached to and also the LED's active state
	LedBlinker(uint8_t iLedPin, ActiveLevel activeState);

	/// Destructor, removes the instance from the static class member vector holding all LED instances (not for AVR)
	~LedBlinker();

	/// Sets the phase time in milliseconds. Phase time x pattern length = period, which is 2 seconds by default.
	/// The timing will be done internally by checking the current uptime (returned by millis()) and the last
	/// update time. If the phase time is set to zero, no internal timing is performed, but the phase next phase
	/// (i.e. the next bit in the pattern) will be taken always. This mode is useful if the update() is already
	/// called in a timed way, e.g. by an interrupt routine.
	void setPhaseTime(uint32_t iPhaseTime_ms = DEFAULT_PHASE_TIME_MS);

	/// Sets the blinking pattern, which is etiher a predefined one (see above enum Pattern)
	/// or an arbitrary, user defined one (consider for example a symbol from the Morse code table).
	/// The second argument indicates the number of useful bits in the pattern (20 by default).
	void setPattern (uint32_t pattern, uint8_t iPatternLength = DEFAULT_PATTERN_LENGTH);
	void setPattern (Pattern pattern, uint8_t iPatternLength = DEFAULT_PATTERN_LENGTH);
	
	/// Sets the ON pattern
	void setOn();
	
	/// Sets the OFF pattern
	void setOff();

	/// This method sets the LED state based on the current blinking pattern
	/// (e.g. fast blinking) and must be called periodically (100ms proposed,
	/// see above the constant definitions).
	void update();

#ifndef __AVR__
	/// Calls setAllOff() for each LedBlinker instance (not for AVR)
	static void setAllOff();

	/// Calls setOn() for each LedBlinker instance (not for AVR)
	static void setAllOn();
	
	/// Calls update() for each LedBlinker instance (not for AVR)
	static void updateAll();
#endif

protected:

#ifndef __AVR__
	// variables for handling ALL LEDs
	static std::vector<LedBlinker*>	m_vecLeds; /// Keeps track of all LedBlinker instances
#endif

	// part of the configuration of a LED, can be defined in the constructor only
	uint8_t			m_iLedPin;          /// configuration: the GPIO pin number
	ActiveLevel		m_activeState;      /// configuration: shows whether the LED needs HIGH or LOW for activated state

	// these variables define the blinking behaviour
	uint32_t		m_iPhaseTime_ms;    /// phase time in milliseconds (Arduino function millis used, if set to zero, no waiting)
	uint32_t		m_pattern;          /// current blinking pattern
	uint8_t			m_iPatternLength;   /// number of used bits in the blinking pattern

	// these are internal variables
	bool			m_ledState;         /// LED's current state (on or off)
	uint8_t			m_iPhase;           /// current blinking phase
	uint32_t		m_iLastTime;        /// timestamp of the last blinking (Arduino function millis used)
	
};
