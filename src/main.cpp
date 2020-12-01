#include <Arduino.h>
#include <math.h>
#include <TaskScheduler.h>
#include <SerialEchoBeacon.h>
#include <LogStorage.h>

#include "LegForwardLeft.h"
#include "LegForwardRight.h"
#include "LegPacingMasterClock.h"

#define SAMPLE_TIME 100
#define LEG_ROTATIONS_PER_MINUTE 15

class L : public Loggable
{
	public:
		LogStorage& operator()(unsigned long int now) 
		{
			return this->log(now);
		}
} logger;

TaskScheduler sched;

LegPacingMasterClock legMasterClock (SAMPLE_TIME);

LegForwardLeft leftForward(SAMPLE_TIME, &legMasterClock );
LegForwardRight rightForward(SAMPLE_TIME, &legMasterClock );

void setup() {
  	logger.setID("Main", "0");
	sched.setID( "Scheduler", "0" );
	
	unsigned long int now = millis();

	//Initilaize the communication.
	Serial.begin(115200);
	Serial.setTimeout(100);
	logger(now) << "\n\n\n\n" << "Hello World again!" << endl;

	leftForward.init();		
	sched.add( &(leftForward.handler) );

	rightForward.init();		
	sched.add( &(rightForward.handler) );

	legMasterClock.setID( "MasterClk", "0");
	sched.add( &legMasterClock );
	
	logger(now) << "Setup complete" << endl;
	Log.sendToSerial();
}

unsigned long int loops = 0;
unsigned long int nextPing = 1;
unsigned long int lastPing = 0;

void loop() {
	unsigned long int now = millis();	
	sched.run();
	
	//Check if we have incoming serial data
	if ( Serial.available() > 0 ) {
		char input;
		int n = Serial.readBytes( &input, 1 );		
		Serial.print( input );
		Serial.print( ": " );
		Serial.println ( n );		
		switch ( input ) {
			case 'i': //Init the legs
				logger(now) << "Starting the initiator" << endl;
				leftForward.initiator.setWantedPosition(0);
				rightForward.initiator.setWantedPosition(0);

				leftForward.handler.startInitiator(now);
				rightForward.handler.startInitiator(now);				
				break;
			case 's': //Set the robot legs to sleep position
				logger(now) << "Setting legs into sleep mode" << endl;
				leftForward.initiator.setWantedPosition(0.42);
				rightForward.initiator.setWantedPosition(0.42);
				leftForward.handler.startInitiator(now);
				rightForward.handler.startInitiator(now);				
				break;
			case 'm': //Start the main cycle
				logger(now) << "Starting main left loop" << endl;
				leftForward.handler.startMainLoop(now);
				rightForward.handler.startMainLoop(now);
				legMasterClock.init( now, LEG_ROTATIONS_PER_MINUTE );
				break;
			case 'f': //Flush out the logger. Can potentially break the interrupts.
				logger(now) << "Dumping log" << endl;
				Log.sendToSerial();
				break;
			case 'r': //Reset the processor
				rstc_start_software_reset(RSTC);
				break;
		}
	}

	//Post regular updates
	loops++;
	if ( now > nextPing ) {
		unsigned long int segmentTime = now - lastPing;
		//logger(now) << "Done with " << loops << " loops in " << segmentTime << " ms for a rate of " << (int)floor(loops / (float)segmentTime) << " loops/ms" << endl;				
		loops = 0;
		lastPing = now;
		nextPing += 5000;
	}
}
