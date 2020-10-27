#ifndef _LEGPROCESSOR_H_
#define _LEGPROCESSOR_H_

#include <pwm_lib.h>
using namespace arduino_due::pwm_lib;

#include <HomingEncoder.h>
#include "MotorDriver.h"
#include "MotorPositionInitiator.h"

 

class LegProcessor
{
    public:
        int encoderPin1;
        int encoderPin2;
        int breakerPin;
        int legOffset;

        int driverPin1;
        int driverPin2;
        pwm_base * driverPWM;

    public:
        HomingEncoder encoder;
        MotorDriver driver;
        MotorPositionInitiator initiator;

    public:        

        template <int N> void init()
        {
            encoder.init<N>( encoderPin1, encoderPin2, breakerPin );
            encoder.setPositionOffset( legOffset );

            driver.init( driverPin1, driverPin2, driverPWM );
            initiator.init ( &driver, &encoder );
        }        
};

#endif