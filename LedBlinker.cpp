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

#include "LedBlinker.h"

#ifndef __AVR__
std::vector<LedBlinker*> LedBlinker::m_vecLeds;
#endif

LedBlinker::LedBlinker(uint8_t iLedPin, ActiveLevel activeState)
	:	m_iLedPin{iLedPin},
		m_activeState{activeState},
		m_iPhaseTime_ms{DEFAULT_PHASE_TIME_MS},
		m_pattern{0},
		m_iPatternLength{DEFAULT_PATTERN_LENGTH},
		m_ledState{false},
		m_iPhase{0},
		m_iLastTime{0}
{
	// set to OFF
	digitalWrite(m_iLedPin, m_activeState == ActiveLevel::ACTIVE_LOW ? HIGH : LOW);
	pinMode(m_iLedPin, OUTPUT);

#ifndef __AVR__
	m_vecLeds.push_back(this);
#endif		
}

LedBlinker::~LedBlinker()
{
#ifndef __AVR__
	m_vecLeds.erase(std::remove(m_vecLeds.begin(), m_vecLeds.end(), this));
#endif	
}

/// Sets the phase time in milliseconds
void LedBlinker::setPhaseTime(uint32_t iPhaseTime_ms /*= DEFAULT_PHASE_TIME_MS*/)
{
	m_iPhaseTime_ms = iPhaseTime_ms;
	m_iLastTime = millis();
}

/// Sets the pattern (bit mask) and the number of useful bits in the pattern
void LedBlinker::setPattern (Pattern pattern, uint8_t iPatternLength /*= DEFAULT_PATTERN_LENGTH*/)
{
	setPattern(static_cast<uint32_t>(pattern), iPatternLength);
}

void LedBlinker::setPattern (uint32_t pattern, uint8_t iPatternLength /*= DEFAULT_PATTERN_LENGTH*/)
{
	if (iPatternLength > 32)
		return;

	if (m_pattern != pattern || m_iPatternLength != iPatternLength)
	{
		m_pattern = pattern;
		m_iPatternLength = iPatternLength;
		m_iPhase = 0;
	}
}

/// Turns ON
void LedBlinker::setOn()
{
	setPattern(Pattern::ON);
}

/// Turns OFF
void LedBlinker::setOff()
{
	setPattern(Pattern::OFF);
}

// must be called periodically
// - e.g. in the Arduino's loop() function - in this case the phase time set is used
// - or e.g. by an interrupt routine - in this case the phase time should be set to zero
void LedBlinker::update()
{
	if (m_iPhaseTime_ms != 0)
	{
		uint32_t now = millis();
		if (now - m_iLastTime < m_iPhaseTime_ms)
			return;	// not yet

		m_iLastTime = now;
	}

	uint32_t iMask = 1;
	iMask <<= m_iPhase;
	bool bNewLedState = (m_pattern & iMask) > 0;

	// is there a change?
	if (m_ledState != bNewLedState)
	{
		// store the new state
		m_ledState = bNewLedState;

		// feed the output led, change polarity if needed
		if (m_activeState == ActiveLevel::ACTIVE_LOW)
			bNewLedState = !bNewLedState;
		digitalWrite(m_iLedPin, bNewLedState ? HIGH : LOW);
	}

	// increase the phase...
	if (++m_iPhase >= m_iPatternLength)
		m_iPhase = 0;
}

#ifndef __AVR__
/// Turns all ON
void LedBlinker::setAllOn()
{
	for (LedBlinker* led : m_vecLeds)
		led->setOn();
}

/// Turns all OFF
void LedBlinker::setAllOff()
{
	for (LedBlinker* led : m_vecLeds)
		led->setOff();
}

void LedBlinker::updateAll()
{
	for (LedBlinker* led : m_vecLeds)
		led->update();
}
#endif
