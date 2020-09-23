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

MotorPIDRegulator::MotorPIDRegulator ( MotorStateHandler* _handler, MotorDriver* _driver, Encoder* _encoder, PID* _pid, int _ID ):
	MotorStateHandlerImpl(_handler, _driver), encoder(_encoder), pid(_pid), setPointRev(0), ID(_ID)
{
}

void MotorPIDRegulator::init() {	
	LOG << "Reseting the encoder." << endl;
	//Make sure encoder things we are at position 0.
	long int oldPos = this->encoder->read();
	this->encoder->write(0);
	long int newPos = this->encoder->read();
	
	LOG << "Old position: " << oldPos << " New pos: " << newPos << endl;	
}

void MotorPIDRegulator::setWantedPositionRev( float _setPointRev) {
	this->setPointRev = _setPointRev;
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
