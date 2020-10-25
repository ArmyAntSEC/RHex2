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

class MotorPositionInitiator: public MotorStateHandlerImpl { // @suppress("Class has a virtual method and non-virtual destructor")
public:
	MotorPositionInitiator (  MotorStateHandler* const _handler, MotorDriver* const _driver, 
		HomingEncoder * const _encoder, MotorPIDRegulator * const _pid, const int _ID );
	virtual void run(unsigned long int now);
	virtual void init() {}
private:
	enum State { NEW, MOVING, ALIGNING, DONE };
	State state;
	HomingEncoder * const encoder;
	MotorPIDRegulator * const pid;
	const int ID;
};

#endif /* MOTORPOSITIONINITIATOR_H_ */
