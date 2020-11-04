/*
 * SerialEchoBeacon.cpp
 *
 *  Created on: 8 juli 2020
 *      Author: Daniel
 */

#include "SerialEchoBeacon.h"
#include <Arduino.h>
#include <LogStorage.h>

SerialEchoBeacon::SerialEchoBeacon(unsigned long int _rate )
: RecurringTask(_rate)
{}

void SerialEchoBeacon::run(unsigned long int now) {
	RecurringTask::run(now);
	log(now) << "Rate: " << getRate() << " Ping...." << endl;
}
