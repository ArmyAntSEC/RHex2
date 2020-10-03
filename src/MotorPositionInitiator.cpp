/*
 * MotorPositionInitiator.cpp
 *
 *  Created on: 15 aug. 2020
 *      Author: Daniel
 */

#include "MotorPositionInitiator.h"
#include <Arduino.h>
#include <LogStorage.h>
#define LOG Log << "Initiator" << this->conf->ID << ": "


MotorPositionInitiator::MotorPositionInitiator(  MotorStateHandler* const _handler, MotorDriver* const _driver, 
		EncoderWrapper * const _encoder, MotorPIDRegulator * const _pid, LegConfig const * const _conf ):
	MotorStateHandlerImpl(_handler, _driver ), state(NEW), encoder(_encoder), 
	pid(_pid), conf(_conf)
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
			LOG << "Edge found. Changing state to ALIGNING.  Moving to configured 0 position" << endl;			
			pid->setWantedPositionRev( this->conf->zeroPositionFromBreaker, now );
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
