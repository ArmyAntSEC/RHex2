/*
 * MotorPDRegulator.h
 *
 *  Created on: 18 aug. 2020
 *      Author: Daniel
 */

#ifndef MOTORSTEPREGULATOR_H_
#define MOTORSTEPREGULATOR_H_

#include "MotorRegulator.h"
#include "Loggable.h"

class MotorStepRegulator: public MotorRegulator, public Loggable {
	public:	
		virtual void run(unsigned long int now)
		{
			float currentPositionRev = this->getCurrentPosRev();
			//float currentPositionClicks = this->encoder->read();

			//Now set the motor power
			float angleDiffRev = this->angleDifferenceRev(this->setPointRev, currentPositionRev );
			float error = this->MotorRegulator::angleDifferenceRev( this->setPointRev, currentPositionRev );
			
			int posAtLastHome = this->encoder->getPosAtLastHome();

			float angleDiffRevAbs = fabs(angleDiffRev);
			float angleDiffDir = (int)(angleDiffRev/angleDiffRevAbs);

			int PwmValue = 0;
			if ( angleDiffRevAbs > 0.1 ) {
				PwmValue = angleDiffDir * this->maxSpeed;
				settled = false;
			} else if (angleDiffRevAbs < 0.01 )  {
				PwmValue = 0;
				settled = true;
			} else {
				PwmValue = angleDiffDir * 40;
				settled = false;
			}		
			
			this->driver->setMotorPWM((int)PwmValue);

			float currentInMilliVolt = this->driver->getCurrentInMilliVolt();

			log(now) << 
				"CurrPos: " << currentPositionRev << ", SetPt: " << this->setPointRev << 
				", Error: " << error << ", PWM: " << PwmValue <<
				", C(V): " << currentInMilliVolt <<
				", HomePos: " << posAtLastHome << endl;
		}

		virtual boolean hasSettled( unsigned long int now ) { return settled; }	
		virtual void setMaxSpeed( unsigned int _maxSpeed )	
	 	{ 
			maxSpeed = _maxSpeed; 
		}
		
	private:	 
		boolean settled = false;
		unsigned int maxSpeed;
};

#endif /* MOTORPIDREGULATOR_H_ */
