/*
 * TaskScheduler.h
 *
 *  Created on: 8 juli 2020
 *      Author: Daniel
 */

#ifndef TASKSCHEDULER_H_
#define TASKSCHEDULER_H_

#include "Task.h"
#include "Loggable.h"

class TaskScheduler: public Loggable {
public:
    TaskScheduler();
    void add ( Task* task );
    void run();
private:
    Task *tasks[10];
    int numTasks;
    int loopsSinceLastRun;
};

#endif /* TASKSCHEDULER_H_ */
