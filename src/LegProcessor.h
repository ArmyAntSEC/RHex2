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
        const int sampleTime;

        int encoderPin1;
        int encoderPin2;
        int breakerPin;
        int legOffset;

        int driverPin1;
        int driverPin2;
        pwm_base * driverPWM;

        double Kp;
        double Ki;
        double Kd;
        

    public:
        HomingEncoder encoder;
        MotorDriver driver;
        MotorPositionInitiator initiator;
        MotorPIDRegulator regulator;
        PID pid;
        MotorStateHandler handler;

    public:       
        LegProcessor(unsigned long int _rate):  
            sampleTime( _rate ), handler( _rate )
        {}

        template <int N> void init()
        {
            encoder.init<N>( encoderPin1, encoderPin2, breakerPin );
            encoder.setPositionOffset( legOffset );
                    
            driver.init( driverPin1, driverPin2, driverPWM );
        
            pid.init( Kp, Ki, Kd, sampleTime, P_ON_E, DIRECT );            
        
            regulator.init( &driver, &encoder, &pid );
            initiator.init ( &handler, &driver, &encoder, &regulator );

            handler.init( &initiator, 0);            
            handler.startInitiator();            
        }        
};

#endif