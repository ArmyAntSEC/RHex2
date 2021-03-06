/*
 * TaskScheduler.cpp
 *
 *  Created on: 8 juli 2020
 *      Author: Daniel
 */

#include "TaskScheduler.h"

#include "Arduino.h"
#include "LogStorage.h"

TaskScheduler::TaskScheduler() :
  numTasks(0), loopsSinceLastRun(0)
{
	memset( &tasks, 0, sizeof(Task)*10);
}

void TaskScheduler::add(Task* task) {
	if ( numTasks < 10 ) {
		tasks[numTasks] = task;
		numTasks++;
	} else {
		Serial.println ( "TaskScheduler: More than 10 tasks cannot be allocated.");
	}
}

void TaskScheduler::run() {
	//Serial.println ( "Running task scheduler" );
	unsigned long int now = millis();
	//unsigned long int nowMicro = micros();
	Task **tpp = tasks;
	int runCount = 0;
	//int maxTaskRan = 0;
	for (int t = 0; t < numTasks; t++) {
		Task *tp = *tpp;				
		if (tp->canRun(now)) {			
			runCount++;
			//maxTaskRan = t;
			tp->run(now);
			break;
		}
		tpp++;
	}
	if ( runCount ) 
	{
		/*
		unsigned long int endTime = micros();
		log(now) << "Scheduler: Task time [us]: " << endTime - nowMicro <<
		" Loops since last run: " << loopsSinceLastRun <<
		" Max task in this run: " << maxTaskRan << 
		" Total tasks: " << numTasks << endl;
		*/
		loopsSinceLastRun = 0;
	} else  {
		this->loopsSinceLastRun++;
	}
}
