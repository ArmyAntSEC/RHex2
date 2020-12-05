#include <Arduino.h>
#include <math.h>
#include <TaskScheduler.h>
#include <SerialEchoBeacon.h>
#include <LogStorage.h>

#include "LegForwardLeft.h"
#include "LegForwardRight.h"
#include "LegPacingMasterClock.h"

#include <CommandParser.h>
typedef CommandParser<> MyCommandParser;

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

MyCommandParser parser;

TaskScheduler sched;

LegPacingMasterClock legMasterClock (SAMPLE_TIME);

LegForwardLeft leftForward(SAMPLE_TIME, &legMasterClock );
LegForwardRight rightForward(SAMPLE_TIME, &legMasterClock );

unsigned long int loops = 0;
unsigned long int nextPing = 1;
unsigned long int lastPing = 0;

const unsigned int lineLength = 64;
char line[lineLength];
unsigned int lineWritePos = 0;

unsigned long int now = 0;

char response[MyCommandParser::MAX_RESPONSE_SIZE];

void moveLegsToPosWithHome(MyCommandParser::Argument *args, char *response)
{
	float wantedPos = args[0].asDouble;

	logger(now) << "Starting the initiator to pos " << wantedPos << endl;
	leftForward.initiator.setWantedPosition(wantedPos);
	rightForward.initiator.setWantedPosition(wantedPos);

	leftForward.handler.startInitiator(now);
	rightForward.handler.startInitiator(now);	

	strlcpy(response, "CmdParse: Move to position intiated", MyCommandParser::MAX_RESPONSE_SIZE);				
}

void flushLog(MyCommandParser::Argument *args, char *response)
{
	logger(now) << "Dumping log" << endl;
	Log.sendToSerial();
	strlcpy(response, "CmdParse: Flush complete.", MyCommandParser::MAX_RESPONSE_SIZE);
}

void takeStep(MyCommandParser::Argument *args, char *response)
{
	logger(now) << "Starting main left loop" << endl;
	logger(now) << "Time Module CurrPos SetPt Error PWM C(V) HomePos" << endl;
	leftForward.handler.startMainLoop(now);
	//rightForward.handler.startMainLoop(now);
	legMasterClock.init( now, LEG_ROTATIONS_PER_MINUTE );
	
	strlcpy(response, "CmdParse: Step initiated.", MyCommandParser::MAX_RESPONSE_SIZE);
}

void restart(MyCommandParser::Argument *args, char *response)
{
	rstc_start_software_reset(RSTC);
	strlcpy(response, "CmdParse: Restart.", MyCommandParser::MAX_RESPONSE_SIZE);
}

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
	
	parser.registerCommand("init", "d", &moveLegsToPosWithHome );
	parser.registerCommand("flush", "", &flushLog );
	parser.registerCommand("step", "", &takeStep );
	parser.registerCommand("restart", "", &restart );

	
	logger(now) << "Setup complete" << endl;
	Log.sendToSerial();	
}

void loop() {
	now = millis();	
	sched.run();
	
	//Check if we have incoming serial data
	if ( Serial.available() > 0 ) {		
		char input;		
		int n = Serial.readBytes( &input, 1 );
		Serial.print ( input );
		//Serial.print ( input );
		//Serial.print ( input, DEC );

		if ( n > 0 ) {
			if ( lineWritePos >= lineLength - 1 ) {
				Serial.println ( "Command input overflow. Discarding." );
				lineWritePos = 0;
			} else if ( input == '\b' && lineWritePos >= 1 ) {
				//A backspace character
				lineWritePos--;				
			} else if ( input != '\n' && input != '\r' ) {				
				line[lineWritePos] = input;
				lineWritePos++;
			} else if ( lineWritePos != 0 && (input == '\n' || input == '\r') ) {
				line[lineWritePos] = 0;
				lineWritePos = 0;

				//Call the command parser with line as argument
				Serial.print ( "Command: " );
				Serial.println ( line );
				
				parser.processCommand(line, response);
    			
				Serial.print ( "Response: " );
				Serial.println(response);
			} else {
				if ( lineWritePos > 0 ) {
					Serial.print ( "This cannot happen" );
				}
			}
		}
	}
		/*
			switch ( line ) {
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
					logger(now) << "Time Module CurrPos SetPt Error PWM C(V) HomePos" << endl;
					leftForward.handler.startMainLoop(now);
					//rightForward.handler.startMainLoop(now);
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
	}*/

	//Post regular updates
	loops++;
	if ( now > nextPing ) {
		//unsigned long int segmentTime = now - lastPing;
		//logger(now) << "Done with " << loops << " loops in " << segmentTime << " ms for a rate of " << (int)floor(loops / (float)segmentTime) << " loops/ms" << endl;				
		loops = 0;
		lastPing = now;
		nextPing += 5000;
	}
}
