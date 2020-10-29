/*
 * MotorPositionInitiator.h
 *
 *  Created on: 15 aug. 2020
 *      Author: Daniel
 */

#ifndef MOTORPOSITIONINITIATOR_H_
#define MOTORPOSITIONINITIATOR_H_

#include "MotorStateHandlerImpl.h"
#include "HomingEncoder.h"
#include "MotorPIDRegulator.h"

#include <Arduino.h>
#include <LogStorage.h>

#undef LOG
#define LOG Log << "Initiator: "

class MotorPositionInitiator: public MotorStateHandlerImpl // @suppress("Class has a virtual method and non-virtual destructor")
{ 
public:
	MotorPositionInitiator ():		
		state(NEW)		
	{		
	}

	virtual void init( MotorStateHandler * _handler, MotorDriver * _driver, 
	HomingEncoder * _encoder, MotorPIDRegulator * _pid )
	{
		MotorStateHandlerImpl::init( _handler, _driver );
		this->encoder = _encoder;
		this->pid = _pid;		
	}

	virtual void run(unsigned long int now)
	{				
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
				pid->setWantedPositionRev(0, now);
				state = ALIGNING;
				LOG << "Edge found. Changing state to DONE." << endl;										
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
			break;		
		default:
			LOG << "Assertion failed. State should never happen." << endl;
		}
	}
	

private:
	enum State { NEW, MOVING, ALIGNING, DONE };
	State state;
	HomingEncoder *  encoder;			
	MotorPIDRegulator * pid;		
};


#endif /* MOTORPOSITIONINITIATOR_H_ */
