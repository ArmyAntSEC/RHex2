/*
 * MotorStateHandler.cpp
 *
 *  Created on: 16 aug. 2020
 *      Author: Daniel
 */

#include "MotorStateHandler.h"
#include <Arduino.h>
#include <Streaming.h>

MotorStateHandler::MotorStateHandler( unsigned long int rate, const int _ID ):
	RecurringTask(rate), initiator(0), mainLoop(0), currentState(0), ID(_ID)
{
}

void MotorStateHandler::startInitiator() {
	initiator->init();
	currentState = initiator;
}
void MotorStateHandler::startMainLoop() {
	mainLoop->init();
	currentState = mainLoop;
}

void MotorStateHandler::setInitiator(MotorStateHandlerImpl* initiator) {
	this->initiator = initiator;
}

void MotorStateHandler::setMainLoop(MotorStateHandlerImpl* mainLoop) {
	this->mainLoop = mainLoop;
}


void MotorStateHandler::run(unsigned long int now) {
	RecurringTask::run(now);
	if ( currentState != 0 ) {
		//Serial << "State handler " << this->ID << ": Running" << endl;
		currentState->run(now);
	}

}
