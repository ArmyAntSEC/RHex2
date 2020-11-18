/*
 * MotorPDRegulator.cpp
 *
 *  Created on: 18 aug. 2020
 *      Author: Daniel
 */

#include "MotorPIDRegulator.h"
#include <Arduino.h>
#include <LogStorage.h>

void MotorPIDRegulator::init ( MotorDriver* _driver, HomingEncoder * _encoder, 
	PID* _pid )	
{
	driver = _driver;
	encoder = _encoder;
	pid = _pid;
	setPointRev = 0;	
}

void MotorPIDRegulator::setWantedPositionRev( float _setPointRev, unsigned long int now ) {
	this->setPointRev = _setPointRev;
	this->lastChangeSetpointTime = now;
}

boolean MotorPIDRegulator::hasSettled( unsigned long int now)
{
	return now - this->lastChangeSetpointTime > 2000;
}

void MotorPIDRegulator::setMaxSpeed( unsigned int maxSpeed )
{
	this->pid->SetOutputLimits( -(int)maxSpeed, maxSpeed );
}

void MotorPIDRegulator::run(unsigned long int now) 
{
	long int currentPositionClicks = this->encoder->read();
	int posAtLastHome = this->encoder->getPosAtLastHome();

	//And compute the current position in radians
	float currentPositionRev =  (float)currentPositionClicks/3591.84; //Convert clicks to rotations.

	float PwmValueTrue = this->pid->Compute(currentPositionRev, this->setPointRev );
	
	//Give an extra kick if we are below the min threshold
	float PwmValueAbs = fabs(PwmValueTrue);
	float PwmValue = PwmValueTrue;
	if ( PwmValueAbs > 10 && PwmValueAbs < 30 ) {
		PwmValue = 30*PwmValue/PwmValueAbs;
	}

	this->driver->setMotorPWM((int)PwmValue);
	log(now) << "Step: " << currentPositionClicks << 
		", CurrPos: " << currentPositionRev << ", SetPt: " << this->setPointRev << 
		", PWM: " << PwmValue << 
		", Pos At last home: " << posAtLastHome << 
		", True PWM: " << PwmValueTrue << endl;	
}