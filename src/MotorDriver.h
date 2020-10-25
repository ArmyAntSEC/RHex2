/*
 * MotorDriver.h
 *
 *  Created on: 24 okt. 2019
 *      Author: Daniel
 */

#ifndef MOTORDRIVER_H_
#define MOTORDRIVER_H_

#include <Arduino.h>

class MotorDriver {
private:
	int driverPinOne;
	int driverPinTwo;
	int driverPinPWM;

public:	
	void init( int _driverPinOne, int _driverPinTwo, int _driverPinPWM ) {
		this->driverPinOne = _driverPinOne;
		this->driverPinTwo = _driverPinTwo;
		this->driverPinPWM = _driverPinPWM;

		pinMode(this->driverPinOne, OUTPUT);
		pinMode(this->driverPinTwo, OUTPUT);
		pinMode(this->driverPinPWM, OUTPUT);
		this->setMotorPWM(0);
	}

	void setMotorPWM( int motorPWM )
	{
		//Stop drivers if power is slow.
		if ( abs(motorPWM) < 20 ) {
			motorPWM = 0;
		}

		if ( motorPWM <= 0 ) {
			digitalWrite ( this->driverPinTwo, LOW);
			digitalWrite ( this->driverPinOne, HIGH);
			analogWrite ( this->driverPinPWM, abs(motorPWM) );
		} else {
			digitalWrite ( this->driverPinTwo, HIGH);
			digitalWrite ( this->driverPinOne, LOW);
			analogWrite ( this->driverPinPWM, abs(motorPWM) );
		}
}

};

#endif /* MOTORDRIVER_H_ */
