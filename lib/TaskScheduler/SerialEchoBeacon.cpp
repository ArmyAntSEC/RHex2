/*
 * SerialEchoBeacon.cpp
 *
 *  Created on: 8 juli 2020
 *      Author: Daniel
 */

#include "SerialEchoBeacon.h"
#include <Arduino.h>
#include <Streaming.h>

SerialEchoBeacon::SerialEchoBeacon(unsigned long int _rate, const int _ID)
: RecurringTask(_rate), ID(_ID)
{}

void SerialEchoBeacon::run(unsigned long int now) {
	RecurringTask::run(now);
	Serial << "Beacon " << this->ID << ": ping...." << endl;
}
