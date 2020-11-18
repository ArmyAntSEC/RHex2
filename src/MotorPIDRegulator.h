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
			PID::setup( 500, 0, 50, 100, P_ON_M, DIRECT );
		}
		
		virtual void run(unsigned long int now)
		{
			long int currentPositionClicks = this->encoder->read();
			int posAtLastHome = this->encoder->getPosAtLastHome();

			//And compute the current position in radians
			float currentPositionRev =  (float)currentPositionClicks/3591.84; //Convert clicks to rotations.

			float PwmValueTrue = PID::Compute(currentPositionRev, this->setPointRev );
			
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
};

#endif /* MOTORPIDREGULATOR_H_ */
