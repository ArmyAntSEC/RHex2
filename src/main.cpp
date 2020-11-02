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
	Serial.setTimeout(0);
	Log << "\n\n\n\n" << "Hello World again!" << endl;

	leftForward.init();		

	sched.add( &(leftForward.handler) );

	beacon_01.init(millis());
	sched.add( &beacon_01 );

	legMasterClock.init( now, LEG_ROTATIONS_PER_MINUTE );
	sched.add( &legMasterClock );
	
	Log << "Setup complete" << endl;
	Log.sendToSerial();
}

unsigned long int loops = 0;
void loop() {
	sched.run();
	
	//Check if we have incoming serial data
	if ( Serial.available() > 0 ) {
		char input;
		Serial.readBytes( &input, 1 );
		switch ( input ) {
			case 'i': //Init the legs
				Serial.println( "Starting the initiator" );
				leftForward.handler.startInitiator(millis());
				break;
			case 's': //Start the main cycle
				Serial.println( "Starting main loop" );
				leftForward.handler.startMainLoop(millis());
				break;
			case 'f': //Flush out the log. Can potentially break the interrupts.
				Serial.println ( "Dumping log" );
				Log.sendToSerial();
				break;
			case 'b': //Break the leg movement
				legMasterClock.stop();
				Serial.println ( "Master clock stopped" );
				break;			
			case 'r': //Restart the main clock
				legMasterClock.start();
				Serial.println ( "Master clock stopped" );
				break;			
		}
	}

	//Post regular updates
	loops++;
	if ( loops % (unsigned long int)1e6 == 0) {
		unsigned long int totalTime = millis();
		LOG << "Done with " << loops << " loops in " << totalTime << " ms for a rate of " << (int)floor(loops / (float)totalTime) << " loops/ms" << endl;				
	}
}
