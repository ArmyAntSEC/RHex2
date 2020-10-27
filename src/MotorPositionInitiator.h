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

class MotorPositionInitiator: public RecurringTask //: public MotorStateHandlerImpl { // @suppress("Class has a virtual method and non-virtual destructor")
{ 
public:
	MotorPositionInitiator ();
	virtual void run(unsigned long int now);
	virtual void init( MotorDriver * _driver, HomingEncoder * _encoder );	
private:
	enum State { NEW, MOVING, ALIGNING, DONE };
	State state;
	MotorDriver * driver;
	HomingEncoder *  encoder;			
};

#endif /* MOTORPOSITIONINITIATOR_H_ */
