/*

PingTimer  --  uses Timer1 Input Capture to read a HC-SR04
     Copyright (C) 2019  David C.

     This program is free software: you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation, either version 3 of the License, or
     (at your option) any later version.

     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with this program.  If not, see <http://www.gnu.org/licenses/>.

     */

#include "PingTimer.h"

PingTimer ping;


PingTimer::PingTimer(){
	newData = false;
	overflowed = false;
	timerVal = 0;
}

void PingTimer::begin() {
	initTimer();
	// ATMEGA328P
//	pinMode(8, INPUT);
//	pinMode(9, OUTPUT);
	// ATMEGA1284P
	pinMode(14, INPUT);
	pinMode(13, OUTPUT);
}


void PingTimer::initTimer(){
	cli();
	TCCR1A = 0;  // no PWM
	// Noise Cancel On, RISING Edge , No prescaler, 4ms range.
	TCCR1B |= ((1 << ICNC1) | (1 << ICES1) | (1 << CS11) | (0 << CS10));
	// make sure analog comparator isn't selected as capture input
	ACSR &= ~(1 << ACIC);
	//  Turn off Timer interrupts
	TIMSK1 = 0;
	// setup COMPA for 10us to time the trigger pulse
	// and COMPB for 12ms for out of range signal (a little over 4 meters)
	OCR1AH = 0;
	OCR1AL = 20;  // With prescaler at 8 each tick is 0.5 us.
	OCR1BH = 0xB5;
	OCR1BL = 0xB0; // 0xB5B0 is 46512.  That's a little over 4 meters with prescaler at 8.
	sei();
}

//  This is a public facing function to call the startPulse so it can stay private
//  since it twiddles registers
void PingTimer::sendPing() {
	startPulse();
}

void PingTimer::startPulse(){
	// set pin high and setup PWM unit to
	// turn the pin off 10us later
	cli();
	TCNT1H = 0x00;
	TCNT1L = 0x00;
	//Set up to get one pulse of PWM
	// Clear OC1A on compare match.  10bit fast PWM
	TCCR1A = ((1 << COM1A1) | (1 << WGM11) | (1 << WGM10));
	// Setup input capture interrupt to catch the rising
	// of the echo pin and start the timer
	// Noise Cancel On, RISING Edge ,prescaler at 8 gives a 32ms timer = 5.5 meter range
	TCCR1B = ((1 << ICNC1) | (1 << ICES1) | (1 << CS11) | (0 << CS10) | (1 << WGM12));
	//  Turn on interrupt Input Capture
	TIFR1 = ((1 << ICF1) | (1 << TOV1));
	TIMSK1 = ((1 << ICIE1));
	overflowed = false;
	newData = false;
	sei();
}



// Called from ISR
void PingTimer::echoHandler() {
	if (ECHO_PIN_PORT & ECHO_PIN_MASK) {  //Rising Pin
		// Kill the OC1A PWM unit and return to normal mode
		TCCR1A = 0;
		// Noise Cancel On, FALLING Edge ,prescaler at 8 gives a 32ms timer = 5.5 meter range
		TCCR1B = (1 << ICNC1) | (0 << ICES1) | (1 << CS11) | (0 << CS10);
		// Reset Timer1
		TCNT1H = 0;
		TCNT1L = 0;
		//  Reset Input Capture and Timer Overflow Interrupts
		TIFR1 = (1 << ICF1) | (1 << TOV1);
		//  Turn on interrupts Input Capture and Timer Overflow
		TIMSK1 = (1 << ICIE1) | (1 << TOIE1);
	} else {  // Falling pin
		// Read the ICR1 Register
		uint8_t low = ICR1L;
		uint8_t high = ICR1H;
		timerVal = ((high << 8) | low) - 4; // Noise canceler adds 4 clock cycles

		//  turn off the timer interrupts
		TIMSK1 = 0;
		newData = true;
	}
}

// Called from ISR  Timer 1 OVF
void PingTimer::overflowHandler(){
	//turn off timer interrupts
	TIMSK1 = 0;
	newData = true;
	overflowed = true;
}



boolean PingTimer::hasNewData(){
	cli();
	boolean retval = newData;
	newData = false;
	sei();

	return retval;
}

boolean PingTimer::hasOverflowed(){
	cli();
	boolean retval = overflowed;
	overflowed = false;
	sei();

	return retval;
}

uint16_t PingTimer::getTimerVal() {
	uint16_t retval = 0;
	cli();
	if (overflowed) {
		retval = -1;
	} else {
		uint16_t copy = timerVal;
		retval = copy;
	}
	sei();
	return retval;
}

int16_t PingTimer::getDistanceMM() {

	int16_t retval = 0;
	uint16_t copy = getTimerVal();
	if (copy == (uint16_t) -1) {
		retval = -1;
	} else {
		// 16Mhz is 62.5 ns/clock
		// We've got the prescaler dividing by 8
		uint32_t nanoSecs = copy * 62.5 * 8;
		//speed of sound in mm/ns
		uint16_t mm = nanoSecs * 0.000344;
		retval = mm / 2;  // half a round trip
	}
	return retval;
}

ISR(TIMER1_CAPT_vect){
	ping.echoHandler();
}

ISR(TIMER1_OVF_vect){
	ping.overflowHandler();
}

ISR(TIMER1_COMPB_vect){
	ping.overflowHandler();
}
