/*
 * MotorPDRegulator.cpp
 *
 *  Created on: 18 aug. 2020
 *      Author: Daniel
 */

#include "MotorPIDRegulator.h"
#include <Arduino.h>
#include <LogStorage.h>
#define LOG Log << "Regulator " << this->ID << ": "

MotorPIDRegulator::MotorPIDRegulator ( MotorDriver* _driver, HomingEncoder * _encoder, PID* _pid, int _ID ):
	driver(_driver), encoder(_encoder), pid(_pid), setPointRev(0), ID(_ID)
{
}

void MotorPIDRegulator::init() {	
	//Nothing to do here
}

void MotorPIDRegulator::setWantedPositionRev( float _setPointRev, unsigned long int now ) {
	this->setPointRev = _setPointRev;
	this->lastChangeSetpointTime = now;
}

boolean MotorPIDRegulator::hasSettled( unsigned long int now)
{
	return now - this->lastChangeSetpointTime > 5000;
}

void MotorPIDRegulator::run(unsigned long int) {
	long int currentPositionClicks = this->encoder->read();

	//And compute the current position in radians
	double currentPositionRev =  (double)currentPositionClicks/3591.84; //Convert clicks to rotations.

	double PwmValue = this->pid->Compute(currentPositionRev, this->setPointRev );

	this->driver->setMotorPWM((int)PwmValue);
	LOG << "Step: " << currentPositionClicks << 
		", " << currentPositionRev << ", " << this->setPointRev << ", " <<
		", " << PwmValue << endl;	
}
