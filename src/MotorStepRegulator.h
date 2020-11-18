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
			float currentPositionClicks = this->encoder->read();

			//Now set the motor power
			float angleDiffRev = this->angleDifferenceRev(this->setPointRev, currentPositionRev );

			int posAtLastHome = this->encoder->getPosAtLastHome();

			float angleDiffRevAbs = fabs(angleDiffRev);
			float angleDiffDir = (int)(angleDiffRev/angleDiffRevAbs);

			int PwmValue = 0;
			if ( angleDiffRevAbs > 0.25 ) {
				PwmValue = angleDiffDir * 96;
				settled = false;
			} else if (angleDiffRevAbs < 0.01 )  {
				PwmValue = 0;
				settled = true;
			} else {
				PwmValue = angleDiffDir * 40;
				settled = false;
			}		
			
			this->driver->setMotorPWM((int)PwmValue);


			log(now) << "Step: " << currentPositionClicks << 
				", CurrPos: " << currentPositionRev << ", SetPt: " << this->setPointRev << 
				", PWM: " << PwmValue << 
				", Pos At last home: " << posAtLastHome << endl;	
		}

		virtual boolean hasSettled( unsigned long int now ) { return settled; }	

	private:	 
		boolean settled = false;
};

#endif /* MOTORPIDREGULATOR_H_ */
