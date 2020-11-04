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

class L : public Loggable
{
	public:
		LogStorage& operator()(unsigned long int now) 
		{
			return this->log(now);
		}
} logger;

SerialEchoBeacon beacon_01(1000);

TaskScheduler sched;

LegPacingMasterClock legMasterClock (SAMPLE_TIME);

LegForwardLeft leftForward(SAMPLE_TIME, &legMasterClock );

void setup() {
  	logger.setID("Main", "0");

	unsigned long int now = millis();

	//Initilaize the communication.
	Serial.begin(9600);
	Serial.setTimeout(0);
	logger(now) << "\n\n\n\n" << "Hello World again!" << endl;

	leftForward.init();		

	sched.add( &(leftForward.handler) );

	beacon_01.init(millis());
	beacon_01.setID( "Beacon", "1" );	
	sched.add( &beacon_01 );

	legMasterClock.init( now, LEG_ROTATIONS_PER_MINUTE );
	sched.add( &legMasterClock );
	
	logger(now) << "Setup complete" << endl;
	Log.sendToSerial();
}

unsigned long int loops = 0;
void loop() {
	unsigned long int now = millis();	
	sched.run();
	
	//Check if we have incoming serial data
	if ( Serial.available() > 0 ) {
		char input;
		Serial.readBytes( &input, 1 );
		switch ( input ) {
			case 'i': //Init the legs
				logger(now) << "Starting the initiator" << endl;
				leftForward.handler.startInitiator(millis());
				break;
			case 's': //Start the main cycle
				logger(now) << "Starting main loop" << endl;
				leftForward.handler.startMainLoop(millis());
				break;
			case 'f': //Flush out the logger. Can potentially break the interrupts.
				logger(now) << "Dumping log" << endl;
				Log.sendToSerial();
				break;
			case 'b': //Break the leg movement
				legMasterClock.stop();
				logger(now) << "Master clock stopped" << endl;
				break;			
			case 'r': //Restart the main clock
				legMasterClock.start();
				logger(now) << "Master clock stopped" << endl;
				break;			
		}
	}

	//Post regular updates
	loops++;
	if ( loops % (unsigned long int)1e6 == 0) {
		unsigned long int totalTime = millis();
		logger(now) << "Done with " << loops << " loops in " << totalTime << " ms for a rate of " << (int)floor(loops / (float)totalTime) << " loops/ms" << endl;				
	}
}
