/*
 * MotorStateHandler.h
 *
 *  Created on: 16 aug. 2020
 *      Author: Daniel
 */

#ifndef MOTORSTATEHANDLER_H_
#define MOTORSTATEHANDLER_H_

#include <Arduino.h>

#include "RecurringTask.h"

#undef LOG
#define LOG Log << "MotorStateHandler: "


class MotorStateHandler: public RecurringTask { // @suppress("Class has a virtual method and non-virtual destructor")

public:
	MotorStateHandler ( unsigned long int _rate ):
		RecurringTask(_rate), initiator(0), mainLoop(0), currentState(0)
	{}

	virtual void run(unsigned long int now)
	{		
		RecurringTask::run(now);	
		if ( currentState != 0 ) {			
			currentState->run(now);
		} else {	
			LOG << "No implementation to run" << endl;	
		}		
	}

	void startInitiator()
	{		
		currentState = initiator;
	}

	void startMainLoop() 
	{	
		currentState = mainLoop;
	}

	void init(MotorStateHandlerImpl* _initiator, MotorStateHandlerImpl* _mainLoop) {
		LOG << "State Handler Init" << endl;
		
		initiator = _initiator;
		mainLoop = _mainLoop;		
		
		this->start();		
	}
private:		
	MotorStateHandlerImpl* initiator;
	MotorStateHandlerImpl* mainLoop;
	MotorStateHandlerImpl* currentState;	
};

#endif /* MOTORSTATEHANDLER_H_ */
