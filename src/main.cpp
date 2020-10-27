#include <Arduino.h>
#include <math.h>
#include <TaskScheduler.h>
#include <SerialEchoBeacon.h>

#include "LegForwardLeft.h"
#include "LegForwardRight.h"

#include <LogStorage.h>
#define LOG Log << "Main: "


SerialEchoBeacon beacon_01(1000, 1);

TaskScheduler sched;

LegForwardLeft leftForward;
LegForwardRight rightForward;

/*
double Kp = 1000;
double Ki = 0;
double Kd = 0;
int sampleTime = 200 ;
PID pidLeft(Kp, Ki, Kd, sampleTime, P_ON_E, REVERSE );
PID pidRight(Kp, Ki, Kd, sampleTime, P_ON_E, REVERSE );

MotorStateHandler stateHandlerLeft ( sampleTime, 1 );
MotorStateHandler stateHandlerRight ( sampleTime, 2 );

MotorPIDRegulator regulatorLeft( &driverLeft, &encoderLeft, &pidLeft, 1 );
MotorPIDRegulator regulatorRight( &driverRight, &encoderRight, &pidRight, 2 );

MotorPositionInitiator initiatorLeft(  &stateHandlerLeft, &driverLeft, 
	&encoderLeft, &regulatorLeft, &leftConfig );
MotorPositionInitiator initiatorRight(  &stateHandlerRight, &driverRight, 
	&encoderRight, &regulatorLeft, &rightConfig );
*/


void setup() {
  	
	//Initilaize the communication.
	Serial.begin(9600);
	Log << "\n\n\n\n" << "Hello World again!" << endl;

	leftForward.init();	
	//leftForward.driver.setMotorPWM(64);		
	rightForward.init();	

	sched.add( &(leftForward.initiator) );

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
