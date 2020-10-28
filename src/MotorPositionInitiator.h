/*
 * MotorPositionInitiator.h
 *
 *  Created on: 15 aug. 2020
 *      Author: Daniel
 */

#ifndef MOTORPOSITIONINITIATOR_H_
#define MOTORPOSITIONINITIATOR_H_

#include "MotorStateHandlerImpl.h"
#include "MotorDriver.h"
#include "HomingEncoder.h"
#include "MotorPIDRegulator.h"

#include <Arduino.h>
#include <LogStorage.h>

class MotorPositionInitiator: public RecurringTask //: public MotorStateHandlerImpl { // @suppress("Class has a virtual method and non-virtual destructor")
{ 
public:
	MotorPositionInitiator ( ):
		RecurringTask(100),
		state(NEW)		
	{		
	}

	virtual void init( MotorDriver * _driver, HomingEncoder * _encoder, 
		MotorPIDRegulator * _pid )
	{
		this->driver = _driver;
		this->encoder = _encoder;
		this->pid = _pid;
		this->start();
	}

	virtual void run(unsigned long int now)
	{
		RecurringTask::run(now);	

		switch ( state ) {
		case NEW:
			Log << "State is NEW." << endl;
			driver->setMotorPWM(64);
			Log << "Changing state to MOVING" << endl;
			state = MOVING;		
			break;

		case MOVING:		
			if ( encoder->isHomed() ) {
				driver->setMotorPWM(0);
				state = ALIGNING;
				Log << "Edge found. Changing state to DONE." << endl;						
			}
			break;
		case ALIGNING:					
			pid->run(now);			
			if ( pid->hasSettled(now) ) {			
				state = DONE;
				Log << "Regulator settled. Done." << endl;
				//handler->startMainLoop();
			}						
			break;		
		case DONE:
			//DO nothing
			break;
		default:
			Log << "Assertion failed. State should never happen." << endl;
		}
	}
	

private:
	enum State { NEW, MOVING, ALIGNING, DONE };
	State state;
	MotorDriver * driver;
	HomingEncoder *  encoder;			
	MotorPIDRegulator * pid;	
	
};


#endif /* MOTORPOSITIONINITIATOR_H_ */
