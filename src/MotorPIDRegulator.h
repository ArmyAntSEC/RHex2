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
#include "Loggable.h"
#include <Arduino.h>

class MotorPIDRegulator: public MotorRegulator, public Loggable, protected PID {

	public:	
		virtual void init( MotorDriver* _driver, HomingEncoder * _encoder )
		{
			MotorRegulator::init(_driver,_encoder);
			PID::setup( 500, 0, 0, 100, P_ON_M, DIRECT );
		}
		
		virtual void run(unsigned long int now)
		{
			long int currentPositionClicks = this->encoder->readCompensated();
			int posAtLastHome = this->encoder->getPosAtLastHome();

			//And compute the current position in radians
			float currentPositionRev =  (float)currentPositionClicks/3591.84; //Convert clicks to rotations.
			float error = this->MotorRegulator::angleDifferenceRev( this->setPointRev, currentPositionRev );
			
			//float PwmValue = PID::Compute(currentPositionRev, this->setPointRev );
			//DIY PID regulator
			float deltaError = error - lastError;			
			float PwmValue = Kp*error + Kd*deltaError;
			lastError = error;

			if (PwmValue > 250) PwmValue = 250;
			if (PwmValue < -250) PwmValue = -250;

			this->driver->setMotorPWM((int)PwmValue);	

			float currentInMilliVolt = this->driver->getCurrentInMilliVolt();

			if ( fabs(PwmValue) > 150 ) {
				log(now) << "Step: " << currentPositionClicks << 
					", CurrPos: " << currentPositionRev << ", SetPt: " << this->setPointRev << 
					", Error: " << error << ", PWM: " << PwmValue <<
					", Current (mV): " << currentInMilliVolt <<
					", Pos At last home: " << posAtLastHome << endl;	
			}
		}

		virtual void setMaxSpeed( unsigned int maxSpeed )
		{
			PID::SetOutputLimits( -(int)maxSpeed, maxSpeed );
		}

		virtual boolean hasSettled( unsigned long int now )
		{ 
			return now - lastChangeSetpointTime > 2000;
		}	


	private:	
		unsigned long int lastChangeSetpointTime;
		const float Kp = 1500;
		const float Kd = 200;
		float lastError = 0;

};

#endif /* MOTORPIDREGULATOR_H_ */
