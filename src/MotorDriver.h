/*
 * MotorDriver.h
 *
 *  Created on: 24 okt. 2019
 *      Author: Daniel
 */

#ifndef MOTORDRIVER_H_
#define MOTORDRIVER_H_

#include <Arduino.h>
#include <pwm_lib.h>
using namespace arduino_due::pwm_lib;

class MotorDriver {
private:
	int driverPinOne;
	int driverPinTwo;	
	pwm_base * driverPinPWM;

public:	
	void init( int _driverPinOne, int _driverPinTwo, pwm_base * _driverPinPWM ) {
		this->driverPinOne = _driverPinOne;
		this->driverPinTwo = _driverPinTwo;
		this->driverPinPWM = _driverPinPWM;

		pinMode(this->driverPinOne, OUTPUT);
		pinMode(this->driverPinTwo, OUTPUT);		

		//Set the period to 5000e-8 seconds and the duty cycle to 0.
		this->driverPinPWM->start(5120,0);		
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
		} else {
			digitalWrite ( this->driverPinTwo, HIGH);
			digitalWrite ( this->driverPinOne, LOW);		
		}

		this->driverPinPWM->set_duty(motorPWM*20); //Convert from 0-255 to 0-5120;
	}
};

#endif /* MOTORDRIVER_H_ */
