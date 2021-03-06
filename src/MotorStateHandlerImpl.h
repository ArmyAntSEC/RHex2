/*
 * MotorStateHandlerImpl.h
 *
 *  Created on: 17 aug. 2020
 *      Author: Daniel
 */

#ifndef MOTORSTATEHANDLERIMPL_H_
#define MOTORSTATEHANDLERIMPL_H_

#include "MotorDriver.h"
#include "HomingEncoder.h"
#include "MotorRegulator.h"

class MotorStateHandler;

class MotorStateHandlerImpl { // @suppress("Class has a virtual method and non-virtual destructor")
public:	
	virtual void run(unsigned long int now) = 0;
	
	virtual void init( MotorStateHandler * _handler, MotorDriver * _driver, 
		HomingEncoder * _encoder, MotorRegulator * _regulator )
	{
		handler = _handler;
		driver = _driver;
		encoder = _encoder;
		regulator = _regulator;
	}

	virtual void restart( unsigned long int _now )
	{}

protected:
	MotorStateHandler* handler;
	MotorDriver* driver;
	HomingEncoder* encoder;
	MotorRegulator* regulator;
};

#include "MotorStateHandler.h"

#endif /* MOTORSTATEHANDLERIMPL_H_ */
