#ifndef _LEGPROCESSOR_H_
#define _LEGPROCESSOR_H_

#include <pwm_lib.h>
using namespace arduino_due::pwm_lib;

#include <HomingEncoder.h>
#include "MotorDriver.h"
#include "MotorPositionInitiator.h"
#include "MotorPositionScheduler.h"
#include "LegPacingMasterClock.h"
#include "MotorStepRegulator.h"

class LegProcessor
{
    public:
        const int sampleTime;
        LegPacingMasterClock * masterClock; 

        int encoderPin1;
        int encoderPin2;
        int breakerPin;
        int outputPin;

        int legOffset;

        int driverPin1;
        int driverPin2;
        pwm_base * driverPWM;

        double Kp = 1000;
        double Ki = 0;
        double Kd = 50;
        

    public:
        HomingEncoder encoder;
        MotorDriver driver;
        MotorPositionInitiator initiator;
        MotorStepRegulator regulator;        
        MotorStateHandler handler;
        MotorPositionScheduler scheduler;
        


    public:       
        LegProcessor(unsigned long int _rate, LegPacingMasterClock * _masterClock ):  
            sampleTime( _rate ), masterClock(_masterClock), handler( _rate )
        {}

        template <int N> void init()
        {
            encoder.init<N>( encoderPin1, encoderPin2, breakerPin, outputPin, legOffset );            
                    
            driver.init( driverPin1, driverPin2, driverPWM );
        
            regulator.init( &driver, &encoder );
            initiator.init ( &handler, &driver, &encoder, &regulator );

            scheduler.init ( &handler, &driver, &encoder, &regulator, masterClock );
            handler.init( &initiator, &scheduler );                        
        }        
};

#endif