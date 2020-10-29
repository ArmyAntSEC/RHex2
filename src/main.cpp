#include <Arduino.h>
#include <math.h>
#include <TaskScheduler.h>
#include <SerialEchoBeacon.h>

#include "LegForwardLeft.h"
#include "LegForwardRight.h"

#include <LogStorage.h>

#define SAMPLE_TIME 100

#undef LOG
#define LOG Log << "Main: "

SerialEchoBeacon beacon_01(500, 1);

TaskScheduler sched;

LegForwardLeft leftForward(SAMPLE_TIME);
LegForwardRight rightForward(SAMPLE_TIME);

void setup() {
  	
	//Initilaize the communication.
	Serial.begin(9600);
	Log << "\n\n\n\n" << "Hello World again!" << endl;

	leftForward.init();	
	rightForward.init();	

	sched.add( &(leftForward.handler) );

	beacon_01.init(millis());
	sched.add( &beacon_01 );

	/*
	stateHandlerLeft.setInitiator(&initiatorLeft);
	stateHandlerLeft.startInitiator();
	stateHandlerLeft.init(millis());
	
	stateHandlerRight.setInitiator(&initiatorRight);
	stateHandlerRight.startInitiator();
	stateHandlerRight.init(millis());

	//Add the objects to the scheduler
	
  	
	sched.add( &stateHandlerLeft );
	sched.add( &stateHandlerRight );
	
	driverLeft.init();
	driverRight.init();
	

	//Make sure the motors are not running
  	driverLeft.setMotorPWM(0);
	driverRight.setMotorPWM(0);

	regulatorLeft.setWantedPositionRev(-1.4, now);  
	regulatorRight.setWantedPositionRev(-1.4, now);  
	*/
}

unsigned long int loops = 0;
bool done = false;
void loop() {
	while ( done == false ) {
		if ( loops++ < 1e6 ) {
			sched.run();
		} else {
			done = true;
			unsigned long int totalTime = millis();			
			LOG << "Done with " << loops << " loops in " << totalTime << " ms for a rate of " << (int)floor(loops / (float)totalTime) << " loops/ms" << endl;			
			Log.sendToSerial();			
			leftForward.driver.setMotorPWM(0);
			rightForward.driver.setMotorPWM(0);			
		}
	}
}
