/*
 * SerialEchoBeacon.h
 *
 *  Created on: 8 juli 2020
 *      Author: Daniel
 */

#ifndef SERIALECHOBEACON_H_
#define SERIALECHOBEACON_H_

#include "RecurringTask.h"

class SerialEchoBeacon: public RecurringTask { // @suppress("Class has a virtual method and non-virtual destructor")

public:
    // Create a new blinker for the specified pin and rate.
	SerialEchoBeacon(unsigned long int _rate, const int _ID) ;
    virtual void run(unsigned long int now);

    private:
        const int ID;
};

#endif /* SERIALECHOBEACON_H_ */
