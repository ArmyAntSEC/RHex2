#include <Arduino.h>
#include <math.h>
#include <TaskScheduler.h>
#include <SerialEchoBeacon.h>
#include <LogStorage.h>

#include "LegForwardLeft.h"
#include "LegForwardRight.h"
#include "LegPacingMasterClock.h"

#define SAMPLE_TIME 100
#define LEG_ROTATIONS_PER_MINUTE 60

#undef LOG
#define LOG Log << "Main: "

SerialEchoBeacon beacon_01(1000, 1);

TaskScheduler sched;

LegPacingMasterClock legMasterClock (SAMPLE_TIME);

LegForwardLeft leftForward(SAMPLE_TIME, &legMasterClock );

void setup() {
  	
	unsigned long int now = millis();

	//Initilaize the communication.
	Serial.begin(9600);
	Log << "\n\n\n\n" << "Hello World again!" << endl;

	leftForward.init();		

	sched.add( &(leftForward.handler) );

	beacon_01.init(millis());
	sched.add( &beacon_01 );

	legMasterClock.init( now, LEG_ROTATIONS_PER_MINUTE );
	sched.add( &legMasterClock );
}

unsigned long int loops = 0;
bool done = false;
void loop() {
	while ( done == false ) {
		if ( loops++ < 2e6 ) {
			sched.run();
		} else {
			done = true;
			unsigned long int totalTime = millis();			
			leftForward.driver.setMotorPWM(0);			
			LOG << "Done with " << loops << " loops in " << totalTime << " ms for a rate of " << (int)floor(loops / (float)totalTime) << " loops/ms" << endl;			
			Log.sendToSerial();			
			
		}
	}
}
