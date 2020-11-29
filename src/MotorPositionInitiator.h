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

#include <Arduino.h>
#include <Loggable.h>

class MotorPositionInitiator: public MotorStateHandlerImpl,
	public Loggable // @suppress("Class has a virtual method and non-virtual destructor")
{ 
public:
	MotorPositionInitiator ():		
		state(NEW), ran(0), wantedPosition(0)		
	{		
	}
	virtual void setWantedPosition(float _wantedPosition)
	{
		wantedPosition = _wantedPosition;
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
				driver->setMotorPWM(-96);
			} else {
				driver->setMotorPWM(96);
			}

			log(now) << "Changing state to MOVING" << endl;
			state = MOVING;		
			break;		
		case MOVING:				
			if ( encoder->isHomed() ) {
				//driver->setMotorPWM(0);				
				regulator->setWantedPositionRev(wantedPosition, now);
				state = ALIGNING;
				log(now) << "Edge found. Changing state to ALIGNING." << endl;										
			}
			break;					
		case ALIGNING:	
			regulator->setMaxSpeed( 96 );				
			regulator->run(now);			
			if ( regulator->hasSettled(now) ) {			
				state = DONE;
				//regulator->setMaxSpeed( 255 );
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
	float wantedPosition;
};


#endif /* MOTORPOSITIONINITIATOR_H_ */
