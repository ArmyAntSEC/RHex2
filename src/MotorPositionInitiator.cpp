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


MotorPositionInitiator::MotorPositionInitiator(  MotorStateHandler* const _handler, MotorDriver* const _driver, 
		HomingEncoder * const _encoder, MotorPIDRegulator * const _pid, const int _ID ):
	MotorStateHandlerImpl(_handler, _driver ), state(NEW), encoder(_encoder), 
	pid(_pid), ID(_ID)
{}

void MotorPositionInitiator::run ( unsigned long int now ) {
	switch ( state ) {
	case NEW:
		LOG << "State is NEW." << endl;
		driver->setMotorPWM(-64);
		LOG << "Changing state to MOVING" << endl;
		state = MOVING;		
		break;

	case MOVING:		
		if ( encoder->isHomed() ) {
			driver->setMotorPWM(0);
			state = ALIGNING;
			LOG << "Edge found. Changing state to ALIGNING." << endl;						
		}
		break;
	case ALIGNING:		
		pid->run(now);
		if ( pid->hasSettled(now) ) {			
			state = DONE;
			LOG << "Regulator settled. Done." << endl;
			handler->startMainLoop();
		}
		break;
	case DONE:
		//DO nothing
	default:
		LOG << "Assertion failed. State should never happen." << endl;
	}
}
