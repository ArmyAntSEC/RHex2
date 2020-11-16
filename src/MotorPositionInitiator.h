/*
 * MotorPositionInitiator.h
 *
 *  Created on: 15 aug. 2020
 *      Author: Daniel
 */

#ifndef MOTORPOSITIONINITIATOR_H_
#define MOTORPOSITIONINITIATOR_H_

#include "MotorStateHandlerImpl.h"
#include "HomingEncoder.h"
#include "MotorPIDRegulator.h"

#include <Arduino.h>
#include <Loggable.h>

class MotorPositionInitiator: public MotorStateHandlerImpl,
	public Loggable // @suppress("Class has a virtual method and non-virtual destructor")
{ 
public:
	MotorPositionInitiator ():		
		state(NEW), ran(0)		
	{		
	}

	virtual void restart( unsigned long int _now )
	{
		this->state = NEW;
		this->encoder->unHome();
	}

	virtual void run(unsigned long int now)
	{				
		switch ( state ) {
		case NEW:						
			ran++;

			if ( ran % 2 ) { //Randomize which direction we turn.
				driver->setMotorPWM(-40);
			} else {
				driver->setMotorPWM(40);
			}

			log(now) << "Changing state to MOVING" << endl;
			state = MOVING;		
			break;		
		case MOVING:				
			if ( encoder->isHomed() ) {
				//driver->setMotorPWM(0);				
				pid->setWantedPositionRev(0, now);
				state = ALIGNING;
				log(now) << "Edge found. Changing state to ALIGNING." << endl;										
			}
			break;					
		case ALIGNING:	
			pid->setMaxSpeed( 64 );				
			pid->run(now);			
			if ( pid->hasSettled(now) ) {			
				state = DONE;
				pid->setMaxSpeed( 255 );
				driver->setMotorPWM(0);
				log(now) << "Regulator settled. Done." << endl;				
			}						
			break;		
		case DONE:
			//DO nothing
			break;		
		default:
			log(now) << "Assertion failed. State should never happen." << endl;
		}
	}
	

private:
	enum State { NEW, MOVING, ALIGNING, DONE };
	State state;	
	unsigned int ran;	
};


#endif /* MOTORPOSITIONINITIATOR_H_ */
