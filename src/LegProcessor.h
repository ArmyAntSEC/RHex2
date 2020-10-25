#ifndef _LEGPROCESSOR_H_
#define _LEGPROCESSOR_H_

#include <HomingEncoder.h>
#include "MotorDriver.h"

class LegProcessor
{
    public:
        int encoderPin1;
        int encoderPin2;
        int breakerPin;
        int legOffset;

        int driverPin1;
        int driverPin2;
        int driverPWM;

    public:
        HomingEncoder encoder;
        MotorDriver driver;

    public:

        template <int N> void init()
        {
            encoder.init<N>( encoderPin1, encoderPin2, breakerPin );
            encoder.setPositionOffset( legOffset );

            driver.init( driverPin1, driverPin2, driverPWM );
        }        
};

#endif