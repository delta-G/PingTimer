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

#ifndef PINGTIMER_H_
#define PINGTIMER_H_

#include "Arduino.h"



class PingTimer {
private:
	volatile uint16_t timerVal;
	volatile boolean overflowed;
	volatile boolean newData;

	void initTimer();

	void startPulse();


public:
	PingTimer();
	void begin();

	void sendPing();
	void echoHandler();
	void overflowHandler();

	boolean hasNewData();
	boolean hasOverflowed();
	uint16_t getTimerVal();

	int16_t getDistanceMM();

};

extern PingTimer ping;

#endif /* PINGTIMER_H_ */
