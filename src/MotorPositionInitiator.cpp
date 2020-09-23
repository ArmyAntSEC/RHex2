/*
 * MotorPositionInitiator.cpp
 *
 *  Created on: 15 aug. 2020
 *      Author: Daniel
 */

#include "MotorPositionInitiator.h"
#include <Arduino.h>
#include <LogStorage.h>
#define LOG Log << "Initiator" << this->ID << ": "


MotorPositionInitiator::MotorPositionInitiator(MotorStateHandler* _handler, MotorDriver* _driver, OptoBreaker* _breaker, const int _ID ):
	MotorStateHandlerImpl(_handler, _driver ), state(NEW), breaker(_breaker), ID(_ID)
{}

void MotorPositionInitiator::run(unsigned long int) {
	switch ( state ) {
	case NEW:
		LOG << "State is NEW." << endl;
		driver->setMotorPWM(-64);
		if ( !breaker->read() ) {
			//In case we are at the opening allready, we need to move off of it to then get a sharp edge.
			state = MOVING;
			LOG << "Changing state to MOVING" << endl;
		}

		break;

	case MOVING:
		//Serial.println ( "Initiator: State is MOVING." );
		if ( breaker->read() ) {
			driver->setMotorPWM(0);
			state = DONE;
			LOG << "Edge found. Done." << endl;
			handler->startMainLoop();
		}
		break;
	default:
		LOG << "Assertion failed. State should never happen." << endl;
	}
}
