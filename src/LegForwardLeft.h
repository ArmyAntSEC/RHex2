#ifndef _LEFTFORWARDLEG_H_
#define _LEFTFORWARDLEG_H_

#include "LegProcessor.h"
#include <pwm_lib.h>
using namespace arduino_due::pwm_lib;


class LegForwardLeft : public LegProcessor {
    private:
        pwm<pwm_pin::PWML7_PC24> driverPinPWM_HW; //Pin 6
        pwm_wrapper<pwm<pwm_pin::PWML7_PC24>> pwmWrapper;

    public:
        LegForwardLeft( unsigned long int _rate, LegPacingMasterClock * _masterClock ): 
            LegProcessor(_rate, _masterClock), pwmWrapper(driverPinPWM_HW)    
        {
            encoderPin1 = 2;
            encoderPin2 = 3;
            breakerPin = 12;
            outputPin = A0;
            legOffset = -150 + 3592*0.4; //Added 0.4 of a rotation to test the pacing.             

            contactFactor = 0.8;

            driverPin1 = 4;
            driverPin2 = 5;   
            driverPWM = &pwmWrapper;               
        }

        void init()
        {
            LegProcessor::init<1>();
            this->initiator.setID("Initiator", "LF" );
            this->stepRegulator.setID("StepRegulator", "LF");
            this->pidRegulator.setID("PidRegulator", "LF");        
            this->scheduler.setID("Scheduler", "LF" );
        }
        
};    
#endif