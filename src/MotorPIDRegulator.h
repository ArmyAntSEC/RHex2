/*
 * MotorPDRegulator.h
 *
 *  Created on: 18 aug. 2020
 *      Author: Daniel
 */

#ifndef MOTORPIDREGULATOR_H_
#define MOTORPIDREGULATOR_H_

#include "PID.h"
#include "MotorDriver.h"
#include <HomingEncoder.h>

class MotorPIDRegulator {
public:	
	virtual void run(unsigned long int now);
	virtual void init( MotorDriver* _driver, HomingEncoder * _encoder, PID* _pid );
	void setWantedPositionRev( float _setPointRev, unsigned long int now );	
	boolean hasSettled( unsigned long int now );
private:
	double angleDifference( double angle1, double angle2 );
	MotorDriver * driver;
	HomingEncoder * encoder;
	PID * pid;
	double setPointRev;
	double zeroPositionRev;
	int ID;
	unsigned long int lastChangeSetpointTime;
};

#endif /* MOTORPIDREGULATOR_H_ */
