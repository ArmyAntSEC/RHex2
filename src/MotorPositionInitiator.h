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
#include "Encoder.h"
#include "LegConfig.h"
#include "MotorPIDRegulator.h"

class MotorPositionInitiator: public MotorStateHandlerImpl { // @suppress("Class has a virtual method and non-virtual destructor")
public:
	MotorPositionInitiator (  MotorStateHandler* const _handler, MotorDriver* const _driver, 
		EncoderWrapper * const _encoder, MotorPIDRegulator * const _pid, LegConfig const * const _conf );
	virtual void run(unsigned long int now);
	virtual void init() {}
private:
	enum State { NEW, MOVING, ALIGNING, DONE };
	State state;
	Encoder * const encoder;
	MotorPIDRegulator * const pid;
	LegConfig const * const conf;
};

#endif /* MOTORPOSITIONINITIATOR_H_ */
