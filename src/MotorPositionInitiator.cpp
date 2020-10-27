/*
 * MotorPositionInitiator.cpp
 *
 *  Created on: 15 aug. 2020
 *      Author: Daniel
 */

#include "MotorPositionInitiator.h"
#include <Arduino.h>
#include <LogStorage.h>
#define LOG Log << "Initiator: "


MotorPositionInitiator::MotorPositionInitiator( ):		
		RecurringTask(100),
		state(NEW)
{}

void MotorPositionInitiator::init( 
	MotorDriver* _driver, HomingEncoder * _encoder )
{
	this->driver = _driver;
	this->encoder = _encoder;
	this->start();
}

void MotorPositionInitiator::run ( unsigned long int now ) {
	RecurringTask::run(now);	

	switch ( state ) {
	case NEW:
		LOG << "State is NEW." << endl;
		driver->setMotorPWM(64);
		LOG << "Changing state to MOVING" << endl;
		state = MOVING;		
		break;

	case MOVING:		
		if ( encoder->isHomed() ) {
			driver->setMotorPWM(0);
			state = DONE;
			LOG << "Edge found. Changing state to DONE." << endl;						
		}
		break;
	case ALIGNING:		
		/*
		pid->run(now);
		if ( pid->hasSettled(now) ) {			
			state = DONE;
			LOG << "Regulator settled. Done." << endl;
			handler->startMainLoop();
		}
		break;
		*/
	case DONE:
		//DO nothing
		break;
	default:
		LOG << "Assertion failed. State should never happen." << endl;
	}
}
