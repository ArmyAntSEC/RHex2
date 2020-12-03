/*
 * MotorDriver.h
 *
 *  Created on: 24 okt. 2019
 *      Author: Daniel
 */

#ifndef MOTORDRIVER_H_
#define MOTORDRIVER_H_

#include <Arduino.h>
#include <LogStorage.h>
#include <pwm_lib.h>
using namespace arduino_due::pwm_lib;

class MotorDriver {
private:
	int driverPinOne;
	int driverPinTwo;	
	pwm_base * driverPinPWM;
	int currentPin;

public:	
	void init( int _driverPinOne, int _driverPinTwo, pwm_base * _driverPinPWM, 
		int _currentPin ) 
	{
		this->driverPinOne = _driverPinOne;
		this->driverPinTwo = _driverPinTwo;
		this->driverPinPWM = _driverPinPWM;

		this->currentPin = _currentPin;

		pinMode(this->driverPinOne, OUTPUT);
		pinMode(this->driverPinTwo, OUTPUT);	
		pinMode(this->currentPin, INPUT );	

		//Set the period to 5000e-8 seconds and the duty cycle to 0.
		this->driverPinPWM->start(5120,0);		
	}

	void setMotorPWM( int motorPWM )
	{		
		
		if ( motorPWM <= 0 ) {
			digitalWrite ( this->driverPinTwo, LOW);
			digitalWrite ( this->driverPinOne, HIGH);			
		} else {
			digitalWrite ( this->driverPinTwo, HIGH);
			digitalWrite ( this->driverPinOne, LOW);		
		}

		this->driverPinPWM->set_duty(abs(motorPWM)*20); //Convert from 0-255 to 0-5120;
	}

	float getCurrentInMilliVolt() {
		float voltageValue = analogRead( this->currentPin ) * 3.3 / 1024; //Read signal in Volts
		return voltageValue / 0.14; //Convert to A.
	}
};

#endif /* MOTORDRIVER_H_ */
